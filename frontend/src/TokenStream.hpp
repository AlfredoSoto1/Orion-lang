#pragma once

#include <expected>

#include "Lexer.hpp"

namespace compiler {

  class TokenStream final {
  public:
    /**
     * @brief Construct a new Token Stream object
     *
     * @param lexer
     * @param buffer_size as token length amount max(256)
     */
    explicit TokenStream(Lexer& lexer, uint8_t buffer_size) noexcept;

    ~TokenStream() noexcept;

    bool hasNext() const noexcept;
    const Token& peek() const noexcept;
    const Token& peekNext() const noexcept;

    Lexer::LexerResult next();

  private:
    Lexer& lexer;
    uint8_t bpos;
    uint8_t buffer_size;
    Token* buffer;
  };

}  // namespace compiler