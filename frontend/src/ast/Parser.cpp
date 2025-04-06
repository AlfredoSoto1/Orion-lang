#include "Parser.hpp"

#include <iostream>
#include <string>
#include <variant>

namespace compiler {

  Parser::Parser(TokenStream& tokens) noexcept : tokens(tokens), ast_stack() {}

  void Parser::parse() {
    // Repeat until stack is 1 and all tokens are consumed
    while (ast_stack.size() > 1 || tokens.hasNext()) {
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

    // Determine the appropriate Rule based on the token type
    Rule rule = Rule::UNKNOWN;
    ASTNode::Value value{};

    // TODO: Handle the cases to determine the rule and value
    // This is a simplified example; you may need to adjust it based on your
    // grammar and AST structure.
    switch (tok.type) {
      case TokenType::KEYWORD:
        rule = Rule::KEYWORD;
        value.keyword = tok.value.keyword;
        break;
      case TokenType::IDENTIFIER:
        rule = Rule::ID;
        break;
      case TokenType::STRING_LITERAL:
        rule = Rule::LITERAL;
        break;
      case TokenType::PUNCTUATOR:
        // rule = Rule::PUNC;
        break;
      default:
        rule = Rule::UNKNOWN;  // THIS SHOULD NEVER HAPPEN
        return;                // Throw error HERE
    }
    // Push the token with the determined grammar type onto the stack
    ASTNode* node = ast_arena.allocate(rule, value);
    ast_stack.shift(node);
    tokens.next();
  }

  bool Parser::tryReduce() {
    using enum Rule;

    if (ast_stack.isEmpty()) {
      return false;
    }

    uint64_t available = 0;
    ASTNode* top[sizeof(uint64_t)];

    ast_stack.peekTop(top, &available, sizeof(uint64_t));
    // Check if the top of the stack matches any of the reduction rules

    uint64_t rule = static_cast<uint8_t>(Rule::UNKNOWN);

    for (uint8_t i = 0; i < available; i++) {
      rule |= static_cast<uint8_t>(top[available - i - 1]->rule) << i;
    }

    // Prepare 3 nodes used for reference
    // ASTNode* top[3];

    // auto match = [&](std::initializer_list<Grammar> pattern) -> bool {
    //   if (ast_stack.size() < pattern.size()) {
    //     return false;
    //   }
    //   std::stack<ASTNode> temp;

    //   // Loop through the pattern using index-based access
    //   for (Grammar grammar : pattern) {
    //     if (ast_stack.top().grammar != grammar) {
    //       // Restore stack on failure
    //       while (!temp.empty()) {
    //         ast_stack.push(temp.top());
    //         temp.pop();
    //       }
    //       return false;
    //     }
    //     temp.push(ast_stack.top());  // Save valid matches
    //     ast_stack.pop();
    //   }

    //   // Restore stack
    //   while (!temp.empty()) {
    //     ast_stack.push(temp.top());
    //     temp.pop();
    //   }
    //   return true;
    // };

    // This is where you implement the reduction logic based on your grammar
    // rules. You need to examine the top elements of the ast_stack_ and see
    // if they match the right-hand side of any of your grammar rules.

    // Example grammar rules (very simplified):
    // 1. Expression -> Expression + Term
    // 2. Expression -> Term
    // 3. Term -> Term * Factor
    // 4. Term -> Factor
    // 5. Factor -> IDENTIFIER
    // 6. Factor -> NUMBER
    // 7. Factor -> ( Expression )

    // Implement checks for these rules (or your actual grammar)

    // // Rule 1: Expression -> Expression + Term
    // if (ast_stack.size() >= 3 && match({PUNC, EXPR, TERM})) {
    //   ASTNode term = ast_stack.top();
    //   ast_stack.pop();
    //   ASTNode plus_op = ast_stack.top();
    //   ast_stack.pop();
    //   ASTNode expr = ast_stack.top();
    //   ast_stack.pop();

    //   if (plus_op.grammar == PUNC &&
    //       std::get<Punctuator>(std::get<Token>(plus_op.value).value.value())
    //       ==
    //           Punctuator::PLUS) {
    //     ast_stack.push(
    //         ASTNode{EXPR, std::vector<ASTNode>{expr, plus_op, term}});
    //     return true;
    //   } else {
    //     // Restore stack if the operator is not a '+'
    //     ast_stack.push(expr);
    //     ast_stack.push(plus_op);
    //     ast_stack.push(term);
    //   }
    // }

    // // Rule 3: Term -> Term * Factor
    // if (ast_stack.size() >= 3 && match({FACTOR, PUNC, TERM})) {
    //   ASTNode factor = ast_stack.top();
    //   ast_stack.pop();
    //   ASTNode mult_op = ast_stack.top();
    //   ast_stack.pop();
    //   ASTNode term = ast_stack.top();
    //   ast_stack.pop();

    //   if (mult_op.grammar == PUNC &&
    //       std::get<Punctuator>(std::get<Token>(mult_op.value).value.value())
    //       ==
    //           Punctuator::STAR) {
    //     ast_stack.push(
    //         ASTNode{TERM, std::vector<ASTNode>{term, mult_op, factor}});
    //     return true;
    //   } else {
    //     // Restore stack if the operator is not '*'
    //     ast_stack.push(term);
    //     ast_stack.push(mult_op);
    //     ast_stack.push(factor);
    //   }
    // }

    // // Rule 7: Factor -> ( Expression )
    // if (ast_stack.size() >= 3 && match({RPAREN, EXPR, LPAREN})) {
    //   ASTNode rparen = ast_stack.top();
    //   ast_stack.pop();
    //   ASTNode expression = ast_stack.top();
    //   ast_stack.pop();
    //   ASTNode lparen = ast_stack.top();
    //   ast_stack.pop();

    //   if (lparen.grammar == LPAREN && rparen.grammar == RPAREN) {
    //     ast_stack.push(
    //         ASTNode{FACTOR, std::vector<ASTNode>{lparen, expression,
    //         rparen}});
    //     return true;
    //   } else {
    //     // Restore stack if parentheses are not valid
    //     ast_stack.push(lparen);
    //     ast_stack.push(expression);
    //     ast_stack.push(rparen);
    //   }
    // }

    // // Rule 5: Factor -> IDENTIFIER
    // if (match({IDENT})) {
    //   ASTNode identifier = ast_stack.top();
    //   ast_stack.pop();
    //   ast_stack.push(ASTNode{FACTOR, std::vector<ASTNode>{identifier}});
    //   return true;
    // }

    // // Rule 6: Factor -> NUMBER
    // if (match({LITERAL})) {
    //   ASTNode literal = ast_stack.top();
    //   ast_stack.pop();
    //   ast_stack.push(ASTNode{FACTOR, std::vector<ASTNode>{literal}});
    //   return true;
    // }

    // // Rule 4: Term -> Factor
    // if (match({FACTOR})) {
    //   ASTNode factor = ast_stack.top();
    //   ast_stack.pop();
    //   ast_stack.push(ASTNode{TERM, std::vector<ASTNode>{factor}});
    //   return true;
    // }

    // // Rule 2: Expression -> Term
    // if (match({TERM})) {
    //   ASTNode term = ast_stack.top();
    //   ast_stack.pop();
    //   ast_stack.push(ASTNode{EXPR, std::vector<ASTNode>{term}});
    //   return true;
    // }

    // If no match is found, return false
    return false;
  }

  //   bool Parser::tryReduce() {
  //     using enum Grammar;

  //     if (ast_stack.empty()) {
  //         return false;
  //     }

  // // Reduction rules
  // std::vector<ASTNode> nodes;

  // auto match = [&](std::initializer_list<Grammar> pattern) -> bool {
  //     if (ast_stack.size() < pattern.size()) {
  //         return false;
  //     }
  //     nodes.clear();
  //     for (auto it = pattern.begin(); it != pattern.end(); ++it) {
  //         nodes.push_back(ast_stack.top());
  //         ast_stack.pop();
  //         if (nodes.back().grammar != *it) {
  //             for (auto rit = nodes.rbegin(); rit != nodes.rend(); ++rit)
  //             {
  //                 ast_stack.push(*rit);
  //             }
  //             return false;
  //         }
  //     }
  //     std::reverse(nodes.begin(), nodes.end());
  //     return true;
  // };

  //     auto reduce = [&](Grammar result) {
  //         ast_stack.emplace(ASTNode{result, nodes});
  //     };

  //     // Primary expression
  //     if (match({IDEN}) || match({LITERAL}) || match({PUNC, EXPR, PUNC}))
  // {
  //         reduce(PRIMARY_EXPR);
  //         return true;
  //     }

  //     // Postfix expressions
  //     if (match({PRIMARY_EXPR})) {
  //         reduce(POSTFIX_EXPR);
  //         return true;
  //     }
  //     if (match({POSTFIX_EXPR, PUNC, EXPR, PUNC}) || match({POSTFIX_EXPR,
  //     PUNC, PUNC})) {
  //         reduce(POSTFIX_EXPR);
  //         return true;
  //     }
  //     if (match({POSTFIX_EXPR, PUNC, IDEN}) || match({POSTFIX_EXPR, PUNC,
  //     PUNC}) ||
  //         match({POSTFIX_EXPR, PUNC}) || match({POSTFIX_EXPR, PUNC})) {
  //         reduce(POSTFIX_EXPR);
  //         return true;
  //     }

  //     // Unary expressions
  //     if (match({POSTFIX_EXPR}) || match({PUNC, UNARY_EXPR}) ||
  // match({PUNC,
  //     TYPE_NAME, PUNC, UNARY_EXPR})) {
  //         reduce(UNARY_EXPR);
  //         return true;
  //     }

  //     // Cast expressions
  //     if (match({UNARY_EXPR}) || match({PUNC, TYPE_NAME, PUNC,
  // CAST_EXPR})) {
  //         reduce(CAST_EXPR);
  //         return true;
  //     }

  //     // Multiplicative expressions
  //     if (match({CAST_EXPR}) || match({MULT_EXPR, PUNC, CAST_EXPR})) {
  //         reduce(MULT_EXPR);
  //         return true;
  //     }

  //     // Additive expressions
  //     if (match({MULT_EXPR}) || match({ADD_EXPR, PUNC, MULT_EXPR})) {
  //         reduce(ADD_EXPR);
  //         return true;
  //     }

  //     // Shift expressions
  //     if (match({ADD_EXPR}) || match({SHIFT_EXPR, PUNC, ADD_EXPR})) {
  //         reduce(SHIFT_EXPR);
  //         return true;
  //     }

  //     // Relational expressions
  //     if (match({SHIFT_EXPR}) || match({REL_EXPR, PUNC, SHIFT_EXPR})) {
  //         reduce(REL_EXPR);
  //         return true;
  //     }

  //     // Equality expressions
  //     if (match({REL_EXPR}) || match({EQ_EXPR, PUNC, REL_EXPR})) {
  //         reduce(EQ_EXPR);
  //         return true;
  //     }

  //     // Bitwise AND expressions
  //     if (match({EQ_EXPR}) || match({AND_EXPR, PUNC, EQ_EXPR})) {
  //         reduce(AND_EXPR);
  //         return true;
  //     }

  //     // Bitwise XOR expressions
  //     if (match({AND_EXPR}) || match({XOR_EXPR, PUNC, AND_EXPR})) {
  //         reduce(XOR_EXPR);
  //         return true;
  //     }

  //     // Bitwise OR expressions
  //     if (match({XOR_EXPR}) || match({OR_EXPR, PUNC, XOR_EXPR})) {
  //         reduce(OR_EXPR);
  //         return true;
  //     }

  //     // Logical AND expressions
  //     if (match({OR_EXPR}) || match({LOG_AND_EXPR, PUNC, OR_EXPR})) {
  //         reduce(LOG_AND_EXPR);
  //         return true;
  //     }

  //     // Logical OR expressions
  //     if (match({LOG_AND_EXPR}) || match({LOG_OR_EXPR, PUNC,
  // LOG_AND_EXPR}))
  //     {
  //         reduce(LOG_OR_EXPR);
  //         return true;
  //     }

  //     // Conditional expressions
  //     if (match({LOG_OR_EXPR}) || match({LOG_OR_EXPR, PUNC, EXPR, PUNC,
  //     COND_EXPR})) {
  //         reduce(COND_EXPR);
  //         return true;
  //     }

  //     // Assignment expressions
  //     if (match({COND_EXPR}) || match({UNARY_EXPR, PUNC, ASSIGN_EXPR})) {
  //         reduce(ASSIGN_EXPR);
  //         return true;
  //     }

  //     // Expression
  //     if (match({ASSIGN_EXPR}) || match({EXPR, PUNC, ASSIGN_EXPR})) {
  //         reduce(EXPR);
  //         return true;
  //     }

  //     return false;
  // }

}  // namespace compiler
