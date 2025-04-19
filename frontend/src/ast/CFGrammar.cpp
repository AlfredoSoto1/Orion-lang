#include "CFGrammar.hpp"

#include <iostream>

#include "parser/Parser.hpp"

#define rule(...) table[makeRule({__VA_ARGS__})] = [this]() -> Symbol

// #define rule(...)                                                 \
//   if (!table.contains(makeRule({__VA_ARGS__})) || is_ambiguous) { \
//     std::cerr << "Grammar is ambigous" << std::endl;              \
//     is_ambiguous = true;                                          \
//     return;                                                       \
//   }                                                               \
//   table[makeRule({__VA_ARGS__})] = [this](Parser & parser)

namespace compiler {

  CFGrammar::CFGrammar() noexcept : is_ambiguous(false) {
    // Build rule table following grammar from Storage AST
    postfix_expr();
    primary_expr();
    arg_expr_list();
    unary_expr();
    unary_op();
    cast_expr();
    multiplicative_expr();
    additive_expr();
    shift_expr();
    relational_expr();
    equality_expr();
    and_expr();
    exclusive_or_expr();
    inclusive_or_expr();
    logical_and_expr();
    logical_or_expr();
    conditional_expr();
    assignment_expr();
    assignment_op();
    expr();
    constant_expr();
    decl();
    decl_specifiers();
    init_decl_list();
    init_decl();
    storage_class_spec();
    type_specifier();
    struct_or_union_specifier();
    struct_or_union();
    struct_declaration_list();
    struct_declaration();
    specifier_qualifier_list();
    struct_declarator_list();
    struct_declarator();
    enum_specifier();
    enumerator_list();
    enumerator();
    type_qualifier();
    type_qualifier_list();
    declarator();
    direct_declarator();
    pointer();
    parameter_type_list();
    parameter_list();
    parameter_declaration();
    identifier_list();
    type_name();
    abstract_declarator();
    direct_abstract_declarator();
    initializer();
    initializer_list();
    statement();
    labeled_statement();
    compound_statement();
    declaration_list();
    statement_list();
    expression_statement();
    selection_statement();
    iteration_statement();
    jump_statement();
    translation_unit();
    external_declaration();
    function_definition();
  }

  // Helper to make a Symbol from components
  Symbol CFGrammar::makeId() const noexcept {
    Symbol sym{};
    sym.type = Symbol::Type::IDENTIFIER;
    sym.terminal.ident_or_lit = 0;
    return sym;
  }

  Symbol CFGrammar::makeLt() const noexcept {
    Symbol sym{};
    sym.type = Symbol::Type::LITERAL;
    sym.terminal.ident_or_lit = 1;
    return sym;
  }

  Symbol CFGrammar::makeKw(Keyword k) const noexcept {
    Symbol sym{};
    sym.type = Symbol::Type::KEYWORD;
    sym.terminal.keyword = k;
    return sym;
  }

  Symbol CFGrammar::makePn(Punctuator p) const noexcept {
    Symbol sym{};
    sym.type = Symbol::Type::PUNCTUATOR;
    sym.terminal.punctuator = p;
    return sym;
  }

  Symbol CFGrammar::makeNt(NonTerminal nt) const noexcept {
    Symbol sym{};
    sym.type = Symbol::Type::NON_TERMINAL;
    sym.nonterminal = nt;
    return sym;
  }

  // Helper to build a rule from up to 10 symbols
  Rule CFGrammar::makeRule(std::initializer_list<Symbol> syms) const noexcept {
    Rule r{};
    size_t i = 0;
    for (Symbol s : syms) {
      if (i >= sizeof(Rule) / sizeof(Symbol)) break;
      r.symbols[i++] = s;
    }
    return r;
  }

  void CFGrammar::primary_expr() {
    using NT = NonTerminal;
    using KW = Keyword;
    using PU = Punctuator;

    // primary-expr → ( expr )
    rule(makePn(PU::LPAREN),  //
         makeNt(NT::EXPR),    //
         makePn(PU::RPAREN)) {
      return makeNt(NT::PRIMARY_EXPR);
    };

    // primary-expr → IDENTIFIER
    rule(makeId()) { return makeNt(NT::PRIMARY_EXPR); };

    // primary-expr → LITERAL
    rule(makeLt()) { return makeNt(NT::PRIMARY_EXPR); };
  }

  void CFGrammar::postfix_expr() {
    using NT = NonTerminal;
    using KW = Keyword;
    using PU = Punctuator;

    // postfix-expr → primary-expr
    rule(makeNt(NT::PRIMARY_EXPR)) { return makeNt(NT::POSTFIX_EXPR); };

    // postfix-expr → postfix-expr [ expr ]
    rule(makeNt(NT::POSTFIX_EXPR),  //
         makePn(PU::LBRACKET),      //
         makeNt(NT::EXPR),          //
         makePn(PU::RBRACKET)) {
      return makeNt(NT::POSTFIX_EXPR);
    };

    // postfix-expr → postfix-expr ( )
    rule(makeNt(NT::POSTFIX_EXPR),  //
         makePn(PU::LPAREN),        //
         makePn(PU::RPAREN)) {
      return makeNt(NT::POSTFIX_EXPR);
    };

    // postfix-expr → postfix-expr ( arg_expr_list )
    rule(makeNt(NT::POSTFIX_EXPR),   //
         makePn(PU::LPAREN),         //
         makeNt(NT::ARG_EXPR_LIST),  //
         makePn(PU::RPAREN)) {
      return makeNt(NT::POSTFIX_EXPR);
    };

    // postfix-expr → postfix-expr . IDENTIFIER
    rule(makeNt(NT::POSTFIX_EXPR),  //
         makePn(PU::DOT),           //
         makeId()) {
      return makeNt(NT::POSTFIX_EXPR);
    };

    // postfix-expr → postfix-expr -> IDENTIFIER
    rule(makeNt(NT::POSTFIX_EXPR),  //
         makePn(PU::RARROW),        //
         makeId()) {
      return makeNt(NT::POSTFIX_EXPR);
    };

    // postfix-expr → postfix-expr ++ IDENTIFIER
    rule(makeNt(NT::POSTFIX_EXPR),  //
         makePn(PU::PLUS_PLUS),     //
         makeId()) {
      return makeNt(NT::POSTFIX_EXPR);
    };

    // postfix-expr → postfix-expr -- IDENTIFIER
    rule(makeNt(NT::POSTFIX_EXPR),  //
         makePn(PU::DASH_DASH),     //
         makeId()) {
      return makeNt(NT::POSTFIX_EXPR);
    };
  }

  void CFGrammar::arg_expr_list() {
    using NT = NonTerminal;
    using PU = Punctuator;

    // arg_expr_list → assignment_expr
    rule(makeNt(NT::ASSIGNMENT_EXPR)) { return makeNt(NT::ARG_EXPR_LIST); };

    // arg_expr_list → arg_expr_list , assignment_expr
    rule(makeNt(NT::ARG_EXPR_LIST),  //
         makePn(PU::COMMA),          //
         makeNt(NT::ASSIGNMENT_EXPR)) {
      return makeNt(NT::ARG_EXPR_LIST);
    };
  }

