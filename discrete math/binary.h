#pragma once

#include <vector>
#include <iostream>

namespace discr_math {

class binary : public std::vector<bool> {
public:
  binary() = default;
  binary( int Val );
  binary operator++( int );
  binary & operator++( void );
  binary operator+( const binary &RV ) const;
  binary & operator+=( const binary &RV );
  binary operator*( int Mul ) const;
  binary & operator*=( int Mul );
  binary pow( int Pow ) const;
  operator int( void ) const;
};

std::ostream & operator<<( std::ostream &Stream, const binary &Bin );

} // End of 'discr_math' namespace
