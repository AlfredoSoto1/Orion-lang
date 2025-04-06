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
      // Emplace node into page slot
      node_count++;
      ASTNodeBlock* block = new (&head->nodes[top_index++])
          ASTNodeBlock{{std::forward<Args>(args)...}, false};
      return &block->data;
    }

    /**
     * @brief Frees the given ASTNode.
     *
     * @param node
     */
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
}  // namespace compiler
