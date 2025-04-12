#pragma once

#include <expected>
#include <memory>
#include <stack>
#include <unordered_map>

#include "ASTArena.hpp"
#include "ASTRules.hpp"
#include "ASTStack.hpp"
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
    enum class Action { SHIFT, REDUCE, ACCEPT, ERROR };

  private:
    TokenStream& tokens;
    ASTStack ast_stack;
    ASTArena ast_arena;

  private:
    void shift();
    bool tryReduce();

  private:
    void parse_ID();
    void parse_LITERAL();
  };
}  // namespace compiler