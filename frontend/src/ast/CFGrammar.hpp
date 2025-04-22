#pragma once

#include <functional>
#include <unordered_map>

#include "StorageAST.hpp"

namespace compiler {

  class Parser;

  /**
   * @brief Grammar rules for the C/C++-like language.
   *
   * This file contains the grammar rules used by the parser to build the AST.
   * The grammar is based on the C/C++ language specifications.
   */
  class CFGrammar final {
  public:
    CFGrammar() noexcept;
    ~CFGrammar() noexcept;

  public:
    using KW = Keyword;
    using PU = Punctuator;
    using NT = NonTerminal;
    using ReductionHandler = std::function<Symbol(Parser&)>;
    std::unordered_multimap<Rule, ReductionHandler, RuleHash, RuleEqual> table;

  private:
    struct Proxy {
      Rule&& key;
      CFGrammar& parent;
      Proxy(CFGrammar& p, Rule&& k) : parent(p), key(std::move(k)) {}

      void operator=(ReductionHandler&& handler) {
        parent.table.insert({std::move(key), std::move(handler)});
      }
    };

    Proxy emplRule(Rule&& rule);

  private:
    Symbol makeId() const noexcept;
    Symbol makeLt() const noexcept;
    Symbol makeKw(KW kw) const noexcept;
    Symbol makePn(PU pu) const noexcept;
    Symbol makeNt(NT nt) const noexcept;

    Rule makeRule(std::initializer_list<Symbol> syms) const noexcept;

  private:
    void postfix_expr();
    void primary_expr();
    void arg_expr_list();
    void unary_expr();
    void unary_op();
    void cast_expr();
    void multiplicative_expr();
    void additive_expr();
    void shift_expr();
    void relational_expr();
    void equality_expr();
    void and_expr();
    void exclusive_or_expr();
    void inclusive_or_expr();
    void logical_and_expr();
    void logical_or_expr();
    void conditional_expr();
    void assignment_expr();
    void assignment_op();
    void expr();
    void constant_expr();
    void decl();
    void decl_specifiers();
    void init_decl_list();
    void init_decl();
    void storage_class_spec();
    void type_specifier();
    void struct_or_union_specifier();
    void struct_or_union();
    void struct_declaration_list();
    void struct_declaration();
    void specifier_qualifier_list();
    void struct_declarator_list();
    void struct_declarator();
    void enum_specifier();
    void enumerator_list();
    void enumerator();
    void type_qualifier();
    void type_qualifier_list();
    void declarator();
    void direct_declarator();
    void pointer();
    void parameter_type_list();
    void parameter_list();
    void parameter_declaration();
    void identifier_list();
    void type_name();
    void abstract_declarator();
    void direct_abstract_declarator();
    void initializer();
    void initializer_list();
    void statement();
    void labeled_statement();
    void compound_statement();
    void declaration_list();
    void statement_list();
    void expression_statement();
    void selection_statement();
    void iteration_statement();
    void jump_statement();
    void translation_unit();
    void external_declaration();
    void function_definition();
  };

}  // namespace compiler
