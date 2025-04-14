#pragma once

#include <expected>
#include <functional>
#include <unordered_map>

#include "StorageAST.hpp"
#include "TokenStream.hpp"

namespace compiler {

  /**
   * @brief A recursive descent parser for a C/C++-like language.
   */
  class Parser final {
  public:
    // using ParseResult = std::expected<ParserError, ParserError>;

  public:
    /**
     * @brief Construct a new Parser object.
     *
     * @param tokens
     */
    explicit Parser(TokenStream& tokens) noexcept;
    ~Parser() noexcept = default;

    /**
     * @brief Parses the input stream and builds the AST.
     *
     */
    void parse();

  private:
    using ReductionHandler = std::function<Symbol(Parser&)>;

    TokenStream& tokens;
    // ASTStack ast_stack;
    // ASTArena ast_arena;

    std::vector<Symbol> reduction_stack;
    std::unordered_map<Rule, ReductionHandler, RuleHash, RuleEqual> rt_handlers;

  private:
    void shift();
    bool reduce();

  private:
    Symbol tokenToSymbol(const Token& token) const noexcept;

    Symbol makeId() const noexcept;
    Symbol makeLt() const noexcept;
    Symbol makeKw(Keyword k) const noexcept;
    Symbol makePn(Punctuator p) const noexcept;
    Symbol makeNt(NonTerminal nt) const noexcept;

    Rule makeRule(std::initializer_list<Symbol> syms) const noexcept;

    void populateRuleTable() noexcept;

  private:
    enum class ActionType { SHIFT, REDUCE, ACCEPT, ERROR };

    /**
     * @brief For SHIFT, 'next' is the next state.
     *        For REDUCE, 'next' is the index into the grammar rules.
     *
     */
    struct Action {
      ActionType type;
      Index next;
    };

    /**
     * @brief Index into ASTStorage (the data-oriented AST you built)
     *
     */
    struct StackEntry {
      // Symbol symbol;
      uint32_t state;
      Index ast_index;
    };
  };
}  // namespace compiler