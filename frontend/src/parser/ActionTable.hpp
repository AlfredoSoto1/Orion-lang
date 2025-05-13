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

    /**
     * @brief Prepares a reduce action
     *
     * @param rule_index
     * @return Action
     */
    static Action reduce(uint32_t rule_index) {
      return Action{.type = REDUCE, .rule_index = rule_index};
    }

    /**
     * @brief Prepares a shift action
     *
     * @param next_state
     * @return Action
     */
    static Action shift(uint32_t next_state) {
      return Action{.type = SHIFT, .next_state = next_state};
    }

    /**
     * @brief Prepares an accept action
     *
     * @return Action
     */
    static Action accept() { return Action{.type = ACCEPT}; }

    /**
     * @brief Prepares an error action
     *
     * @return Action
     */
    static Action error() { return Action{.type = ERROR}; }
  };

  /**
   * @brief
   *
   */
  class ActionTable final {
  public:
    using State = size_t;
    using StateSymbol = std::pair<State, Symbol>;

  public:
    /**
     * @brief Construct a new Action Table object
     *
     * @param grammar
     */
    explicit ActionTable(const Grammar& grammar) noexcept;

    /**
     * @brief Returns the corresponding action from a state symbol.
     *
     * @param state_symbol
     * @return Action
     */
    Action actionFrom(StateSymbol&& state_symbol);

    /**
     * @brief Returns the corresponding state from a state symbol.
     *        Returns (-1) if the state symbol doesnt map to a state.
     *
     * @param state_symbol
     * @return State
     */
    State gotoFrom(StateSymbol&& state_symbol);

  private:
    const Grammar& grammar;

  public:
    struct Item {
      size_t rule_index;
      size_t dot_position;
      bool operator==(const Item& other) const;
    };

    struct ItemHasher {
      size_t operator()(const Item& item) const;
    };

    using ItemSet = std::unordered_set<Item, ItemHasher>;

    struct ItemSetHasher {
      size_t operator()(const ItemSet& set) const;
    };

  public:
    using SymbolSet = std::unordered_set<Symbol, SymbolHash>;
    using ItemSetState = std::unordered_map<ItemSet, State, ItemSetHasher>;

    using Table = std::unordered_map<StateSymbol, State, StateSymbolHash>;
    using ATable = std::unordered_map<StateSymbol, Action, StateSymbolHash>;
    using GTable = std::unordered_map<StateSymbol, State, StateSymbolHash>;

    GTable goto_table;
    ATable action_table;
    SymbolSet terminals;

    // // Just for testing, can be removed once everything works
    Table transitions;
    std::vector<ItemSet> states;

  private:
    void obtainAllTerminals();

    ItemSet closure(const ItemSet& items);
    ItemSet goTo(const ItemSet& items, const Symbol& symbol);

    void buildStates(std::vector<ItemSet>& states, Table& transitions);
    void buildTables(std::vector<ItemSet>& states, Table& transitions);
  };
}  // namespace compiler
