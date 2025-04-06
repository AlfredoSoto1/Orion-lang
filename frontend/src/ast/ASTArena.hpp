#pragma once

#include <variant>
#include <vector>

#include "ASTRules.hpp"
#include "Tokens.hpp"

namespace compiler {

  class ASTArena final {
  public:
    /**
     * @brief Construct a new ASTArena object.
     *
     */
    explicit ASTArena() noexcept;
    ~ASTArena() noexcept;

    /**
     * @brief Returns the number of allocated nodes.
     *
     * @return size_t
     */
    size_t used() const;

    /**
     * @brief Returns the number of available nodes.
     *
     * @return size_t
     */
    size_t available() const;

    /**
     * @brief Allocates a new ASTNode and returns a pointer to it.
     *
     * @param rule
     * @param content
     * @return ASTNode*
     */
    ASTNode* allocate(const Rule& rule, const ASTNode::Value& content);

    /**
     * @brief Frees the given ASTNode.
     *
     * @param node
     */
    void free(ASTNode* node);

  private:
    struct ASTNodeBlock {
      bool is_free = true;
      ASTNode data{};
    };

    struct Page {
      static constexpr uint8_t PAGE_SIZE = 64;
      Page* prev = nullptr;
      ASTNodeBlock nodes[PAGE_SIZE]{};
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
}  // namespace compiler
