
#include "ParserTest.hpp"

namespace compiler {

  Parser::Parser(TokenStream& tokens) noexcept : tokens(tokens), ast_stack() {}

  void Parser::parse() {
    // Repeat until stack is 1 and all tokens are consumed
    while (ast_stack.size() > 1 || tokens.hasNext()) {
      // Check the top of the ast_stack, it will let us know which grammar rule
      // we are currently applying.

      // Check if the detected grammar allows a reduction
      if (isReducible()) {
        applyReduction();
      } else {
        // If we can't reduce, shift the next token into the AST stack
        ast_stack.push(ASTNode{GrammarType::UNFINISHED, tokens.peek()});
        tokens.next();  // Resolve lexer error
      }
    }
  }

  bool Parser::reduce() {
    // Take the AST and reduce it to a single node.
    // This gets pushed to the ast_stack and marked
    // as unfinished if the AST is not completed.

    // If the AST can be built completely, apply a
    // reduction and push it to the stack.
  }

  bool Parser::isReducible() {
    // Check if the currenct set of tokens match to a grammar.
  }

  void Parser::applyReduction() {
    // Combine multiple AST from stack and push it back to stack.
  }

  // std::shared_ptr<ASTNode> makeASTLeaf(const compiler::Token& token) {
  //   // Here, we decide based on token type.
  //   // For example, an identifier or literal is a leaf.
  //   switch (token.type) {
  //     case compiler::TokenType::IDENTIFIER:
  //       return std::make_shared<ASTNode>(
  //           NodeType::IDENTIFIER,
  //           std::get<compiler::Identifier>(token.value.value()).name);
  //     case compiler::TokenType::LITERAL:
  //       // For literal, convert to string (this is simplistic)
  //       return std::make_shared<ASTNode>(NodeType::LITERAL, "literal");
  //     case compiler::TokenType::KEYWORD:
  //       // Keywords can be leaf nodes too.
  //       // (You may need to convert the keyword to a string.)
  //       return std::make_shared<ASTNode>(NodeType::IDENTIFIER, "keyword");
  //     case compiler::TokenType::PUNCTUATOR:
  //       // Punctuators may be used in operators.
  //       return std::make_shared<ASTNode>(NodeType::PUNCTUATOR, "punctuator");
  //     default:
  //       return std::make_shared<ASTNode>(NodeType::IDENTIFIER, "unknown");
  //   }
  // }

  // class Parser {
  // public:
  //   explicit Parser(TokenStream& tokens) : tokens(tokens) {}

  //   void parse() {
  //     while (tokens.hasNext()) {
  //       parse_stack.push(tokens.next());
  //       reduce();
  //     }
  //   }

  // private:
  //   TokenStream& tokens;
  //   std::stack<Token> parse_stack;

  //   void reduce() {
  //     if (parse_stack.size() >= 3) {
  //       Token third = parse_stack.top();
  //       parse_stack.pop();
  //       Token second = parse_stack.top();
  //       parse_stack.pop();
  //       Token first = parse_stack.top();

  //       if (isFunctionDefinition(first, second, third)) {
  //         parseFunction();
  //       } else if (isVariableDeclaration(first, second, third)) {
  //         parseVariableAssignment();
  //       } else {
  //         parse_stack.push(second);
  //         parse_stack.push(third);
  //       }
  //     }
  //   }

  //   bool isFunctionDefinition(const Token& first, const Token& second,
  //                             const Token& third) {
  //     return first.type == TokenType::KEYWORD &&
  //            second.type == TokenType::IDENTIFIER &&
  //            std::get<Punctuator>(third.value.value()) == Punctuator::LPAREN;
  //   }

  //   bool isVariableDeclaration(const Token& type, const Token& ident,
  //                              const Token& punctuator) {
  //     return type.type == TokenType::KEYWORD &&
  //            ident.type == TokenType::IDENTIFIER &&
  //            std::get<Punctuator>(punctuator.value.value()) ==
  //            Punctuator::EQ;
  //   }

  //   void parseFunction() {
  //     while (tokens.hasNext()) {
  //       Token next = tokens.next();
  //       parse_stack.push(next);
  //       if (next.value == ")") break;
  //     }
  //     Token body_start = tokens.next();
  //     if (body_start.value != "{") {
  //       throw std::runtime_error("Expected '{' at start of function body");
  //     }
  //     parse_stack.push(body_start);
  //     parseBlock();
  //   }

  //   void parseVariableAssignment() {
  //     Token expr = tokens.next();
  //     if (expr.type != TokenType::LITERAL &&
  //         expr.type != TokenType::IDENTIFIER) {
  //       throw std::runtime_error("Expected a literal or identifier after
  //       '='");
  //     }
  //     parse_stack.push(expr);
  //     Token semicolon = tokens.next();
  //     if (semicolon.value != ";") {
  //       throw std::runtime_error("Expected ';' at end of statement");
  //     }
  //     parse_stack.push(semicolon);
  //   }

  //   void parseBlock() {
  //     while (tokens.hasNext()) {
  //       Token next = tokens.next();
  //       parse_stack.push(next);
  //       if (next.value == "}") break;
  //       if (next.type == TokenType::KEYWORD &&
  //           (next.value == "if" || next.value == "while" ||
  //            next.value == "for" || next.value == "do" ||
  //            next.value == "switch")) {
  //         parseControlStructure();
  //       }
  //     }
  //   }

  //   void parseControlStructure() {
  //     Token condition_start = tokens.next();
  //     if (condition_start.value != "(") {
  //       throw std::runtime_error("Expected '(' after control keyword");
  //     }
  //     parse_stack.push(condition_start);
  //     while (tokens.hasNext()) {
  //       Token next = tokens.next();
  //       parse_stack.push(next);
  //       if (next.value == ")") break;
  //     }
  //     Token body_start = tokens.next();
  //     if (body_start.value == "{") {
  //       parse_stack.push(body_start);
  //       parseBlock();
  //     }
  //   }
  // };
}  // namespace compiler
