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

    void emplace(const ASTNode&& node);

    const ASTNode& start() const;

    size_t size() const;

  private:
    /**
     * @brief
     *
     */
    struct Page {
      static constexpr uint8_t PAGE_SIZE = 64;
      ASTNode* nodes[PAGE_SIZE]{};
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
