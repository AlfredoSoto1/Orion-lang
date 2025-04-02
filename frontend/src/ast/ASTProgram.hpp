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

  class ASTProgram final {
  public:
    /**
     * @brief Construct a new ASTProgram object
     *
     */
    explicit ASTProgram() noexcept;
    ~ASTProgram() noexcept;

    const ASTNode& start() const;

    /**
     * @brief Size of the program
     *
     * @return size_t
     */
    size_t size() const;

    /**
     * @brief Emplace a node at the top of the program
     *
     * @tparam Args
     * @param args
     */
    template <typename... Args>
    ASTNode* emplace(Args&&... args) {
      if (!head || top_index >= Page::PAGE_SIZE) {
        // Add a new page when overflows
        addPage();
      }

      // Emplace node into page slot
      node_count++;
      return new (&head->nodes[top_index++])
          ASTNode(std::forward<Args>(args)...);
    }

  private:
    /**
     * @brief
     *
     */
    struct Page {
      static constexpr uint8_t PAGE_SIZE = 64;
      ASTNode nodes[PAGE_SIZE]{};
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
