#include "ASTStack.hpp"

namespace compiler {

  ASTStack::ASTStack() noexcept : top_index(0), page_count(0), head(nullptr) {}

  ASTStack::~ASTStack() noexcept { clear(); }

  void ASTStack::shift(ASTNode* node) {
    if (!head || top_index >= Page::PAGE_SIZE) {
      addPage();
    }
    head->nodes[top_index++] = node;
  }

  void ASTStack::pop(uint8_t stack_ptr) {
    for (uint8_t i = 0; i < stack_ptr; ++i) {
      // If the index is zero, it will force to remove a page
      // and continue popping.
      if (top_index == 0) {
        if (!head || page_count == 0) {
          // throw std::runtime_error("ASTStack underflow");
        }
        removePage();
      }
      --top_index;
    }
  }

  void ASTStack::peekTop(ASTNode** buffer, uint8_t size) {
    if (!head || size == 0) return;

    uint64_t count = 0;
    Page* current = head;
    uint64_t index = top_index;

    while (current && count < size) {
      while (index > 0 && count < size) {
        buffer[count++] = current->nodes[--index];
      }
      current = current->prev;
      index = current ? Page::PAGE_SIZE : 0;
    }
  }

  void ASTStack::addPage() {
    // Create a new page and make head reference it.
    Page* new_page = new Page;
    new_page->prev = head;
    head = new_page;

    // Set the top of the index inside the page to zero
    // and increase the page count.
    top_index = 0;
    ++page_count;
  }

  void ASTStack::removePage() {
    if (!head) return;
    // Cache current page and obtain previous page reference.
    Page* old_page = head;
    head = head->prev;
    delete old_page;

    // Reset top index and page count
    top_index = Page::PAGE_SIZE;
    --page_count;
  }

  void ASTStack::clear() {
    while (head) {
      removePage();
    }
  }
}  // namespace compiler
