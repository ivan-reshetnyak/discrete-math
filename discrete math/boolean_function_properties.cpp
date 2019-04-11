#include "stdafx.h"

#include <cmath>

#include "boolean_function.h"

namespace discr_math {

bool boolean_function::isSelfDual( void ) {
  truth_table Table(Signature);
  signature NewSign = Signature;

  for (int i = 0, Size = 1 << Signature.getNumArgs(); i < Size; i++)
    NewSign[i] = !Table(intToBoolVec(~i));

  return NewSign == Signature;
}

bool boolean_function::isOneSaving( void ) {
  truth_table Table(Signature);

  return Table(one());
}

bool boolean_function::isZeroSaving( void ) {
  truth_table Table(Signature);

  return !Table(zero());
}

bool boolean_function::isMonotonous( void ) {
  truth_table Table(Signature);

  bool Val = 0;
  for (int i = 0, Size = 1 << Signature.getNumArgs(); i < Size; i++)
    if (!Table(intToBoolVec(i)) && Val == 0)
      return false;

  return true;
}

bool boolean_function::isLinear( void ) {
  zhegalkin_poly Poly(Signature);

  for (int i = 0, Size = 1 << Signature.getNumArgs(); i < Size; i++)
    if (Poly[intToBoolVec(i)] && (i != (1 << (int)log2(i))))
      return false;
  return true;
}

bool boolean_function::isSymmetric( void ) {
  truth_table Table(Signature);
  std::vector<int> ValuesAtNumberOfOnes(Signature.getNumArgs() + 1, -1);

  for (int i = 0, Size = 1 << Signature.getNumArgs(); i < Size; i++) {
    int NumberOfOnes = 0;
    for (int Bit = 0, Size = Signature.getNumArgs(); Bit < Size; Bit++)
      if ((i >> Bit) & 1)
        NumberOfOnes++;

    bool Val = Table(intToBoolVec(i));
    if (ValuesAtNumberOfOnes[NumberOfOnes] == -1)
      ValuesAtNumberOfOnes[NumberOfOnes] = (int)Val;
    else if (ValuesAtNumberOfOnes[NumberOfOnes] != (int)Val)
      return false;
  }

  return true;
}

} // End of 'discr_math' namespace
