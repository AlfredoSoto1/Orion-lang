#pragma once

#include <expected>
#include <string_view>
#include <vector>

#include "TokenStream.hpp"

// TEMP
struct ParserError {};

namespace compiler {

  /**
   * @brief A recursive descent parser for a C/C++-like language.
   */
  class Parser final {
  public:
    // using ParseResult = std::expected<ParserError, ParserError>;

  public:
    explicit Parser(TokenStream& tokens) noexcept;

    void parse();

  private:
    TokenStream& tokens;
    std::vector<Token> parse_stack;

  private:
    bool shift();
    bool reduce();
    bool handleReduce();
    bool isReducible();
    void applyReduction();
    bool isTerminal(const Token& token);
  };
}  // namespace compiler
