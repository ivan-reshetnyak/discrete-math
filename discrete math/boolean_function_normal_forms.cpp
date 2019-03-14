#include "stdafx.h"

#include <deque>

#include "boolean_function.h"

namespace discr_math {

boolean_function::normal_form::element::element( type Type, element *L, element *R )
    : Type(Type), Left(L), Right(R) {
}

boolean_function::normal_form::element::element( type Type, int OperandNum )
    : Type(Type), Left(nullptr), Right(nullptr), OperandNum(OperandNum) {
}

bool boolean_function::normal_form::element::eval( const std::vector<bool> &Args ) const {
  switch (Type) {
  case type::OPERAND:
    return Args[OperandNum];
  case type::INVERSE_OPERAND:
    return !Args[OperandNum];
  case type::OPERATOR_AND:
    return Left->eval(Args) && Right->eval(Args);
  case type::OPERATOR_OR:
    return Left->eval(Args) || Right->eval(Args);
  default:
    throw std::exception("Error evaluating normal form node!");
  }
}

boolean_function::normal_form::normal_form( void )
    : Root(nullptr) {
}

void boolean_function::normal_form::clear( boolean_function::normal_form::element *&Node ) {
  if (Node == nullptr)
    return;
  clear(Node->Left);
  clear(Node->Right);
  delete Node;
  Node = nullptr;
}

void boolean_function::normal_form::clear( void ) {
  clear(Root);
}

bool boolean_function::normal_form::operator()( const std::vector<bool> &Args ) const {
  if (Root == nullptr)
    throw std::exception("Error evaluating normal form: Root is nullptr!");
  return Root->eval(Args);
}

boolean_function::normal_form::~normal_form( void ) {
  clear();
}

boolean_function::ccnf::ccnf( const truth_table &Table ) : Signature(Table) {
  std::deque<element *> TopOrEls;
  int NoofArgs = Table.getNumArgs();
  for (int Row = 0, SignSize = 1 << NoofArgs; Row < SignSize; Row++)
    if (!Table.signBit(Row)) {
      std::deque<element *> ArgEls;
      // Pushing arguments
      for (int Arg = 0, Size = Table.getNumArgs(); Arg < NoofArgs; Arg++) {
        ArgEls.push_back(new element(Table.argsBit(Row, Arg) ? element::type::OPERAND : element::type::INVERSE_OPERAND, Arg));
        ArgsLinearized.push_back(ArgEls.back());
      }

      while (ArgEls.size() > 1) {
        element *E0 = ArgEls[0], *E1 = ArgEls[1];
        ArgEls.pop_front();
        ArgEls.pop_front();
        ArgEls.push_front(new element(element::type::OPERATOR_OR, E0, E1));
      }

      TopOrEls.push_back(ArgEls[0]);
    }

  while (TopOrEls.size() > 1) {
    element *E0 = TopOrEls[0], *E1 = TopOrEls[1];
    TopOrEls.pop_front();
    TopOrEls.pop_front();
    TopOrEls.push_front(new element(element::type::OPERATOR_AND, E0, E1));
  }
  Root = TopOrEls[0];
}

bool boolean_function::ccnf::operator()( const std::vector<bool> &Args ) const {
  return normal_form::operator()( Args );
}

std::ostream & operator<<( std::ostream &Stream, const boolean_function::ccnf &Form ) {
  int NoofArgs = Form.Signature.getNumArgs(), ArgNum = 0, Size = Form.ArgsLinearized.size();
  while (ArgNum < Size) {
    Stream << "(";
    for (int j = 0; j < NoofArgs; j++) {
      Stream << (Form.ArgsLinearized[ArgNum]->Type == boolean_function::normal_form::element::type::OPERAND ? "x" : "!x") <<
        (ArgNum % NoofArgs) << (j < NoofArgs - 1 ? " V " : "");
      ArgNum++;
    }
    Stream << ")" << ((ArgNum < Size - 1) ? " & " : " ");
  }
  return Stream;
}

boolean_function::cdnf::cdnf( const truth_table &Table ) : Signature(Table) {
  std::deque<element *> TopOrEls;
  int NoofArgs = Table.getNumArgs();
  for (int Row = 0, SignSize = 1 << NoofArgs; Row < SignSize; Row++)
    if (Table.signBit(Row)) {
      std::deque<element *> ArgEls;
      // Pushing arguments
      for (int Arg = 0, Size = Table.getNumArgs(); Arg < NoofArgs; Arg++) {
        ArgEls.push_back(new element(Table.argsBit(Row, Arg) ? element::type::OPERAND : element::type::INVERSE_OPERAND, Arg));
        ArgsLinearized.push_back(ArgEls.back());
      }

      while (ArgEls.size() > 1) {
        element *E0 = ArgEls[0], *E1 = ArgEls[1];
        ArgEls.pop_front();
        ArgEls.pop_front();
        ArgEls.push_front(new element(element::type::OPERATOR_AND, E0, E1));
      }

      TopOrEls.push_back(ArgEls[0]);
    }

  while (TopOrEls.size() > 1) {
    element *E0 = TopOrEls[0], *E1 = TopOrEls[1];
    TopOrEls.pop_front();
    TopOrEls.pop_front();
    TopOrEls.push_front(new element(element::type::OPERATOR_OR, E0, E1));
  }
  Root = TopOrEls[0];
}

bool boolean_function::cdnf::operator()( const std::vector<bool> &Args ) const {
  return normal_form::operator()( Args );
}

std::ostream & operator<<( std::ostream &Stream, const boolean_function::cdnf &Form ) {
  int NoofArgs = Form.Signature.getNumArgs(), ArgNum = 0, Size = Form.ArgsLinearized.size();
  while (ArgNum < Size) {
    Stream << "(";
    for (int j = 0; j < NoofArgs; j++) {
      Stream << (Form.ArgsLinearized[ArgNum]->Type == boolean_function::normal_form::element::type::OPERAND ? "x" : "!x") <<
        (ArgNum % NoofArgs) << (j < NoofArgs - 1 ? " & " : "");
      ArgNum++;
    }
    Stream << ")" << ((ArgNum < Size - 1) ? " V " : " ");
  }
  return Stream;
}

} // End of 'discr_math' namespace
