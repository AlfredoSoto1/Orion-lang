#include "ActionTable.hpp"

#include <iostream>
#include <queue>
#include <stack>

namespace compiler {

  ActionTable::ActionTable(const Grammar& grammar) noexcept
      : grammar(grammar) {}

  void ActionTable::build() {
    // Create a local table and a list of tiemsets
    Table transitions;
    std::vector<ItemSet> states;

    // Build the action table
    buildStates(states, transitions);
    buildTables(states, transitions);
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

        // If its a terminal, continue
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

      // Get symbol at the dot position
      auto it = rule.rhs.begin();
      std::advance(it, item.dot_position);
      const Symbol& current = *it;

      // If it matches the input symbol, move the dot forward
      if (current == symbol) {
        moved.insert({item.rule_index, item.dot_position + 1});
      }
    }

    // Return closure of all advanced items
    return closure(moved);
  }

  void ActionTable::buildStates(std::vector<ItemSet>& states,
                                Table& transitions) {
    // Augmented start rule: assume rule 0 is S' → S
    ItemSet start_set = closure({Item{0, 0}});
    states.push_back(start_set);

    std::queue<State> worklist;
    worklist.push(0);

    while (!worklist.empty()) {
      size_t current_state = worklist.front();
      worklist.pop();

      const ItemSet& items = states[current_state];
      std::unordered_map<Symbol, ItemSet, SymbolHash> symbol_to_itemset;

      // Compute GOTO(I, X) for every symbol after dot in each item
      for (const auto& item : items) {
        const auto& rule = grammar[item.rule_index];
        if (item.dot_position >= rule.rhs.size()) continue;

        Symbol next_symbol = *(rule.rhs.begin() + item.dot_position);
        Item next_item{item.rule_index, item.dot_position + 1};
        symbol_to_itemset[next_symbol].insert(next_item);
      }

      for (const auto& [symbol, raw_items] : symbol_to_itemset) {
        ItemSet next_closure = closure(raw_items);

        // Check if already in states
        auto it = std::find(states.begin(), states.end(), next_closure);
        size_t next_state;

        if (it == states.end()) {
          next_state = states.size();
          states.push_back(next_closure);
          worklist.push(next_state);
        } else {
          next_state = std::distance(states.begin(), it);
        }

        transitions[{current_state, symbol}] = next_state;
      }
    }
  }

  void ActionTable::buildTables(std::vector<ItemSet>& states,
                                Table& transitions) {
    for (size_t state_id = 0; state_id < states.size(); ++state_id) {
      const ItemSet& item_set = states[state_id];

      // Process all items in the current state
      for (const Item& item : item_set) {
        const Rule& rule = grammar[item.rule_index];

        // Dot at the end → REDUCE or ACCEPT
        if (item.dot_position < rule.rhs.size()) {
          // Dot not at the end → SHIFT
          Symbol next_symbol = *(rule.rhs.begin() + item.dot_position);

          // Check if a transition on this symbol exists
          auto it = transitions.find({state_id, next_symbol});
          if (it == transitions.end()) {
            continue;
          }

          // Add SHIFT action to table
          Action action{};
          action.type = Action::Type::SHIFT;
          action.next_state = it->second;
          action_table[{state_id, next_symbol}] = action;
          continue;
        }

        // S' → S . => ACCEPT
        if (item.rule_index == 0) {
          Symbol eof;
          eof.type = Symbol::Type::EOF_TERMINAL;
          eof.terminal.eof = 2;
          action_table[{state_id, eof}] = Action{Action::Type::ACCEPT};

          continue;
        }

        // A → α . => REDUCE by rule_index on any terminal
        for (uint8_t t = 0; t <= (uint8_t)Symbol::Type::COUNT; ++t) {
          Symbol sym;
          sym.type = (Symbol::Type)t;

          // Only reduce on terminals (not non-terminals)
          if (sym.type == Symbol::Type::NON_TERMINAL ||
              sym.type == Symbol::Type::UNKNOWN) {
            continue;
          }

          // Add REDUCE action to table
          Action action{};
          action.type = Action::Type::REDUCE;
          action.rule_index = item.rule_index;
          action_table[{state_id, sym}] = action;
        }
      }
    }
  }

}  // namespace compiler
