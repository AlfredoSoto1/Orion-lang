#pragma once

#include "ASTArena.hpp"
#include "ASTRules.hpp"

namespace compiler {

  class ASTStack final {
  public:
    /**
     * @brief Construct a new ASTStack object
     *
     */
    explicit ASTStack() noexcept;
    ~ASTStack() noexcept;

    /**
     * @brief Shifts into stack a node
     *
     * @param node
     */
    void shift(ASTNode* node);

    /**
     * @brief Pops the stack
     *
     */
    void pop(uint8_t stack_ptr);

    /**
     * @brief Peeks the first size elements in the stack
     *        and stores it in buffer. The first element
     *        of the buffer is the top of the stack.
     *
     * @param buffer
     * @param available
     * @param size
     */
    void peekTop(ASTNode** buffer, uint64_t* available, uint8_t size);

    /**
     * @brief Size of the stack being in use
     *
     * @return size_t
     */
    size_t size() const;

    /**
     * @brief Checks if the stack has any nodes.
     *
     * @return true if empty false otherwise
     */
    bool isEmpty() const;

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
    uint64_t stack_size;
    Page* head;

  private:
    void clear();
    void addPage();
    void removePage();
  };
}  // namespace compiler
