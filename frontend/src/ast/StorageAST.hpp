#pragma once

#include <vector>

#include "tokens/Tokens.hpp"

namespace compiler {

  // program → function
  //
  // function → type IDENTIFIER ( params ) block
  //
  // params → param_list
  //        | ε
  //
  // param_list → param , param_list
  //            | param
  //
  // param → type IDENTIFIER
  //
  // block → { stmt_list }
  //
  // stmt_list → stmt stmt_list
  //           | stmt
  //           | ε
  //
  // stmt → if (expr) block
  //      | expr;
  //      | return expr;
  //
  // expr → expr + expr
  //      | expr - expr
  //      | expr * expr
  //      | expr / expr
  //      | expr % expr
  //      | IDENTIFIER
  //      | LITERAL
  //      | (expr)
  //
  // LITERAL → string
  //         | integer
  //         | floating
  //         | boolean
  //         | char
  //
  // IDENTIFIER → (hash, id)

  using Index = uint32_t;

  // IDENTIFIER → (hash, id)
  struct IDAST {
    uint64_t uid;
    Identifier id;
  };

  // LITERAL → string
  //         | integer
  //         | floating
  //         | boolean
  //         | char
  struct LiteralAST {
    Literal literal;
  };

  struct BinaryExprAST {
    Index left;
    Index right;
    Punctuator op;
  };

  // expr → expr + expr
  //      | expr - expr
  //      | expr * expr
  //      | expr / expr
  //      | expr % expr
  //      | IDENTIFIER
  //      | LITERAL
  //      | (expr)
  struct ExprAST {
    enum class Type : uint8_t {
      ID,
      LITERAL,
      PAREN_EXPR,
      BINARY_EXPR,
    } type;
    Index index;
  };

  // stmt → if (expr) block
  //      | expr;
  //      | return expr;
  struct StmtAST {
    enum class Type : uint8_t {
      IF,
      EXPR,
      RETURN,
    } type;
    Index index;
  };

  struct IfStmtAST {
    Index condition;
    Index block;
  };

  struct ReturnStmtAST {
    Index index;
  };

  // stmt_list → stmt stmt_list
  //           | stmt
  //           | ε
  struct StmtListAST {
    enum class Type : uint8_t {
      EMPTY,
      SINGLE,
      MULTIPLE,
    } type;
    Index stmt;
    Index next;
  };

  // block → { stmt_list }
  struct BlockAST {
    Index stmt_list;
  };

  // param → type IDENTIFIER
  struct ParamAST {
    Index type;
    Index id;
  };

  // param_list → param , param_list
  //            | param
  struct ParamListAST {
    enum class Type : uint8_t {
      LIST,
      PARAM,
    } type;
    Index param;
    Index next;
  };

  // params → param_list
  //        | ε
  struct ParamsAST {
    enum class Type : uint8_t {
      EMPTY,
      PARAMS,
    } type;
    Index index;
  };

  // function → type IDENTIFIER ( params ) block
  struct FunctionAST {
    Index type;
    Index id;
    Index params;
    Index block;
  };

  // program → function
  struct ProgramAST {
    Index function;
  };

  struct ASTStorage {
    std::vector<ProgramAST> programs;
    std::vector<FunctionAST> functions;

    std::vector<IDAST> ids;
    std::vector<LiteralAST> literals;

    std::vector<ExprAST> exprs;
    std::vector<BinaryExprAST> binary_exprs;

    std::vector<StmtAST> stmts;
    std::vector<IfStmtAST> if_stmts;
    std::vector<ReturnStmtAST> return_stmts;

    std::vector<StmtListAST> stmt_lists;
    std::vector<BlockAST> blocks;

    std::vector<ParamAST> params;
    std::vector<ParamListAST> param_lists;
    std::vector<ParamsAST> params_list;
  };

  union NodeAST {
    ProgramAST program;
    FunctionAST function;
    IDAST id;
    LiteralAST literal;
    ExprAST expr;
    BinaryExprAST binary_expr;
    StmtAST stmt;
    IfStmtAST if_stmt;
    ReturnStmtAST return_stmt;
    StmtListAST stmt_list;
    BlockAST block;
    ParamAST param;
    ParamListAST param_list;
    ParamsAST params_list;
  };

  union Terminal {
    Keyword keyword;        // Keyword
    Punctuator punctuator;  // Punctuator
    uint8_t ident_or_lit;   // Identifier or literal
  };

