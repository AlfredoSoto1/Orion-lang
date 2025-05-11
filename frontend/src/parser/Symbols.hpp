#pragma once

#include <vector>

#include "tokens/Keyword.hpp"
#include "tokens/Operators.hpp"
#include "tokens/Punctuator.hpp"
#include "tokens/Tokens.hpp"

namespace compiler {

  /**
   * @brief Represents a non-terminal symbol in the RHS of a grammar rule.
   *
   *        These are used during parsing to identify abstract syntactic
   *        categories like expressions, terms, or factors. They help the parser
   *        decide how to transition between states in the parsing automaton.
   */
  enum class NonTerminal : uint8_t {
    UNKNOWN = 0,

    // START → EXPR

    // EXPR → EXPR + TERM
    //      | TERM

    // TERM → TERM * FACT
    //      | FACT

    // FACT → ( EXPR )
    //      | number

    START,
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
   * @brief Represents a terminal symbol, which is a compact form of a token.
   *
   *        This is used for grammar rule definitions and comparisons during
   *        parsing. It abstracts tokens like keywords, identifiers, literals,
   *        punctuators, and the end-of-file marker.
   */
  union Terminal {
    uint8_t eof;            // End of file
    Keyword keyword;        // Keyword
    uint8_t literal;        // Literal
    uint8_t identifier;     // Identifier
    Punctuator punctuator;  // Punctuator
  };

  /**
   * @brief Represents a grammar symbol, which can be a terminal or
   *        non-terminal.
   *
   *        This is used in the right-hand side of grammar rules and within
   *        parsing states. The `type` field indicates whether the symbol is a
   *        terminal or non-terminal. The union holds the corresponding value.
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
   * @brief Represents a single production rule in a context-free grammar.
   *
   *        Each rule has a left-hand side (a non-terminal) and a right-hand
   *        side, which is a sequence of symbols (terminals or non-terminals).
   */
  struct Rule {
    NonTerminal lhs;
    std::initializer_list<Symbol> rhs;
  };

  /**
   * @brief A collection of grammar rules representing a full grammar.
   *
   */
  using Grammar = std::vector<Rule>;

  /**
   * @brief Hash function for Symbol, allowing use in hash-based containers.
   *
   */
  struct SymbolHash {
    std::size_t operator()(const Symbol& s) const {
      return std::hash<uint8_t>()((uint8_t)s.type) ^
             (std::hash<uint8_t>()(s.comparison) << 1);
    }
  };

  /**
   * @brief Hash function for a (state, symbol) pair.
   *
   *        Useful for indexing transitions in parsing tables like ACTION or
   *        GOTO.
   */
  struct StateSymbolHash {
    std::size_t operator()(const std::pair<size_t, Symbol>& p) const {
      return std::hash<size_t>()(p.first) ^
             (std::hash<size_t>()(SymbolHash{}(p.second)) << 1);
    }
  };
}  // namespace compiler