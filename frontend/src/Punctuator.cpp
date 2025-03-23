#include "Punctuator.hpp"

namespace compiler {

  Punctuator PunctuatorHandler::from(std::string_view str) {
    switch (str[0]) {
      case '@':
        return Punctuator::AT;
      case '.':
        return Punctuator::DOT;
      case ',':
        return Punctuator::COMMA;
      case ':':
        return Punctuator::COLON;
      case ';':
        return Punctuator::SEMI_COLON;
      case '{':
        return Punctuator::LBRACE;
      case '}':
        return Punctuator::RBRACE;
      case '(':
        return Punctuator::LPAREN;
      case ')':
        return Punctuator::RPAREN;
      case '[':
        return Punctuator::LBRACKET;
      case ']':
        return Punctuator::RBRACKET;
      case '?':
        return Punctuator::QUESTION;
      case '<':
        return Punctuator::LGENERIC;
      case '>':
        return Punctuator::RGENERIC;
      default:
        if (str == "<-") {
          return Punctuator::LARROW;
        } else if (str == "=>") {
          return Punctuator::RARROW;
        } else if (str == "//") {
          return Punctuator::LINE_COMMENT;
        } else if (str == "/*") {
          return Punctuator::LBLOCK_COMMENT;
        } else if (str == "*/") {
          return Punctuator::RBLOCK_COMMENT;
        } else {
          return Punctuator::UNKNOWN;
        }
    }
  }
}  // namespace compiler
