#pragma once

#include <functional>
#include <iostream>
#include <optional>
#include <unordered_map>

#include "ParserError.hpp"
#include "ast/CFGrammar.hpp"
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
    explicit Parser(TokenStream& tokens, const CFGrammar& grammar) noexcept;
    ~Parser() noexcept = default;

    /**
     * @brief Parses the input stream and builds the AST.
     *
     */
    void parse();

  private:
    friend class CFGrammar;

  private:
    TokenStream& tokens;
    const CFGrammar& grammar;
    std::vector<Symbol> parse_stack;

  private:
    void shift();
    bool reduce();

    Symbol tokenToSymbol(const Token& token) const noexcept;
  };
}  // namespace compiler