#include "Operators.hpp"

namespace compiler {

  Operator OperatorHandler::from(std::string_view str) {
    if (str == "+") return Operator::ADD;
    if (str == "-") return Operator::SUB;
    if (str == "*") return Operator::MUL;
    if (str == "/") return Operator::DIV;
    if (str == "%") return Operator::MOD;
    if (str == "++") return Operator::INC;
    if (str == "--") return Operator::DEC;
    if (str == "=") return Operator::ASSIGN;
    if (str == "+=") return Operator::ADD_EQ;
    if (str == "-=") return Operator::SUB_EQ;
    if (str == "*=") return Operator::MUL_EQ;
    if (str == "/=") return Operator::DIV_EQ;
    if (str == "%=") return Operator::MOD_EQ;
    if (str == "&") return Operator::BAND;
    if (str == "|") return Operator::BOR;
    if (str == "^") return Operator::BXOR;
    if (str == "~") return Operator::BNOT;
    if (str == "<<") return Operator::LSHIFT;
    if (str == ">>") return Operator::RSHIFT;
    if (str == "<<=") return Operator::LSHIFT_EQ;
    if (str == ">>=") return Operator::RSHIFT_EQ;
    if (str == "&=") return Operator::AND_EQ;
    if (str == "|=") return Operator::OR_EQ;
    if (str == "^=") return Operator::XOR_EQ;
    if (str == "==") return Operator::EQ;
    if (str == "!=") return Operator::NEQ;
    if (str == "<") return Operator::LT;
    if (str == ">") return Operator::GT;
    if (str == "<=") return Operator::LTE;
    if (str == ">=") return Operator::GTE;
    if (str == "?:") return Operator::TERNARY;

    return Operator::UNKNOWN;
  }

  bool OperatorHandler::isUnary(Operator op) { return false; }
  bool OperatorHandler::isBinary(Operator op) { return false; }
  bool OperatorHandler::isTernary(Operator op) { return false; }
  bool OperatorHandler::isAssignment(Operator op) { return false; }
  bool OperatorHandler::isLogical(Operator op) { return false; }
  bool OperatorHandler::isBitwise(Operator op) { return false; }
  bool OperatorHandler::isComparison(Operator op) { return false; }
  bool OperatorHandler::isIncrementDecrement(Operator op) { return false; }
}  // namespace compiler