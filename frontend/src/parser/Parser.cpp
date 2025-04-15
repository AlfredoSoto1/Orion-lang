#include "Parser.hpp"

#include <string>

namespace compiler {

  Parser::Parser(TokenStream& tokens) noexcept
      : tokens(tokens), reduction_stack(), rt_handlers() {
    // Build rule table following grammar from Storage AST
    populateRuleTable();
  }

  void Parser::parse() {
    do {
      // Keep reducing while possible
      while (reduce());

      // If no more tokens left, exit
      if (!tokens.hasNext()) {
        break;
      }

      // Shift the next token
      shift();

    } while (true);

    if (reduction_stack.size() != 1 ||
        reduction_stack.back().type != Symbol::Type::NON_TERMINAL) {
      ParserError{ParserErrorType::INCOMPLETE_TREE_REDUCTION, {}};
      return;
    }
  }

  void Parser::shift() {
    // Obtain current token and move to next
    auto result = tokens.next();

    // Validate if token has no defect to continue
    if (!result) {
      ParserError{ParserErrorType::LEXER_ERROR, result.error()};
      return;
    }

    // Convert token to symbol
    Symbol sym = tokenToSymbol(*result);

    // Validate symbol generated
    if (sym.type == Symbol::Type::UNKNOWN) {
      // cannot match token to a symbol
      // Note: This should never happen. This is because a defect token must've
      // been handled in previous if statement.
      ParserError{ParserErrorType::UNKNOWN_SYMBOL, {}};
      return;
    }

    // Push to stack the new symbol
    reduction_stack.push_back(sym);
  }

  bool Parser::reduce() {
    bool reduced = false;

    // Scan for a possible symbol combination starting from the 10th to 1rst
    for (int len = std::min(10, static_cast<int>(reduction_stack.size()));
         len >= 1; --len) {
      // Check if the symbols to be scanned are greater than the maxium amount
      // of symbols reduced.
      if (len > reduction_stack.size()) {
        return false;
      }

      // Build the rule from the sequence of symbols
      Rule candidate{};
      for (int i = 0; i < len; ++i)
        candidate.symbols[i] =
            reduction_stack[reduction_stack.size() - len + i];

      // Check if the grammar rule exists.
      auto it = rt_handlers.find(candidate);
      if (it == rt_handlers.end()) {
        continue;
      }

      // Apply reduction
      Symbol sym = it->second(*this);

      // If the symbol is part of a bigger context, skip this reduction
      if (sym.type == Symbol::Type::BIG_CONTEXT) {
        return false;
      }

      // Push the new reduced grammar into stack
      reduction_stack.push_back(sym);
      reduced = true;
    }

    return reduced;
  }

  Symbol Parser::tokenToSymbol(const Token& token) const noexcept {
    Symbol s = {};
    switch (token.type) {
      case TokenType::KEYWORD:
        s.type = Symbol::Type::KEYWORD;
        s.terminal.keyword = token.value.keyword;
        break;
      case TokenType::PUNCTUATOR:
        s.type = Symbol::Type::PUNCTUATOR;
        s.terminal.punctuator = token.value.punctuator;
        break;
      case TokenType::IDENTIFIER:
        s.type = Symbol::Type::IDENTIFIER;
        s.terminal.ident_or_lit = 0;
        break;
      case TokenType::CHAR_LITERAL:
      case TokenType::STRING_LITERAL:
      case TokenType::BOOLEAN_LITERAL:
      case TokenType::INTEGER_LITERAL:
      case TokenType::FLOATING_LITERAL:
        s.type = Symbol::Type::LITERAL;
        s.terminal.ident_or_lit = 1;
        break;
      default:
        // This should never happen
        s.type = Symbol::Type::UNKNOWN;
        s.nonterminal = NonTerminal::UNKNOWN;
    }
    return s;
  }

