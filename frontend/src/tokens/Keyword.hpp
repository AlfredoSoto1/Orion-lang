#pragma once

#include <string_view>

namespace compiler {

  enum class Keyword : uint8_t {
    UNKNOWN = 0,
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
    WHEN,  // Case of cool

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
    CONSTCAST,
    DYNAMICCAST,
    STATICCAST,
    REINTERPRETCAST,
    TYPECAST,
    THROW,
    TRY,
    CATCH,
    FINALLY,
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
    PUBLIC,
    PRIVATE,
    PROTECTED,
    ABSTRACT,
    UNDEFINED,
  };

  class KeywordHandler {
  public:
    /**
     * @brief Converts a string to a Keyword.
     *
     * @param lexeme
     * @return Keyword
     */
    static Keyword from(std::string_view lexeme);

    /**
     * @brief Converts a keyword to a string.
     *
     * @param keyword
     * @return str
     */
    static std::string_view to_string(Keyword kw);
  };

}  // namespace compiler
