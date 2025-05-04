// #include "Parser.hpp"

// #include <iostream>
// #include <string>

// namespace compiler {

//   Parser::Parser(TokenStream& tokens, const CFGrammar& grammar) noexcept
//       : tokens(tokens), grammar(grammar), parse_stack() {}

//   void Parser::parse() {
//     do {
//       // Keep reducing while possible
//       while (reduce());

//       // If no more tokens left, exit
//       if (!tokens.hasNext()) {
//         break;
//       }

//       // Shift the next token
//       shift();

//     } while (true);

//     if (parse_stack.size() != 1 ||
//         parse_stack.back().type != Symbol::Type::NON_TERMINAL) {
//       ParserError{ParserErrorType::INCOMPLETE_TREE_REDUCTION, {}};
//       std::cout << "Error: Incomplete tree reduction." << std::endl;
//       return;
//     }
//   }

//   void Parser::shift() {
//     // Obtain current token and move to next
//     auto result = tokens.next();

//     // Validate if token has no defect to continue
//     if (!result) {
//       ParserError{ParserErrorType::LEXER_ERROR, result.error()};
//       return;
//     }

//     // Convert token to symbol
//     Symbol sym = tokenToSymbol(*result);

//     // Validate symbol generated
//     if (sym.type == Symbol::Type::UNKNOWN) {
//       // cannot match token to a symbol
//       // Note: This should never happen. This is because a defect token
//       must've
//       // been handled in previous if statement.
//       ParserError{ParserErrorType::UNKNOWN_SYMBOL, {}};
//       return;
//     }

//     // Push to stack the new symbol
//     parse_stack.push_back(sym);
//   }

//   bool Parser::reduce() {
//     bool reduced = false;

//     // Scan for a possible symbol combination starting from the 10th to 1rst
//     for (size_t len = std::min(10ull, parse_stack.size()); len >= 1; --len) {
//       // Check if the symbols to be scanned are greater than the maxium
//       amount
//       // of symbols reduced.
//       if (len > parse_stack.size()) {
//         return false;
//       }

//       // Build the rule from the sequence of symbols
//       Rule candidate{};
//       for (size_t i = 0; i < len; ++i)
//         candidate.symbols[i] = parse_stack[parse_stack.size() - len + i];

//       // Check if the rule exists in the grammar table
//       auto range = grammar.table.equal_range(candidate);
//       for (auto it = range.first; it != range.second; ++it) {
//         // Apply reduction
//         Symbol reduced_symbol = it->second(*this);

//         // Check if the reduced symbol is depends on a higher context
//         if (reduced_symbol.type == Symbol::Type::DEPENDS_ON_CONTEXT) {
//           continue;
//         }

//         // Remove the symbols that were reduced from the stack
//         for (size_t i = 0; i < len; ++i) parse_stack.pop_back();

//         // Push the new reduced grammar into stack
//         parse_stack.push_back(reduced_symbol);
//         reduced = true;
//         break;
//       }
//     }

//     return reduced;
//   }

//   Symbol Parser::tokenToSymbol(const Token& token) const noexcept {
//     Symbol s = {};
//     switch (token.type) {
//       case TokenType::KEYWORD:
//         s.type = Symbol::Type::KEYWORD;
//         s.terminal.keyword = token.value.keyword;
//         break;
//       case TokenType::PUNCTUATOR:
//         s.type = Symbol::Type::PUNCTUATOR;
//         s.terminal.punctuator = token.value.punctuator;
//         break;
//       case TokenType::IDENTIFIER:
//         s.type = Symbol::Type::IDENTIFIER;
//         s.terminal.ident_or_lit = 0;
//         break;
//       case TokenType::CHAR_LITERAL:
//       case TokenType::STRING_LITERAL:
//       case TokenType::BOOLEAN_LITERAL:
//       case TokenType::INTEGER_LITERAL:
//       case TokenType::FLOATING_LITERAL:
//         s.type = Symbol::Type::LITERAL;
//         s.terminal.ident_or_lit = 1;
//         break;
//       default:
//         // This should never happen
//         s.type = Symbol::Type::UNKNOWN;
//         s.nonterminal = NonTerminal::UNKNOWN;
//     }
//     return s;
//   }

// }  // namespace compiler
