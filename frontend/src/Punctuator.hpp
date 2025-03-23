#pragma once

#include <string>

namespace compiler {

  enum class Punctuator {
    AT,              // @
    DOT,             // .
    COMMA,           // ,
    COLON,           // :
    SEMI_COLON,      // ;
    LBRACE,          // {
    RBRACE,          // }
    LPAREN,          // (
    RPAREN,          // )
    LBRACKET,        // [
    RBRACKET,        // ]
    QUESTION,        // ?
    LGENERIC,        // <
    RGENERIC,        // >
    LARROW,          // <-
    RARROW,          // =>
    LINE_COMMENT,    // //
    LBLOCK_COMMENT,  // /*
    RBLOCK_COMMENT,  // */
    UNKNOWN,
  };

  class PunctuatorHandler {
  public:
    /**
     * @brief Converts a string to a Punctuator.
     *
     * @param str
     * @return Punctuator
     */
    static Punctuator from(std::string_view str);
  };
}  // namespace compiler
