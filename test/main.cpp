#include <iostream>
#include <conio.h>

#include "../discrete math/boolean_function.h"

using namespace discr_math;
using std::cout;
using std::endl;

int main( void ) {
  boolean_function::signature Signature(3, 6);
  boolean_function::truth_table TruthTable(Signature);
  boolean_function::ccnf CCNF(Signature);
  boolean_function::cdnf CDNF(Signature);
  boolean_function Func(Signature);

  cout << Signature << endl << endl;
  cout << TruthTable << endl << endl;
  cout << CCNF << endl << endl;
  cout << CDNF << endl << endl;
  _getch();
  return 0;
}
