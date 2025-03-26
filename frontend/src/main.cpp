#include <iostream>
#include <string>
#include <variant>
#include <vector>

#include "Lexer.hpp"

using namespace compiler;

void testLexer(const std::string& input, const std::string& testName) {
  Lexer lexer(input);
  std::cout << "Testing input: \"" << input << "\" (" << testName << ")\n";

  while (true) {
    auto result = lexer.advance();

    if (result) {
      Token token = *result;

      switch (token.type) {
        case TokenType::KEYWORD:
          std::cout << "Token: Type=" << static_cast<int>(token.type)
                    << " - Keyword: "
                    << KeywordHandler::from(
                           std::get<Keyword>(token.value.value()))
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
                    << " - Special Punctuation: " << "\n";
          break;

        case TokenType::ENDOF:
          std::cout << "Token: Type=" << static_cast<int>(token.type)
                    << " - End of File\n";
          return;

        default:
          std::cerr << "Unknown token type.\n";

          // End of the token stream
          if (token.type == TokenType::ENDOF) {
            return;
          }
      }
    } else {
      LexerError error = result.error();
      std::cerr << "Lexer Error at line " << error.line << ", pos " << error.pos
                << ": " << error.message << "\n";
      break;
    }
  }
  std::cout << "--------------------------------\n";
}

int main() {
  // // Test Identifiers and Keywords
  // testLexer("int main return void function identifier123 and_others _ _no",
  //           "Identifiers and Keywords");

  // // Test Numeric Literals
  // testLexer("123 456.78 0xABC 0777 0b101", "Numeric Literals");

  // // Test String Literals
  // testLexer(R"( 'A' "YES")", "String Literals");

  // Test Special Punctuation
  testLexer("a += -a **ptr", "Special Punctuation");

  // // Test Unterminated String Error
  // testLexer("\"hello world", "Unterminated String Literal");

  // // Test Invalid Numeric Literal Length (Too long)
  // testLexer(std::string(300, '9'), "Invalid Numeric Literal Length");

  // // Test Unknown Character Error
  // testLexer("@", "Unknown Character Error");

  // // Test Edge Case: Empty String (Nothing to parse)
  // testLexer("", "Empty String");

  // // Test Edge Case: Single Character (just an identifier)
  // testLexer("a", "Single Character Identifier");

  // // Test Edge Case: Reserved Keyword as Identifier (e.g., 'return')
  // testLexer("return", "Keyword as Identifier");

  // // Test Edge Case: Just a number with no space
  // testLexer("123456789012345", "Large Numeric Literal");

  // // Test Edge Case: Invalid characters
  // testLexer("a@b$c", "Invalid Characters");

  return 0;
}
