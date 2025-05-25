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

    /**
     * @brief Peeks the next token in the stream without consuming it.
     *
     * @return const Token&
     */
    const Token& peekNext() const noexcept;

    /**
     * @brief Returns the next token in the stream.
     *
     * @return Lexer::LexerResult
     */
    Lexer::LexerResult next();

    /**
     * @brief Returns the current state of the lexer.
     *
     * @return const LexerState&
     */
    const LexerState& state() const noexcept;

  private:
    Lexer& lexer;
    uint8_t bpos;
    uint8_t buffer_size;
    Token* buffer;
  };

}  // namespace compiler