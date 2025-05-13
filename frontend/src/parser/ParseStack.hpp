#pragma once

#include <cstdint>

#include "ActionTable.hpp"
#include "Symbols.hpp"

namespace compiler {

  struct ASTSymbolState {
    Symbol symbol;
    ActionTable::State state;
  };

  class ParseStack final {
  public:
    struct Page;

    struct Iterator {
      Page* curr_page = nullptr;
      int32_t top;

      void next();
      bool hasNext();

      ASTSymbolState& operator*();
      ASTSymbolState* operator->();
    };

  public:
    /**
     * @brief Construct a new ParseStack object
     *
     */
    explicit ParseStack() noexcept;
    ~ParseStack() noexcept;

    /**
     * @brief
     *
     * @param node
     */
    void push(ASTSymbolState&& ast_state_symbol);

    /**
     * @brief
     *
     */
    void pop(int32_t stack_ptr);

    /**
     * @brief
     *
     * @return const ASTSymbolState&
     */
    const ASTSymbolState& peekTop() const;

    /**
     * @brief
     *
     * @param top
     */
    Iterator peekTop(int32_t stack_ptr);

    /**
     * @brief
     *
     * @return size_t
     */
    size_t size() const;

    /**
     * @brief
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
      ASTSymbolState nodes[PAGE_SIZE]{};
      Page* prev = nullptr;
    };

  private:
    int32_t top_index;
    uint32_t page_count;
    uint32_t stack_size;
    Page* head;

  private:
    void clear();
    void addPage();
    void removePage();
  };
}  // namespace compiler