  void CFGrammar::unary_expr() {
    using NT = NonTerminal;
    using PU = Punctuator;
    using KW = Keyword;

    // unary_expr → postfix_expr
    rule(makeNt(NT::POSTFIX_EXPR)) { return makeNt(NT::UNARY_EXPR); };

    // unary_expr → ++ unary_expr
    rule(makePn(PU::PLUS_PLUS), makeNt(NT::UNARY_EXPR)) {
      return makeNt(NT::UNARY_EXPR);
    };

    // unary_expr → -- unary_expr
    rule(makePn(PU::DASH_DASH), makeNt(NT::UNARY_EXPR)) {
      return makeNt(NT::UNARY_EXPR);
    };

    // unary_expr → unary_op cast_expr
    rule(makeNt(NT::UNARY_OP), makeNt(NT::CAST_EXPR)) {
      return makeNt(NT::UNARY_EXPR);
    };

    // unary_expr → sizeof unary_expr
    rule(makeKw(KW::SIZEOF), makeNt(NT::UNARY_EXPR)) {
      return makeNt(NT::UNARY_EXPR);
    };

    // unary_expr → sizeof ( type_name )
    rule(makeKw(KW::SIZEOF),     //
         makePn(PU::LPAREN),     //
         makeNt(NT::TYPE_NAME),  //
         makePn(PU::RPAREN)) {
      return makeNt(NT::UNARY_EXPR);
    };
  }

  void CFGrammar::unary_op() {
    using PU = Punctuator;
    using NT = NonTerminal;

    // unary_op → & | * | + | - | ~ | !
    for (auto op :
         {PU::BAND, PU::STAR, PU::PLUS, PU::DASH, PU::BNOT, PU::NOT}) {
      rule(makePn(op)) { return makeNt(NT::UNARY_OP); };
    }
  }

  void CFGrammar::cast_expr() {
    using NT = NonTerminal;
    using PU = Punctuator;

    // cast_expr → unary_expr
    rule(makeNt(NT::UNARY_EXPR)) { return makeNt(NT::CAST_EXPR); };

    // cast_expr → ( type_name ) cast_expr
    rule(makePn(PU::LPAREN),     //
         makeNt(NT::TYPE_NAME),  //
         makePn(PU::RPAREN),     //
         makeNt(NT::CAST_EXPR)) {
      return makeNt(NT::CAST_EXPR);
    };
  }

  void CFGrammar::multiplicative_expr() {
    using NT = NonTerminal;
    using PU = Punctuator;

    // multiplicative_expr → cast_expr
    rule(makeNt(NT::CAST_EXPR)) { return makeNt(NT::MULTIPLICATIVE_EXPR); };

    // multiplicative_expr → multiplicative_expr * cast_expr
    rule(makeNt(NT::MULTIPLICATIVE_EXPR),  //
         makePn(PU::STAR),                 //
         makeNt(NT::CAST_EXPR)) {
      return makeNt(NT::MULTIPLICATIVE_EXPR);
    };

    // multiplicative_expr → multiplicative_expr / cast_expr
    rule(makeNt(NT::MULTIPLICATIVE_EXPR),  //
         makePn(PU::SLASH),                //
         makeNt(NT::CAST_EXPR)) {
      return makeNt(NT::MULTIPLICATIVE_EXPR);
    };

    // multiplicative_expr → multiplicative_expr % cast_expr
    rule(makeNt(NT::MULTIPLICATIVE_EXPR),  //
         makePn(PU::MOD),                  //
         makeNt(NT::CAST_EXPR)) {
      return makeNt(NT::MULTIPLICATIVE_EXPR);
    };
  }

  void CFGrammar::additive_expr() {
    using NT = NonTerminal;
    using PU = Punctuator;

    // additive_expr → multiplicative_expr
    rule(makeNt(NT::MULTIPLICATIVE_EXPR)) { return makeNt(NT::ADDITIVE_EXPR); };

    // additive_expr → additive_expr + multiplicative_expr
    rule(makeNt(NT::ADDITIVE_EXPR),  //
         makePn(PU::PLUS),           //
         makeNt(NT::MULTIPLICATIVE_EXPR)) {
      return makeNt(NT::ADDITIVE_EXPR);
    };

    // additive_expr → additive_expr - multiplicative_expr
    rule(makeNt(NT::ADDITIVE_EXPR),  //
         makePn(PU::DASH),           //
         makeNt(NT::MULTIPLICATIVE_EXPR)) {
      return makeNt(NT::ADDITIVE_EXPR);
    };
  }

  void CFGrammar::shift_expr() {
    using NT = NonTerminal;
    using PU = Punctuator;

    // shift_expr → additive_expr
    rule(makeNt(NT::ADDITIVE_EXPR)) { return makeNt(NT::SHIFT_EXPR); };

    // shift_expr → shift_expr << additive_expr
    rule(makeNt(NT::SHIFT_EXPR),  //
         makePn(PU::LSHIFT),      //
         makeNt(NT::ADDITIVE_EXPR)) {
      return makeNt(NT::SHIFT_EXPR);
    };

    // shift_expr → shift_expr >> additive_expr
    rule(makeNt(NT::SHIFT_EXPR),  //
         makePn(PU::RSHIFT),      //
         makeNt(NT::ADDITIVE_EXPR)) {
      return makeNt(NT::SHIFT_EXPR);
    };
  }

  void CFGrammar::relational_expr() {
    using NT = NonTerminal;
    using PU = Punctuator;

    // relational_expr → shift_expr
    rule(makeNt(NT::SHIFT_EXPR)) { return makeNt(NT::RELATIONAL_EXPR); };

    // relational_expr → relational_expr < shift_expr
    rule(makeNt(NT::RELATIONAL_EXPR),  //
         makePn(PU::LT),               //
         makeNt(NT::SHIFT_EXPR)) {
      return makeNt(NT::RELATIONAL_EXPR);
    };

    // relational_expr → relational_expr > shift_expr
    rule(makeNt(NT::RELATIONAL_EXPR),  //
         makePn(PU::GT),               //
         makeNt(NT::SHIFT_EXPR)) {
      return makeNt(NT::RELATIONAL_EXPR);
    };

    // relational_expr → relational_expr <= shift_expr
    rule(makeNt(NT::RELATIONAL_EXPR),  //
         makePn(PU::LTE),              //
         makeNt(NT::SHIFT_EXPR)) {
      return makeNt(NT::RELATIONAL_EXPR);
    };

    // relational_expr → relational_expr >= shift_expr
    rule(makeNt(NT::RELATIONAL_EXPR),  //
         makePn(PU::GTE),              //
         makeNt(NT::SHIFT_EXPR)) {
      return makeNt(NT::RELATIONAL_EXPR);
    };
  }

