#include "Punctuator.hpp"

namespace compiler {

  Punctuator PunctuatorHandler::from(std::string_view str) {
    switch (str[0]) {
      case '@':
        return Punctuator::AT;
      case '#':
        return Punctuator::HASH;
      case '$':
        return Punctuator::DOLLAR;
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
      case '_':
        return Punctuator::UNDER_SCORE;
      case '+':
        return Punctuator::PLUS;
      case '-':
        return Punctuator::DASH;
      case '*':
        return Punctuator::STAR;
      case '/':
        return Punctuator::SLASH;
      case '%':
        return Punctuator::MOD;
      case '=':
        return Punctuator::EQ;
      case '&':
        return Punctuator::BAND;
      case '|':
        return Punctuator::BOR;
      case '^':
        return Punctuator::BXOR;
      case '~':
        return Punctuator::BNOT;
      case '!':
        return Punctuator::NOT;
      case '<':
        return Punctuator::LT;
      case '>':
        return Punctuator::GT;
      case '?':
        return Punctuator::QUESTION;
      default:
        if (str == "<-")
          return Punctuator::LARROW;
        else if (str == "->")
          return Punctuator::RARROW;
        else if (str == "//")
          return Punctuator::LINE_COMMENT;
        else if (str == "/*")
          return Punctuator::LBLOCK_COMMENT;
        else if (str == "*/")
          return Punctuator::RBLOCK_COMMENT;
        else if (str == "++")
          return Punctuator::PLUS_PLUS;
        else if (str == "--")
          return Punctuator::DASH_DASH;
        else if (str == "+=")
          return Punctuator::PLUS_EQ;
        else if (str == "-=")
          return Punctuator::DASH_EQ;
        else if (str == "*=")
          return Punctuator::STAR_EQ;
        else if (str == "/=")
          return Punctuator::SLASH_EQ;
        else if (str == "%=")
          return Punctuator::MOD_EQ;
        else if (str == "<<")
          return Punctuator::LSHIFT;
        else if (str == ">>")
          return Punctuator::RSHIFT;
        else if (str == "<<=")
          return Punctuator::LSHIFT_EQ;
        else if (str == ">>=")
          return Punctuator::RSHIFT_EQ;
        else if (str == "&=")
          return Punctuator::AND_EQ;
        else if (str == "|=")
          return Punctuator::OR_EQ;
        else if (str == "^=")
          return Punctuator::XOR_EQ;
        else if (str == "~=")
          return Punctuator::NOT_EQ;
        else if (str == "&&")
          return Punctuator::AND;
        else if (str == "||")
          return Punctuator::OR;
        else if (str == "==")
          return Punctuator::EQ_EQ;
        else if (str == "!=")
          return Punctuator::NEQ;
        else if (str == "<=")
          return Punctuator::LTE;
        else if (str == ">=")
          return Punctuator::GTE;
        else
          return Punctuator::UNKNOWN;
    }
  }

  std::string_view PunctuatorHandler::from(Punctuator punc) {
    switch (punc) {
      case Punctuator::AT:
        return "@";
      case Punctuator::HASH:
        return "#";
      case Punctuator::DOLLAR:
        return "$";
      case Punctuator::DOT:
        return ".";
      case Punctuator::COMMA:
        return ",";
      case Punctuator::COLON:
        return ":";
      case Punctuator::SEMI_COLON:
        return ";";
      case Punctuator::LBRACE:
        return "{";
      case Punctuator::RBRACE:
        return "}";
      case Punctuator::LPAREN:
        return "(";
      case Punctuator::RPAREN:
        return ")";
      case Punctuator::LBRACKET:
        return "[";
      case Punctuator::RBRACKET:
        return "]";
      case Punctuator::LARROW:
        return "<-";
      case Punctuator::RARROW:
        return "->";
      case Punctuator::LINE_COMMENT:
        return "//";
      case Punctuator::LBLOCK_COMMENT:
        return "/*";
      case Punctuator::RBLOCK_COMMENT:
        return "*/";
      case Punctuator::UNDER_SCORE:
        return "_";
      case Punctuator::PLUS:
        return "+";
      case Punctuator::DASH:
        return "-";
      case Punctuator::STAR:
        return "*";
      case Punctuator::SLASH:
        return "/";
      case Punctuator::MOD:
        return "%";
      case Punctuator::PLUS_PLUS:
        return "++";
      case Punctuator::DASH_DASH:
        return "--";
      case Punctuator::EQ:
        return "=";
      case Punctuator::PLUS_EQ:
        return "+=";
      case Punctuator::DASH_EQ:
        return "-=";
      case Punctuator::STAR_EQ:
        return "*=";
      case Punctuator::SLASH_EQ:
        return "/=";
      case Punctuator::MOD_EQ:
        return "%=";
      case Punctuator::BAND:
        return "&";
      case Punctuator::BOR:
        return "|";
      case Punctuator::BXOR:
        return "^";
      case Punctuator::BNOT:
        return "~";
      case Punctuator::LSHIFT:
        return "<<";
      case Punctuator::RSHIFT:
        return ">>";
      case Punctuator::LSHIFT_EQ:
        return "<<=";
      case Punctuator::RSHIFT_EQ:
        return ">>=";
      case Punctuator::AND_EQ:
        return "&=";
      case Punctuator::OR_EQ:
        return "|=";
      case Punctuator::XOR_EQ:
        return "^=";
      case Punctuator::NOT_EQ:
        return "~=";
      case Punctuator::AND:
        return "&&";
      case Punctuator::OR:
        return "||";
      case Punctuator::NOT:
        return "!";
      case Punctuator::EQ_EQ:
        return "==";
      case Punctuator::NEQ:
        return "!=";
      case Punctuator::LT:
        return "<";
      case Punctuator::GT:
        return ">";
      case Punctuator::LTE:
        return "<=";
      case Punctuator::GTE:
        return ">=";
      case Punctuator::QUESTION:
        return "?";
      default:
        return "UNKNOWN";
    }
  }
}  // namespace compiler
