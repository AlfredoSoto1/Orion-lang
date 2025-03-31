#pragma once

#include <array>
#include <memory>

#include "ASTProgram.hpp"

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
     *        and stores it in buffer.
     *
     */
    void peekTop(ASTNode** buffer, uint8_t size);

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
    Page* head;

  private:
    void clear();
    void addPage();
    void removePage();
  };
}  // namespace compiler