  void CFGrammar::equality_expr() {
    using NT = NonTerminal;
    using PU = Punctuator;

    // equality_expr → relational_expr
    rule(makeNt(NT::RELATIONAL_EXPR)) { return makeNt(NT::EQUALITY_EXPR); };

    // equality_expr → equality_expr == relational_expr
    rule(makeNt(NT::EQUALITY_EXPR),  //
         makePn(PU::EQ_EQ),          //
         makeNt(NT::RELATIONAL_EXPR)) {
      return makeNt(NT::EQUALITY_EXPR);
    };

    // equality_expr → equality_expr != relational_expr
    rule(makeNt(NT::EQUALITY_EXPR),  //
         makePn(PU::NEQ),            //
         makeNt(NT::RELATIONAL_EXPR)) {
      return makeNt(NT::EQUALITY_EXPR);
    };
  }

  void CFGrammar::and_expr() {
    using NT = NonTerminal;
    using PU = Punctuator;

    // and_expr → equality_expr
    rule(makeNt(NT::EQUALITY_EXPR)) { return makeNt(NT::AND_EXPR); };

    // and_expr → and_expr & equality_expr
    rule(makeNt(NT::AND_EXPR),  //
         makePn(PU::BAND),      //
         makeNt(NT::EQUALITY_EXPR)) {
      return makeNt(NT::AND_EXPR);
    };
  }

  void CFGrammar::exclusive_or_expr() {
    using NT = NonTerminal;
    using PU = Punctuator;

    // exclusive_or_expr → and_expr
    rule(makeNt(NT::AND_EXPR)) { return makeNt(NT::EXCLUSIVE_OR_EXPR); };

    // exclusive_or_expr → exclusive_or_expr ^ and_expr
    rule(makeNt(NT::EXCLUSIVE_OR_EXPR),  //
         makePn(PU::BXOR),               //
         makeNt(NT::AND_EXPR)) {
      return makeNt(NT::EXCLUSIVE_OR_EXPR);
    };
  }

  void CFGrammar::inclusive_or_expr() {
    using NT = NonTerminal;
    using PU = Punctuator;

    // inclusive_or_expr → exclusive_or_expr
    rule(makeNt(NT::EXCLUSIVE_OR_EXPR)) {
      return makeNt(NT::INCLUSIVE_OR_EXPR);
    };

    // inclusive_or_expr → inclusive_or_expr | exclusive_or_expr
    rule(makeNt(NT::INCLUSIVE_OR_EXPR),  //
         makePn(PU::BOR),                //
         makeNt(NT::EXCLUSIVE_OR_EXPR)) {
      return makeNt(NT::INCLUSIVE_OR_EXPR);
    };
  }

  void CFGrammar::logical_and_expr() {
    using NT = NonTerminal;
    using PU = Punctuator;

    // logical_and_expr → inclusive_or_expr
    rule(makeNt(NT::INCLUSIVE_OR_EXPR)) {
      return makeNt(NT::LOGICAL_AND_EXPR);
    };

    // logical_and_expr → logical_and_expr && inclusive_or_expr
    rule(makeNt(NT::LOGICAL_AND_EXPR),  //
         makePn(PU::AND),               //
         makeNt(NT::INCLUSIVE_OR_EXPR)) {
      return makeNt(NT::LOGICAL_AND_EXPR);
    };
  }

  void CFGrammar::logical_or_expr() {
    using NT = NonTerminal;
    using PU = Punctuator;

    // logical_or_expr → logical_and_expr
    rule(makeNt(NT::LOGICAL_AND_EXPR)) { return makeNt(NT::LOGICAL_OR_EXPR); };

    // logical_or_expr → logical_or_expr || logical_and_expr
    rule(makeNt(NT::LOGICAL_OR_EXPR),  //
         makePn(PU::OR),               //
         makeNt(NT::LOGICAL_AND_EXPR)) {
      return makeNt(NT::LOGICAL_OR_EXPR);
    };
  }

  void CFGrammar::conditional_expr() {
    using NT = NonTerminal;
    using PU = Punctuator;

    // conditional_expr → logical_or_expr
    rule(makeNt(NT::LOGICAL_OR_EXPR)) { return makeNt(NT::CONDITIONAL_EXPR); };

    // conditional_expr → logical_or_expr ? expr : conditional_expr
    rule(makeNt(NT::LOGICAL_OR_EXPR),  //
         makePn(PU::QUESTION),         //
         makeNt(NT::EXPR),             //
         makePn(PU::COLON),            //
         makeNt(NT::CONDITIONAL_EXPR)) {
      return makeNt(NT::CONDITIONAL_EXPR);
    };
  }

  void CFGrammar::assignment_expr() {
    using NT = NonTerminal;
    using PU = Punctuator;

    // assignment_expr → conditional_expr
    rule(makeNt(NT::CONDITIONAL_EXPR)) { return makeNt(NT::ASSIGNMENT_EXPR); };

    // assignment_expr → unary_expr assignment_operator assignment_expr
    rule(makeNt(NT::UNARY_EXPR),     //
         makeNt(NT::ASSIGNMENT_OP),  //
         makeNt(NT::ASSIGNMENT_EXPR)) {
      return makeNt(NT::ASSIGNMENT_EXPR);
    };
  }

  void CFGrammar::assignment_op() {
    using NT = NonTerminal;
    using PU = Punctuator;

    for (auto op : {PU::EQ, PU::STAR_EQ, PU::SLASH_EQ, PU::MOD_EQ, PU::PLUS_EQ,
                    PU::DASH_EQ, PU::LSHIFT_EQ, PU::RSHIFT_EQ, PU::AND_EQ,
                    PU::OR_EQ, PU::XOR_EQ}) {
      rule(makePn(op)) { return makeNt(NT::ASSIGNMENT_OP); };
    }
  }

  void CFGrammar::expr() {
    using NT = NonTerminal;
    using PU = Punctuator;

    // expr → assignment_expr
    rule(makeNt(NT::ASSIGNMENT_EXPR)) { return makeNt(NT::EXPR); };

    // expr → expr , assignment_expr
    rule(makeNt(NT::EXPR),   //
         makePn(PU::COMMA),  //
         makeNt(NT::ASSIGNMENT_EXPR)) {
      return makeNt(NT::EXPR);
    };
  }

  void CFGrammar::constant_expr() {
    using NT = NonTerminal;

    // constant_expr → conditional_expr
    rule(makeNt(NT::CONDITIONAL_EXPR)) { return makeNt(NT::CONSTANT_EXPR); };
  }

  void CFGrammar::decl() {
    using NT = NonTerminal;
    using PU = Punctuator;

    // decl → decl_specifiers ;
    rule(makeNt(NT::DECLARATION_SPECIFIERS), makePn(PU::SEMI_COLON)) {
      return makeNt(NT::DECLARATION);
    };

    // decl → decl_specifiers init_declarator_list ;
    rule(makeNt(NT::DECLARATION_SPECIFIERS),  //
         makeNt(NT::INIT_DECLARATOR_LIST),    //
         makePn(PU::SEMI_COLON)) {
      return makeNt(NT::DECLARATION);
    };
  }

