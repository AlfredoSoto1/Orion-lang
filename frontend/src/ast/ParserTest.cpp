
#include "ParserTest.hpp"

namespace compiler {

  Parser::Parser(TokenStream& tokens) noexcept : tokens(tokens) {}

  void Parser::parse() {
    while (tokens.hasNext()) {
      // Push to stack the next token
      parse_stack.push(tokens.next());

      // check if the current state is reducible.
      // To know if it is reducible, it needs to check its punctuators.
      // for example, if we hit '(' that means that its either a function call
      // or a function declaration. We know that its a function declaration
      // because it has to tokens prior a TypeSpecifier Identifier '('. This
      // means that it can deduce that its a function delcaration, meaning that
      // its expecting function parameters. Then, parameters are given as
      // Typespecifier identifier, ....

      if (parse_stack.size() >= 3) {
        Token third_from_top = parse_stack.at(
            parse_stack.size() - 3);  // The third token from the top
        Token second_from_top = parse_stack.at(
            parse_stack.size() - 2);     // The second token from the top
        Token top = parse_stack.back();  // The most recent token

        if (third_from_top.type == TokenType::KEYWORD &&
            second_from_top.type == TokenType::IDENTIFIER &&
            top.type == TokenType::PUNCTUATOR &&
            std::get<Punctuator>(top.value.value()) == Punctuator::LPAREN) {
          // We have encountered a potential function declaration
          // Now, we expect function parameters
          // parse_function_parameters();
        }
      }
    }
  }

  bool Parser::reduce() {
    if (!isReducible()) return false;

    // Apply the reduction rule
    applyReduction();
    return true;
  }

  bool Parser::isReducible() {
    // Define the patterns we want to reduce
    if (parse_stack.size() < 3) return false;

    // For example: reducing a return expression like "return expr;"
    Token& top = parse_stack.top();
    if (top.type == TokenType::KEYWORD &&
        std::get<Keyword>(top.value.value()) == Keyword::RETURN) {
      return true;
    }

    return false;
  }

  void Parser::applyReduction() {
    // Apply a reduction rule based on the top of the stack
    Token top = parse_stack.top();
    parse_stack.pop();

    // Example: If "return" is at the top, reduce it to a statement
    if (top.type == TokenType::KEYWORD &&
        std::get<Keyword>(top.value.value()) == Keyword::RETURN) {
      Token statement = {TokenType::PUNCTUATOR, {}};
      parse_stack.push(statement);
    }
  }

  bool Parser::isTerminal(const Token& token) {
    // A terminal is a token that we don't reduce further, e.g., identifiers,
    // literals
    return token.type == TokenType::LITERAL ||
           token.type == TokenType::IDENTIFIER;
  }
}  // namespace compiler
