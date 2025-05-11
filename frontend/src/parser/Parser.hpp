#pragma once

#include <expected>
#include <unordered_map>

#include "ActionTable.hpp"
#include "ParserError.hpp"
#include "Symbols.hpp"
#include "tokens/TokenStream.hpp"

namespace compiler {

  /**
   * @brief A recursive descent parser for a C/C++-like language.
   *
   *        This parser processes a token stream according to a provided
   *        grammar, identifying syntactic structures and generating
   *        intermediate representations.
   */
  class Parser final {
  public:
    /**
     * @brief The result of a parsing operation.
     *
     *        Contains either a successfully parsed Symbol or a ParserError
     *        describing the failure.
     */
    using ParserResult = std::expected<Symbol, ParserError>;

  public:
    /**
     * @brief Constructs a new Parser object.
     *
     * @param tokens   A stream of tokens to be parsed.
     * @param grammar  A list of production rules representing the grammar.
     */
    explicit Parser(TokenStream& tokens, const Grammar& grammar) noexcept;
    ~Parser() noexcept = default;

    /**
     * @brief Parses the token stream using the provided grammar.
     *
     *        This function drives the parsing process. It iteratively applies
     *        grammar rules and builds up a higher-level representation of the
     *        input structure.
     */
    void parse();

  private:
    TokenStream& tokens;
    ActionTable action_table;

  private:
    bool reduce();
    ParserResult nextSymbol();
  };
}  // namespace compiler