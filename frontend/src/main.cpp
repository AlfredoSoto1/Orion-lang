#include <cassert>
#include <iostream>
#include <string>
#include <variant>
#include <vector>

#include "ast/Parser.hpp"

using namespace compiler;

bool testToken(const Token& token) {
  switch (token.type) {
    case TokenType::KEYWORD:
      std::cout << "Token: Type=" << static_cast<int>(token.type)
                << " - Keyword: " << KeywordHandler::from(token.value.keyword)
                << "\n";
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
                << PunctuatorHandler::from(token.value.punctuator) << "\n";
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

  testParser(R"(
    a
    *a
    &a
    5
    5.0
    "Hello, World!"
    'a'
    a(b, c, d)
   (a)
  )",
             "PARSER TEST");

  std::cout << "ASTNode " << sizeof(ASTNode) << "\n";
  std::cout << "Rule " << sizeof(Rule) << "\n";
  std::cout << "Token " << sizeof(Token) << "\n";
  std::cout << "4xASTNode* " << sizeof(ASTNode*) * 4 << "\n";

  std::cout << "TokenType " << sizeof(TokenType) << "\n";
  std::cout << "Literal " << sizeof(Literal) << "\n";
  std::cout << "Identifier " << sizeof(Identifier) << "\n";
  std::cout << "TokenValue " << sizeof(TokenValue) << "\n";
  std::cout << "TokenStream " << sizeof(TokenStream) << "\n";
  std::cout << "ShortString_view " << sizeof(string_view) << "\n";
  std::cout << "string_view " << sizeof(std::string_view) << "\n";

  std::cout << "All tests passed!\n";
  return 0;
}