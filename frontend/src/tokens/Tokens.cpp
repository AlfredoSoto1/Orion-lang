#include "Tokens.hpp"

#include <sstream>

namespace compiler {

  std::string Token::toString(std::string_view source) const noexcept {
    std::ostringstream oss;

    switch (type) {
      case TokenType::KEYWORD:
        oss << KeywordHandler::toString(value.keyword);
        break;

      case TokenType::IDENTIFIER:
        oss << value.identifier.view(source);
        break;

      case TokenType::CHAR_LITERAL:
        oss << value.literal.character;
        break;

      case TokenType::STR8_LITERAL:
      case TokenType::STR16_LITERAL:
        oss << value.literal.string.view(source);
        break;

      case TokenType::BOOL_LITERAL:
        oss << (value.literal.boolean ? "true" : "false");
        break;

      case TokenType::INT8_LITERAL:
      case TokenType::INT16_LITERAL:
      case TokenType::INT32_LITERAL:
      case TokenType::INT64_LITERAL:
        oss << value.literal.integer;
        break;
      case TokenType::UINT8_LITERAL:
      case TokenType::UINT16_LITERAL:
      case TokenType::UINT32_LITERAL:
      case TokenType::UINT64_LITERAL:
        oss << value.literal.integer;
        break;

      case TokenType::FLOAT32_LITERAL:
      case TokenType::FLOAT64_LITERAL:
        oss << value.literal.floating;
        break;

      case TokenType::PUNCTUATOR:
        oss << PunctuatorHandler::toString(value.punctuator);
        break;

      case TokenType::COMMENT:
        oss << "[Comment]";
        break;

      case TokenType::ENDOF:
        oss << "[EOF]";
        break;
      default:
        oss << "Unknown token type.";
    }

    return oss.str();
  }
}  // namespace compiler