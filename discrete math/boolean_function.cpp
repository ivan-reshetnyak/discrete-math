#include "stdafx.h"

#include <cmath>

#include <algorithm>
#include <vector>
#include <iostream>

#include "boolean_function.h"

namespace discr_math {

boolean_function::signature::signature( int NoofArgs, long long InSignature ) 
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

boolean_function::truth_table::operator boolean_function::signature( void ) const {
  return Signature;
}

std::vector<bool> boolean_function::intToBoolVec( int NoofArgs, long long InSign ) {
  std::vector<bool> Res;
  int Size = NoofArgs;
  Res.resize(Size, 0);

  for (int i = 0; i < Size; i++) {
    Res[i] = InSign & 1;
    InSign >>= 1;
  }

  return Res;
}

boolean_function::boolean_function( const signature &Signature ) : Signature(Signature) {
}

} // End of 'discr_math' namespace
