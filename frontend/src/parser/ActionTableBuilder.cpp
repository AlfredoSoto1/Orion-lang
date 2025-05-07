#include "ActionTableBuilder.hpp"

#include <iostream>
#include <stack>

namespace compiler {
  ActionTableBuilder::ActionTableBuilder(const Grammar& grammar) noexcept
      : grammar(grammar) {
    // // === STEP 1: Build Action Table and get rules ===
    // std::vector<RuleNew> rules;

    // Symbol IDENTIFIER;
    // IDENTIFIER.type = Symbol::Type::IDENTIFIER;
    // IDENTIFIER.terminal.ident_or_lit = 0;

    // Symbol CONSTANT;
    // CONSTANT.type = Symbol::Type::LITERAL;
    // CONSTANT.terminal.ident_or_lit = 1;

    // Symbol EXPR;
    // EXPR.type = Symbol::Type::NON_TERMINAL;
    // EXPR.nonterminal = NonTerminal::EXPR;

    // Symbol PRIMARY_EXPR;
    // PRIMARY_EXPR.type = Symbol::Type::NON_TERMINAL;
    // PRIMARY_EXPR.nonterminal = NonTerminal::PRIMARY_EXPR;

    // Symbol ASSIGNMENT_EXPR;
    // ASSIGNMENT_EXPR.type = Symbol::Type::NON_TERMINAL;
    // ASSIGNMENT_EXPR.nonterminal = NonTerminal::ASSIGNMENT_EXPR;

    // Symbol EQ;
    // EQ.type = Symbol::Type::PUNCTUATOR;
    // EQ.terminal.punctuator = Punctuator::EQ;

    // Symbol END;
    // END.type = Symbol::Type::ENDOF;

    // // Same rule set
    // rules = {
    //     RuleNew{EXPR, {IDENTIFIER}},                       // Rule 0
    //     RuleNew{EXPR, {CONSTANT}},                         // Rule 1
    //     RuleNew{EXPR, {PRIMARY_EXPR}},                     // Rule 2
    //     RuleNew{PRIMARY_EXPR, {IDENTIFIER}},               // Rule 3
    //     RuleNew{ASSIGNMENT_EXPR, {IDENTIFIER, EQ, EXPR}},  // Rule 4
    // };

    // ActionTable table;
    // generateActionTable(rules, table);

    // // === STEP 2: Simulate parsing IDENTIFIER = CONSTANT ===
    // std::vector<Symbol> input = {IDENTIFIER, EQ, CONSTANT, END};
    // std::stack<uint32_t> state_stack;
    // std::stack<Symbol> symbol_stack;

    // state_stack.push(0);  // start state
    // size_t pos = 0;

    // while (true) {
    //   uint32_t state = state_stack.top();
    //   Symbol lookahead = input[pos];

    //   auto it = table.find({state, lookahead});
    //   if (it == table.end()) {
    //     std::cerr << "ERROR: No action at state " << state << " with symbol "
    //               << symbolToString(lookahead) << "\n";
    //     break;
    //   }

    //   Action action = it->second;
    //   if (action.type == Action::SHIFT) {
    //     std::cout << "SHIFT " << symbolToString(lookahead) << " -> state "
    //               << action.next_state << "\n";
    //     symbol_stack.push(lookahead);
    //     state_stack.push(action.next_state);
    //     ++pos;
    //   } else if (action.type == Action::REDUCE) {
    //     const RuleNew& rule = rules[action.rule_index];
    //     std::cout << "REDUCE using rule " << action.rule_index << ": "
    //               << symbolToString(rule.lhs) << " -> ";
    //     for (const Symbol& s : rule.rhs) std::cout << symbolToString(s) << "
    //     "; std::cout << "\n";

    //     for (size_t i = 0; i < rule.rhs.size(); ++i) {
    //       symbol_stack.pop();
    //       state_stack.pop();
    //     }

    //     Symbol lhs = rule.lhs;
    //     symbol_stack.push(lhs);

    //     uint32_t top = state_stack.top();
    //     auto goto_it = table.find({top, lhs});
    //     if (goto_it == table.end()) {
    //       std::cerr << "ERROR: No GOTO after reduce at state " << top
    //                 << " with lhs " << symbolToString(lhs) << "\n";
    //       break;
    //     }

    //     state_stack.push(goto_it->second.next_state);
    //   } else if (action.type == Action::ACCEPT) {
    //     std::cout << "ACCEPTED\n";
    //     break;
    //   } else {
    //     std::cerr << "ERROR action\n";
    //     break;
    //   }
    // }

    // Symbol IDENTIFIER;
    // IDENTIFIER.type = Symbol::Type::IDENTIFIER;
    // IDENTIFIER.terminal.ident_or_lit = 0;

    // Symbol CONSTANT;
    // CONSTANT.type = Symbol::Type::LITERAL;
    // CONSTANT.terminal.ident_or_lit = 1;

    // Symbol EXPR;
    // EXPR.type = Symbol::Type::NON_TERMINAL;
    // EXPR.nonterminal = NonTerminal::EXPR;

    // Symbol PRIMARY_EXPR;
    // PRIMARY_EXPR.type = Symbol::Type::NON_TERMINAL;
    // PRIMARY_EXPR.nonterminal = NonTerminal::PRIMARY_EXPR;

    // Symbol ASSIGNMENT_EXPR;
    // ASSIGNMENT_EXPR.type = Symbol::Type::NON_TERMINAL;
    // ASSIGNMENT_EXPR.nonterminal = NonTerminal::ASSIGNMENT_EXPR;

    // Terminal t{};
    // t.punctuator = Punctuator::EQ;

    // // Define the rules
    // std::vector<RuleNew> rules = {
    //     // Rule 1: EXPR -> IDENTIFIER
    //     RuleNew{EXPR, {IDENTIFIER}},
    //     // Rule 2: EXPR -> CONSTANT
    //     RuleNew{EXPR, {CONSTANT}},
    //     // Rule 3: EXPR -> PRIMARY_EXPR
    //     RuleNew{EXPR, {PRIMARY_EXPR}},
    //     // Rule 4: PRIMARY_EXPR -> IDENTIFIER
    //     RuleNew{PRIMARY_EXPR, {IDENTIFIER}},
    //     // Rule 5: ASSIGNMENT_EXPR -> IDENTIFIER '=' EXPR
    //     RuleNew{ASSIGNMENT_EXPR,
    //             {IDENTIFIER, Symbol{Symbol::Type::PUNCTUATOR, t}, EXPR}}};

    // // Define action table
    // ActionTable actionTable;

    // // Call the generateActionTable function
    // generateActionTable(rules, actionTable);

    // // Print the action table for verification
    // for (const auto& entry : actionTable) {
    //   const auto& [state_symbol, action] = entry;
    //   const auto& [state, symbol] = state_symbol;

    //   std::cout << "State: " << state << ", Symbol: " <<
    //   symbolToString(symbol)
    //             << ", Action: ";
    //   if (action.type == Action::SHIFT) {
    //     std::cout << "SHIFT to state " << action.next_state << "\n";
    //   } else if (action.type == Action::REDUCE) {
    //     std::cout << "REDUCE using rule " << action.rule_index << "\n";
    //   } else if (action.type == Action::ACCEPT) {
    //     std::cout << "ACCEPT\n";
    //   } else {
    //     std::cout << "ERROR\n";
    //   }
    // }
  }