  void CFGrammar::decl_specifiers() {
    using NT = NonTerminal;

    // decl_specifiers → storage_class_specifier
    rule(makeNt(NT::STORAGE_CLASS_SPECIFIER)) {
      return makeNt(NT::DECL_SPECIFIERS);
    };

    // decl_specifiers → storage_class_specifier declaration_specifiers
    rule(makeNt(NT::STORAGE_CLASS_SPECIFIER),  //
         makeNt(NT::DECL_SPECIFIERS)) {
      return makeNt(NT::DECL_SPECIFIERS);
    };

    // decl_specifiers → type_specifier
    rule(makeNt(NT::TYPE_SPECIFIER)) { return makeNt(NT::DECL_SPECIFIERS); };

    // decl_specifiers → type_specifier declaration_specifiers
    rule(makeNt(NT::TYPE_SPECIFIER),  //
         makeNt(NT::DECL_SPECIFIERS)) {
      return makeNt(NT::DECL_SPECIFIERS);
    };

    // decl_specifiers → type_qualifier
    rule(makeNt(NT::TYPE_QUALIFIER)) { return makeNt(NT::DECL_SPECIFIERS); };

    // decl_specifiers → type_qualifier declaration_specifiers
    rule(makeNt(NT::TYPE_QUALIFIER), makeNt(NT::DECL_SPECIFIERS)) {
      return makeNt(NT::DECL_SPECIFIERS);
    };
  }

  void CFGrammar::init_decl_list() {
    using NT = NonTerminal;
    using PU = Punctuator;

    // init_decl_list → init_declarator
    rule(makeNt(NT::INIT_DECL)) { return makeNt(NT::INIT_DECL_LIST); };

    // init_decl_list → init_declarator_list , init_declarator
    rule(makeNt(NT::INIT_DECL_LIST),  //
         makePn(PU::COMMA),           //
         makeNt(NT::INIT_DECL)) {
      return makeNt(NT::INIT_DECL_LIST);
    };
  }

  void CFGrammar::init_decl() {
    using NT = NonTerminal;
    using PU = Punctuator;

    // init_decl → declarator
    rule(makeNt(NT::DECLARATOR)) { return makeNt(NT::INIT_DECL); };

    // init_decl → declarator = initializer
    rule(makeNt(NT::DECLARATOR),  //
         makePn(PU::EQ),          //
         makeNt(NT::INITIALIZER)) {
      return makeNt(NT::INIT_DECL);
    };
  }

  void CFGrammar::storage_class_spec() {
    using NT = NonTerminal;
    using KW = Keyword;

    // storage_class_specifier → typedef | extern | static | auto
    for (auto kw : {KW::TYPEDEF, KW::EXTERN, KW::STATIC, KW::AUTO}) {
      rule(makeKw(kw)) { return makeNt(NT::STORAGE_CLASS_SPECIFIER); };
    }
  }

  void CFGrammar::type_specifier() {
    using NT = NonTerminal;
    using KW = Keyword;
    using PU = Punctuator;

    // type_specifier → VOID | CHAR | SHORT | INT | LONG | FLOAT | DOUBLE |
    // SIGNED | UNSIGNED
    for (auto kw : {KW::VOID, KW::CHAR, KW::SHORT, KW::INT, KW::LONG, KW::FLOAT,
                    KW::DOUBLE, KW::SIGNED, KW::UNSIGNED, KW::BOOL}) {
      rule(makeKw(kw)) { return makeNt(NT::TYPE_SPECIFIER); };
    }

    // type_specifier → struct_or_union_specifier
    rule(makeNt(NT::STRUCT_OR_UNION_SPECIFIER)) {
      return makeNt(NT::TYPE_SPECIFIER);
    };

    // type_specifier → enum_specifier
    rule(makeNt(NT::ENUM_SPECIFIER)) { return makeNt(NT::TYPE_SPECIFIER); };

    // type_specifier → TYPE_NAME
    rule(makeNt(NT::TYPE_NAME)) { return makeNt(NT::TYPE_SPECIFIER); };
  }

  void CFGrammar::struct_or_union_specifier() {
    using NT = NonTerminal;
    using KW = Keyword;
    using PU = Punctuator;

    // struct_or_union_specifier → struct_or_union IDENTIFIER '{'
    // struct_declaration_list '}'
    rule(makeNt(NT::STRUCT_OR_UNION),          //
         makeId(),                             //
         makePn(PU::LBRACE),                   //
         makeNt(NT::STRUCT_DECLARATION_LIST),  //
         makePn(PU::RBRACE)) {
      return makeNt(NT::STRUCT_OR_UNION_SPECIFIER);
    };

    // struct_or_union_specifier → struct_or_union '{' struct_declaration_list
    // '}'
    rule(makeNt(NT::STRUCT_OR_UNION),          //
         makePn(PU::LBRACE),                   //
         makeNt(NT::STRUCT_DECLARATION_LIST),  //
         makePn(PU::RBRACE)) {
      return makeNt(NT::STRUCT_OR_UNION_SPECIFIER);
    };

    // struct_or_union_specifier → struct_or_union IDENTIFIER
    rule(makeNt(NT::STRUCT_OR_UNION),  //
         makeId()) {
      return makeNt(NT::STRUCT_OR_UNION_SPECIFIER);
    };
  }

  void CFGrammar::struct_or_union() {
    using NT = NonTerminal;
    using KW = Keyword;
    using PU = Punctuator;

    // struct_or_union → STRUCT
    rule(makeKw(KW::STRUCT)) { return makeNt(NT::STRUCT_OR_UNION); };

    // struct_or_union → UNION
    rule(makeKw(KW::UNION)) { return makeNt(NT::STRUCT_OR_UNION); };
  }

  void CFGrammar::struct_declaration_list() {
    using NT = NonTerminal;

    // struct_declaration_list → struct_declaration
    rule(makeNt(NT::STRUCT_DECLARATION)) {
      return makeNt(NT::STRUCT_DECLARATION_LIST);
    };

    // struct_declaration_list → struct_declaration_list struct_declaration
    rule(makeNt(NT::STRUCT_DECLARATION_LIST),  //
         makeNt(NT::STRUCT_DECLARATION)) {
      return makeNt(NT::STRUCT_DECLARATION_LIST);
    };
  }

  void CFGrammar::struct_declaration() {
    using NT = NonTerminal;
    using PU = Punctuator;

    // struct_declaration → specifier_qualifier_list struct_declarator_list
    // ';'
    rule(makeNt(NT::SPECIFIER_QUALIFIER_LIST),  //
         makeNt(NT::STRUCT_DECLARATOR_LIST),    //
         makePn(PU::SEMI_COLON)) {
      return makeNt(NT::STRUCT_DECLARATION);
    };
  }

