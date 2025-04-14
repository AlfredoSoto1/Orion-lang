#include <bitset>
#include <cassert>
#include <iostream>
#include <string>
#include <variant>
#include <vector>

#include "ast/Parser.hpp"
#include "ast/StorageAST.hpp"

using namespace compiler;

void print_indent(int depth) {
  for (int i = 0; i < depth; ++i) std::cout << "  ";
}

void print_expr(const ASTStorage& ast, Index expr_idx, int depth = 0) {
  const ExprAST& expr = ast.exprs[expr_idx];
  print_indent(depth);
  switch (expr.type) {
    case ExprAST::Type::ID: {
      const IDAST& id = ast.ids[expr.index];
      std::cout << "ID: " << id.id.name << "\n";
      break;
    }
    case ExprAST::Type::LITERAL: {
      std::cout << "Literal: <unsupported printer>\n";
      break;
    }
    case ExprAST::Type::PAREN_EXPR: {
      std::cout << "(Paren Expr)\n";
      break;
    }
    case ExprAST::Type::BINARY_EXPR: {
      const BinaryExprAST& bin = ast.binary_exprs[expr.index];
      std::cout << "BinaryExpr: " << PunctuatorHandler::to_string(bin.op)
                << "\n";
      print_expr(ast, bin.left, depth + 1);
      print_expr(ast, bin.right, depth + 1);
      break;
    }
  }
}

void print_stmt(const ASTStorage& ast, Index stmt_idx, int depth = 0) {
  const StmtAST& stmt = ast.stmts[stmt_idx];
  print_indent(depth);
  switch (stmt.type) {
    case StmtAST::Type::RETURN: {
      std::cout << "Return:\n";
      print_expr(ast, ast.exprs.size() - 1,
                 depth + 1);  // assuming last expr is the return one
      break;
    }
    case StmtAST::Type::EXPR:
      std::cout << "ExprStmt:\n";
      print_expr(ast, stmt.index, depth + 1);
      break;
    case StmtAST::Type::IF:
      std::cout << "IfStmt: <not implemented>\n";
      break;
  }
}

void print_block(const ASTStorage& ast, Index block_idx, int depth = 0) {
  const BlockAST& block = ast.blocks[block_idx];
  print_indent(depth);
  if (block.type == BlockAST::Type::STMT) {
    std::cout << "Block:\n";
    print_stmt(ast, block.block, depth + 1);
  } else {
    std::cout << "{ stmt_list } Block: <not implemented>\n";
  }
}

void print_function(const ASTStorage& ast, Index fn_idx, int depth = 0) {
  const FunctionAST& fn = ast.functions[fn_idx];
  const IDAST& id = ast.ids[fn.id];
  const IDAST& type = ast.ids[fn.type];

  print_indent(depth);
  std::cout << "Function: " << type.id.name << " " << id.id.name << "(\n";

  const ParamsAST& params = ast.params_list[fn.params];
  if (params.type == ParamsAST::Type::PARAMS) {
    Index param_list_idx = params.index;
    while (param_list_idx != static_cast<Index>(-1)) {
      const ParamListAST& param_list = ast.param_lists[param_list_idx];
      const ParamAST& param = ast.params[param_list.param];
      const IDAST& ptype = ast.ids[param.type];
      const IDAST& pname = ast.ids[param.id];
      print_indent(depth + 1);
      std::cout << ptype.id.name << " " << pname.id.name << "\n";
      if (param_list.type == ParamListAST::Type::LIST) {
        param_list_idx = param_list.next;
      } else
        break;
    }
  }

  print_indent(depth);
  std::cout << ")\n";

  print_block(ast, fn.block, depth + 1);
}

void print_program(const ASTStorage& ast) {
  const ProgramAST& program = ast.programs.back();
  std::cout << "=== AST ===\n";
  print_function(ast, program.function);
  std::cout << "===========\n";
}

