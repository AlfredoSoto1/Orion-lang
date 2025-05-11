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

    static Action reduce(uint32_t rule_index) {
      return Action{.type = REDUCE, .rule_index = rule_index};
    }

    static Action shift(uint32_t next_state) {
      return Action{.type = SHIFT, .next_state = next_state};
    }

    static Action accept() { return Action{.type = ACCEPT}; }
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
        return std::hash<size_t>()(item.rule_index) ^
               (std::hash<size_t>()(item.dot_position) << 1);
      }
    };

  public:
    const Grammar& grammar;

    using State = size_t;
    using ItemSet = std::unordered_set<Item, ItemHasher>;

    struct ItemSetHasher {
      size_t operator()(const ItemSet& set) const {
        size_t hash = set.size();
        for (const Item& element : set) {
          hash ^= ItemHasher{}(element);
        }
        return hash;
      }
    };

    using SymbolSet = std::unordered_set<Symbol, SymbolHash>;
    using ItemSetState = std::unordered_map<ItemSet, State, ItemSetHasher>;

    using StateSymbol = std::pair<State, Symbol>;
    using Table = std::unordered_map<StateSymbol, State, StateSymbolHash>;
    using ATable = std::unordered_map<StateSymbol, Action, StateSymbolHash>;
    using GTable = std::unordered_map<StateSymbol, State, StateSymbolHash>;

    GTable goto_table;
    ATable action_table;
    SymbolSet terminals;

    Table transitions;
    std::vector<ItemSet> states;

  public:
    ItemSet closure(const ItemSet& items);
    ItemSet goTo(const ItemSet& items, const Symbol& symbol);

    void obtainAllTerminals();

    void buildStates(std::vector<ItemSet>& states, Table& transitions);
    void buildTables(std::vector<ItemSet>& states, Table& transitions);
  };
}  // namespace compiler