  void CFGrammar::specifier_qualifier_list() {
    using NT = NonTerminal;
    using KW = Keyword;
    using PU = Punctuator;

    // specifier_qualifier_list → type_specifier specifier_qualifier_list
    rule(makeNt(NT::TYPE_SPECIFIER),  //
         makeNt(NT::SPECIFIER_QUALIFIER_LIST)) {
      return makeNt(NT::SPECIFIER_QUALIFIER_LIST);
    };

    // specifier_qualifier_list → type_specifier
    rule(makeNt(NT::TYPE_SPECIFIER)) {
      return makeNt(NT::SPECIFIER_QUALIFIER_LIST);
    };

    // specifier_qualifier_list → type_qualifier specifier_qualifier_list
    rule(makeNt(NT::TYPE_QUALIFIER),  //
         makeNt(NT::SPECIFIER_QUALIFIER_LIST)) {
      return makeNt(NT::SPECIFIER_QUALIFIER_LIST);
    };

    // specifier_qualifier_list → type_qualifier
    rule(makeNt(NT::TYPE_QUALIFIER)) {
      return makeNt(NT::SPECIFIER_QUALIFIER_LIST);
    };
  }

  void CFGrammar::struct_declarator_list() {
    using NT = NonTerminal;
    using PU = Punctuator;

    // struct_declarator_list → struct_declarator
    rule(makeNt(NT::STRUCT_DECLARATOR)) {
      return makeNt(NT::STRUCT_DECLARATOR_LIST);
    };

    // struct_declarator_list → struct_declarator_list ',' struct_declarator
    rule(makeNt(NT::STRUCT_DECLARATOR_LIST),  //
         makePn(PU::COMMA),                   //
         makeNt(NT::STRUCT_DECLARATOR)) {
      return makeNt(NT::STRUCT_DECLARATOR_LIST);
    };
  }

  void CFGrammar::struct_declarator() {
    using NT = NonTerminal;
    using PU = Punctuator;

    // struct_declarator → declarator
    rule(makeNt(NT::DECLARATOR)) { return makeNt(NT::STRUCT_DECLARATOR); };

    // struct_declarator → ':' constant_expression
    rule(makePn(PU::COLON),  //
         makeNt(NT::CONSTANT_EXPRESSION)) {
      return makeNt(NT::STRUCT_DECLARATOR);
    };

    // struct_declarator → declarator ':' constant_expression
    rule(makeNt(NT::DECLARATOR),  //
         makePn(PU::COLON),       //
         makeNt(NT::CONSTANT_EXPRESSION)) {
      return makeNt(NT::STRUCT_DECLARATOR);
    };
  }

  void CFGrammar::enum_specifier() {
    using NT = NonTerminal;
    using KW = Keyword;
    using PU = Punctuator;

    // enum_specifier → ENUM '{' enumerator_list '}'
    rule(makeKw(KW::ENUM),             //
         makePn(PU::LBRACE),           //
         makeNt(NT::ENUMERATOR_LIST),  //
         makePn(PU::RBRACE)) {
      return makeNt(NT::ENUM_SPECIFIER);
    };

    // enum_specifier → ENUM IDENTIFIER '{' enumerator_list '}'
    rule(makeKw(KW::ENUM),             //
         makeId(),                     //
         makePn(PU::LBRACE),           //
         makeNt(NT::ENUMERATOR_LIST),  //
         makePn(PU::RBRACE)) {
      return makeNt(NT::ENUM_SPECIFIER);
    };

    // enum_specifier → ENUM IDENTIFIER
    rule(makeKw(KW::ENUM),  //
         makeId()) {
      return makeNt(NT::ENUM_SPECIFIER);
    };
  }

  void CFGrammar::enumerator_list() {
    using NT = NonTerminal;
    using KW = Keyword;
    using PU = Punctuator;

    // enumerator_list → enumerator
    rule(makeNt(NT::ENUMERATOR)) { return makeNt(NT::ENUMERATOR_LIST); };

    // enumerator_list → enumerator_list ',' enumerator
    rule(makeNt(NT::ENUMERATOR_LIST),  //
         makePn(PU::COMMA),            //
         makeNt(NT::ENUMERATOR)) {
      return makeNt(NT::ENUMERATOR_LIST);
    };
  }

  void CFGrammar::enumerator() {
    using NT = NonTerminal;
    using PU = Punctuator;

    // enumerator → IDENTIFIER
    rule(makeId()) { return makeNt(NT::ENUMERATOR); };

    // enumerator → IDENTIFIER '=' constant_expression
    rule(makeId(),        //
         makePn(PU::EQ),  //
         makeNt(NT::CONSTANT_EXPRESSION)) {
      return makeNt(NT::ENUMERATOR);
    };
  }

  void CFGrammar::type_qualifier() {
    using KW = Keyword;
    using NT = NonTerminal;

    // type_qualifier → CONST
    rule(makeKw(KW::CONST)) { return makeNt(NT::TYPE_QUALIFIER); };

    // type_qualifier → VOLATILE
    rule(makeKw(KW::VOLATILE)) { return makeNt(NT::TYPE_QUALIFIER); };
  }

  void CFGrammar::type_qualifier_list() {
    using NT = NonTerminal;

    // type_qualifier_list → type_qualifier
    rule(makeNt(NT::TYPE_QUALIFIER)) {
      return makeNt(NT::TYPE_QUALIFIER_LIST);
    };

    // type_qualifier_list → type_qualifier_list type_qualifier
    rule(makeNt(NT::TYPE_QUALIFIER_LIST),  //
         makeNt(NT::TYPE_QUALIFIER)) {
      return makeNt(NT::TYPE_QUALIFIER_LIST);
    };
  }

  void CFGrammar::declarator() {
    using NT = NonTerminal;

    // declarator → pointer direct_declarator
    rule(makeNt(NT::POINTER),  //
         makeNt(NT::DIRECT_DECLARATOR)) {
      return makeNt(NT::DECLARATOR);
    };

    // declarator → direct_declarator
    rule(makeNt(NT::DIRECT_DECLARATOR)) { return makeNt(NT::DECLARATOR); };
  }

  void CFGrammar::direct_declarator() {
    using NT = NonTerminal;
    using PU = Punctuator;

    // direct_declarator → IDENTIFIER
    rule(makeId()) { return makeNt(NT::DIRECT_DECLARATOR); };

    // direct_declarator → '(' declarator ')'
    rule(makePn(PU::LBRACE),      //
         makeNt(NT::DECLARATOR),  //
         makePn(PU::RBRACE)) {
      return makeNt(NT::DIRECT_DECLARATOR);
    };

    // direct_declarator → direct_declarator '[' constant_expression ']'
    rule(makeNt(NT::DIRECT_DECLARATOR),    //
         makePn(PU::LBRACKET),             //
         makeNt(NT::CONSTANT_EXPRESSION),  //
         makePn(PU::RBRACKET)) {
      return makeNt(NT::DIRECT_DECLARATOR);
    };

    // direct_declarator → direct_declarator '[' ']'
    rule(makeNt(NT::DIRECT_DECLARATOR),  //
         makePn(PU::LBRACKET),           //
         makePn(PU::RBRACKET)) {
      return makeNt(NT::DIRECT_DECLARATOR);
    };

    // direct_declarator → direct_declarator '(' parameter_type_list ')'
    rule(makeNt(NT::DIRECT_DECLARATOR),    //
         makePn(PU::LBRACE),               //
         makeNt(NT::PARAMETER_TYPE_LIST),  //
         makePn(PU::RBRACE)) {
      return makeNt(NT::DIRECT_DECLARATOR);
    };

    // direct_declarator → direct_declarator '(' identifier_list ')'
    rule(makeNt(NT::DIRECT_DECLARATOR),  //
         makePn(PU::LBRACE),             //
         makeNt(NT::IDENTIFIER_LIST),    //
         makePn(PU::RBRACE)) {
      return makeNt(NT::DIRECT_DECLARATOR);
    };

    // direct_declarator → direct_declarator '(' ')'
    rule(makeNt(NT::DIRECT_DECLARATOR),  //
         makePn(PU::LBRACE),             //
         makePn(PU::RBRACE)) {
      return makeNt(NT::DIRECT_DECLARATOR);
    };
  }

