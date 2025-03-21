#pragma once

#include <string>

namespace compiler {

  class PunctuatorHandler {
  public:
    /**
     * @brief Converts a string to a Punctuator.
     *
     * @param str
     * @return Punctuator
     */
    static Punctuator fromString(const std::string& str);
  };

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
  };
}  // namespace compiler
