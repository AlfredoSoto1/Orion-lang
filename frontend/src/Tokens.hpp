#pragma once

#include <optional>
#include <variant>

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
  };

  enum class Keyword {
    // Control flow & loops
    IF,
    ELSE,
    FOR,
    DO,
    WHILE,
    SWITCH,
    CASE,
    BREAK,
    CONTINUE,
    DEFAULT,
    INSTANCEOF,
    WHEN,

    // Storage specifiers
    STATIC,
    CONST,
    MUTABLE,
    FINAL,
    VOLATILE,
    EXTERN,
    IMPORT,
    EXPORT,
    MODULE,
    PACKAGE,
    USING,
    NAMESPACE,
    TYPEDEF,
    AUTO,

    // Function specifiers
    INLINE,
    NOINLINE,
    NORETURN,
    NOEXCEPT,
    PURE,
    CONSTEVAL,
    CONSTINIT,
    EXPLICIT,
    IMPLICIT,
    CONSTEXPR,
    VIRTUAL,
    OVERRIDE,
    FINALIZE,
    ABSRACT,
    RETURN,

    // Memory management
    NEW,
    DELETE,
    DELETE_ARR,
    NEW_ARR,

    // Type specifiers
    CLASS,
    STRUCT,
    ENUM,
    UNION,
    INTERFACE,
    TRAIT,
    IMPLEMENTS,
    EXTENDS,
    SUPER,
    THIS,
    SELF,
    SIZEOF,
    TYPEOF,
    ALIGNOF,
    TYPEID,
    DECLTYPE,
    STATIC_ASSERT,
    USING_NAMESPACE,
    USING_TYPE,
    USING_ALIAS,
    FRIEND,
    OPERATOR,
    AS,
    IS,
    IN,
    OUT,
    REF,
    VAL,
    VAR,
    LET,
    CONSTEXPR,
    CONSTINIT,
    CONSTCAST,
    DYNAMICCAST,
    STATICCAST,
    REINTERPRETCAST,
    TYPECAST,
    TYPEID,
    DECLTYPE,
    SIZEOF,
    ALIGNOF,
    NOEXCEPT,
    THROW,
    TRY,
    CATCH,
    FINALLY,
    THROW,
    VOID,
    BOOL,
    CHAR,
    NULL_OBJ,
    NULLPTR,
    TRUE,
    FALSE,
    INT,
    FLOAT,
    DOUBLE,
    LONG,
    SHORT,
    UNSIGNED,
    SIGNED,
    CONST,
    VOLATILE,
    MUTABLE,
    EXPLICIT,
    IMPLICIT,
    PUBLIC,
    PRIVATE,
    PROTECTED,
    FRIEND,
    STATIC,
    VIRTUAL,
    OVERRIDE,
    FINAL,
    ABSTRACT,
    OPERATOR,
    USING,
    NAMESPACE,
    TYPEDEF,
    AUTO,
    INLINE,
    NOINLINE,
    NORETURN,
    NOEXCEPT,
    UNDEFINED,
  };

  enum class ArithOperator {
    PLUS,   // +
    MINUS,  // -
    STAR,   // *
    SLASH,  // /
    MOD,    // %
    INC,    // ++
    DEC,    // --
  };

  enum class AssignOperator {
    ASSIGN,    // =
    PLUS_EQ,   // +=
    MINUS_EQ,  // -=
    STAR_EQ,   // *=
    SLASH_EQ,  // /=
    MOD_EQ,    // %=
    XOR,       // ^=
    AND_EQ,    // &=
    OR_EQ,     // |=
    NOT_EQ,    // ~=
  };

  enum class LogicOperator {
    EQ,          // ==
    NOT_EQ,      // !=
    LESS,        // <
    GREATER,     // >
    LESS_EQ,     // <=
    GREATER_EQ,  // >=
    AND,         // &&
    OR,          // ||
    NOT,         // !
  };

  enum class BitwiseOperator {
    NOT,        // ~
    AMP,        // &
    OR,         // |
    XOR,        // ^
    LSHIFT,     // <<
    RSHIFT,     // >>
    LSHIFT_EQ,  // <<=
    RSHIFT_EQ,  // >>=
  };

  /**
   * @brief Represents an identifier in the source code.
   *
   */
  struct Identifier {
    uint64_t uid;
    std::string name;
  };

  enum class LiteralType {
    INTEGER,
    FLOAT,
    CHAR,
    STRING,
  };

  using LiteralValue = std::variant<char,    // Single Byte character
                                    double,  // Double precision floating point
                                    uint64_t,      // Unsigned 64-bit integer
                                    int64_t,       // Signed 64-bit integer
                                    std::string>;  // String

  struct Literal {
    LiteralType type;
    std::variant<LiteralValue> value;
  };

  using TokenValue = std::variant<Keyword,           // Keyword
                                  Literal,           // Literal
                                  Identifier,        // Identifier
                                  Punctuator,        // Punctuator
                                  ArithOperator,     // ArithOperator
                                  AssignOperator,    // AssignOperator
                                  LogicOperator,     // LogicOperator
                                  BitwiseOperator>;  // BitwiseOperator

  enum class TokenType {
    FIRST,
    KEYWORD,
    LITERAL,
    IDENTIFIER,
    PUNCTUATOR,
    OPERATOR,
    COMMENT,
    ENDOF,
    UNKNOWN,
  };

  /**
   * @brief Represents a token in the source code.
   *
   */
  struct Token {
    TokenType type;
    std::optional<TokenValue> value;
  };

  struct MetaToken {
    Token* token;
    size_t line;
    size_t column;
  };
}  // namespace compiler