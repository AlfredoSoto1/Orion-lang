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

    /**
     * @brief
     *
     */
    struct SymbolHash {
      std::size_t operator()(const Symbol& s) const {
        std::size_t h = std::hash<uint8_t>{}(static_cast<uint8_t>(s.type));
        switch (s.type) {
          case Symbol::Type::NON_TERMINAL:
          case Symbol::Type::KEYWORD:
          case Symbol::Type::PUNCTUATOR:
          case Symbol::Type::IDENTIFIER:
          case Symbol::Type::LITERAL:
            h ^= std::hash<uint8_t>{}(static_cast<uint8_t>(s.nonterminal)) << 1;
        }
        return h;
      }
    };

    struct StateSymbolHash {
      std::size_t operator()(const std::pair<size_t, Symbol>& p) const {
        std::size_t h1 = std::hash<size_t>{}(p.first);
        std::size_t h2 = SymbolHash{}(p.second);
        return h1 ^ (h2 << 1);
      }
    };

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

  public:
    const Grammar& grammar;

    using ItemSet = std::unordered_set<Item, ItemHasher>;
    using StateSymbol = std::pair<size_t, Symbol>;
    using Table = std::unordered_map<StateSymbol, size_t, StateSymbolHash>;
    using ActionTable =
        std::unordered_map<StateSymbol, Action, StateSymbolHash>;

    ActionTable action_table;

  public:
    ItemSet closure(const ItemSet& items);
    ItemSet goTo(const ItemSet& items, const Symbol& symbol);

    void buildStates(std::vector<ItemSet>& states, Table& transitions);
    void buildTables(std::vector<ItemSet>& states, Table& transitions);
  };
}  // namespace compiler
