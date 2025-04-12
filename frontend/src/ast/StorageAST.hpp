#pragma once

#include <vector>

#include "Tokens.hpp"

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
  // block → stmt
  //       | { stmt_list }
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
    Operator op;
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
    enum class Type {
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
    enum class Type {
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

  // stmt_list → stmt stmt_list
  //           | stmt
  //           | ε
  struct StmtListAST {
    enum class Type {
      EMPTY,
      SINGLE,
      MULTIPLE,
    } type;
    Index stmt;
    Index next;
  };

  // block → stmt
  //       | { stmt_list }
  struct BlockAST {
    enum class Type {
      STMT,
      LIST,
    } type;
    Index block;
  };

  // param → type IDENTIFIER
  struct ParamAST {
    Index type;
    Index id;
  };

  // param_list → param , param_list
  //            | param
  struct ParamListAST {
    enum class Type {
      LIST,
      PARAM,
    } type;
    Index param;
    Index next;
  };

  // params → param_list
  //        | ε
  struct ParamsAST {
    enum class Type {
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

    std::vector<StmtListAST> stmt_lists;
    std::vector<BlockAST> blocks;

    std::vector<ParamAST> params;
    std::vector<ParamListAST> param_lists;
    std::vector<ParamsAST> params_list;
  };

}  // namespace compiler
