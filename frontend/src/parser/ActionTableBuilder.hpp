#pragma once

#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "ast/StorageAST.hpp"

namespace compiler {

  /**
   * @brief
   *
   */
  struct RuleN {
    NonTerminal lhs;
    std::initializer_list<Symbol> rhs;
  };

  /**
   * @brief
   *
   */
  using Grammar = std::vector<RuleN>;

  /**
   * @brief
   *
   */
  class ActionTableBuilder final {
  public:
    explicit ActionTableBuilder(const Grammar& grammar) noexcept;

    /**
     * @brief Builds the action table
     *
     */
    void build();

  public:
    /**
     * @brief
     *
     */
    struct Item {
      size_t rule_index;
      size_t dot_position;

      bool operator==(const Item& other) const {
        return rule_index == other.rule_index &&
               dot_position == other.dot_position;
      }
    };

    /**
     * @brief
     *
     */
    struct ItemHasher {
      size_t operator()(const Item& item) const {
        return std::hash<uint32_t>()(item.rule_index) ^
               (std::hash<uint32_t>()(item.dot_position) << 1);
      }
    };

  public:
    const Grammar& grammar;
    using ItemSet = std::unordered_set<Item, ItemHasher>;

  public:
    ItemSet closure(const ItemSet& items);
    ItemSet goTo(const ItemSet& items, const Symbol& symbol);

    // /**
    //  * @brief
    //  *
    //  */
    // struct ItemHasher {
    //   size_t operator()(const Item& item) const {
    //     return std::hash<uint32_t>()(item.rule_index) ^
    //            (std::hash<uint32_t>()(item.dot_position) << 1);
    //   }
    // };

    // /**
    //  * @brief
    //  *
    //  */
    // struct ItemEqual {
    //   bool operator()(const Item& lhs, const Item& rhs) const {
    //     return lhs.rule_index == rhs.rule_index &&
    //            lhs.dot_position == rhs.dot_position;
    //   }
    // };

    // struct SymbolHash {
    //   std::size_t operator()(const Symbol& s) const {
    //     std::size_t h = std::hash<uint8_t>{}(static_cast<uint8_t>(s.type));
    //     switch (s.type) {
    //       case Symbol::Type::NON_TERMINAL:
    //         h ^= std::hash<uint8_t>{}(static_cast<uint8_t>(s.nonterminal)) <<
    //         1; break;
    //       case Symbol::Type::KEYWORD:
    //       case Symbol::Type::PUNCTUATOR:
    //       case Symbol::Type::IDENTIFIER:
    //       case Symbol::Type::LITERAL:
    //         h ^= std::hash<uint8_t>{}(s.terminal.ident_or_lit) << 1;
    //         break;
    //       default:
    //         break;
    //     }
    //     return h;
    //   }
    // };

    // struct SymbolEqual {
    //   bool operator()(const Symbol& a, const Symbol& b) const {
    //     if (a.type != b.type) return false;
    //     switch (a.type) {
    //       case Symbol::Type::NON_TERMINAL:
    //         return a.nonterminal == b.nonterminal;
    //       case Symbol::Type::KEYWORD:
    //       case Symbol::Type::PUNCTUATOR:
    //       case Symbol::Type::IDENTIFIER:
    //       case Symbol::Type::LITERAL:
    //         return a.terminal.ident_or_lit == b.terminal.ident_or_lit;
    //       default:
    //         return true;
    //     }
    //   }
    // };

    // struct StateSymbolPairHash {
    //   std::size_t operator()(const std::pair<uint32_t, Symbol>& p) const {
    //     std::size_t h1 = std::hash<uint32_t>{}(p.first);
    //     std::size_t h2 = SymbolHash{}(p.second);
    //     return h1 ^ (h2 << 1);
    //   }
    // };

    // struct StateSymbolPairEqual {
    //   bool operator()(const std::pair<uint32_t, Symbol>& a,
    //                   const std::pair<uint32_t, Symbol>& b) const {
    //     return a.first == b.first && SymbolEqual{}(a.second, b.second);
    //   }
    // };

