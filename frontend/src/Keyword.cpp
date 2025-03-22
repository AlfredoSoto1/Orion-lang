#include "Keyword.hpp"

#include <unordered_map>

namespace compiler {

  Keyword KeywordHandler::from(std::string_view lexeme) {
    static const std::unordered_map<std::string_view, Keyword> keyword_map = {
        {"if", Keyword::IF},
        {"else", Keyword::ELSE},
        {"for", Keyword::FOR},
        {"do", Keyword::DO},
        {"while", Keyword::WHILE},
        {"switch", Keyword::SWITCH},
        {"case", Keyword::CASE},
        {"break", Keyword::BREAK},
        {"continue", Keyword::CONTINUE},
        {"default", Keyword::DEFAULT},
        {"instanceof", Keyword::INSTANCEOF},
        {"when", Keyword::WHEN},
        {"static", Keyword::STATIC},
        {"const", Keyword::CONST},
        {"mutable", Keyword::MUTABLE},
        {"final", Keyword::FINAL},
        {"volatile", Keyword::VOLATILE},
        {"extern", Keyword::EXTERN},
        {"import", Keyword::IMPORT},
        {"export", Keyword::EXPORT},
        {"module", Keyword::MODULE},
        {"package", Keyword::PACKAGE},
        {"using", Keyword::USING},
        {"namespace", Keyword::NAMESPACE},
        {"typedef", Keyword::TYPEDEF},
        {"auto", Keyword::AUTO},
        {"inline", Keyword::INLINE},
        {"noinline", Keyword::NOINLINE},
        {"noreturn", Keyword::NORETURN},
        {"noexcept", Keyword::NOEXCEPT},
        {"pure", Keyword::PURE},
        {"void", Keyword::VOID},
        {"bool", Keyword::BOOL},
        {"char", Keyword::CHAR},
        {"null", Keyword::NULL_OBJ},
        {"nullptr", Keyword::NULLPTR},
        {"true", Keyword::TRUE},
        {"false", Keyword::FALSE},
        {"int", Keyword::INT},
        {"float", Keyword::FLOAT},
        {"double", Keyword::DOUBLE},
        {"long", Keyword::LONG},
        {"short", Keyword::SHORT},
        {"unsigned", Keyword::UNSIGNED},
        {"signed", Keyword::SIGNED},
        {"const", Keyword::CONST},
        {"volatile", Keyword::VOLATILE},
        {"mutable", Keyword::MUTABLE},
        {"explicit", Keyword::EXPLICIT},
        {"implicit", Keyword::IMPLICIT},
        {"public", Keyword::PUBLIC},
        {"private", Keyword::PRIVATE},
        {"protected", Keyword::PROTECTED},
        {"friend", Keyword::FRIEND},
        {"static", Keyword::STATIC},
        {"virtual", Keyword::VIRTUAL},
        {"override", Keyword::OVERRIDE},
        {"final", Keyword::FINAL},
        {"abstract", Keyword::ABSTRACT},
        {"operator", Keyword::OPERATOR},
        {"using", Keyword::USING},
        {"namespace", Keyword::NAMESPACE},
        {"typedef", Keyword::TYPEDEF},
        {"auto", Keyword::AUTO},
        {"inline", Keyword::INLINE},
        {"noinline", Keyword::NOINLINE},
        {"noreturn", Keyword::NORETURN},
        {"noexcept", Keyword::NOEXCEPT},
    };

    // If the lexeme matches any keyword, return its enum equivalent
    auto it = keyword_map.find(lexeme);
    if (it != keyword_map.end()) {
      return it->second;
    }

    return Keyword::UNDEFINED;
  }

  bool KeywordHandler::isControlFlow(Keyword kw) {
    return kw == Keyword::IF || kw == Keyword::ELSE || kw == Keyword::FOR ||
           kw == Keyword::DO || kw == Keyword::WHILE || kw == Keyword::SWITCH ||
           kw == Keyword::CASE || kw == Keyword::BREAK ||
           kw == Keyword::CONTINUE || kw == Keyword::DEFAULT ||
           kw == Keyword::INSTANCEOF || kw == Keyword::WHEN;
  }

  bool KeywordHandler::isStorageSpecifier(Keyword kw) {
    return kw == Keyword::STATIC || kw == Keyword::CONST ||
           kw == Keyword::MUTABLE || kw == Keyword::FINAL ||
           kw == Keyword::VOLATILE || kw == Keyword::EXTERN ||
           kw == Keyword::IMPORT || kw == Keyword::EXPORT ||
           kw == Keyword::MODULE || kw == Keyword::PACKAGE ||
           kw == Keyword::USING || kw == Keyword::NAMESPACE ||
           kw == Keyword::TYPEDEF || kw == Keyword::AUTO;
  }

  bool KeywordHandler::isFunctionSpecifier(Keyword kw) {
    return kw == Keyword::INLINE || kw == Keyword::NOINLINE ||
           kw == Keyword::NORETURN || kw == Keyword::NOEXCEPT ||
           kw == Keyword::PURE || kw == Keyword::CONSTEVAL ||
           kw == Keyword::CONSTINIT || kw == Keyword::EXPLICIT ||
           kw == Keyword::IMPLICIT;
  }

  bool KeywordHandler::isMemoryManagement(Keyword kw) {
    return kw == Keyword::NEW || kw == Keyword::DELETE;
  }

  bool KeywordHandler::isTypeSpecifier(Keyword kw) {
    return kw == Keyword::VOID || kw == Keyword::BOOL || kw == Keyword::CHAR ||
           kw == Keyword::NULL_OBJ || kw == Keyword::NULLPTR ||
           kw == Keyword::TRUE || kw == Keyword::FALSE || kw == Keyword::INT ||
           kw == Keyword::FLOAT || kw == Keyword::DOUBLE ||
           kw == Keyword::LONG || kw == Keyword::SHORT ||
           kw == Keyword::UNSIGNED || kw == Keyword::SIGNED ||
           kw == Keyword::CONST || kw == Keyword::VOLATILE ||
           kw == Keyword::MUTABLE;
  }

}  // namespace compiler
