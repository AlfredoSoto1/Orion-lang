#pragma once

#include <variant>
#include <vector>

#include "Tokens.hpp"

namespace compiler {

  enum class Grammar : uint64_t;

  /**
   * @brief This needs to be an interface
   *
   */
  struct ASTNode {
    Grammar grammar;
    Token token;
    std::vector<uint64_t> branches;
  };

  class ASTArena final {
  public:
    explicit ASTArena() noexcept;
    ~ASTArena() noexcept;

    size_t used() const;
    size_t available() const;

    template <typename... Args>
    ASTNode* allocate(Args&&... args) {
      if (!head || top_index >= Page::PAGE_SIZE) {
        // Add a new page when overflows
        addPage();
      }
      // Emplace node into page slot
      node_count++;
      ASTNodeBlock* block = new (&head->nodes[top_index++])
          ASTNodeBlock{{std::forward<Args>(args)...}, false};
      return &block->data;
    }

    void free(ASTNode* node);

  private:
    struct ASTNodeBlock {
      ASTNode data;
      bool is_free = true;
    };

    struct Page {
      static constexpr uint8_t PAGE_SIZE = 64;
      ASTNodeBlock nodes[PAGE_SIZE]{};
      Page* prev = nullptr;
    };

  private:
    uint8_t top_index;
    uint64_t page_count;
    uint64_t node_count;
    Page* head;

  private:
    void clear();
    void addPage();
    void removePage();
  };

  enum class Grammar : uint64_t {
    UNKNOWN = 0,
    PUNC,
    EXPR,
    FACTOR,
    LITERAL,
    OPERATOR,
    TERM,
    IDENT,
    LPAREN,
    RPAREN,
    KEYWORD,

    // DECLARATION,
    // FUNCTION,
    // PARAMETER,
    // BLOCK,
    // STATEMENT,
    // EXPRESSION,
    // TERM,
    // IDENTIFIER,
    // LITERAL,
    // OPERATOR,
    // UNFINISHED,

    // UNKNOWN,
    // KEYWORD,
    // LITERAL,
    // IDENTIFIER,
    // PUNCTUATOR,
    // COMMENT,
    // ENDOF,
  };
}  // namespace compiler
