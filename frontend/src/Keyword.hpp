#pragma once

#include <string>

namespace compiler {

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

}  // namespace compiler
