#pragma once

#include <stack>

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
     * @tparam Args
     * @param args
     * @return ASTNode*
     */
    template <typename... Args>
    ASTNode* allocate(Args&&... args) {
      if (!head || top_index >= Page::PAGE_SIZE) {
        // Add a new page when overflows
        addPage();
      }

      if (!free_nodes.empty()) {
        // Reuse a free node if available
        ASTNode* node = free_nodes.top();
        free_nodes.pop();
        node->is_free = false;
        node_count++;
        return new (node) ASTNode{false, std::forward<Args>(args)...};
      }

      // Emplace node into page slot
      node_count++;
      return new (&head->nodes[top_index++])
          ASTNode{false, std::forward<Args>(args)...};
    }

    /**
     * @brief Frees the given ASTNode.
     *
     * @param node
     */
    void free(ASTNode* node);

  private:
    struct Page {
      static constexpr uint8_t PAGE_SIZE = 64;
      Page* prev = nullptr;
      ASTNode nodes[PAGE_SIZE]{};
    };

  private:
    uint8_t top_index;
    uint64_t page_count;
    uint64_t node_count;
    Page* head;

    // TEMP
    std::stack<ASTNode*> free_nodes;

  private:
    void clear();
    void addPage();
    void removePage();
  };
}  // namespace compiler
