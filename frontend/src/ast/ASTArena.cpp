// #include "ASTArena.hpp"

// namespace compiler {

//   ASTArena::ASTArena() noexcept
//       : top_index(0),
//         page_count(0),
//         node_count(0),
//         head(nullptr),
//         free_nodes() {}

//   ASTArena::~ASTArena() noexcept {
//     clear();
//     while (!free_nodes.empty()) {
//       free_nodes.pop();
//     }
//   }

//   size_t ASTArena::used() const { return node_count; }
//   size_t ASTArena::available() const { return page_count * Page::PAGE_SIZE; }

//   void ASTArena::free(ASTNode* node) {
//     if (node->is_free) {
//       // Already freed, do nothing
//       return;
//     }
//     node->is_free = true;
//     node_count--;

//     // TODO: Add to a stack the address of the freed block to use next
//     // time a new allocation is set up.
//     free_nodes.push(node);
//   }

//   void ASTArena::clear() {
//     while (head) {
//       removePage();
//     }
//   }

//   void ASTArena::addPage() {
//     // Create a new page and make head reference it.
//     Page* new_page = new Page;
//     new_page->prev = head;
//     head = new_page;

//     // Set the top of the index inside the page to zero
//     // and increase the page count.
//     top_index = 0;
//     ++page_count;
//   }

//   void ASTArena::removePage() {
//     if (!head) return;
//     // Cache current page and obtain previous page reference.
//     Page* old_page = head;
//     head = head->prev;
//     delete old_page;

//     // Reset top index and page count
//     top_index = Page::PAGE_SIZE;
//     --page_count;
//   }

// }  // namespace compiler
