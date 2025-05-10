#pragma once

#include <vector>

#include "tokens/Keyword.hpp"
#include "tokens/Operators.hpp"
#include "tokens/Punctuator.hpp"
#include "tokens/Tokens.hpp"

namespace compiler {

  /**
   * @brief Represents a non-terminal symbol in the RHS of the grammar rule.
   *        This let know the parsing machine to keep moving between states.
   *
   */
  enum class NonTerminal : uint8_t {
    UNKNOWN = 0,

    // Expr    → Expr + Term
    //     | Term

    // Term    → Term * Factor
    //         | Factor

    // Factor  → ( Expr )
    //         | number

    EXPR,
    TERM,
    FACT,

    // https://www.lysator.liu.se/c/ANSI-C-grammar-y.html#direct-declarator
    // EXPR,
    // PRIMARY_EXPR,
    // POSTFIX_EXPR,
    // ARG_EXPR_LIST,
    // ASSIGNMENT_EXPR,
    // UNARY_EXPR,
    // UNARY_OP,
    // CAST_EXPR,
    // TYPE_NAME,
    // MULTIPLICATIVE_EXPR,
    // ADDITIVE_EXPR,
    // SHIFT_EXPR,
    // RELATIONAL_EXPR,
    // EQUALITY_EXPR,
    // AND_EXPR,
    // EXCLUSIVE_OR_EXPR,
    // INCLUSIVE_OR_EXPR,
    // LOGICAL_AND_EXPR,
    // LOGICAL_OR_EXPR,
    // CONDITIONAL_EXPR,
    // ASSIGNMENT_OP,
    // CONSTANT_EXPR,
    // DECLARATION_SPECIFIERS,
    // INIT_DECLARATOR_LIST,
    // DECLARATION,
    // STORAGE_CLASS_SPECIFIER,
    // TYPE_SPECIFIER,
    // TYPE_QUALIFIER,
    // DECL_SPECIFIERS,
    // INIT_DECL,
    // INIT_DECL_LIST,
    // DECLARATOR,
    // INITIALIZER,
    // STRUCT_OR_UNION_SPECIFIER,
    // ENUM_SPECIFIER,
    // STRUCT_OR_UNION,
    // STRUCT_DECLARATION_LIST,
    // STRUCT_DECLARATION,
    // SPECIFIER_QUALIFIER_LIST,
    // STRUCT_DECLARATOR_LIST,
    // STRUCT_DECLARATOR,
    // CONSTANT_EXPRESSION,
    // ENUMERATOR_LIST,
    // ENUMERATOR,
    // TYPE_QUALIFIER_LIST,
    // POINTER,
    // DIRECT_DECLARATOR,
    // PARAMETER_TYPE_LIST,
    // IDENTIFIER_LIST,
    // PARAMETER_LIST,
    // PARAMETER_DECLARATION,
    // ABSTRACT_DECLARATOR,
    // DIRECT_ABSTRACT_DECLARATOR,
    // ASSIGNMENT_EXPRESSION,
    // INITIALIZER_LIST,
    // LABELED_STATEMENT,
    // STATEMENT,
    // COMPOUND_STATEMENT,
    // EXPRESSION_STATEMENT,
    // SELECTION_STATEMENT,
    // ITERATION_STATEMENT,
    // JUMP_STATEMENT,
    // STATEMENT_LIST,
    // DECLARATION_LIST,
    // EXTERNAL_DECLARATION,
    // TRANSLATION_UNIT,
    // FUNCTION_DEFINITION,
  };

  /**
   * @brief Represents a terminal symbol in the RHS of the grammar.
   *        This is a compact version of a token. This is just used to represent
   *        that a token is a terminal but with less information.
   *
   */
  union Terminal {
    uint8_t eof;            // End of file
    Keyword keyword;        // Keyword
    uint8_t literal;        // Literal
    uint8_t identifier;     // Identifier
    Punctuator punctuator;  // Punctuator
  };

  /**
   * @brief Unit representation of a symbol. It can either be a terminal or a
   *        non-terminal.
   *
   */
  struct Symbol {
    enum class Type : uint8_t {
      UNKNOWN = 0,
      KW_TERMINAL,
      ID_TERMINAL,
      LIT_TERMINAL,
      PUN_TERMINAL,
      EOF_TERMINAL,
      NON_TERMINAL,
      COUNT
    } type;
    union {
      Terminal terminal;
      NonTerminal nonterminal;

      // This is used to cast the union into a value for comparison.
      uint8_t comparison;
    };

    bool operator==(const Symbol& other) const {
      if (type != other.type) return false;
      return comparison == other.comparison;
    }
  };

  /**
   * @brief
   *
   */
  struct Rule {
    NonTerminal lhs;
    std::initializer_list<Symbol> rhs;
  };

  /**
   * @brief
   *
   */
  using Grammar = std::vector<Rule>;

  /**
   * @brief
   *
   */
  struct SymbolHash {
    std::size_t operator()(const Symbol& s) const {
      return std::hash<uint8_t>()((uint8_t)s.type) ^
             (std::hash<uint8_t>()(s.comparison) << 1);
    }
  };

  /**
   * @brief
   *
   */
  struct StateSymbolHash {
    std::size_t operator()(const std::pair<size_t, Symbol>& p) const {
      return std::hash<size_t>()(p.first) ^
             (std::hash<size_t>()(SymbolHash{}(p.second)) << 1);
    }
  };
}  // namespace compiler