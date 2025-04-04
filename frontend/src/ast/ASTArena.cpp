#include "ASTArena.hpp"

namespace compiler {

  ASTArena::ASTArena() noexcept
      : top_index(0), page_count(0), node_count(0), head(nullptr) {}

  ASTArena::~ASTArena() noexcept { clear(); }

  size_t ASTArena::used() const { return node_count; }
  size_t ASTArena::available() const { return page_count * Page::PAGE_SIZE; }

  void ASTArena::free(ASTNode* node) {
    // The address of the node must be the same as the address of the block
    // in the page. This is a bit of a hack, but it works because we control the
    // allocation of the nodes in the arena.
    ASTNodeBlock* block = reinterpret_cast<ASTNodeBlock*>(node);
    if (block->is_free) {
      // Already freed, do nothing
      return;
    }
    block->is_free = true;
    node_count--;
    // TODO: Add to a stack the address of the freed block to use next
    // time a new allocation is set up.
  }

  void ASTArena::clear() {
    while (head) {
      removePage();
    }
  }

  void ASTArena::addPage() {
    // Create a new page and make head reference it.
    Page* new_page = new Page;
    new_page->prev = head;
    head = new_page;

    // Set the top of the index inside the page to zero
    // and increase the page count.
    top_index = 0;
    ++page_count;
  }

  void ASTArena::removePage() {
    if (!head) return;
    // Cache current page and obtain previous page reference.
    Page* old_page = head;
    head = head->prev;
    delete old_page;

    // Reset top index and page count
    top_index = Page::PAGE_SIZE;
    --page_count;
  }

}  // namespace compiler
