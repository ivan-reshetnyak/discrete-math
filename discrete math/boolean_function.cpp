#include "stdafx.h"

#include <cmath>
#include <functional>

#include <algorithm>
#include <iomanip>
#include <vector>
#include <deque>
#include <iostream>

#include "boolean_function.h"

namespace discr_math {

boolean_function::signature::signature( const std::vector<bool> &InVec ) : std::vector<bool>(InVec) {
  this->NoofArgs = (int)log2(size());
  resize(1 << NoofArgs);
}

boolean_function::signature::signature( int NoofArgs, int InSignature ) 
      : NoofArgs(NoofArgs) {
  int Size = 1 << NoofArgs;
  resize(Size, 0);

  for (int i = 0; i < Size; i++) {
    at(i) = InSignature & 1;
    InSignature >>= 1;
  }
  std::reverse(begin(), end());
}

int boolean_function::signature::getNumArgs( void ) const {
  return NoofArgs;
}

std::ostream & operator<<( std::ostream &Stream, const boolean_function::signature &Signature ) { 
  for (const auto &Bit : Signature )
    Stream << Bit;
  return Stream;
}

boolean_function::truth_table::truth_table( const signature &InSign ) : Signature(InSign) {
}

int boolean_function::truth_table::getNumArgs( void ) const {
  return Signature.getNumArgs();
}

bool boolean_function::truth_table::operator()( const std::vector<bool> &Args ) const {
  int ArgsBits = 0, BitNum = 0;
  for (const auto &Bit : Args)
    ArgsBits |= ((int)Bit << BitNum++);

  return Signature[ArgsBits];
}

bool boolean_function::truth_table::signBit( int Num ) const {
  return Signature[Num];
}

bool boolean_function::truth_table::argsBit( int Row, int Col ) const {
  return (int)((Row >> Col) & 1);
}

std::ostream & operator<<( std::ostream &Stream, const boolean_function::truth_table &Table ) {
  int Size = 1 << Table.Signature.getNumArgs();
  for (int Args = 0; Args < Size; Args++) {
    Stream << Args << " | ";
    for (int ArgBit = 0; ArgBit < Table.getNumArgs(); ArgBit++)
      Stream << (int)Table.argsBit(Args, ArgBit) << " ";
    Stream << " | " << (int)Table.signBit(Args) << std::endl;
  }

  return Stream;
}

int boolean_function::boolVecToInt( const std::vector<bool> &Vec ) {
  int Res = 0;

  for (int i = 0, Size = Vec.size(); i < Size; i++)
    Res |= (int)Vec[i] << i;

  return Res;
}

std::vector<bool> boolean_function::intToBoolVec( int NoofArgs, int InSign ) {
  std::vector<bool> Res;
  int Size = NoofArgs;
  Res.resize(Size, 0);

  for (int i = 0; i < Size; i++) {
    Res[i] = InSign & 1;
    InSign >>= 1;
  }

  return Res;
}

int  boolean_function::binToGray( int Bin ) {
  return Bin ^ (Bin >> 1);
}

void boolean_function::binToGray( std::vector<bool> &Binary ) {
  Binary[0] = Binary[0] ^ false;
  for (int Bit = 1, Size = Binary.size(); Bit < Size; Bit++)
    Binary[Bit] = Binary[Bit] ^ Binary[Bit - 1];
}

boolean_function::boolean_function( const signature &Signature ) : Signature(Signature) {
}

std::vector<bool>::reference boolean_function::operator[]( const std::vector<bool> &Args ) {
  return Signature[boolVecToInt(Args)];
}

boolean_function::karnaugh_map::karnaugh_map( const truth_table &Table ) : Table(Table) {
}

bool boolean_function::karnaugh_map::operator()( const std::vector<bool> &Args ) const {
  return Table(Args);
}

/*
std::function<bool(const std::vector<bool> &)> boolean_function::karnaugh_map::operator[]( const std::vector<bool> &Front ) const {
  return []( const std::vector<bool> &Back ) -> bool {
    };
}
*/

std::ostream & operator<<( std::ostream &Stream, const boolean_function::karnaugh_map &Map ) {
  int
    CellSizeY = Map.Table.getNumArgs() >> 1,
    CellSizeX = Map.Table.getNumArgs() - (Map.Table.getNumArgs() >> 1),
    SizeX = 1 << CellSizeX,
    SizeY = 1 << CellSizeY;

  Stream << std::setw(CellSizeX + 1) << "";
  for (int Col = 0; Col < SizeX; Col++) {
    Stream << " ";
    for (int Bit = 0; Bit < CellSizeX; Bit++)
      Stream << ((boolean_function::binToGray(Col) >> Bit) & 1);
  }
  Stream << std::endl;

  for (int Row = 0; Row < SizeY; Row++) {
    Stream << std::setw(1 + CellSizeX - CellSizeY) << " ";
    for (int Bit = 0; Bit < CellSizeY; Bit++)
      Stream << ((boolean_function::binToGray(Row) >> Bit) & 1);
    for (int Col = 0; Col < SizeX; Col++) {
      auto
        RowBits = boolean_function::intToBoolVec(CellSizeY, boolean_function::binToGray(Row)),
        ColBits = boolean_function::intToBoolVec(CellSizeX, boolean_function::binToGray(Col));
      RowBits.insert(RowBits.end(), ColBits.begin(), ColBits.end());
      Stream << std::setw(CellSizeX + 1) << (int)Map(RowBits);
    }
    Stream << std::endl;
  }
  return Stream;
}

boolean_function::zhegalkin_poly::zhegalkin_poly( const truth_table &Table ) : Signature(Table) {
  int MaxSize = 1 << Table.getNumArgs();
  std::vector<bool> Column = (signature)Table;

  if (Column[0])
    Xors.push_back(boolean_function::intToBoolVec(Table.getNumArgs(), 0));
  for (int Size = MaxSize, ColNum = 1; Size > 1; Size--, ColNum++) {
    for (int i = 0; i < Size - 1; i++)
      Column[i] = Column[i] ^ Column[i + 1];
    if (Column[0])
      Xors.push_back(boolean_function::intToBoolVec(Table.getNumArgs(), ColNum));
  }
  //if (Column[0])
  //  Xors.push_back(boolean_function::intToBoolVec(Table.getNumArgs(), MaxSize - 1));
}

bool boolean_function::zhegalkin_poly::operator()( const std::vector<bool> &Args ) const {
  bool Res = false;
  for (int XorNum = 0, Size = Xors.size(); XorNum < Size; XorNum++) {
    bool AndRes = 1;
    for (int ArgNum = 0, Size = Args.size(); ArgNum < Size; ArgNum++)
      if (Xors[XorNum][ArgNum])
        AndRes &= Args[ArgNum];
    if (XorNum == 0)
      Res = AndRes;
    else
      Res = Res ^ AndRes;
  }
  return Res;
}

bool boolean_function::zhegalkin_poly::operator[]( const std::vector<bool> &Args ) const {
  for (const auto &Xor : Xors)
    if (Xor == Args)
      return true;

  return false;
}

std::ostream & operator<<( std::ostream &Stream, const boolean_function::zhegalkin_poly &Poly ) {
  if (Poly.Xors.size() == 0) {
    Stream << "1 XOR 1";
    return Stream;
  }

  for (int XorNum = 0, Size = Poly.Xors.size(); XorNum < Size; XorNum++) {
    for (int ArgNum = 0, Size = Poly.Xors[XorNum].size(); ArgNum < Size; ArgNum++)
      if (Poly.Xors[XorNum][ArgNum])
        Stream << "x" << ArgNum;
    Stream << (XorNum < Size - 1 ? " XOR " : "");
  }
  return Stream;
}

} // End of 'discr_math' namespace
