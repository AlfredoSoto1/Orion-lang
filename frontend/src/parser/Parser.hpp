#pragma once

#include <functional>
#include <iostream>
#include <optional>
#include <unordered_map>
#include <variant>

#include "ParserError.hpp"
#include "ast/StorageAST.hpp"
#include "tokens/TokenStream.hpp"

namespace compiler {

  /**
   * @brief A recursive descent parser for a C/C++-like language.
   */
  class Parser final {
  public:
    using ParseResult = std::optional<ParserError>;

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
  };
}  // namespace compiler