  // Helper to make a Symbol from components
  Symbol Parser::makeId() const noexcept {
    Symbol sym{};
    sym.type = Symbol::Type::IDENTIFIER;
    sym.terminal.ident_or_lit = 0;
    return sym;
  }

  Symbol Parser::makeLt() const noexcept {
    Symbol sym{};
    sym.type = Symbol::Type::LITERAL;
    sym.terminal.ident_or_lit = 1;
    return sym;
  }

  Symbol Parser::makeKw(Keyword k) const noexcept {
    Symbol sym{};
    sym.type = Symbol::Type::KEYWORD;
    sym.terminal.keyword = k;
    return sym;
  }

  Symbol Parser::makePn(Punctuator p) const noexcept {
    Symbol sym{};
    sym.type = Symbol::Type::PUNCTUATOR;
    sym.terminal.punctuator = p;
    return sym;
  }

  Symbol Parser::makeNt(NonTerminal nt) const noexcept {
    Symbol sym{};
    sym.type = Symbol::Type::NON_TERMINAL;
    sym.nonterminal = nt;
    return sym;
  }

  // Helper to build a rule from up to 10 symbols
  Rule Parser::makeRule(std::initializer_list<Symbol> syms) const noexcept {
    Rule r{};
    size_t i = 0;
    for (Symbol s : syms) {
      if (i >= sizeof(Rule) / sizeof(Symbol)) break;
      r.symbols[i++] = s;
    }
    return r;
  }

