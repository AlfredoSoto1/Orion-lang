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
        {"explicit", Keyword::EXPLICIT},
        {"implicit", Keyword::IMPLICIT},
        {"public", Keyword::PUBLIC},
        {"private", Keyword::PRIVATE},
        {"protected", Keyword::PROTECTED},
        {"friend", Keyword::FRIEND},
        {"virtual", Keyword::VIRTUAL},
        {"override", Keyword::OVERRIDE},
        {"abstract", Keyword::ABSTRACT},
        {"operator", Keyword::OPERATOR},
    };

    // If the lexeme matches any keyword, return its enum equivalent
    auto it = keyword_map.find(lexeme);
    return (it != keyword_map.end()) ? it->second : Keyword::UNDEFINED;
  }

  std::string_view KeywordHandler::from(Keyword kw) {
    switch (kw) {
      case Keyword::IF:
        return "if";
      case Keyword::ELSE:
        return "else";
      case Keyword::FOR:
        return "for";
      case Keyword::DO:
        return "do";
      case Keyword::WHILE:
        return "while";
      case Keyword::SWITCH:
        return "switch";
      case Keyword::CASE:
        return "case";
      case Keyword::BREAK:
        return "break";
      case Keyword::CONTINUE:
        return "continue";
      case Keyword::DEFAULT:
        return "default";
      case Keyword::INSTANCEOF:
        return "instanceof";
      case Keyword::WHEN:
        return "when";
      case Keyword::STATIC:
        return "static";
      case Keyword::CONST:
        return "const";
      case Keyword::MUTABLE:
        return "mutable";
      case Keyword::FINAL:
        return "final";
      case Keyword::VOLATILE:
        return "volatile";
      case Keyword::EXTERN:
        return "extern";
      case Keyword::IMPORT:
        return "import";
      case Keyword::EXPORT:
        return "export";
      case Keyword::MODULE:
        return "module";
      case Keyword::PACKAGE:
        return "package";
      case Keyword::USING:
        return "using";
      case Keyword::NAMESPACE:
        return "namespace";
      case Keyword::TYPEDEF:
        return "typedef";
      case Keyword::AUTO:
        return "auto";
      case Keyword::INLINE:
        return "inline";
      case Keyword::NOINLINE:
        return "noinline";
      case Keyword::NORETURN:
        return "noreturn";
      case Keyword::NOEXCEPT:
        return "noexcept";
      case Keyword::PURE:
        return "pure";
      case Keyword::VOID:
        return "void";
      case Keyword::BOOL:
        return "bool";
      case Keyword::CHAR:
        return "char";
      case Keyword::NULL_OBJ:
        return "null";
      case Keyword::NULLPTR:
        return "nullptr";
      case Keyword::TRUE:
        return "true";
      case Keyword::FALSE:
        return "false";
      case Keyword::INT:
        return "int";
      case Keyword::FLOAT:
        return "float";
      case Keyword::DOUBLE:
        return "double";
      case Keyword::LONG:
        return "long";
      case Keyword::SHORT:
        return "short";
      case Keyword::UNSIGNED:
        return "unsigned";
      case Keyword::SIGNED:
        return "signed";
      case Keyword::EXPLICIT:
        return "explicit";
      case Keyword::IMPLICIT:
        return "implicit";
      case Keyword::PUBLIC:
        return "public";
      case Keyword::PRIVATE:
        return "private";
      case Keyword::PROTECTED:
        return "protected";
      case Keyword::FRIEND:
        return "friend";
      case Keyword::VIRTUAL:
        return "virtual";
      case Keyword::OVERRIDE:
        return "override";
      case Keyword::ABSTRACT:
        return "abstract";
      case Keyword::OPERATOR:
        return "operator";
      default:
        return "UNKNOWN";
    }
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