  enum class NonTerminal : uint8_t {
    UNKNOWN = 0,
    // https://www.lysator.liu.se/c/ANSI-C-grammar-y.html#direct-declarator
    EXPR,
    PRIMARY_EXPR,
    POSTFIX_EXPR,
    ARG_EXPR_LIST,
    ASSIGNMENT_EXPR,
    UNARY_EXPR,
    UNARY_OP,
    CAST_EXPR,
    TYPE_NAME,
    MULTIPLICATIVE_EXPR,
    ADDITIVE_EXPR,
    SHIFT_EXPR,
    RELATIONAL_EXPR,
    EQUALITY_EXPR,
    AND_EXPR,
    EXCLUSIVE_OR_EXPR,
    INCLUSIVE_OR_EXPR,
    LOGICAL_AND_EXPR,
    LOGICAL_OR_EXPR,
    CONDITIONAL_EXPR,
    ASSIGNMENT_OP,
    CONSTANT_EXPR,
    DECLARATION_SPECIFIERS,
    INIT_DECLARATOR_LIST,
    DECLARATION,
    STORAGE_CLASS_SPECIFIER,
    TYPE_SPECIFIER,
    TYPE_QUALIFIER,
    DECL_SPECIFIERS,
    INIT_DECL,
    INIT_DECL_LIST,
    DECLARATOR,
    INITIALIZER,
    STRUCT_OR_UNION_SPECIFIER,
    ENUM_SPECIFIER,
    STRUCT_OR_UNION,
    STRUCT_DECLARATION_LIST,
    STRUCT_DECLARATION,
    SPECIFIER_QUALIFIER_LIST,
    STRUCT_DECLARATOR_LIST,
    STRUCT_DECLARATOR,
    CONSTANT_EXPRESSION,
    ENUMERATOR_LIST,
    ENUMERATOR,
    TYPE_QUALIFIER_LIST,
    POINTER,
    DIRECT_DECLARATOR,
    PARAMETER_TYPE_LIST,
    IDENTIFIER_LIST,
    PARAMETER_LIST,
    PARAMETER_DECLARATION,
    ABSTRACT_DECLARATOR,
    DIRECT_ABSTRACT_DECLARATOR,
    ASSIGNMENT_EXPRESSION,
    INITIALIZER_LIST,
    LABELED_STATEMENT,
    STATEMENT,
    COMPOUND_STATEMENT,
    EXPRESSION_STATEMENT,
    SELECTION_STATEMENT,
    ITERATION_STATEMENT,
    JUMP_STATEMENT,
    STATEMENT_LIST,
    DECLARATION_LIST,
    EXTERNAL_DECLARATION,
    TRANSLATION_UNIT,
    FUNCTION_DEFINITION,
  };

  struct Symbol {
    enum class Type : uint8_t {
      UNKNOWN = 0,
      NON_TERMINAL,
      KEYWORD,
      IDENTIFIER,
      PUNCTUATOR,
      LITERAL,
    } type;
    union {
      Terminal terminal;
      NonTerminal nonterminal;
    };
  };

  union Rule {
    Symbol symbols[10];
    struct {
      uint32_t i1;
      uint32_t i2;
      uint32_t i3;
      uint32_t i4;
      uint32_t i5;
    };
  };

  struct RuleHash {
    std::size_t operator()(const Rule& rule) const noexcept {
      std::size_t h = 0xcbf29ce484222325;
      h ^= static_cast<std::size_t>(rule.i1) + 0x9e3779b9 + (h << 6) + (h >> 2);
      h ^= static_cast<std::size_t>(rule.i2) + 0x9e3779b9 + (h << 6) + (h >> 2);
      h ^= static_cast<std::size_t>(rule.i3) + 0x9e3779b9 + (h << 6) + (h >> 2);
      h ^= static_cast<std::size_t>(rule.i4) + 0x9e3779b9 + (h << 6) + (h >> 2);
      h ^= static_cast<std::size_t>(rule.i5) + 0x9e3779b9 + (h << 6) + (h >> 2);
      return h;
    }
  };

  struct RuleEqual {
    bool operator()(const Rule& a, const Rule& b) const noexcept {
      return a.i1 == b.i1 && a.i2 == b.i2 && a.i3 == b.i3 && a.i4 == b.i4 &&
             a.i5 == b.i5;
    }
  };
}  // namespace compiler