  void CFGrammar::pointer() {
    using NT = NonTerminal;
    using PU = Punctuator;

    // pointer → '*'
    rule(makePn(PU::STAR)) { return makeNt(NT::POINTER); };

    // pointer → '*' type_qualifier_list
    rule(makePn(PU::STAR),  //
         makeNt(NT::TYPE_QUALIFIER_LIST)) {
      return makeNt(NT::POINTER);
    };

    // pointer → '*' pointer
    rule(makePn(PU::STAR),  //
         makeNt(NT::POINTER)) {
      return makeNt(NT::POINTER);
    };

    // pointer → '*' type_qualifier_list pointer
    rule(makePn(PU::STAR),                 //
         makeNt(NT::TYPE_QUALIFIER_LIST),  //
         makeNt(NT::POINTER)) {
      return makeNt(NT::POINTER);
    };
  }

  void CFGrammar::parameter_type_list() {
    using NT = NonTerminal;
    using PU = Punctuator;
    using KW = Keyword;

    // parameter_type_list → parameter_list
    rule(makeNt(NT::PARAMETER_LIST)) {
      return makeNt(NT::PARAMETER_TYPE_LIST);
    };

    // parameter_type_list → parameter_list ',' ELLIPSIS
    rule(makeNt(NT::PARAMETER_LIST),  //
         makePn(PU::COMMA),           //
         makePn(PU::ELLIPSIS)) {
      return makeNt(NT::PARAMETER_TYPE_LIST);
    };
  }

  void CFGrammar::parameter_list() {
    using NT = NonTerminal;
    using PU = Punctuator;

    // parameter_list → parameter_declaration
    rule(makeNt(NT::PARAMETER_DECLARATION)) {
      return makeNt(NT::PARAMETER_LIST);
    };

    // parameter_list → parameter_list ',' parameter_declaration
    rule(makeNt(NT::PARAMETER_LIST),  //
         makePn(PU::COMMA),           //
         makeNt(NT::PARAMETER_DECLARATION)) {
      return makeNt(NT::PARAMETER_LIST);
    };
  }

  void CFGrammar::parameter_declaration() {
    using NT = NonTerminal;

    // parameter_declaration → declaration_specifiers declarator
    rule(makeNt(NT::DECLARATION_SPECIFIERS),  //
         makeNt(NT::DECLARATOR)) {
      return makeNt(NT::PARAMETER_DECLARATION);
    };

    // parameter_declaration → declaration_specifiers abstract_declarator
    rule(makeNt(NT::DECLARATION_SPECIFIERS),  //
         makeNt(NT::ABSTRACT_DECLARATOR)) {
      return makeNt(NT::PARAMETER_DECLARATION);
    };

    // parameter_declaration → declaration_specifiers
    rule(makeNt(NT::DECLARATION_SPECIFIERS)) {
      return makeNt(NT::PARAMETER_DECLARATION);
    };
  }

  void CFGrammar::identifier_list() {
    using NT = NonTerminal;
    using PU = Punctuator;

    // identifier_list → IDENTIFIER
    rule(makeId()) { return makeNt(NT::IDENTIFIER_LIST); };

    // identifier_list → identifier_list ',' IDENTIFIER
    rule(makeNt(NT::IDENTIFIER_LIST),  //
         makePn(PU::COMMA),            //
         makeId()) {
      return makeNt(NT::IDENTIFIER_LIST);
    };
  }

  void CFGrammar::type_name() {
    using NT = NonTerminal;

    // type_name → specifier_qualifier_list
    rule(makeNt(NT::SPECIFIER_QUALIFIER_LIST)) {
      return makeNt(NT::TYPE_NAME);
    };

    // type_name → specifier_qualifier_list abstract_declarator
    rule(makeNt(NT::SPECIFIER_QUALIFIER_LIST),  //
         makeNt(NT::ABSTRACT_DECLARATOR)) {
      return makeNt(NT::TYPE_NAME);
    };
  }

  void CFGrammar::abstract_declarator() {
    using NT = NonTerminal;

    // abstract_declarator → pointer
    rule(makeNt(NT::POINTER)) { return makeNt(NT::ABSTRACT_DECLARATOR); };

    // abstract_declarator → direct_abstract_declarator
    rule(makeNt(NT::DIRECT_ABSTRACT_DECLARATOR)) {
      return makeNt(NT::ABSTRACT_DECLARATOR);
    };

    // abstract_declarator → pointer direct_abstract_declarator
    rule(makeNt(NT::POINTER),  //
         makeNt(NT::DIRECT_ABSTRACT_DECLARATOR)) {
      return makeNt(NT::ABSTRACT_DECLARATOR);
    };
  }

