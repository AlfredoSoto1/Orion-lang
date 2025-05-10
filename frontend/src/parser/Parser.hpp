#pragma once

#include <expected>
#include <unordered_map>

#include "ParserError.hpp"
#include "Symbols.hpp"
#include "tokens/TokenStream.hpp"

namespace compiler {

  /**
   * @brief A recursive descent parser for a C/C++-like language.
   */
  class Parser final {
  public:
    using ParserResult = std::expected<Symbol, ParserError>;

  public:
    /**
     * @brief Construct a new Parser object.
     *
     * @param tokens
     */
    explicit Parser(TokenStream& tokens, const Grammar& grammar) noexcept;
    ~Parser() noexcept = default;

    /**
     * @brief Parses the input stream and builds the AST.
     *
     */
    void parse();

  private:
    TokenStream& tokens;

  private:
    // bool reduce();
    // ParserResult nextSymbol();
  };
}  // namespace compiler