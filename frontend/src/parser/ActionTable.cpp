#include "ActionTable.hpp"

#include <iostream>
#include <queue>
#include <stack>

namespace compiler {

  bool ActionTable::Item::operator==(const Item& other) const {
    return rule_index == other.rule_index && dot_position == other.dot_position;
  }

  size_t ActionTable::ItemHasher::operator()(const Item& item) const {
    return std::hash<size_t>()(item.rule_index) ^
           (std::hash<size_t>()(item.dot_position) << 1);
  }

  size_t ActionTable::ItemSetHasher::operator()(const ItemSet& set) const {
    size_t hash = set.size();
    for (const Item& element : set) {
      hash ^= ItemHasher{}(element);
    }
    return hash;
  }

  ActionTable::ActionTable(const Grammar& grammar) noexcept
      : grammar(grammar), terminals() {
    obtainAllTerminals();
  }

  void ActionTable::build() {
    // Stack allocate the table transitions and item-set states.
    // This is just a one time creation, meaning that there is no need
    // to keep a reference alive after build method returns. Unless
    // its for debugging.
    // Table transitions;
    // std::vector<ItemSet> states;

    // Build the action table
    buildStates(states, transitions);
    buildTables(states, transitions);
  }

  Action ActionTable::actionFrom(StateSymbol&& state_symbol) {
    auto it = action_table.find(state_symbol);
    return it != action_table.end() ? it->second : Action::error();
  }

  ActionTable::State ActionTable::stateFrom(StateSymbol&& state_symbol) {
    auto it = goto_table.find(state_symbol);
    return it != goto_table.end() ? it->second : -1;
  }

  void ActionTable::obtainAllTerminals() {
    for (const auto& rule : grammar) {
      for (const auto& sym : rule.rhs) {
        if (sym.type != Symbol::Type::NON_TERMINAL) {
          terminals.insert(sym);
        }
      }
    }

    Symbol end_sym;
    end_sym.type = Symbol::Type::EOF_TERMINAL;
    end_sym.terminal.eof = 2;
    terminals.insert(end_sym);
  }

  ActionTable::ItemSet ActionTable::closure(const ItemSet& items) {
    bool changed = true;
    ItemSet result = items;

    while (changed) {
      changed = false;
      ItemSet new_items;

      // For every item obtain the rule associated
      for (const auto& item : result) {
        const Rule& rule = grammar[item.rule_index];

        // Make sure the position hasnt reached the end of the rule
        if (item.dot_position >= rule.rhs.size()) {
          continue;
        }

        // Obtain the symbol from the current position
        auto it = rule.rhs.begin();
        std::advance(it, item.dot_position);
        const Symbol& sym = *it;

        // If its a terminal, skip operation
        if (sym.type != Symbol::Type::NON_TERMINAL) {
          continue;
        }

        // Look for the LHS non-terminal that matches the RHS of the current
        // rule we are scanning.
        for (size_t i = 0; i < grammar.size(); ++i) {
          if (grammar[i].lhs != sym.nonterminal) {
            continue;
          }

          Item new_item{i, 0};

          // Make sure the new_item doesn't exist in set.
          if (result.find(new_item) != result.end()) {
            continue;
          }

          new_items.insert(new_item);
          changed = true;
        }
      }

      result.insert(new_items.begin(), new_items.end());
    }

    return result;
  }

  ActionTable::ItemSet ActionTable::goTo(const ItemSet& items,
                                         const Symbol& symbol) {
    ItemSet moved;

    for (const auto& item : items) {
      const Rule& rule = grammar[item.rule_index];

      // Skip if dot is at the end
      if (item.dot_position >= rule.rhs.size()) continue;

      // Get symbol at the current dot position
      auto it = rule.rhs.begin();
      std::advance(it, item.dot_position);
      const Symbol& curr_sym = *it;

      // If it matches the input symbol, move the dot forward
      if (curr_sym == symbol) {
        moved.insert({item.rule_index, item.dot_position + 1});
      }
    }

    // Return closure of all advanced items
    return closure(moved);
  }

  void ActionTable::buildStates(std::vector<ItemSet>& states,
                                Table& transitions) {
    // Augmented start rule: assume rule 0 is S' → S
    states.push_back(closure({Item{0, 0}}));

    ItemSetState itemset_to_state;
    itemset_to_state[states[0]] = 0;

    std::queue<State> worklist;
    worklist.push(0);

    while (!worklist.empty()) {
      State current_state = worklist.front();
      worklist.pop();

      ItemSet state_items = states[current_state];

      // Collect all symbols that appear just after a dot
      SymbolSet next_symbols;
      for (const Item& item : state_items) {
        const Rule& rule = grammar[item.rule_index];
        if (item.dot_position < rule.rhs.size()) {
          // Get symbol at the current dot position
          auto it = rule.rhs.begin();
          std::advance(it, item.dot_position);
          next_symbols.insert(*it);
        }
      }

      for (const Symbol& sym : next_symbols) {
        ItemSet next = goTo(state_items, sym);
        if (next.empty()) continue;

        State target_state;
        auto it = itemset_to_state.find(next);
        if (it != itemset_to_state.end()) {
          target_state = it->second;
        } else {
          target_state = states.size();
          states.push_back(next);
          itemset_to_state[next] = target_state;
          worklist.push(target_state);
        }

        transitions[{current_state, sym}] = target_state;
      }
    }
  }

  void ActionTable::buildTables(std::vector<ItemSet>& states,
                                Table& transitions) {
    for (size_t state = 0; state < states.size(); ++state) {
      const auto& items = states[state];

      for (const Item& item : items) {
        const Rule& rule = grammar[item.rule_index];

        if (item.dot_position < rule.rhs.size()) {
          // Get symbol at the current dot position
          auto it = rule.rhs.begin();
          std::advance(it, item.dot_position);

          // Dot is not at end → try shift
          Symbol sym = *it;
          if (sym.type != Symbol::Type::NON_TERMINAL) {
            auto it = transitions.find({state, sym});
            if (it != transitions.end()) {
              action_table[{state, sym}] = Action::shift(it->second);
            }
          }
        } else {
          // Dot at end → reduce or accept
          if (item.rule_index == 0) {
            // Accept on end-of-input symbol
            action_table[{state, Symbol::endOF()}] = Action::accept();
          } else {
            // Reduce by this rule for all terminals
            for (const Symbol& terminal : terminals) {
              action_table[{state, terminal}] = Action::reduce(item.rule_index);
            }
          }
        }
      }

      // Handle gotos for non-terminals
      for (const auto& [key, target] : transitions) {
        if (key.first == state &&
            key.second.type == Symbol::Type::NON_TERMINAL) {
          goto_table[{state, key.second}] = target;
        }
      }
    }
  }
}  // namespace compiler