  void CFGrammar::direct_abstract_declarator() {
    using NT = NonTerminal;
    using PU = Punctuator;

    // direct_abstract_declarator → '(' abstract_declarator ')'
    rule(makePn(PU::LBRACE),               //
         makeNt(NT::ABSTRACT_DECLARATOR),  //
         makePn(PU::RBRACE)) {
      return makeNt(NT::DIRECT_ABSTRACT_DECLARATOR);
    };

    // direct_abstract_declarator → '[' ']'
    rule(makePn(PU::LBRACKET),  //
         makePn(PU::RBRACKET)) {
      return makeNt(NT::DIRECT_ABSTRACT_DECLARATOR);
    };

    // direct_abstract_declarator → '[' constant_expression ']'
    rule(makePn(PU::LBRACKET),             //
         makeNt(NT::CONSTANT_EXPRESSION),  //
         makePn(PU::RBRACKET)) {
      return makeNt(NT::DIRECT_ABSTRACT_DECLARATOR);
    };

    // direct_abstract_declarator → direct_abstract_declarator '[' ']'
    rule(makeNt(NT::DIRECT_ABSTRACT_DECLARATOR),  //
         makePn(PU::LBRACKET),                    //
         makePn(PU::RBRACKET)) {
      return makeNt(NT::DIRECT_ABSTRACT_DECLARATOR);
    };

    // direct_abstract_declarator → direct_abstract_declarator '['
    // constant_expression ']'
    rule(makeNt(NT::DIRECT_ABSTRACT_DECLARATOR),  //
         makePn(PU::LBRACKET),                    //
         makeNt(NT::CONSTANT_EXPRESSION),         //
         makePn(PU::RBRACKET)) {
      return makeNt(NT::DIRECT_ABSTRACT_DECLARATOR);
    };

    // direct_abstract_declarator → '(' ')'
    rule(makePn(PU::LBRACE),  //
         makePn(PU::RBRACE)) {
      return makeNt(NT::DIRECT_ABSTRACT_DECLARATOR);
    };

    // direct_abstract_declarator → '(' parameter_type_list ')'
    rule(makePn(PU::LBRACE),               //
         makeNt(NT::PARAMETER_TYPE_LIST),  //
         makePn(PU::RBRACE)) {
      return makeNt(NT::DIRECT_ABSTRACT_DECLARATOR);
    };

    // direct_abstract_declarator → direct_abstract_declarator '(' ')'
    rule(makeNt(NT::DIRECT_ABSTRACT_DECLARATOR),  //
         makePn(PU::LBRACE),                      //
         makePn(PU::RBRACE)) {
      return makeNt(NT::DIRECT_ABSTRACT_DECLARATOR);
    };

    // direct_abstract_declarator → direct_abstract_declarator '('
    // parameter_type_list ')'
    rule(makeNt(NT::DIRECT_ABSTRACT_DECLARATOR),  //
         makePn(PU::LBRACE),                      //
         makeNt(NT::PARAMETER_TYPE_LIST),         //
         makePn(PU::RBRACE)) {
      return makeNt(NT::DIRECT_ABSTRACT_DECLARATOR);
    };
  }

  void CFGrammar::initializer() {
    using NT = NonTerminal;

    // initializer → assignment_expression
    rule(makeNt(NT::ASSIGNMENT_EXPRESSION)) { return makeNt(NT::INITIALIZER); };

    // initializer → '{' initializer_list '}'
    rule(makePn(Punctuator::LBRACE),    //
         makeNt(NT::INITIALIZER_LIST),  //
         makePn(Punctuator::RBRACE)) {
      return makeNt(NT::INITIALIZER);
    };

    // initializer → '{' initializer_list ',' '}'
    rule(makePn(Punctuator::LBRACE),    //
         makeNt(NT::INITIALIZER_LIST),  //
         makePn(Punctuator::COMMA),     //
         makePn(Punctuator::RBRACE)) {
      return makeNt(NT::INITIALIZER);
    };
  }

  void CFGrammar::initializer_list() {
    using NT = NonTerminal;

    // initializer_list → initializer
    rule(makeNt(NT::INITIALIZER)) { return makeNt(NT::INITIALIZER_LIST); };

    // initializer_list → initializer_list ',' initializer
    rule(makeNt(NT::INITIALIZER_LIST),  //
         makePn(Punctuator::COMMA),     //
         makeNt(NT::INITIALIZER)) {
      return makeNt(NT::INITIALIZER_LIST);
    };
  }

  void CFGrammar::statement() {
    using NT = NonTerminal;

    // statement → labeled_statement
    rule(makeNt(NT::LABELED_STATEMENT)) { return makeNt(NT::STATEMENT); };

    // statement → compound_statement
    rule(makeNt(NT::COMPOUND_STATEMENT)) { return makeNt(NT::STATEMENT); };

    // statement → expression_statement
    rule(makeNt(NT::EXPRESSION_STATEMENT)) { return makeNt(NT::STATEMENT); };

    // statement → selection_statement
    rule(makeNt(NT::SELECTION_STATEMENT)) { return makeNt(NT::STATEMENT); };

    // statement → iteration_statement
    rule(makeNt(NT::ITERATION_STATEMENT)) { return makeNt(NT::STATEMENT); };

    // statement → jump_statement
    rule(makeNt(NT::JUMP_STATEMENT)) { return makeNt(NT::STATEMENT); };
  }

  void CFGrammar::labeled_statement() {
    using NT = NonTerminal;

    // labeled_statement → IDENTIFIER ':' statement
    rule(makeId(),                   //
         makePn(Punctuator::COLON),  //
         makeNt(NT::STATEMENT)) {
      return makeNt(NT::LABELED_STATEMENT);
    };

    // labeled_statement → CASE constant_expression ':' statement
    rule(makeKw(Keyword::CASE),            //
         makeNt(NT::CONSTANT_EXPRESSION),  //
         makePn(Punctuator::COLON),        //
         makeNt(NT::STATEMENT)) {
      return makeNt(NT::LABELED_STATEMENT);
    };

    // labeled_statement → DEFAULT ':' statement
    rule(makeKw(Keyword::DEFAULT),   //
         makePn(Punctuator::COLON),  //
         makeNt(NT::STATEMENT)) {
      return makeNt(NT::LABELED_STATEMENT);
    };
  }

  void CFGrammar::compound_statement() {
    using NT = NonTerminal;
    using PU = Punctuator;

    // compound_statement → '{' '}'
    rule(makePn(PU::LBRACE),  //
         makePn(PU::RBRACE)) {
      return makeNt(NT::COMPOUND_STATEMENT);
    };

    // compound_statement → '{' statement_list '}'
    rule(makePn(PU::LBRACE),          //
         makeNt(NT::STATEMENT_LIST),  //
         makePn(PU::RBRACE)) {
      return makeNt(NT::COMPOUND_STATEMENT);
    };

    // compound_statement → '{' declaration_list '}'
    rule(makePn(PU::LBRACE),            //
         makeNt(NT::DECLARATION_LIST),  //
         makePn(PU::RBRACE)) {
      return makeNt(NT::COMPOUND_STATEMENT);
    };

    // compound_statement → '{' declaration_list statement_list '}'
    rule(makePn(PU::LBRACE),            //
         makeNt(NT::DECLARATION_LIST),  //
         makeNt(NT::STATEMENT_LIST),    //
         makePn(PU::RBRACE)) {
      return makeNt(NT::COMPOUND_STATEMENT);
    };
  }

  void CFGrammar::declaration_list() {
    using NT = NonTerminal;

    // declaration_list → declaration
    rule(makeNt(NT::DECLARATION)) { return makeNt(NT::DECLARATION_LIST); };

    // declaration_list → declaration_list declaration
    rule(makeNt(NT::DECLARATION_LIST),  //
         makeNt(NT::DECLARATION)) {
      return makeNt(NT::DECLARATION_LIST);
    };
  }

  void CFGrammar::statement_list() {
    using NT = NonTerminal;

    // statement_list → statement
    rule(makeNt(NT::STATEMENT)) { return makeNt(NT::STATEMENT_LIST); };

    // statement_list → statement_list statement
    rule(makeNt(NT::STATEMENT_LIST),  //
         makeNt(NT::STATEMENT)) {
      return makeNt(NT::STATEMENT_LIST);
    };
  }