    /**
     * @brief
     *
     */
    struct Action {
      enum Type { SHIFT, REDUCE, ACCEPT, ERROR } type;
      union {
        uint32_t rule_index;
        uint32_t next_state;
      };
    };

    // using ActionTable =
    //     std::unordered_map<std::pair<uint32_t, Symbol>, Action,
    //                        StateSymbolPairHash, StateSymbolPairEqual>;

    // using ItemSet = std::unordered_set<Item, ItemHasher, ItemEqual>;
    // using SymbolSet = std::unordered_set<Symbol, SymbolHash, SymbolEqual>;

    // struct ItemSetHasher {
    //   size_t operator()(const ActionTableBuilder::ItemSet& items) const {
    //     size_t seed = items.size();
    //     for (const auto& item : items) {
    //       seed ^= std::hash<size_t>{}(item.rule_index) + 0x9e3779b9 +
    //               (seed << 6) + (seed >> 2);
    //       seed ^= std::hash<size_t>{}(item.dot_position) + 0x9e3779b9 +
    //               (seed << 6) + (seed >> 2);
    //     }
    //     return seed;
    //   }
    // };

    // struct ItemSetEqual {
    //   bool operator()(const ActionTableBuilder::ItemSet& a,
    //                   const ActionTableBuilder::ItemSet& b) const {
    //     return a == b;  // Rely on std::set's equality operator
    //   }
    // };

  public:
    // std::string symbolToString(const Symbol& s);

    // ItemSet closure(const ItemSet& items, const std::vector<RuleNew>& rules);

    // ItemSet goTo(const ItemSet& items, const Symbol& sym,
    //              const std::vector<RuleNew>& rules);

    // SymbolSet allSymbols(const std::vector<RuleNew>& rules);

    // SymbolSet allTerminals(const std::vector<RuleNew>& rules);

    // std::vector<ItemSet> buildStates(const std::vector<RuleNew>& rules);

    // void generateActionTable(const std::vector<RuleNew>& rules,
    //                          ActionTable& table);

  public:
    // Helper function to compare ActionTable entries for equality
    // bool actionTableEntriesEqual(const Action& a, const Action& b) {
    //   if (a.type != b.type) return false;
    //   if (a.type == Action::SHIFT) {
    //     return a.next_state == b.next_state;
    //   } else if (a.type == Action::REDUCE) {
    //     return a.rule_index == b.rule_index;
    //   }
    //   // For ACCEPT, no additional data needs to be compared.
    //   return true;
    // }

    // // Helper function to compare ActionTables for equality
    // bool actionTablesEqual(const ActionTable& expected,
    //                        const ActionTable& actual) {
    //   if (expected.size() != actual.size()) {
    //     return false;
    //   }
    //   for (const auto& [key, expected_action] : expected) {
    //     auto actual_it = actual.find(key);
    //     if (actual_it == actual.end()) {
    //       return false;  // Key not found in actual table
    //     }
    //     if (!actionTableEntriesEqual(expected_action, actual_it->second)) {
    //       return false;  // Actions are different
    //     }
    //   }
    //   return true;
    // }

    // void testGenerateActionTable_SimpleGrammar() {
    //   // Define symbols
    //   Symbol IDENTIFIER{Symbol::Type::IDENTIFIER, {}};
    //   Symbol CONSTANT{Symbol::Type::LITERAL, {}};
    //   Symbol EXPR;
    //   EXPR.type = Symbol::Type::NON_TERMINAL;
    //   EXPR.nonterminal = NonTerminal::EXPR;
    //   Symbol PLUS;
    //   PLUS.type = Symbol::Type::PUNCTUATOR;
    //   PLUS.terminal.punctuator = Punctuator::PLUS;
    //   Symbol END_OF_INPUT;
    //   END_OF_INPUT.type = Symbol::Type::ENDOF;