void test_build_expression(ASTStorage& ast) {
  // ID nodes
  Index id_a = static_cast<Index>(ast.ids.size());
  ast.ids.push_back({123, Identifier{"a"}});

  Index id_b = static_cast<Index>(ast.ids.size());
  ast.ids.push_back({456, Identifier{"b"}});

  Index id_c = static_cast<Index>(ast.ids.size());
  ast.ids.push_back({789, Identifier{"c"}});

  // Expr: ID "a"
  Index expr_a = static_cast<Index>(ast.exprs.size());
  ast.exprs.push_back({ExprAST::Type::ID, id_a});

  Index expr_b = static_cast<Index>(ast.exprs.size());
  ast.exprs.push_back({ExprAST::Type::ID, id_b});

  Index expr_c = static_cast<Index>(ast.exprs.size());
  ast.exprs.push_back({ExprAST::Type::ID, id_c});

  // Binary: b * c
  Index mul = static_cast<Index>(ast.binary_exprs.size());
  ast.binary_exprs.push_back({expr_b, expr_c, Punctuator::STAR});

  Index mul_expr = static_cast<Index>(ast.exprs.size());
  ast.exprs.push_back({ExprAST::Type::BINARY_EXPR, mul});

  // Binary: a + (b * c)
  Index add = static_cast<Index>(ast.binary_exprs.size());
  ast.binary_exprs.push_back({expr_a, mul_expr, Punctuator::PLUS});

  Index final_expr = static_cast<Index>(ast.exprs.size());
  ast.exprs.push_back({ExprAST::Type::BINARY_EXPR, add});

  // Verifications
  assert(ast.exprs[final_expr].type == ExprAST::Type::BINARY_EXPR);
  assert(ast.binary_exprs[add].op == Punctuator::PLUS);
  assert(ast.binary_exprs[add].left == expr_a);
  assert(ast.binary_exprs[add].right == mul_expr);
}

void test_return_stmt(ASTStorage& ast) {
  // Use previous expression as return value
  Index expr = static_cast<Index>(ast.exprs.size()) - 1;

  // Create stmt
  Index return_stmt = static_cast<Index>(ast.stmts.size());
  ast.stmts.push_back(
      {StmtAST::Type::RETURN, static_cast<Index>(ast.if_stmts.size())});
  ast.if_stmts.push_back({});  // Placeholder, not used for RETURN

  // In this design, we should have a separate vector for ReturnStmtAST.
  // Let's simulate return_expr inside Expr itself instead for now.
  assert(ast.stmts[return_stmt].type == StmtAST::Type::RETURN);
}

void test_function(ASTStorage& ast) {
  // Reuse IDs a and b
  Index id_int = static_cast<Index>(ast.ids.size());
  ast.ids.push_back({1000, Identifier{"int"}});

  Index id_func = static_cast<Index>(ast.ids.size());
  ast.ids.push_back({1001, Identifier{"add"}});

  // Params: int a, int b
  Index id_a = 0;  // From earlier
  Index id_b = 1;  // From earlier

  Index param_a = static_cast<Index>(ast.params.size());
  ast.params.push_back({id_int, id_a});

  Index param_b = static_cast<Index>(ast.params.size());
  ast.params.push_back({id_int, id_b});

  Index param_list = static_cast<Index>(ast.param_lists.size());
  ast.param_lists.push_back(
      {ParamListAST::Type::PARAM, param_b, static_cast<Index>(-1)});
  ast.param_lists.push_back({ParamListAST::Type::LIST, param_a, param_list});

  Index params = static_cast<Index>(ast.params_list.size());
  ast.params_list.push_back({ParamsAST::Type::PARAMS, param_list + 1});

  // Block with return statement
  Index stmt = static_cast<Index>(ast.stmts.size()) - 1;
  Index block = static_cast<Index>(ast.blocks.size());
  ast.blocks.push_back({BlockAST::Type::STMT, stmt});

  // Function
  Index fn = static_cast<Index>(ast.functions.size());
  ast.functions.push_back({id_int, id_func, params, block});

  assert(ast.functions[fn].id == id_func);
  assert(ast.functions[fn].params == params);
}

void test_program(ASTStorage& ast) {
  Index fn = static_cast<Index>(ast.functions.size()) - 1;
  ast.programs.push_back({fn});
  assert(ast.programs.back().function == fn);
}

void run_all_tests() {
  ASTStorage ast;
  test_build_expression(ast);
  test_return_stmt(ast);
  test_function(ast);
  test_program(ast);

  std::cout << "✅ AST built successfully.\n";
  print_program(ast);
}

bool testToken(const Token& token) {
  switch (token.type) {
    case TokenType::KEYWORD:
      std::cout << "Token: Type=" << static_cast<int>(token.type)
                << " - Keyword: "
                << KeywordHandler::to_string(token.value.keyword) << "\n";
      break;

    case TokenType::IDENTIFIER:
      std::cout << "Token: Type=" << static_cast<int>(token.type)
                << " - Identifier: " << token.value.identifier.name << "\n";
      break;

    case TokenType::CHAR_LITERAL:
      std::cout << "Token: Type=" << static_cast<int>(token.type)
                << " - Char Literal: " << token.value.literal.character << "\n";
      break;

    case TokenType::STRING_LITERAL:
      std::cout << "Token: Type=" << static_cast<int>(token.type)
                << " - String Literal: " << token.value.literal.string << "\n";
      break;

    case TokenType::BOOLEAN_LITERAL:
      std::cout << "Token: Type=" << static_cast<int>(token.type)
                << " - Boolean Literal: " << token.value.literal.boolean
                << "\n";
      break;

    case TokenType::INTEGER_LITERAL:
      std::cout << "Token: Type=" << static_cast<int>(token.type)
                << " - Integer Literal: " << token.value.literal.integer
                << "\n";
      break;

    case TokenType::FLOATING_LITERAL:
      std::cout << "Token: Type=" << static_cast<int>(token.type)
                << " - Floating Literal: " << token.value.literal.floating
                << "\n";
      break;

    case TokenType::PUNCTUATOR:
      std::cout << "Token: Type=" << static_cast<int>(token.type)
                << " - Special Punctuation: "
                << PunctuatorHandler::to_string(token.value.punctuator) << "\n";
      break;

    case TokenType::COMMENT:
      std::cout << "Token: Type=" << static_cast<int>(token.type)
                << " - Comment\n";
      break;

    case TokenType::ENDOF:
      std::cout << "Token: Type=" << static_cast<int>(token.type)
                << " - End of File\n";
      return true;

    default:
      std::cerr << "Unknown token type.\n";
  }

  return false;
}

