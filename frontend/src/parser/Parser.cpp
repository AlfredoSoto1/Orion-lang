#include "Parser.hpp"

#include <iostream>
#include <string>

namespace compiler {

  Parser::Parser(TokenStream& tokens, const CFGrammar& grammar) noexcept
      : tokens(tokens), grammar(grammar), symbols(), states(), rules() {}

  Parser::Parser(TokenStream& tokens) noexcept
      : tokens(tokens), grammar(grammar), symbols(), states(), rules() {
    // Define the symbols
    Symbol IDENTIFIER;
    IDENTIFIER.type = Symbol::Type::IDENTIFIER;
    IDENTIFIER.terminal.ident_or_lit = 0;

    Symbol CONSTANT;
    CONSTANT.type = Symbol::Type::LITERAL;
    CONSTANT.terminal.ident_or_lit = 1;

    Symbol EXPR;
    EXPR.type = Symbol::Type::NON_TERMINAL;
    EXPR.nonterminal = NonTerminal::EXPR;

    Symbol PRIMARY_EXPR;
    PRIMARY_EXPR.type = Symbol::Type::NON_TERMINAL;
    PRIMARY_EXPR.nonterminal = NonTerminal::PRIMARY_EXPR;

    Symbol ASSIGNMENT_EXPR;
    ASSIGNMENT_EXPR.type = Symbol::Type::NON_TERMINAL;
    ASSIGNMENT_EXPR.nonterminal = NonTerminal::ASSIGNMENT_EXPR;

    Terminal t{.punctuator = Punctuator::EQ};

    // Define the rules
    std::vector<RuleNew> rules = {
        // Rule 1: EXPR -> IDENTIFIER
        RuleNew{EXPR, {IDENTIFIER}},
        // Rule 2: EXPR -> CONSTANT
        RuleNew{EXPR, {CONSTANT}},
        // Rule 3: EXPR -> PRIMARY_EXPR
        RuleNew{EXPR, {PRIMARY_EXPR}},
        // Rule 4: PRIMARY_EXPR -> IDENTIFIER
        RuleNew{PRIMARY_EXPR, {IDENTIFIER}},
        // Rule 5: ASSIGNMENT_EXPR -> IDENTIFIER '=' EXPR
        RuleNew{ASSIGNMENT_EXPR,
                {IDENTIFIER, Symbol{Symbol::Type::PUNCTUATOR, t}, EXPR}}};

    // Define action table
    ActionTable actionTable;

    // Call the generateActionTable function
    generateActionTable(rules, actionTable);

    // Print the action table for verification
    for (const auto& entry : actionTable) {
      const auto& [state_symbol, action] = entry;
      const auto& [state, symbol] = state_symbol;

      std::cout << "State: " << state << ", Symbol: " << symbolToString(symbol)
                << ", Action: ";
      if (action.type == Action::SHIFT) {
        std::cout << "SHIFT to state " << action.next_state << "\n";
      } else if (action.type == Action::REDUCE) {
        std::cout << "REDUCE using rule " << action.rule_index << "\n";
      } else if (action.type == Action::ACCEPT) {
        std::cout << "ACCEPT\n";
      } else {
        std::cout << "ERROR\n";
      }
    }
  }

  std::string Parser::symbolToString(const Symbol& s) {
    switch (s.type) {
      case Symbol::Type::IDENTIFIER:
        return "IDENTIFIER";
      case Symbol::Type::LITERAL:
        return "CONSTANT";
      case Symbol::Type::PUNCTUATOR:
        return "PUNCTUATOR_" +
               std::to_string(static_cast<int>(s.terminal.punctuator));
      case Symbol::Type::NON_TERMINAL:
        return "NT_" + std::to_string(static_cast<int>(s.nonterminal));
      case Symbol::Type::ENDOF:
        return "ENDOF";
      default:
        return "UNKNOWN";
    }
  }

  void Parser::parse() {
    // Initial conditions
    states.push_back(0);
    ParserResult lookahead = nextSymbol();

    do {
      // Validate that the symbol is not garbage.
      if (lookahead) {
        return;  // Error
      }

      uint32_t state = states.back();
      Action action = actionTable[{state, *lookahead}];

      switch (action.type) {
        case Action::SHIFT: {
          // Shift the current symbol to stack and push to the next state.
          symbols.push_back(*lookahead);
          states.push_back(action.next_state);

          // Advance to the next symbol.
          lookahead = nextSymbol();
          break;
        }

        case Action::REDUCE: {
          break;
        }

        case Action::ACCEPT:
          std::cout << "Parsing successful.\n";
          return;

        case Action::ERROR:
          // Capture state and use that to display error type
          std::cerr << "Syntax error.\n";
          return;
      }

      // // Keep reducing while possible
      // while (reduce());

      // // If no more tokens left, exit
      // if (!tokens.hasNext()) {
      //   break;
      // }

      // // Shift the next token
      // shift();

    } while (true);
  }

