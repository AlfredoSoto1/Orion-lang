// #include "Operators.hpp"

// namespace compiler {

//   Operator OperatorHandler::from(std::string_view str) {
//     if (str == "+") return Operator::ADD;
//     if (str == "-") return Operator::SUB;
//     if (str == "*") return Operator::MUL;
//     if (str == "/") return Operator::DIV;
//     if (str == "%") return Operator::MOD;
//     if (str == "++") return Operator::INC;
//     if (str == "--") return Operator::DEC;
//     if (str == "=") return Operator::ASSIGN;
//     if (str == "+=") return Operator::ADD_EQ;
//     if (str == "-=") return Operator::SUB_EQ;
//     if (str == "*=") return Operator::MUL_EQ;
//     if (str == "/=") return Operator::DIV_EQ;
//     if (str == "%=") return Operator::MOD_EQ;
//     if (str == "&") return Operator::BAND;
//     if (str == "|") return Operator::BOR;
//     if (str == "^") return Operator::BXOR;
//     if (str == "~") return Operator::BNOT;
//     if (str == "<<") return Operator::LSHIFT;
//     if (str == ">>") return Operator::RSHIFT;
//     if (str == "<<=") return Operator::LSHIFT_EQ;
//     if (str == ">>=") return Operator::RSHIFT_EQ;
//     if (str == "&=") return Operator::AND_EQ;
//     if (str == "|=") return Operator::OR_EQ;
//     if (str == "^=") return Operator::XOR_EQ;
//     if (str == "==") return Operator::EQ;
//     if (str == "!=") return Operator::NEQ;
//     if (str == "<") return Operator::LT;
//     if (str == ">") return Operator::GT;
//     if (str == "<=") return Operator::LTE;
//     if (str == ">=") return Operator::GTE;
//     if (str == "?:") return Operator::TERNARY;

//     return Operator::UNKNOWN;
//   }

//   std::string_view OperatorHandler::to_string(Operator op) {
//     using enum Operator;
//     switch (op) {
//       case ADD:
//         return "+";
//       case SUB:
//         return "-";
//       case MUL:
//         return "*";
//       case DIV:
//         return "/";
//       case MOD:
//         return "%";

//       case INC:
//         return "++";
//       case DEC:
//         return "--";

//       case ASSIGN:
//         return "=";
//       case ADD_EQ:
//         return "+=";
//       case SUB_EQ:
//         return "-=";
//       case MUL_EQ:
//         return "*=";
//       case DIV_EQ:
//         return "/=";
//       case MOD_EQ:
//         return "%=";

//       case BAND:
//         return "&";
//       case BOR:
//         return "|";
//       case BXOR:
//         return "^";
//       case BNOT:
//         return "~";
//       case LSHIFT:
//         return "<<";
//       case RSHIFT:
//         return ">>";

//       case LSHIFT_EQ:
//         return "<<=";
//       case RSHIFT_EQ:
//         return ">>=";
//       case AND_EQ:
//         return "&=";
//       case OR_EQ:
//         return "|=";
//       case XOR_EQ:
//         return "^=";
//       case NOT_EQ:
//         return "~=";

//       case AND:
//         return "&&";
//       case OR:
//         return "||";
//       case NOT:
//         return "!";

//       case EQ:
//         return "==";
//       case NEQ:
//         return "!=";
//       case LT:
//         return "<";
//       case GT:
//         return ">";
//       case LTE:
//         return "<=";
//       case GTE:
//         return ">=";

//       case TERNARY:
//         return "?";
//       case LARROW:
//         return "<-";
//       case RARROW:
//         return "->";
//       case REFERENCE:
//         return "&";
//       case EXPR_REFERENCE:
//         return "&&";
//       case PTR:
//         return "*";
//       case PTR_DEREF:
//         return "*";

//       default:
//         return "unknown";
//     }
//   }
// }  // namespace compiler