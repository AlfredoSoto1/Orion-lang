#include "ASTProgram.hpp"

namespace compiler {

  ASTProgram::ASTProgram() noexcept
      : top_index(0), page_count(0), node_count(0), head(nullptr) {}

  ASTProgram::~ASTProgram() noexcept { clear(); }

  void ASTProgram::emplace(const ASTNode&& node) {
    if (!head || top_index >= Page::PAGE_SIZE) {
      // Add a new page when overflows
      addPage();
    }

    // Emplace node into page slot
    ASTNode* ref = new (&head->nodes[top_index++]) ASTNode(std::move(node));
    ref->branches.emplace_back(top_index++);
    node_count++;
  }

  // const ASTNode& ASTProgram::head() const {

  // }

  size_t ASTProgram::size() const { return node_count; }

  void ASTProgram::clear() {
    while (head) {
      removePage();
    }
  }

  void ASTProgram::addPage() {
    // Create a new page and make head reference it.
    Page* new_page = new Page;
    new_page->prev = head;
    head = new_page;

    // Set the top of the index inside the page to zero
    // and increase the page count.
    top_index = 0;
    ++page_count;
  }

  void ASTProgram::removePage() {
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
