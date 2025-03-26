#pragma once

#include <expected>
#include <string>
#include <string_view>

// TEMP
struct ParserError {};

namespace compiler {

  /**
   * @brief A recursive descent parser for a C/C++-like language.
   */
  class Parser final {
  public:
    using ParseResult = std::expected<int, ParserError>;

  public:
    explicit Parser() noexcept;

    void parse(std::string_view program);  // <program>

  private:
    // High-level parsing functions
    void parseProgram();
    // <program> ::= <declaration>+

    void parseDeclaration();
    // <declaration> ::= <enum_decl>
    //                |  <class_decl>
    //                |  <struct_decl>
    //                |  <singleton_decl>
    //                |  <interface_decl>
    //                |  <blueprint_decl>
    //                |  <record_decl>
    //                |  <import_stmt>
    //                |  <module_stmt>

    // Import & Module parsing
    void parseImportStatement();
    // <import_stmt> ::= "import" <ident> ";"

    void parseModuleStatement();
    // <module_stmt> ::= "module" <ident> [ ":" <ident> ] ";"

    void parseInterface();
    // <interface_decl> ::=
    // "interface" <ident> ["implements" <ident>+] "{" <interface_body> "}"

    void parseRecord();
    // <record_decl> ::= "record" <ident> "{" <class_body> "}"

    void parseEnum();
    // <enum_decl> ::= "enum" <ident> ["extends" <ident>+] "{" <enum_body> "}"

    // Class parsing functions
    void parseClass();
    // <class_decl> ::=
    // "class" < ident > ["inherits" < ident > ]["implements" <ident>+] "{"
    // <class_body> "};"

    void parseClassBody();
    // <class_body> ::= <class_feature>*

    void parseClassFeature();
    // <class_feature> ::= <class_attribute>
    //                  |  <class_method>

    void parseClassAttribute();
    // <class_attribute> ::= <type> <ident> ";"

    void parseClassMethod();
    // <class_method> ::= <type> <ident> "(" <param_list> ")" <block>

    // Function parsing
    void parseFunction();
    // <func_decl> ::= <type> <ident> "(" <param_list> ")" <block>

    void parseFunctionParams();
    // <param_list> ::= [ <param> { "," <param> } ]

    void parseFunctionBody();
    // <block>

    // Statement parsing
    void parseStatement();
    // <stmt> ::= <block>
    //         |  <if_stmt>
    //         |  <while_stmt>
    //         |  <for_stmt>
    //         |  <return_stmt>
    //         |  <expr> ";"

    void parseBlock();
    // <block> ::= { <stmt>* }

    void parseIfStatement();
    // <if_stmt> ::= if ( <expr> ) <stmt>
    //            |  if ( <expr> ) <stmt> else <stmt>

    void parseWhileStatement();
    // <while_stmt> ::= while ( <expr> ) <stmt>

    void parseForStatement();
    // <for_stmt> ::= for ( <var_decl> ; <expr> ; <expr> ) <stmt>
    // <for_stmt> ::= for ( <var_decl> : <expr> ) <stmt>

    void parseReturnStatement();
    // <return_stmt> ::= "return" <expr> ";"

    // Variable parsing
    void parseVariableDeclaration();
    // <var_decl> ::= <type> <ident> [ "=" <expr>,+ ] ";"

    // Expression parsing
    void parseExpression();
    // <expr>

    void parseBinaryExpression();
    // <expr> ::= <expr> "=" <expr>
    //         |  <expr> "==" <expr>
    //         |  <expr> "&&" <expr>
    //         |  <expr> "||" <expr>
    //         |  <expr> "<" <expr>
    //         |  <expr> "<=" <expr>
    //         |  <expr> ">" <expr>
    //         |  <expr> ">=" <expr>
    //         |  <expr> "+" <expr>
    //         |  <expr> "-" <expr>
    //         |  <expr> "*" <expr>
    //         |  <expr> "/" <expr>
    //         |  <expr> "%" <expr>

    // Miscellaneous
    void parseIdent();
    // <ident> ::= [a-zA-Z_] [a-zA-Z0-9_]*
  };

}  // namespace compiler
