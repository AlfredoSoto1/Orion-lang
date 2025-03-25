#include "Punctuator.hpp"

#include <unordered_map>

namespace compiler {

  Punctuator PunctuatorHandler::from(std::string_view str) {
    static const std::unordered_map<std::string_view, Punctuator>
        punctuator_map = {{"@", Punctuator::AT},
                          {"#", Punctuator::HASH},
                          {"$", Punctuator::DOLLAR},
                          {".", Punctuator::DOT},
                          {",", Punctuator::COMMA},
                          {":", Punctuator::COLON},
                          {";", Punctuator::SEMI_COLON},
                          {"{", Punctuator::LBRACE},
                          {"}", Punctuator::RBRACE},
                          {"(", Punctuator::LPAREN},
                          {")", Punctuator::RPAREN},
                          {"[", Punctuator::LBRACKET},
                          {"]", Punctuator::RBRACKET},
                          {"_", Punctuator::UNDER_SCORE},
                          {"+", Punctuator::PLUS},
                          {"-", Punctuator::DASH},
                          {"*", Punctuator::STAR},
                          {"/", Punctuator::SLASH},
                          {"%", Punctuator::MOD},
                          {"=", Punctuator::EQ},
                          {"&", Punctuator::BAND},
                          {"|", Punctuator::BOR},
                          {"^", Punctuator::BXOR},
                          {"~", Punctuator::BNOT},
                          {"!", Punctuator::NOT},
                          {"<", Punctuator::LT},
                          {">", Punctuator::GT},
                          {"?", Punctuator::QUESTION},
                          {"<-", Punctuator::LARROW},
                          {"->", Punctuator::RARROW},
                          {"//", Punctuator::LINE_COMMENT},
                          {"/*", Punctuator::LBLOCK_COMMENT},
                          {"*/", Punctuator::RBLOCK_COMMENT},
                          {"++", Punctuator::PLUS_PLUS},
                          {"--", Punctuator::DASH_DASH},
                          {"+=", Punctuator::PLUS_EQ},
                          {"-=", Punctuator::DASH_EQ},
                          {"*=", Punctuator::STAR_EQ},
                          {"/=", Punctuator::SLASH_EQ},
                          {"%=", Punctuator::MOD_EQ},
                          {"<<", Punctuator::LSHIFT},
                          {">>", Punctuator::RSHIFT},
                          {"<<=", Punctuator::LSHIFT_EQ},
                          {">>=", Punctuator::RSHIFT_EQ},
                          {"&=", Punctuator::AND_EQ},
                          {"|=", Punctuator::OR_EQ},
                          {"^=", Punctuator::XOR_EQ},
                          {"~=", Punctuator::NOT_EQ},
                          {"&&", Punctuator::AND},
                          {"||", Punctuator::OR},
                          {"==", Punctuator::EQ_EQ},
                          {"!=", Punctuator::NEQ},
                          {"<=", Punctuator::LTE},
                          {">=", Punctuator::GTE}};

    auto it = punctuator_map.find(str);
    return (it != punctuator_map.end()) ? it->second : Punctuator::UNKNOWN;
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
