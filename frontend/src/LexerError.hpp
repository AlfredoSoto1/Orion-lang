#pragma once

#include <string>

namespace compiler {

  enum class LexerErrorType {
    // General errors
    UNKNOWN_ERROR,      // Catch-all error for unexpected issues
    UNKNOWN_TOKEN,      // Unknown token generated
    UNEXPECTED_EOF,     // Unexpected end of file
    INVALID_CHARACTER,  // Unknown or unhandled character encountered

    // Whitespace errors
    UNEXPECTED_WHITESPACE,  // Unexpected whitespace in an invalid position
    INDENTATION_ERROR,      // Incorrect indentation (useful for Python-like
                            // languages)

    // String errors
    UNCLOSED_STRING_LITERAL,  // String literal missing closing quote
    INVALID_ESCAPE_SEQUENCE,  // Unrecognized escape sequence
    UNTERMINATED_RAW_STRING,  // Raw string missing termination

    // Character literal errors
    INVALID_CHAR_LITERAL,     // Character literal is invalid
    UNCLOSED_CHAR_LITERAL,    // Missing closing character for character literal
    MULTI_CHAR_CHAR_LITERAL,  // More than one character inside character
                              // literal

    // Number errors
    INVALID_FLOAT_LITERAL,    // Float format is incorrect
    INVALID_NUMBER_LITERAL,   // Number format is incorrect
    UNEXPECTED_RADIX_PREFIX,  // Misuse of radix prefix (e.g., "0xG1" in hex)
    OVERFLOW_NUMBER_LITERAL,  // Numeric literal exceeds storage capacity

    // Comment errors
    UNCLOSED_BLOCK_COMMENT,  // Block comment missing closing delimiter
    INVALID_NESTED_COMMENT,  // Detected an invalid nested comment (if
                             // unsupported)

    // Punctuator and Operator errors
    UNKNOWN_PUNCTUATOR,         // Unrecognized punctuator
    INVALID_IDENTIFIER_LENGTH,  // Identifier is too big max(256)
    INVALID_UTF8_SEQUENCE,      // Unexpected invalid UTF-8 character sequence
  };

  /**
   * @brief
   *
   */
  class LexerError {
  public:
    uint64_t line;
    uint64_t pos;
    LexerErrorType type;

  public:
    /**
     * @brief Parses the error type into a string literal
     *
     * @return std::string
     */
    std::string toString();
  };
}  // namespace compiler
