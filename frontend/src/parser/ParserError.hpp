#pragma once

#include <optional>
#include <string_view>

#include "lexer/LexerError.hpp"

namespace compiler {

  enum class ParserErrorType {
    // General
    UNKNOWN_ERROR,   // Catch-all for unexpected issues
    UNKNOWN_SYMBOL,  // Unknown symbol from token
    LEXER_ERROR,     // Lexer Error when reading token

    // // Syntax-related
    // UNKNOWN_SYMBOL,          // Unknown symbol passed by lexer
    // UNEXPECTED_SYMBOL,       // Symbol not expected in current context
    // UNEXPECTED_NONTERMINAL,  // Found a non-terminal where it shouldn't be
    // UNEXPECTED_TERMINAL,     // Found a terminal where it shouldn't be
    // MISSING_SEMICOLON,       // Expected semicolon not found
    // MISSING_PARENTHESIS,     // Expected '(' or ')' not found
    // MISSING_BRACE,           // Expected '{' or '}' not found

    // // Grammar issues
    // AMBIGUOUS_GRAMMAR,          // Multiple possible reductions (not handled)
    // NO_VALID_REDUCTION,         // Could not reduce symbol sequence into
    // grammar
    // EMPTY_RULE_NOT_ALLOWED,     // Empty rule used in invalid context
    // INCOMPLETE_TREE_REDUCTION,  // Reduction tree did not converged into a
    //                             // single node

    // // Expression-specific
    // INVALID_EXPRESSION,         // Expression is malformed
    // INCOMPLETE_EXPRESSION,      // Expression was incomplete
    // OPERATOR_WITHOUT_OPERANDS,  // Binary operator without valid operands

    // // AST errors
    // AST_CONSTRUCTION_FAILED,  // Couldn't construct AST node for valid rule
    // AST_INDEX_OUT_OF_BOUNDS,  // Index into AST storage is invalid
    // AST_INVALID_NODE_TYPE,    // Unexpected AST node type encountered

    // // Control flow
    // INVALID_IF_SYNTAX,             // if-statement structure is invalid
    // INVALID_RETURN_SYNTAX,         // return-statement structure is invalid
    // INVALID_FUNCTION_DECLARATION,  // Malformed function declaration
    // INVALID_PARAMETER_LIST,        // Parameter list structure is broken

    // // Type-specific
    // INVALID_TYPE_DECLARATION,  // Type usage/declaration is invalid
    // TYPE_EXPECTED,             // Expected a type but something else was
    // found

    // // Internal parser engine
    // REDUCTION_FAILED,  // Handler failed to produce a result
  };

  class ParserError final {
  public:
    ParserErrorType type;
    std::optional<LexerError> lexer_error;

  public:
    /**
     * @brief Parses the error type into a string literal
     *
     * @return std::string
     */
    std::string_view to_string();
  };

}  // namespace compiler
