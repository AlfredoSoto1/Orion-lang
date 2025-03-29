#include "TokenStream.hpp"

#include <iostream>

namespace compiler {

  TokenStream::TokenStream(Lexer& lexer, uint8_t buffer_size) noexcept
      : lexer(lexer), bpos(0), buffer_size(buffer_size) {
    // Separate a new token buffer
    buffer = new Token[buffer_size];

    // Pre-fill the buffer with the first N amount of tokens.
    do {
      Lexer::LexerResult result = lexer.advance();

      // Check for any token defect
      if (!result) {
        LexerError error = result.error();
        std::cerr << "Lexer Error at line " << error.line << ", col "
                  << error.pos << ": " << error.toString() << "\n";
        break;
      }

      // If we reached the end, stop filling the buffer
      if ((*result).type == TokenType::ENDOF) {
        break;
      }

      // Fill the buffer and move to the next slot.
      buffer[bpos++] = *result;
    } while (bpos != buffer_size);
  }

  TokenStream::~TokenStream() noexcept {
    // Free buffer from memory
    delete[] buffer;
  }

  Lexer::LexerResult TokenStream::next() {
    // If there are no more tokens left, meaning we reached ENDOF
    // it will keep returning ENDOF token.
    if (!hasNext()) return buffer[bpos % buffer_size];

    Lexer::LexerResult result = lexer.advance();

    // Check for any token defect
    if (!result) {
      return result;
    }

    // Cache the current token and consume it from buffer by
    // replacing it with the next token from the lexer.
    Token token = peek();
    buffer[bpos++ % buffer_size] = *result;
    return token;
  }

  const Token& TokenStream::peek() const noexcept {
    // Return the current token being pointed in buffer.
    return buffer[bpos % buffer_size];
  }

  const Token& TokenStream::peekNext() const noexcept {
    // Return the next token being pointed in buffer.
    return buffer[(bpos + 1) % buffer_size];
  }

  bool TokenStream::hasNext() const noexcept {
    // Checks if the current token is ENDOF. This means that
    // there are no more tokens after.
    return buffer[bpos % buffer_size].type != TokenType::ENDOF;
  }
}  // namespace compiler