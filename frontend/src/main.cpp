#include <iostream>
#include <string>
#include <variant>
#include <vector>

#include "TokenStream.hpp"
#include "ast/ParserTest.hpp"

using namespace compiler;

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

  testTokenStream(R"(
    int a = 5;
    int b = add(a, b); // Another variable

    void main(int a) {
      a += b * 20;
    }
  )",
                  "TOKEN STREAM TEST");

  return 0;
}