    //   // Define rules (same as the example grammar)
    //   RuleNew rule1{EXPR, {IDENTIFIER}};
    //   RuleNew rule2{EXPR, {CONSTANT}};
    //   RuleNew rule3{EXPR, {EXPR, PLUS, EXPR}};
    //   std::vector<RuleNew> rules = {rule1, rule2, rule3};

    //   // Manually construct the expected ActionTable.  This is CRUCIAL for
    //   // testing!
    //   ActionTable expectedTable;

    //   // Rule 0: S' -> EXPR
    //   //  Assuming states are numbered as in the example.  You'll need to
    //   work
    //   //  this out based on the output of your buildStates() function for
    //   this
    //   //  grammar.
    //   expectedTable[{0, IDENTIFIER}] = {Action::SHIFT, 1};
    //   expectedTable[{0, CONSTANT}] = {Action::SHIFT, 2};
    //   expectedTable[{0, EXPR}] = {Action::SHIFT,
    //                               3};  // Corrected from REDUCE to SHIFT
    //   expectedTable[{0, END_OF_INPUT}] = {
    //       Action::ACCEPT};  // Added accept for augmented rule

    //   expectedTable[{1, PLUS}] = {Action::REDUCE, 0};
    //   expectedTable[{1, END_OF_INPUT}] = {Action::REDUCE, 0};

    //   expectedTable[{2, PLUS}] = {Action::REDUCE, 1};
    //   expectedTable[{2, END_OF_INPUT}] = {Action::REDUCE, 1};

    //   expectedTable[{3, PLUS}] = {Action::SHIFT, 4};
    //   expectedTable[{3, END_OF_INPUT}] = {Action::REDUCE, 2};

    //   expectedTable[{4, IDENTIFIER}] = {Action::SHIFT, 1};
    //   expectedTable[{4, CONSTANT}] = {Action::SHIFT, 2};
    //   expectedTable[{4, EXPR}] = {Action::SHIFT, 5};

    //   expectedTable[{5, PLUS}] = {Action::REDUCE, 2};
    //   expectedTable[{5, END_OF_INPUT}] = {Action::REDUCE, 2};

    //   // Create an ActionTableBuilder and generate the ActionTable
    //   ActionTableBuilder builder;
    //   ActionTable actualTable;
    //   builder.generateActionTable(rules, actualTable);

    //   // Compare the generated table with the expected table
    //   if (actionTablesEqual(expectedTable, actualTable)) {
    //     std::cout << "testGenerateActionTable_SimpleGrammar passed\n";
    //   } else {
    //     std::cout << "testGenerateActionTable_SimpleGrammar failed\n";
    //     std::cout << "Expected Table:\n";
    //     for (const auto& [key, action] : expectedTable) {
    //       std::cout << "State: " << key.first
    //                 << ", Symbol: " << builder.symbolToString(key.second)
    //                 << ", Action: ";
    //       if (action.type == Action::SHIFT)
    //         std::cout << "SHIFT " << action.next_state;
    //       else if (action.type == Action::REDUCE)
    //         std::cout << "REDUCE " << action.rule_index;
    //       else if (action.type == Action::ACCEPT)
    //         std::cout << "ACCEPT";
    //       std::cout << std::endl;
    //     }
    //     std::cout << "Actual Table:\n";
    //     for (const auto& [key, action] : actualTable) {
    //       std::cout << "State: " << key.first
    //                 << ", Symbol: " << builder.symbolToString(key.second)
    //                 << ", Action: ";
    //       if (action.type == Action::SHIFT)
    //         std::cout << "SHIFT " << action.next_state;
    //       else if (action.type == Action::REDUCE)
    //         std::cout << "REDUCE " << action.rule_index;
    //       else if (action.type == Action::ACCEPT)
    //         std::cout << "ACCEPT";
    //       std::cout << std::endl;
    //     }
    //   }
    // }
  };
}  // namespace compiler
