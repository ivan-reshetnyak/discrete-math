#include "stdafx.h"

#include "boolean_function.h"

namespace discr_math {

#define signatureConversion(type)                                             \
type::operator boolean_function::signature( void ) const {  \
  return Signature;                                                           \
}

#define signatureTableConversion(type)                                        \
type::operator boolean_function::signature( void ) const {  \
  return (signature)Table;                                                    \
}

signatureConversion(boolean_function)
signatureConversion(boolean_function::truth_table)
signatureConversion(boolean_function::ccnf)
signatureConversion(boolean_function::cdnf)
signatureConversion(boolean_function::rdnf)
signatureTableConversion(boolean_function::karnaugh_map)
signatureConversion(boolean_function::zhegalkin_poly)

} // End of 'discr_math' namespace
