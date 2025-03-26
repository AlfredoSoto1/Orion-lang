#pragma once

#include <string_view>

namespace compiler {

  enum class Operator {
    // Arithmetic operators
    ADD,  // +
    SUB,  // -
    MUL,  // *
    DIV,  // /
    MOD,  // %

    // Increment/Decrement operators
    INC,  // ++
    DEC,  // --

    // Assignment operators
    ASSIGN,  // =
    ADD_EQ,  // +=
    SUB_EQ,  // -=
    MUL_EQ,  // *=
    DIV_EQ,  // /=
    MOD_EQ,  // %=

    // Bitwise operators
    BAND,    // &
    BOR,     // |
    BXOR,    // ^
    BNOT,    // ~
    LSHIFT,  // <<
    RSHIFT,  // >>

    // Bitwise assignment operators
    LSHIFT_EQ,  // <<=
    RSHIFT_EQ,  // >>=
    AND_EQ,     // &=
    OR_EQ,      // |=
    XOR_EQ,     // ^=
    NOT_EQ,     // ~=

    // Logical operators
    AND,  // &&
    OR,   // ||
    NOT,  // !

    // Comparison operators
    EQ,   // ==
    NEQ,  // !=
    LT,   // <
    GT,   // >
    LTE,  // <=
    GTE,  // >=

    // Other operators
    TERNARY,         // ?
    LARROW,          // <-
    RARROW,          // ->
    REFERENCE,       // &
    EXPR_REFERENCE,  // &&
    PTR,             // *
    PTR_DEREF,       // *

    UNKNOWN,
  };

  class OperatorHandler {
  public:
    /**
     * @brief Converts a string to an Operator.
     *
     * @param str
     * @return * Operator
     */
    static Operator from(std::string_view str);

    static bool isUnary(Operator op);
    static bool isBinary(Operator op);
    static bool isTernary(Operator op);
    static bool isAssignment(Operator op);
    static bool isLogical(Operator op);
    static bool isBitwise(Operator op);
    static bool isComparison(Operator op);
    static bool isIncrementDecrement(Operator op);
  };
}  // namespace compiler