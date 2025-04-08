#include "Parser.hpp"

#include <iostream>
#include <string>
#include <variant>

namespace compiler {

  Parser::Parser(TokenStream& tokens) noexcept : tokens(tokens), ast_stack() {}

  void Parser::parse() {
    // Repeat until stack is 1 and all tokens are consumed
    while (ast_stack.size() > 1) {
      // Check if there are any tokens left to process
      if (!tokens.hasNext()) {
        break;
      }

      bool reduced = tryReduce();
      if (!reduced) {
        shift();
      }
    }
  }

  void Parser::shift() {
    // If we can't reduce, shift the next token into the AST stack
    const Token& tok = tokens.peek();

    // Skip comment tokens
    if (tok.type == TokenType::COMMENT) {
      tokens.next();
      return;
    }

    // Determine the grammar type based on the token type
    Rule rule = Rule::UNKNOWN;

    switch (tok.type) {
      case TokenType::KEYWORD:
        rule = Rule::KEYWORD;
        break;
      case TokenType::IDENTIFIER:
        rule = Rule::ID;
        break;
      case TokenType::CHAR_LITERAL:
      case TokenType::STRING_LITERAL:
      case TokenType::BOOLEAN_LITERAL:
      case TokenType::INTEGER_LITERAL:
      case TokenType::FLOATING_LITERAL:
        rule = Rule::LITERAL;
        break;
      case TokenType::PUNCTUATOR:
        rule = Rule::PUNCTUATOR;
        break;
      default:
        rule = Rule::UNKNOWN;
        break;
    }

    // Push the token with the determined grammar type onto the stack
    ast_stack.shift(ast_arena.allocate(rule, tok));
    tokens.next();
  }

  bool Parser::tryReduce() {
    using enum Rule;

    if (ast_stack.isEmpty()) {
      return false;
    }

    ASTNode* top[4];
    ast_stack.peekTop(top);

    uint64_t encoded = 0;

    for (int i = 0; i < 4; ++i) {
      uint64_t rule_val =
          static_cast<uint64_t>(top[i] ? top[i]->rule : Rule::UNKNOWN);
      // each rule occupies 8 bits
      encoded |= (rule_val & 0xFF) << (i * 8);
    }

    // SINGLE_EXPR → ID
    constexpr uint64_t PAT_ID = (static_cast<uint8_t>(Rule::ID) << 24);

    // SINGLE_EXPR → *ID
    constexpr uint64_t PAT_STAR_ID =
        (static_cast<uint8_t>(Rule::ID) << 24) |
        (static_cast<uint8_t>(Rule::PUNCTUATOR) << 16);

    // SINGLE_EXPR → &ID
    constexpr uint64_t PAT_BAND_ID =
        (static_cast<uint8_t>(Rule::ID) << 24) |
        (static_cast<uint8_t>(Rule::PUNCTUATOR) << 16);

    // SINGLE_EXPR → LITERAL
    constexpr uint64_t PAT_LITERAL =
        (static_cast<uint8_t>(Rule::LITERAL) << 24);

    // SINGLE_EXPR → ID ( args )
    constexpr uint64_t PAT_ID_LPAREN_ARGS_RPAREN =
        (static_cast<uint8_t>(Rule::PUNCTUATOR) << 24) |
        (static_cast<uint8_t>(Rule::ARGS) << 16) |
        (static_cast<uint8_t>(Rule::PUNCTUATOR) << 8) |
        (static_cast<uint8_t>(Rule::ID));

    // SINGLE_EXPR → ( expr )
    constexpr uint64_t PAT_LPAREN_EXPR_RPAREN =
        (static_cast<uint8_t>(Rule::PUNCTUATOR) << 24) |
        (static_cast<uint8_t>(Rule::EXPR) << 16) |
        (static_cast<uint8_t>(Rule::PUNCTUATOR) << 8);

    // SINGLE_EXPR → ID
    if (encoded == PAT_ID) {
      top[3]->rule = SINGLE_EXPR;
      return true;
    }

    // SINGLE_EXPR → *ID
    if (encoded == PAT_STAR_ID &&
        top[2]->token.value.punctuator == Punctuator::STAR) {
      ast_stack.pop(2);
      ast_arena.free(top[2]);  // Free the '*'
      top[3]->rule = SINGLE_EXPR;
      ast_stack.shift(top[3]);
      return true;
    }

    // SINGLE_EXPR → &ID
    if (encoded == PAT_BAND_ID &&
        top[2]->token.value.punctuator == Punctuator::BAND) {
      ast_stack.pop(2);
      ast_arena.free(top[2]);  // Free the '&'
      top[3]->rule = SINGLE_EXPR;
      ast_stack.shift(top[3]);
      return true;
    }

    // SINGLE_EXPR → LITERAL
    if (encoded == PAT_LITERAL) {
      top[3]->rule = SINGLE_EXPR;
      return true;
    }

    // SINGLE_EXPR → ID ( args )
    if (encoded == PAT_ID_LPAREN_ARGS_RPAREN &&
        top[3]->token.value.punctuator == Punctuator::RPAREN &&
        top[1]->token.value.punctuator == Punctuator::LPAREN) {
      ast_stack.pop(4);
      ASTNode* call = ast_arena.allocate(SINGLE_EXPR);
      call->branch[0] = top[0];  // ID
      call->branch[1] = top[2];  // ARGS
      ast_arena.free(top[1]);    // LPAREN
      ast_arena.free(top[3]);    // RPAREN
      ast_stack.shift(call);
      return true;
    }

    // SINGLE_EXPR → ( expr )
    if (encoded == PAT_LPAREN_EXPR_RPAREN &&
        top[3]->token.value.punctuator == Punctuator::RPAREN &&
        top[1]->token.value.punctuator == Punctuator::LPAREN) {
      ast_stack.pop(3);
      ast_arena.free(top[1]);      // LPAREN
      ast_arena.free(top[3]);      // RPAREN
      top[2]->rule = SINGLE_EXPR;  // EXPR node
      ast_stack.shift(top[2]);
      return true;
    }

    // If no match is found, return false
    return false;
  }
}  // namespace compiler