  void Parser::populateRuleTable() noexcept {
    using NT = NonTerminal;
    using KW = Keyword;
    using PU = Punctuator;

    // program -> function
    rt_handlers[makeRule({makeNt(NT::FUNCTION_DECL)})] = [this](Parser&) {
      reduction_stack.pop_back();
      return makeNt(NT::PARAM_LIST);
    };

    // function -> type IDENTIFIER ( params ) block
    rt_handlers[makeRule({makeId(),            //
                          makeId(),            //
                          makePn(PU::LPAREN),  //
                          makeNt(NT::PARAMS),  //
                          makePn(PU::RPAREN),  //
                          makeNt(NT::BLOCK)})] = [this](Parser&) {
      reduction_stack.pop_back();
      reduction_stack.pop_back();
      reduction_stack.pop_back();
      reduction_stack.pop_back();
      reduction_stack.pop_back();
      reduction_stack.pop_back();
      return makeNt(NT::FUNCTION_DECL);
    };

    // params -> param_list
    rt_handlers[makeRule({makeNt(NT::PARAM_LIST)})] = [this](Parser&) {
      reduction_stack.pop_back();
      return makeNt(NT::PARAMS);
    };

    // params -> ε
    rt_handlers[makeRule({makeNt(NT::PARAM_EMPTY)})] = [this](Parser&) {
      reduction_stack.pop_back();
      return makeNt(NT::PARAMS);
    };

    // param_list -> param , param_list
    rt_handlers[makeRule({makeNt(NT::PARAM),  //
                          makePn(PU::COMMA),  //
                          makeNt(NT::PARAM_LIST)})] = [this](Parser&) {
      reduction_stack.pop_back();
      reduction_stack.pop_back();
      reduction_stack.pop_back();
      return makeNt(NT::PARAM_LIST);
    };

    // param_list -> param
    rt_handlers[makeRule({makeNt(NT::PARAM)})] = [this](Parser&) {
      reduction_stack.pop_back();
      return makeNt(NT::PARAM_LIST);
    };

    // param -> type IDENTIFIER
    rt_handlers[makeRule({makeId(), makeId()})] = [this](Parser&) {
      reduction_stack.pop_back();
      reduction_stack.pop_back();
      return makeNt(NT::PARAM);
    };

    // block -> stmt
    rt_handlers[makeRule({makeNt(NT::STMT)})] = [this](Parser&) {
      reduction_stack.pop_back();
      return makeNt(NT::BLOCK);
    };

    // block -> { stmt_list }
    rt_handlers[makeRule({makePn(PU::LBRACE),     //
                          makeNt(NT::STMT_LIST),  //
                          makePn(PU::RBRACE)})] = [this](Parser&) {
      reduction_stack.pop_back();
      reduction_stack.pop_back();
      reduction_stack.pop_back();
      return makeNt(NT::BLOCK);
    };

    // stmt_list -> stmt stmt_list
    rt_handlers[makeRule({makeNt(NT::STMT), makeNt(NT::STMT_LIST)})] =
        [this](Parser&) {
          reduction_stack.pop_back();
          reduction_stack.pop_back();
          return makeNt(NT::STMT_LIST);
        };

    // stmt_list -> stmt
    rt_handlers[makeRule({makeNt(NT::STMT)})] = [this](Parser&) {
      reduction_stack.pop_back();
      return makeNt(NT::STMT_LIST);
    };

    // stmt_list -> ε
    rt_handlers[makeRule({makeNt(NT::STMT_EMPTY)})] = [this](Parser&) {
      reduction_stack.pop_back();
      return makeNt(NT::STMT_LIST);
    };

    // stmt -> if ( expr ) block
    rt_handlers[makeRule({makeKw(KW::IF),      //
                          makePn(PU::LPAREN),  //
                          makeNt(NT::EXPR),    //
                          makePn(PU::RPAREN),  //
                          makeNt(NT::BLOCK)})] = [this](Parser&) {
      reduction_stack.pop_back();
      reduction_stack.pop_back();
      reduction_stack.pop_back();
      reduction_stack.pop_back();
      reduction_stack.pop_back();
      return makeNt(NT::STMT);
    };

    // stmt -> expr ;
    rt_handlers[makeRule({makeNt(NT::EXPR), makePn(PU::SEMI_COLON)})] =
        [this](Parser&) {
          reduction_stack.pop_back();
          reduction_stack.pop_back();
          return makeNt(NT::STMT);
        };

    // stmt -> return expr ;
    rt_handlers[makeRule({makeKw(KW::RETURN),  //
                          makeNt(NT::EXPR),    //
                          makePn(PU::SEMI_COLON)})] = [this](Parser&) {
      reduction_stack.pop_back();
      reduction_stack.pop_back();
      reduction_stack.pop_back();
      return makeNt(NT::STMT);
    };

    // expr → expr + expr (and similar binary ops)
    std::vector<Punctuator> binary_ops = {PU::PLUS, PU::DASH, PU::STAR,
                                          PU::SLASH, PU::MOD};

    for (auto op : binary_ops) {
      rt_handlers[makeRule({makeNt(NT::EXPR),  //
                            makePn(op),        //
                            makeNt(NT::EXPR)})] = [this](Parser&) {
        reduction_stack.pop_back();
        reduction_stack.pop_back();
        reduction_stack.pop_back();
        return makeNt(NT::EXPR);
      };
    }

    // expr → ( expr )
    rt_handlers[makeRule({makePn(PU::LPAREN),  //
                          makeNt(NT::EXPR),    //
                          makePn(PU::RPAREN)})] = [this](Parser&) {
      // Check if it forms part of a bigger context
      if (reduction_stack.size() > 3) {
        if (reduction_stack[reduction_stack.size() - 4].type ==
            Symbol::Type::KEYWORD) {
          return Symbol{Symbol::Type::BIG_CONTEXT};
        }
      }
      reduction_stack.pop_back();
      reduction_stack.pop_back();
      reduction_stack.pop_back();
      return makeNt(NT::EXPR);
    };

    // expr → IDENTIFIER
    rt_handlers[makeRule({makeId()})] = [this](Parser&) {
      reduction_stack.pop_back();
      return makeNt(NT::EXPR);
    };

    // expr → LITERAL
    rt_handlers[makeRule({makeLt()})] = [this](Parser&) {
      reduction_stack.pop_back();
      return makeNt(NT::EXPR);
    };

  }  // end of populate_rule_table
}  // namespace compiler
