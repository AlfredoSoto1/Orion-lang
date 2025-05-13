#include "ParseStack.hpp"

namespace compiler {

  ParseStack::ParseStack() noexcept
      : top_index(0), page_count(0), stack_size(0), head(nullptr) {}

  ParseStack::~ParseStack() noexcept { clear(); }

  bool ParseStack::Iterator::hasNext() {
    return curr_page != nullptr && top >= 0;
  }

  void ParseStack::Iterator::next() {
    if (top > 0) {
      --top;
    } else if (curr_page->prev != nullptr) {
      curr_page = curr_page->prev;
      top = Page::PAGE_SIZE - 1;
    } else {
      top = -1;
      curr_page = nullptr;
    }
  }

  ASTSymbolState& ParseStack::Iterator::operator*() {
    return curr_page->nodes[top];
  }
  ASTSymbolState* ParseStack::Iterator::operator->() {
    return &curr_page->nodes[top];
  }

  void ParseStack::push(ASTSymbolState&& ast_state_symbol) {
    if (!head || top_index >= Page::PAGE_SIZE) {
      // Add a new page when overflows
      addPage();
    }

    // Emplace on page the
    new (&head->nodes[top_index++]) ASTSymbolState{
        std::move(ast_state_symbol.symbol),  // Symbol
        std::move(ast_state_symbol.state)    // State
    };
    stack_size++;
  }

  void ParseStack::pop(int32_t stack_ptr) {
    stack_size -= stack_ptr;
    top_index -= stack_ptr;

    while (top_index < 0) {
      // If no pages are left, end popping
      if (!head || page_count == 0) {
        top_index = 0;
        return;  // do nothing
      }

      // Obtain the difference between [below_top, 0)
      // Remove the page and with the difference saved,
      // reduce the top_index by the difference amount.
      int32_t diff = -top_index;
      removePage();
      top_index -= diff;
    }
  }

  const ASTSymbolState& ParseStack::peekTop() const {
    return head->nodes[top_index - 1];
  }

  ParseStack::Iterator ParseStack::peekTop(int32_t stack_ptr) {
    return Iterator{head, top_index};
  }

  size_t ParseStack::size() const { return stack_size; }

  bool ParseStack::isEmpty() const { return stack_size == 0; }

  void ParseStack::addPage() {
    // Create a new page and make head reference it.
    Page* new_page = new Page;
    new_page->prev = head;
    head = new_page;

    // Set the top of the index inside the page to zero
    // and increase the page count.
    top_index = 0;
    ++page_count;
  }

  void ParseStack::removePage() {
    if (!head) return;
    // Cache current page and obtain previous page reference.
    Page* old_page = head;
    head = head->prev;
    delete old_page;

    // Reset top index and page count
    top_index = Page::PAGE_SIZE;
    --page_count;
  }

  void ParseStack::clear() {
    while (head) {
      removePage();
    }
  }
}  // namespace compiler