  void CFGrammar::expression_statement() {
    using NT = NonTerminal;
    using PU = Punctuator;

    // expression_statement → ';'
    rule(makePn(PU::SEMI_COLON)) { return makeNt(NT::EXPRESSION_STATEMENT); };

    // expression_statement → expression ';'
    rule(makeNt(NT::EXPR),  //
         makePn(PU::SEMI_COLON)) {
      return makeNt(NT::EXPRESSION_STATEMENT);
    };
  }

  void CFGrammar::selection_statement() {
    using KW = Keyword;
    using PU = Punctuator;
    using NT = NonTerminal;

    // selection_statement → IF '(' expression ')' statement
    rule(makeKw(KW::IF),      //
         makePn(PU::LPAREN),  //
         makeNt(NT::EXPR),    //
         makePn(PU::RPAREN),  //
         makeNt(NT::STATEMENT)) {
      return makeNt(NT::SELECTION_STATEMENT);
    };

    // selection_statement → IF '(' expression ')' statement ELSE statement
    rule(makeKw(KW::IF),         //
         makePn(PU::LPAREN),     //
         makeNt(NT::EXPR),       //
         makePn(PU::RPAREN),     //
         makeNt(NT::STATEMENT),  //
         makeKw(KW::ELSE),       //
         makeNt(NT::STATEMENT)) {
      return makeNt(NT::SELECTION_STATEMENT);
    };

    // selection_statement → SWITCH '(' expression ')' statement
    rule(makeKw(KW::SWITCH),  //
         makePn(PU::LPAREN),  //
         makeNt(NT::EXPR),    //
         makePn(PU::RPAREN),  //
         makeNt(NT::STATEMENT)) {
      return makeNt(NT::SELECTION_STATEMENT);
    };
  }

  void CFGrammar::iteration_statement() {
    using KW = Keyword;
    using PU = Punctuator;
    using NT = NonTerminal;

    // iteration_statement → WHILE '(' expression ')' statement
    rule(makeKw(KW::WHILE),   //
         makePn(PU::LPAREN),  //
         makeNt(NT::EXPR),    //
         makePn(PU::RPAREN),  //
         makeNt(NT::STATEMENT)) {
      return makeNt(NT::ITERATION_STATEMENT);
    };

    // iteration_statement → DO statement WHILE '(' expression ')' ';'
    rule(makeKw(KW::DO),         //
         makeNt(NT::STATEMENT),  //
         makeKw(KW::WHILE),      //
         makePn(PU::LPAREN),     //
         makeNt(NT::EXPR),       //
         makePn(PU::RPAREN),     //
         makePn(PU::SEMI_COLON)) {
      return makeNt(NT::ITERATION_STATEMENT);
    };

    // iteration_statement → FOR '(' expression_statement expression_statement
    // ')' statement
    rule(makeKw(KW::FOR),                   //
         makePn(PU::LPAREN),                //
         makeNt(NT::EXPRESSION_STATEMENT),  //
         makeNt(NT::EXPRESSION_STATEMENT),  //
         makePn(PU::RPAREN),                //
         makeNt(NT::STATEMENT)) {
      return makeNt(NT::ITERATION_STATEMENT);
    };

    // iteration_statement → FOR '(' expression_statement expression_statement
    // expression ')' statement
    rule(makeKw(KW::FOR),                   //
         makePn(PU::LPAREN),                //
         makeNt(NT::EXPRESSION_STATEMENT),  //
         makeNt(NT::EXPRESSION_STATEMENT),  //
         makeNt(NT::EXPR),                  //
         makePn(PU::RPAREN),                //
         makeNt(NT::STATEMENT)) {
      return makeNt(NT::ITERATION_STATEMENT);
    };
  }

  void CFGrammar::jump_statement() {
    using KW = Keyword;
    using PU = Punctuator;
    using NT = NonTerminal;

    // jump_statement → GOTO IDENTIFIER ';'
    rule(makeKw(KW::GOTO),  //
         makeId(),          //
         makePn(PU::SEMI_COLON)) {
      return makeNt(NT::JUMP_STATEMENT);
    };

    // jump_statement → CONTINUE ';'
    rule(makeKw(KW::CONTINUE),  //
         makePn(Punctuator::SEMI_COLON)) {
      return makeNt(NT::JUMP_STATEMENT);
    };

    // jump_statement → BREAK ';'
    rule(makeKw(KW::BREAK),  //
         makePn(PU::SEMI_COLON)) {
      return makeNt(NT::JUMP_STATEMENT);
    };

    // jump_statement → RETURN ';'
    rule(makeKw(KW::RETURN),  //
         makePn(PU::SEMI_COLON)) {
      return makeNt(NT::JUMP_STATEMENT);
    };

    // jump_statement → RETURN expression ';'
    rule(makeKw(KW::RETURN),  //
         makeNt(NT::EXPR),    //
         makePn(PU::SEMI_COLON)) {
      return makeNt(NT::JUMP_STATEMENT);
    };
  }

  void CFGrammar::translation_unit() {
    using NT = NonTerminal;

    // translation_unit → external_declaration
    rule(makeNt(NT::EXTERNAL_DECLARATION)) {
      return makeNt(NT::TRANSLATION_UNIT);
    };

    // translation_unit → translation_unit external_declaration
    rule(makeNt(NT::TRANSLATION_UNIT),  //
         makeNt(NT::EXTERNAL_DECLARATION)) {
      return makeNt(NT::TRANSLATION_UNIT);
    };
  }

  void CFGrammar::external_declaration() {
    using NT = NonTerminal;

    // external_declaration → function_definition
    rule(makeNt(NT::FUNCTION_DEFINITION)) {
      return makeNt(NT::EXTERNAL_DECLARATION);
    };

    // external_declaration → declaration
    rule(makeNt(NT::DECLARATION)) { return makeNt(NT::EXTERNAL_DECLARATION); };
  }

  void CFGrammar::function_definition() {
    using NT = NonTerminal;

    // function_definition → declaration_specifiers declarator
    // declaration_list compound_statement
    rule(makeNt(NT::DECLARATION_SPECIFIERS),  //
         makeNt(NT::DECLARATOR),              //
         makeNt(NT::DECLARATION_LIST),        //
         makeNt(NT::COMPOUND_STATEMENT)) {
      return makeNt(NT::FUNCTION_DEFINITION);
    };

    // function_definition → declaration_specifiers declarator
    // compound_statement
    rule(makeNt(NT::DECLARATION_SPECIFIERS),  //
         makeNt(NT::DECLARATOR),              //
         makeNt(NT::COMPOUND_STATEMENT)) {
      return makeNt(NT::FUNCTION_DEFINITION);
    };

    // function_definition → declarator declaration_list compound_statement
    rule(makeNt(NT::DECLARATOR),        //
         makeNt(NT::DECLARATION_LIST),  //
         makeNt(NT::COMPOUND_STATEMENT)) {
      return makeNt(NT::FUNCTION_DEFINITION);
    };

    // function_definition → declarator compound_statement
    rule(makeNt(NT::DECLARATOR),  //
         makeNt(NT::COMPOUND_STATEMENT)) {
      return makeNt(NT::FUNCTION_DEFINITION);
    };
  }

}  // namespace compiler
