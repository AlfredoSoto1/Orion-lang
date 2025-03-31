#pragma once

#include <memory>
#include <vector>

#include "Tokens.hpp"

namespace compiler {

  enum class Grammar : uint64_t {
    UNKNOWN = 0,
    PUNC,
    EXPR,
    FACTOR,
    LITERAL,
    OPERATOR,
    TERM,
    IDENT,
    LPAREN,
    RPAREN,
    KEYWORD,

    // DECLARATION,
    // FUNCTION,
    // PARAMETER,
    // BLOCK,
    // STATEMENT,
    // EXPRESSION,
    // TERM,
    // IDENTIFIER,
    // LITERAL,
    // OPERATOR,
    // UNFINISHED,

    // UNKNOWN,
    // KEYWORD,
    // LITERAL,
    // IDENTIFIER,
    // PUNCTUATOR,
    // COMMENT,
    // ENDOF,
  };

  /**
   * @brief
   *
   */
  struct ASTNode {
    Grammar grammar;
    std::variant<Token, std::vector<ASTNode>> value;
  };

  /**
   * @brief
   *
   */
  struct ASTProgram {
    uint64_t node_count;
    std::unique_ptr<ASTNode> program;
  };

}  // namespace compiler
