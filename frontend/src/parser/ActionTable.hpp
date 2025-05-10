#pragma once

#include <unordered_map>
#include <unordered_set>

#include "Symbols.hpp"

namespace compiler {

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

  /**
   * @brief
   *
   */
  class ActionTable final {
  public:
    explicit ActionTable(const Grammar& grammar) noexcept;

    /**
     * @brief Builds the action table
     *
     */
    void build();

    /**
     * @brief
     *
     */
    void from();

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

    using State = size_t;
    using ItemSet = std::unordered_set<Item, ItemHasher>;

    using StateSymbol = std::pair<State, Symbol>;
    using Table = std::unordered_map<StateSymbol, State, StateSymbolHash>;
    using ATable = std::unordered_map<StateSymbol, Action, StateSymbolHash>;

    ATable action_table;

  public:
    ItemSet closure(const ItemSet& items);
    ItemSet goTo(const ItemSet& items, const Symbol& symbol);

    void buildStates(std::vector<ItemSet>& states, Table& transitions);
    void buildTables(std::vector<ItemSet>& states, Table& transitions);
  };
}  // namespace compiler
