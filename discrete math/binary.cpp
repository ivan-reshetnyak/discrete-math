#include "stdafx.h"

#include "binary.h"

namespace discr_math {

binary::binary( int Val ) {
  while (Val > 0) {
    push_back((Val & 1) == 1);
    Val >>= 1;
  }
}

binary & binary::operator++( void ) {
  auto it = begin();

  while (it < end() && (*it)) {
    *it = false;
    it++;
  }

  if (it == end())
    push_back(true);
  else
    *it = true;
  return *this;
}

binary binary::operator++( int ) {
  binary ret = *this;
  ++(*this);
  return ret;
}

std::ostream & operator<<( std::ostream &Stream, const binary &Bin ) {
  for (const auto &it : Bin)
    Stream << it ? '1' : '0';
  return Stream;
}

binary binary::operator+( const binary &RV ) const {
  int Size1 = size(), Size2 = RV.size();
  auto
    it1 = begin(), it2 = RV.begin(),
    End1 = end(), End2 = RV.end();
  if (Size1 < Size2) {
    std::swap(it1, it2);
    std::swap(End1, End2);
    std::swap(Size1, Size2);
  }
  binary Ret;
  Ret.resize(Size1);
  std::copy(it1, End1, Ret.begin());
  auto it = Ret.begin();

  bool Carry = false;
  while (it2 < End2) {
    int Sum = (int)Carry + (int)(*it) + (int)(*it2);

    Carry = (Sum >= 2);
    *it = (bool)(Sum % 2);
    it++;
    it2++;
  }
  if (Carry)
    if (it == Ret.end())
      Ret.push_back(true);
    else {
      while (it < end() && (*it)) {
        *it = false;
        it++;
      }

      if (it == Ret.end())
        Ret.push_back(true);
      else
        *it = true;
    }

  return Ret;
}

binary & binary::operator+=( const binary &RV ) {
  return (*this = *this + RV);
}

binary binary::operator*( int Mul ) const {
  binary Res;
  for (int i = 0; i < Mul; i++)
    Res += *this;
  return Res;
}

binary & binary::operator*=( int Mul ) {
  return (*this = *this * Mul);
}

binary binary::pow( int Pow ) const {
  binary Res = 1;
  int ThisInt = (int)(*this);

  for (int i = 0; i < Pow; i++)
    Res *= ThisInt;;
  return Res;
}

binary::operator int( void ) const {
  int Res = 0, Digit = 1;
  for (const auto &it : *this) {
    if (it)
      Res += Digit;
    Digit <<= 1;
  }
  return Res;
}

} // End of 'discr_math' namespace
