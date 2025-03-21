#pragma once

#include <string>
#include <unordered_set>
#include <vector>

#include "Tokens.hpp"

namespace compiler {

  class LexerError {};

  class Lexer {
  public:
    Lexer(const std::string& src);
    std::vector<Token> tokenize();

  private:
    std::string path;  // Defines the path of where is working
    std::string source;
    size_t pos = 0;   // Horizontal line position
    size_t line = 0;  // Vertical line position

    std::unordered_set<std::string> keywords = {
        "int",   "float",  "double", "char",  "if",     "else",   "for",
        "while", "return", "void",   "class", "struct", "public", "private"};

    bool isOperator(char c);
    bool isSymbol(char c);

    char peek();
    char advance();
    void skipWhitespace();

    Token makeNumber();
    Token makeString();
    Token makeComment();
    Token makeOperator();
    Token makeIdentifier();
  };

}  // namespace compiler