  bool Parser::reduce() {
    bool reduced = false;

    // Scan for a possible symbol combination starting from the 10th to 1rst
    for (size_t len = std::min(10ull, symbols.size()); len >= 1; --len) {
      // Check if the symbols to be scanned are greater than the maxium
      // amount of symbols reduced.
      if (len > symbols.size()) {
        return false;
      }

      // Build the rule from the sequence of symbols
      Rule candidate{};
      for (size_t i = 0; i < len; ++i)
        candidate.symbols[i] = symbols[symbols.size() - len + i];

      // Check if the rule exists in the grammar table
      auto range = grammar.table.equal_range(candidate);
      for (auto it = range.first; it != range.second; ++it) {
        // Apply reduction
        Symbol reduced_symbol = it->second(*this);

        // Check if the reduced symbol is depends on a higher context
        // if (reduced_symbol.type == Symbol::Type::DEPENDS_ON_CONTEXT) {
        //   continue;
        // }

        // Remove the symbols that were reduced from the stack
        for (size_t i = 0; i < len; ++i) symbols.pop_back();

        // Push the new reduced grammar into stack
        symbols.push_back(reduced_symbol);
        reduced = true;
        break;
      }
    }

    return reduced;
  }

  Parser::ParserResult Parser::nextSymbol() {
    auto result = tokens.next();

    // Check if token has any defect
    if (!result) {
      return std::unexpected(
          ParserError{ParserErrorType::LEXER_ERROR, result.error()});
    }

    // Map the token to the specified symbol
    Symbol sym = {};
    switch (result->type) {
      case TokenType::KEYWORD:
        sym.type = Symbol::Type::KEYWORD;
        sym.terminal.keyword = result->value.keyword;
        break;
      case TokenType::PUNCTUATOR:
        sym.type = Symbol::Type::PUNCTUATOR;
        sym.terminal.punctuator = result->value.punctuator;
        break;
      case TokenType::IDENTIFIER:
        sym.type = Symbol::Type::IDENTIFIER;
        sym.terminal.ident_or_lit = 0;
        break;
      case TokenType::CHAR_LITERAL:
      case TokenType::BOOL_LITERAL:
      case TokenType::STR8_LITERAL:
      case TokenType::STR16_LITERAL:
      case TokenType::INT8_LITERAL:
      case TokenType::INT16_LITERAL:
      case TokenType::INT32_LITERAL:
      case TokenType::INT64_LITERAL:
      case TokenType::UINT8_LITERAL:
      case TokenType::UINT16_LITERAL:
      case TokenType::UINT32_LITERAL:
      case TokenType::UINT64_LITERAL:
      case TokenType::FLOAT32_LITERAL:
      case TokenType::FLOAT64_LITERAL:
        sym.type = Symbol::Type::LITERAL;
        sym.terminal.ident_or_lit = 1;
        break;
      case TokenType::ENDOF:
        sym.type = Symbol::Type::ENDOF;
        break;
      default:
        // This should never happen
        return std::unexpected(
            ParserError{ParserErrorType::UNKNOWN_SYMBOL, result.error()});
    }

    return sym;
  }

  void Parser::generateActionTable(std::vector<RuleNew>& rules,
                                   ActionTable& actionTable) {
    uint32_t state = 0;

    for (size_t rule_index = 0; rule_index < rules.size(); ++rule_index) {
      RuleNew& rule = rules[rule_index];
      state = 0;

      // For each terminal in the RHS, create a shift action
      for (const Symbol& symbol : rule.rhs) {
        if (symbol.type != Symbol::Type::NON_TERMINAL) {
          Action shift_action;
          shift_action.type = Action::SHIFT;
          shift_action.next_state = state++;

          actionTable[{state, symbol}] = shift_action;
        } else {
          Action reduce_action;
          reduce_action.type = Action::REDUCE;
          reduce_action.rule_index = rule_index;

          actionTable[{state, rule.lhs}] = reduce_action;
        }
      }

      // // Add a REDUCE action on the LHS (symbol to reduce to)
      // Action reduce_action;
      // reduce_action.type = Action::REDUCE;
      // reduce_action.rule_index = rule_index;

      // actionTable[{state, rule.lhs}] = reduce_action;
    }

    // ACCEPT on end-of-file (ENDOF)
    Symbol endOf;
    endOf.type = Symbol::Type::ENDOF;

    Action accept_action;
    accept_action.type = Action::ACCEPT;
    accept_action.next_state = 0;

    actionTable[{0, endOf}] = accept_action;
  }
}  // namespace compiler
