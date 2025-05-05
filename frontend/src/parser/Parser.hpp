#pragma once

#include <expected>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

#include "ParserError.hpp"
#include "ast/CFGrammar.hpp"
#include "ast/StorageAST.hpp"
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
    explicit Parser(TokenStream& tokens) noexcept;
    explicit Parser(TokenStream& tokens, const CFGrammar& grammar) noexcept;
    ~Parser() noexcept = default;

    /**
     * @brief Parses the input stream and builds the AST.
     *
     */
    void parse();

  private:
    friend class CFGrammar;

  private:
    struct Action {
      enum Type { SHIFT, REDUCE, ACCEPT, ERROR } type;
      union {
        uint32_t rule_index;
        uint32_t next_state;
      };
    };

    TokenStream& tokens;
    const CFGrammar& grammar;
    std::vector<Symbol> symbols;
    std::vector<uint32_t> states;
    std::vector<RuleNew> rules;
    using ActionTable =
        std::unordered_map<std::pair<uint32_t, Symbol>, Action,
                           StateSymbolPairHash, StateSymbolPairEqual>;

    ActionTable actionTable;

  private:
    bool reduce();
    ParserResult nextSymbol();

    void generateActionTable(std::vector<RuleNew>& rules,
                             ActionTable& actionTable);

    std::string symbolToString(const Symbol& s);
  };
}  // namespace compiler