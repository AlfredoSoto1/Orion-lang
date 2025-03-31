#include <cassert>
#include <iostream>
#include <string>
#include <variant>
#include <vector>

#include "TokenStream.hpp"
#include "ast/ASTStack.hpp"
#include "ast/Parser.hpp"

using namespace compiler;

void test_push_and_peek() {
  ASTStack stack = ASTStack();
  ASTNode node1 = {Grammar::EXPR};
  ASTNode node2 = {Grammar::EXPR};
  stack.shift(&node1);
  stack.shift(&node2);
  ASTNode* buffer[1];
  stack.peekTop(buffer, 1);
  assert(buffer[0]->grammar == Grammar::EXPR);
  std::cout << "test_push_and_peek passed!\n";
}

void test_pop() {
  ASTStack stack = ASTStack();
  ASTNode node1 = {Grammar::EXPR};
  ASTNode node2 = {Grammar::EXPR};
  stack.shift(&node1);
  stack.shift(&node2);
  stack.pop(1);
  ASTNode* buffer[1];
  stack.peekTop(buffer, 1);
  assert(buffer[0]->grammar == Grammar::EXPR);
  stack.pop(1);

  // Does nothing
  stack.peekTop(buffer, 1);
  std::cout << "test_pop passed!\n";
}

void test_multiple_pages() {
  ASTStack stack = ASTStack();
  ASTNode nodes[70];
  for (int i = 0; i < 70; ++i) nodes[i] = {Grammar::EXPR};
  for (int i = 0; i < 70; ++i) stack.shift(&nodes[i]);
  ASTNode* buffer[1];
  stack.peekTop(buffer, 1);
  assert(buffer[0]->grammar == Grammar::EXPR);
  stack.pop(10);
  stack.peekTop(buffer, 1);
  assert(buffer[0]->grammar == Grammar::EXPR);
  std::cout << "test_multiple_pages passed!\n";
}

void test_peek_buffer() {
  ASTStack stack = ASTStack();
  ASTNode nodes[5] = {{Grammar::EXPR},
                      {Grammar::EXPR},
                      {Grammar::EXPR},
                      {Grammar::EXPR},
                      {Grammar::EXPR}};
  for (int i = 0; i < 5; ++i) stack.shift(&nodes[i]);
  ASTNode* buffer[4];
  stack.peekTop(buffer, 4);
  assert(buffer[0]->grammar == Grammar::EXPR);
  assert(buffer[1]->grammar == Grammar::EXPR);
  assert(buffer[2]->grammar == Grammar::EXPR);
  assert(buffer[3]->grammar == Grammar::EXPR);
  std::cout << "test_peek_buffer passed!\n";
}

bool testToken(const Token& token) {
  switch (token.type) {
    case TokenType::KEYWORD:
      std::cout << "Token: Type=" << static_cast<int>(token.type)
                << " - Keyword: "
                << KeywordHandler::from(std::get<Keyword>(token.value.value()))
                << "\n";
      break;

    case TokenType::IDENTIFIER:
      std::cout << "Token: Type=" << static_cast<int>(token.type)
                << " - Identifier: "
                << std::get<Identifier>(token.value.value()).name << "\n";
      break;

    case TokenType::LITERAL: {
      std::cout << "Token: Type=" << static_cast<int>(token.type)
                << " - Numeric Literal: ";

      Literal lt = std::get<Literal>(token.value.value());
      switch (lt.type) {
        case LiteralType::INTEGER:
          std::cout << std::get<uint64_t>(lt.value) << "\n";
          break;
        case LiteralType::FLOAT:
          std::cout << std::get<double>(lt.value) << "\n";
          break;
        case LiteralType::CHAR:
          std::cout << std::get<char>(lt.value) << "\n";
          break;
        case LiteralType::STRING:
          std::cout << std::get<std::string>(lt.value) << "\n";
          break;
        default:
          std::cerr << "Unknown literal type\n";
      }
    } break;

    case TokenType::PUNCTUATOR:
      std::cout << "Token: Type=" << static_cast<int>(token.type)
                << " - Special Punctuation: "
                << PunctuatorHandler::from(
                       std::get<Punctuator>(token.value.value()))
                << "\n";
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
                << ": " << error.toString() << "\n";
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
                << ": " << error.toString() << "\n";
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

  // testParser(R"(
  //   a + b
  //   int a;
  //   int a = 5;
  //   int a = 5;
  // )",
  //            "PARSER TEST");

  test_push_and_peek();
  test_pop();
  test_multiple_pages();
  test_peek_buffer();

  std::cout << "All tests passed!\n";
  return 0;
}