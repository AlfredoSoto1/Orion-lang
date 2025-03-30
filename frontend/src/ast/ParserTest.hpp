#pragma once

#include <expected>
#include <stack>
#include <string_view>
#include <vector>

#include "TokenStream.hpp"

// TEMP
struct ParserError {};

namespace compiler {

  enum class GrammarType {
    DECLARATION,
    FUNCTION,
    PARAMETER,
    BLOCK,
    STATEMENT,
    EXPRESSION,
    TERM,
    IDENTIFIER,
    LITERAL,
    OPERATOR,
    UNFINISHED,

    UNKNOWN,
    KEYWORD,
    LITERAL,
    IDENTIFIER,
    PUNCTUATOR,
    COMMENT,
    ENDOF,
  };

  struct ASTNode {
    GrammarType type;
    std::variant<Token, std::vector<ASTNode>> value;
    // You can store additional info like the line and column here if needed.
  };

  /**
   * @brief A recursive descent parser for a C/C++-like language.
   */
  class Parser final {
  public:
    // using ParseResult = std::expected<ParserError, ParserError>;

  public:
    explicit Parser(TokenStream& tokens) noexcept;

    void parse();

  private:
    TokenStream& tokens;
    std::stack<ASTNode> ast_stack;

  private:
    bool reduce();
    bool isReducible();
    void applyReduction();
  };
}  // namespace compiler

/*
Program:
    Program → Function* | Declaration* | Program Function | Program Declaration

Declaration:
    Declaration → Type Identifier ;
    Declaration → Type Identifier = Expression [, Expression]* ;

Function:
    Function → Type Identifier ( Parameters ) { Block }

Parameters:
    Parameters → Type Identifier | Parameters , Type Identifier | ε

Block:
    Block → Statement | Block Statement

Statement:
    Statement → Declaration
    Statement → Expression ;
    Statement → if ( Expression ) Statement else Statement
    Statement → while ( Expression ) Statement
    Statement → for ( Expression ; Expression ; Expression ) Statement
    Statement → return Expression ;

Expression:
    Expression → Expression + Term
    Expression → Expression - Term
    Expression → Term

Term:
    Term → Term * Factor
    Term → Term / Factor
    Term → Factor

Factor:
    Factor → Identifier
    Factor → Number
    Factor → ( Expression )

*/