  // std::string ActionTableBuilder::symbolToString(const Symbol& s) {
  //   switch (s.type) {
  //     case Symbol::Type::IDENTIFIER:
  //       return "IDENTIFIER";
  //     case Symbol::Type::LITERAL:
  //       return "CONSTANT";
  //     case Symbol::Type::PUNCTUATOR:
  //       return "PUNCTUATOR_" +
  //              std::to_string(static_cast<int>(s.terminal.punctuator));
  //     case Symbol::Type::NON_TERMINAL:
  //       return "NT_" + std::to_string(static_cast<int>(s.nonterminal));
  //     case Symbol::Type::ENDOF:
  //       return "ENDOF";
  //     default:
  //       return "UNKNOWN";
  //   }
  // }

  void ActionTableBuilder::build() {
    //
  }

  ActionTableBuilder::ItemSet ActionTableBuilder::closure(
      const ItemSet& items) {
    // Prepare item set result and stop condition flag
    bool changed = true;
    ItemSet result = items;

    while (changed) {
      changed = false;
      ItemSet new_items;

      // For every item obtain the rule associated
      for (const auto& item : result) {
        const RuleN& rule = grammar[item.rule_index];

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

  ActionTableBuilder::ItemSet ActionTableBuilder::goTo(const ItemSet& items,
                                                       const Symbol& symbol) {
    ItemSet moved;

    for (const auto& item : items) {
      const RuleN& rule = grammar[item.rule_index];

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

  // ActionTableBuilder::ItemSet ActionTableBuilder::goTo(
  //     const ItemSet& items, const Symbol& sym,
  //     const std::vector<RuleNew>& rules) {
  //   ItemSet moved;

  //   // For each item, obtain the rule associated
  //   for (const auto& item : items) {
  //     const RuleNew& rule = rules[item.rule_index];

  //     // Check if the item hasnt completed a full rhs grammar rule
  //     // and if the symbol is expected at the dot position of the item.
  //     if (item.dot_position < rule.rhs.size() &&
  //         rule.rhs[item.dot_position] == sym) {
  //       moved.insert({item.rule_index, item.dot_position + 1});
  //     }
  //   }

  //   return closure(moved, rules);
  // }

  // ActionTableBuilder::SymbolSet ActionTableBuilder::allSymbols(
  //     const std::vector<RuleNew>& rules) {
  //   SymbolSet symbols;

  //   // Add the LHS (left-hand side) of each rule
  //   for (const RuleNew& rule : rules) {
  //     symbols.insert(rule.lhs);
  //   }

  //   // Add the RHS (right-hand side) symbols of each rule
  //   for (const RuleNew& rule : rules) {
  //     for (const Symbol& symbol : rule.rhs) {
  //       symbols.insert(symbol);
  //     }
  //   }

  //   return symbols;
  // }

  // ActionTableBuilder::SymbolSet ActionTableBuilder::allTerminals(
  //     const std::vector<RuleNew>& rules) {
  //   SymbolSet terminals;

  //   for (const RuleNew& rule : rules) {
  //     for (const Symbol& symbol : rule.rhs) {
  //       if (symbol.type != Symbol::Type::NON_TERMINAL) {
  //         terminals.insert(symbol);
  //       }
  //     }
  //   }

  //   return terminals;
  // }

  // std::vector<ActionTableBuilder::ItemSet> ActionTableBuilder::buildStates(
  //     const std::vector<RuleNew>& rules) {
  //   std::vector<ItemSet> states;
  //   std::unordered_map<ItemSet, uint32_t, ItemSetHasher, ItemSetEqual>
  //       state_ids;

  //   // Create the initial item set (closure of the augmented start rule)
  //   if (rules.empty()) {
  //     return states;
  //   }

  //   // Assume the LHS of the first rule is the start symbol for augmentation
  //   Symbol start_symbol = rules[0].lhs;
  //   Symbol augmented_start_symbol;
  //   augmented_start_symbol.type = Symbol::Type::NON_TERMINAL;
  //   augmented_start_symbol.nonterminal = NonTerminal::TEMP_START;
  //   RuleNew augmented_rule{augmented_start_symbol, {start_symbol}};

  //   std::vector<RuleNew> augmented_rules = {augmented_rule};
  //   augmented_rules.insert(augmented_rules.end(), rules.begin(),
  //   rules.end());

  //   ItemSet initial_items = closure({{0, 0}}, augmented_rules);
  //   states.push_back(initial_items);
  //   state_ids[initial_items] = 0;

  //   std::queue<ItemSet> queue;
  //   queue.push(initial_items);

  //   uint32_t next_state_id = 1;

  //   // 2. Iterate through the states and compute transitions
  //   while (!queue.empty()) {
  //     ItemSet current_state = queue.front();
  //     queue.pop();
  //     uint32_t current_state_index = state_ids[current_state];

  //     // Find all possible next symbols from the current state
  //     SymbolSet next_symbols;
  //     for (const auto& item : current_state) {
  //       const RuleNew& rule = augmented_rules[item.rule_index];
  //       if (item.dot_position < rule.rhs.size()) {
  //         next_symbols.insert(rule.rhs[item.dot_position]);
  //       }
  //     }

  //     // For each next symbol, compute the next state (using goTo)
  //     for (const auto& symbol : next_symbols) {
  //       ItemSet next_state = goTo(current_state, symbol, augmented_rules);

  //       if (!next_state.empty()) {
  //         // If the next state hasn't been encountered, add it to the list of
  //         // states and the queue
  //         if (state_ids.find(next_state) == state_ids.end()) {
  //           states.push_back(next_state);
  //           state_ids[next_state] = next_state_id++;
  //           queue.push(next_state);
  //         }
  //       }
  //     }
  //   }

  //   return states;
  //   // ItemSet start_items = closure({{0, 0}}, rules);
  //   // states.push_back(start_items);
  //   // state_ids[start_items] = 0;

  //   // std::queue<ItemSet> queue;
  //   // queue.push(start_items);

  //   // while (!queue.empty()) {
  //   //   auto current = queue.front();
  //   //   queue.pop();

  //   //   for (const Symbol& symbol : allSymbols(rules)) {
  //   //     auto next = goTo(current, symbol, rules);
  //   //     if (!next.empty() && state_ids.find(next) == state_ids.end()) {
  //   //       state_ids[next] = states.size();
  //   //       states.push_back(next);
  //   //       queue.push(next);
  //   //     }
  //   //   }
  //   // }
  //   //
  //   // return states;
  // }

  // void ActionTableBuilder::generateActionTable(
  //     const std::vector<RuleNew>& rules, ActionTable& table) {
  //   // 1. Build the states (LR(0) item sets)
  //   std::vector<ItemSet> states = buildStates(rules);

  //   // 2. Create a mapping from states to their IDs (indices)
  //   std::unordered_map<ItemSet, uint32_t, ItemSetHasher, ItemSetEqual>
  //       state_ids;
  //   for (size_t i = 0; i < states.size(); ++i) {
  //     state_ids[states[i]] = i;
  //   }

  //   // 3. Iterate through each state and construct the action table entries
  //   for (size_t i = 0; i < states.size(); ++i) {
  //     const ItemSet& items = states[i];

  //     for (const auto& item : items) {
  //       const RuleNew& rule = rules[item.rule_index];

  //       if (item.dot_position < rule.rhs.size()) {
  //         // 3.1. Handle shift actions
  //         Symbol symbol = rule.rhs[item.dot_position];
  //         ItemSet next_state = goTo(items, symbol, rules);
  //         uint32_t next_state_id = state_ids[next_state];

  //         Action shift_action;
  //         shift_action.type = Action::SHIFT;
  //         shift_action.next_state = next_state_id;
  //         table[{i, symbol}] = shift_action;
  //       } else {
  //         // 3.2. Handle reduce and accept actions
  //         if (item.rule_index == 0) {  // Augmented start rule
  //           Symbol end_of_input;
  //           end_of_input.type = Symbol::Type::ENDOF;
  //           Action accept_action;
  //           accept_action.type = Action::ACCEPT;
  //           table[{i, end_of_input}] = accept_action;
  //         } else {
  //           // This is where you'd implement SLR(1) logic:
  //           // - Calculate FOLLOW(rule.lhs)
  //           // - Add reduce actions only for terminals in FOLLOW(rule.lhs)

  //           // For now, we're implementing LR(0): reduce on all terminals.
  //           for (const Symbol& terminal : allTerminals(rules)) {
  //             Action reduce_action;
  //             reduce_action.type = Action::REDUCE;
  //             reduce_action.rule_index = item.rule_index;
  //             table[{i, terminal}] = reduce_action;
  //           }
  //           // Add reduce for ENDOF as well for LR(0)
  //           Symbol end_of_input;
  //           end_of_input.type = Symbol::Type::ENDOF;
  //           Action reduce_action;
  //           reduce_action.type = Action::REDUCE;
  //           reduce_action.rule_index = item.rule_index;
  //           table[{i, end_of_input}] = reduce_action;
  //         }
  //       }
  //     }
  //   }
  // }

  // void ActionTableBuilder::generateActionTable(
  //     const std::vector<RuleNew>& rules, ActionTable& table) {
  //   auto states = buildStates(rules);
  //   std::unordered_map<ItemSet, uint32_t, ItemSetHasher, ItemSetEqual>
  //       state_ids;
  //   for (size_t i = 0; i < states.size(); ++i) state_ids[states[i]] = i;

  //   for (size_t i = 0; i < states.size(); ++i) {
  //     const auto& items = states[i];

  //     for (const auto& item : items) {
  //       const RuleNew& rule = rules[item.rule_index];

  //       if (item.dot_position < rule.rhs.size()) {
  //         Symbol sym = rule.rhs[item.dot_position];
  //         auto next = goTo(items, sym, rules);
  //         uint32_t next_id = state_ids[next];

  //         Action shift_action;
  //         shift_action.type = Action::SHIFT;
  //         shift_action.next_state = next_id;
  //         table[{i, sym}] = shift_action;
  //       } else {
  //         if (item.rule_index == 0) {
  //           Symbol end;
  //           end.type = Symbol::Type::ENDOF;
  //           table[{i, end}] = Action{Action::ACCEPT};
  //         } else {
  //           // For all terminals, add a REDUCE (can be SLR(1) optimized)
  //           for (const Symbol& terminal : allTerminals(rules)) {
  //             Action reduce_action;
  //             reduce_action.type = Action::REDUCE;
  //             reduce_action.rule_index = item.rule_index;
  //             table[{i, terminal}] = reduce_action;
  //           }
  //         }
  //       }
  //     }
  //   }
  // }

}  // namespace compiler
