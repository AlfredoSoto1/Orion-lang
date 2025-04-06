#pragma once

#include <cstdint>
#include <string_view>

namespace compiler {

  enum class Punctuator : uint8_t {
    UNKNOWN = 0,
    AT,              // @
    HASH,            // #
    DOLLAR,          // $
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
    LARROW,          // <-
    RARROW,          // ->
    LINE_COMMENT,    // //
    LBLOCK_COMMENT,  // /*
    RBLOCK_COMMENT,  // */
    UNDER_SCORE,     // _
    PLUS,            // +
    DASH,            // -
    STAR,            // *
    SLASH,           // /
    MOD,             // %
    PLUS_PLUS,       // ++
    DASH_DASH,       // --
    EQ,              // =
    PLUS_EQ,         // +=
    DASH_EQ,         // -=
    STAR_EQ,         // *=
    SLASH_EQ,        // /=
    MOD_EQ,          // %=
    BAND,            // &
    BOR,             // |
    BXOR,            // ^
    BNOT,            // ~
    LSHIFT,          // <<
    RSHIFT,          // >>
    LSHIFT_EQ,       // <<=
    RSHIFT_EQ,       // >>=
    AND_EQ,          // &=
    OR_EQ,           // |=
    XOR_EQ,          // ^=
    NOT_EQ,          // ~=
    AND,             // &&
    OR,              // ||
    NOT,             // !
    EQ_EQ,           // ==
    NEQ,             // !=
    LT,              // <
    GT,              // >
    LTE,             // <=
    GTE,             // >=
    QUESTION,        // ?
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

    /**
     * @brief Converts a Punctuator to a string.
     *
     * @param Punctuator
     * @return str
     */
    static std::string_view from(Punctuator punc);
  };
}  // namespace compiler
