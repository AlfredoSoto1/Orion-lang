#pragma once

#include "lexer/Lexer.hpp"

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

  public:
    /**
     * @brief Checks if there is a next token in the stream.
     *
     * @return true
     * @return false
     */
    bool hasNext() const noexcept;

    /**
     * @brief Peeks the next token in the stream.
     *
     * @return const Token&
     */
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