void testLexer(const std::string& input, const std::string& testName) {
  Lexer lexer(input);
  std::cout << "Testing input: \"" << input << "\" (" << testName << ")\n";

  while (true) {
    auto result = lexer.advance();

    if (result) {
      if (testToken(*result)) return;
    } else {
      LexerError error = result.error();
      std::cerr << "Lexer Error at line " << error.line << ", col " << error.pos
                << ": " << error.to_string() << "\n";
      break;
    }
  }
  std::cout << "--------------------------------\n";
}

void testTokenStream(const std::string& input, const std::string& testName) {
  Lexer lexer(input);
  std::cout << "Testing input: \"" << input << "\" (" << testName << ")\n";

  TokenStream stream = TokenStream(lexer, 10);

  while (stream.hasNext()) {
    auto result = stream.next();
    if (!result.has_value()) {
      LexerError error = result.error();
      std::cerr << "Lexer Error at line " << error.line << ", col " << error.pos
                << ": " << error.to_string() << "\n";
      break;
    }

    if ((*result).type != TokenType::ENDOF) {
      testToken((*result));
      std::cout << "Next Token: " << std::endl;
    } else {
      std::cout << "End of Stream" << std::endl;
      break;
    }
  }
}

void testParser(const std::string& input, const std::string& testName) {
  Lexer lexer(input);
  std::cout << "Testing input: \"" << input << "\" (" << testName << ")\n";

  TokenStream stream = TokenStream(lexer, 10);

  Parser parser = Parser(stream);
  parser.parse();
}

int main() {
  // testLexer(R"(
  //   int a = 5;
  //   int b = 10; // Another variable

  //   void main() {
  //     a += b * 20;
  //   }
  // )",
  //           "LEXER TEST");

  // testTokenStream(R"(
  //   int a = 5;
  //   int b = add(a, b); // Another variable

  //   void main(int a) {
  //     a += b * 20;
  //   }
  // )",
  //                 "TOKEN STREAM TEST");

  run_all_tests();

  testParser(R"(
    a + b
  )",
             "PARSER TEST");

  // std::cout << "ASTNode " << sizeof(ASTNode) << "\n";
  // std::cout << "Rule " << sizeof(Rule) << "\n";
  // std::cout << "Token " << sizeof(Token) << "\n";
  // std::cout << "4xASTNode* " << sizeof(ASTNode*) * 4 << "\n";
  // std::cout << "TokenType " << sizeof(TokenType) << "\n";
  // std::cout << "Literal " << sizeof(Literal) << "\n";
  // std::cout << "Identifier " << sizeof(Identifier) << "\n";
  // std::cout << "TokenValue " << sizeof(TokenValue) << "\n";
  // std::cout << "TokenStream " << sizeof(TokenStream) << "\n";
  // std::cout << "ShortString_view " << sizeof(string_view) << "\n";
  // std::cout << "string_view " << sizeof(std::string_view) << "\n";

  // Rule r1 = {};
  // r1.symbols[0].type = Symbol::Type::NON_TERMINAL;
  // r1.symbols[0].nonterminal = NonTerminal::EXPR;
  // r1.symbols[1].type = Symbol::Type::IDENTIFIER;
  // r1.symbols[1].terminal.ident_or_lit_or_eof = 1;
  // r1.symbols[2].type = Symbol::Type::NON_TERMINAL;
  // r1.symbols[2].nonterminal = NonTerminal::EXPR;

  // std::cout << "Rule " << sizeof(Rule) << " " << std::bitset<32>(r1.i1) <<
  // ":"
  //           << std::bitset<32>(r1.i2) << ":" << std::bitset<32>(r1.i3) << ":"
  //           << ":" << std::bitset<32>(r1.i4) << ":" << std::bitset<32>(r1.i5)
  //           << "\n";

  std::cout << "All tests passed!\n";
  return 0;
}