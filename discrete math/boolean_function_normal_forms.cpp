#include "stdafx.h"

#include <cmath>

#include <deque>

#include "boolean_function.h"

namespace discr_math {

boolean_function::ccnf::ccnf( const truth_table &Table ) : Signature(Table) {
  int NoofArgs = Table.getNumArgs();
  for (int Row = 0, SignSize = 1 << NoofArgs; Row < SignSize; Row++)
    if (!Table.signBit(Row)) {
      Conjunctions.push_back(args());
      args &CBack = Conjunctions.back();
      for (int Arg = 0, Size = Table.getNumArgs(); Arg < NoofArgs; Arg++)
        CBack.push_back(!Table.argsBit(Row, Arg));
    }
}

bool boolean_function::ccnf::operator()( const std::vector<bool> &ArgsIn ) const {
  bool Res = 1;
  int NoofArgs = Signature.getNumArgs();

  for (const auto &Args : Conjunctions) {
    bool ArgsRes = 0;
    for (int i = 0; i < NoofArgs; i++)
      ArgsRes |= (Args[i] == ArgsIn[i]);
    Res &= ArgsRes;
  }

  return Res;
}

std::ostream & operator<<( std::ostream &Stream, const boolean_function::ccnf &Form ) {
  int NoofArgs = Form.Signature.getNumArgs(), NoofConj = Form.Conjunctions.size();

  for (int ConjNum = 0; ConjNum < NoofConj; ConjNum++) {
    Stream << "(";
    bool ArgsRes = 0;
    for (int ArgNum = 0; ArgNum < NoofArgs; ArgNum++)
      Stream << (Form.Conjunctions[ConjNum][ArgNum] ? "x" : "!x") <<
        ArgNum << (ArgNum < NoofArgs - 1 ? " V " : "");
    Stream << ")" << ((ConjNum < NoofConj - 1) ? " & " : " ");
  }

  return Stream;
}

boolean_function::cdnf::cdnf( const truth_table &Table ) : Signature(Table) {
  int NoofArgs = Table.getNumArgs();
  for (int Row = 0, SignSize = 1 << NoofArgs; Row < SignSize; Row++)
    if (Table.signBit(Row)) {
      Disjunctions.push_back(args());
      args &CBack = Disjunctions.back();
      for (int Arg = 0, Size = Table.getNumArgs(); Arg < NoofArgs; Arg++)
        CBack.push_back(Table.argsBit(Row, Arg));
    }
}

bool boolean_function::cdnf::operator()( const std::vector<bool> &ArgsIn ) const {
  bool Res = 0;
  int NoofArgs = Signature.getNumArgs();

  for (const auto &Args : Disjunctions) {
    bool ArgsRes = 1;
    for (int i = 0; i < NoofArgs; i++)
      ArgsRes &= (Args[i] == ArgsIn[i]);
    Res |= ArgsRes;
  }

  return Res;
}

std::ostream & operator<<( std::ostream &Stream, const boolean_function::cdnf &Form ) {
  int NoofArgs = Form.Signature.getNumArgs(), NoofDisj = Form.Disjunctions.size();

  for (int DisjNum = 0; DisjNum < NoofDisj; DisjNum++) {
    Stream << "(";
    bool ArgsRes = 0;
    for (int ArgNum = 0; ArgNum < NoofArgs; ArgNum++)
      Stream << (Form.Disjunctions[DisjNum][ArgNum] ? "x" : "!x") <<
        ArgNum << (ArgNum < NoofArgs - 1 ? " & " : "");
    Stream << ")" << ((DisjNum < NoofDisj - 1) ? " V " : " ");
  }

  return Stream;
}

boolean_function::rdnf::rdnf( const truth_table &Table ) : Signature(Table) {
  std::vector<args> DisjTmp;
  int NoofDisj = 1 << Table.getNumArgs();
  DisjTmp.resize(NoofDisj);
  int NoofArgs = Table.getNumArgs();
  for (int Row = 0, SignSize = 1 << NoofArgs; Row < SignSize; Row++) {
    int GrayRow = binToGray(Row);
    if (Table.signBit(GrayRow)) {
      for (int Arg = 0, Size = Table.getNumArgs(); Arg < NoofArgs; Arg++)
        DisjTmp[Row].push_back({Table.argsBit(GrayRow, Arg), Arg});
    }
  }

  bool Changed = true;
  while (Changed) {
    Changed = false;
    NoofDisj = DisjTmp.size();
    for (int i = 0; i < NoofDisj - 1; i++)
      if (DisjTmp[i].size() > 0) {
        for (int j = i + 1; j < NoofDisj; j++) {
          if  (DisjTmp[j].size() == DisjTmp[i].size()) {
            int Pos = 0, Diff = 0, Size = DisjTmp[i].size();
            for (int k = 0; k < Size; k++)
              if (DisjTmp[i][k].Value != DisjTmp[j][k].Value) {
                Pos = k;
                Diff++;
              }
            if (Diff == 1) {
              DisjTmp[i].erase(DisjTmp[i].begin() + (int)Pos);
              DisjTmp.erase(DisjTmp.begin() + j);
              Changed = true;
              break;
            } else if (Diff == Size) {
              DisjTmp.erase(DisjTmp.begin() + j);
              Changed = true;
              break;
            }
          }
        }
        if (Changed)
          break;
      }
  }

  for (const auto &it : DisjTmp)
    if (it.size() > 0)
      Disjunctions.push_back(it);
}

bool boolean_function::rdnf::operator()( const std::vector<bool> &ArgsIn ) const {
  bool Res = 0;
  //int NoofArgs = Signature.getNumArgs();

  for (const auto &Args : Disjunctions) {
    bool ArgsRes = 1;
    for (int i = 0, Size = Args.size(); i < Size; i++)
      ArgsRes &= (Args[i].Value == ArgsIn[Args[i].Num]);
    Res |= ArgsRes;
  }

  return Res;
}

std::ostream & operator<<( std::ostream &Stream, const boolean_function::rdnf &Form ) {
  int NoofArgs = Form.Signature.getNumArgs(), NoofDisj = Form.Disjunctions.size();

  for (int DisjNum = 0; DisjNum < NoofDisj; DisjNum++) {
    Stream << "(";
    bool ArgsRes = 0;
    for (int ArgNum = 0, Size = Form.Disjunctions[DisjNum].size(); ArgNum < Size; ArgNum++)
      Stream << (Form.Disjunctions[DisjNum][ArgNum].Value ? "x" : "!x") <<
          Form.Disjunctions[DisjNum][ArgNum].Num << (ArgNum < Size - 1 ? " & " : "");
    Stream << ")" << ((DisjNum < NoofDisj - 1) ? " V " : " ");
  }

  return Stream;
}

} // End of 'discr_math' namespace
