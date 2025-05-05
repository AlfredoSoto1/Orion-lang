#include "ParserError.hpp"

namespace compiler {

  std::string_view ParserError::to_string() {
    switch (type) {
      case ParserErrorType::UNKNOWN_ERROR:
        return "Unknown error.";
      case ParserErrorType::UNKNOWN_SYMBOL:
        return "Unknown symbol from token.";
      case ParserErrorType::LEXER_ERROR:
        return "Tokenizer error.";
    }
    return "Unspecified parser error.";  // Fallback case (should never happen)
  }
}  // namespace compiler
