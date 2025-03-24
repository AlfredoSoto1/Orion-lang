#pragma once

#include <string>

namespace compiler {

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
    static std::string_view from(Keyword kw);

    /**
     * @brief Checks if a keyword is a control flow keyword.
     *
     * @param kw
     * @return true
     * @return false
     */
    static bool isControlFlow(Keyword kw);

    /**
     * @brief Checks if a keyword is a storage specifier.
     *
     * @param kw
     * @return true
     * @return false
     */
    static bool isStorageSpecifier(Keyword kw);

    /**
     * @brief Checks if a keyword is a function specifier.
     *
     * @param kw
     * @return true
     * @return false
     */
    static bool isFunctionSpecifier(Keyword kw);

    /**
     * @brief Checks if a keyword is a memory management keyword.
     *
     * @param kw
     * @return true
     * @return false
     */
    static bool isMemoryManagement(Keyword kw);

    /**
     * @brief Checks if a keyword is a type specifier.
     *
     * @param kw
     * @return true
     * @return false
     */
    static bool isTypeSpecifier(Keyword kw);
  };

}  // namespace compiler
