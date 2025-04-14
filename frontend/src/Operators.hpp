// #pragma once

// #include <string_view>

// namespace compiler {

//   enum class Operator : uint8_t {
//     UNKNOWN = 0,

//     // Arithmetic operators
//     ADD,  // +
//     SUB,  // -
//     MUL,  // *
//     DIV,  // /
//     MOD,  // %

//     // Increment/Decrement operators
//     INC,  // ++
//     DEC,  // --

//     // Assignment operators
//     ASSIGN,  // =
//     ADD_EQ,  // +=
//     SUB_EQ,  // -=
//     MUL_EQ,  // *=
//     DIV_EQ,  // /=
//     MOD_EQ,  // %=

//     // Bitwise operators
//     BAND,    // &
//     BOR,     // |
//     BXOR,    // ^
//     BNOT,    // ~
//     LSHIFT,  // <<
//     RSHIFT,  // >>

//     // Bitwise assignment operators
//     LSHIFT_EQ,  // <<=
//     RSHIFT_EQ,  // >>=
//     AND_EQ,     // &=
//     OR_EQ,      // |=
//     XOR_EQ,     // ^=
//     NOT_EQ,     // ~=

//     // Logical operators
//     AND,  // &&
//     OR,   // ||
//     NOT,  // !

//     // Comparison operators
//     EQ,   // ==
//     NEQ,  // !=
//     LT,   // <
//     GT,   // >
//     LTE,  // <=
//     GTE,  // >=

//     // Other operators
//     TERNARY,         // ?
//     LARROW,          // <-
//     RARROW,          // ->
//     REFERENCE,       // &
//     EXPR_REFERENCE,  // &&
//     PTR,             // *
//     PTR_DEREF,       // *
//   };

//   class OperatorHandler {
//   public:
//     /**
//      * @brief Converts a string to an Operator.
//      *
//      * @param str
//      * @return * Operator
//      */
//     static Operator from(std::string_view str);

//     /**
//      * @brief Converts an operator to a string_view
//      *
//      * @param op
//      * @return std::string_view
//      */
//     static std::string_view to_string(Operator op);
//   };
// }  // namespace compiler