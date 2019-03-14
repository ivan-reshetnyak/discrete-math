#include <iostream>
#include <conio.h>

#include <gtest/gtest.h>
#include "../discrete math/boolean_function.h"

using namespace discr_math;
using std::cout;
using std::endl;

static const int
  SignatureInt = 191,
  NumOfArgs = 11;

TEST( truth_table_comparison, ccnf ) {
  boolean_function::signature Signature(NumOfArgs, SignatureInt);
  boolean_function::truth_table TruthTable(Signature);
  boolean_function::ccnf CCNF(Signature);

  for (int Args = 0; Args < (1 << NumOfArgs); Args++) {
    auto ArgsVec = boolean_function::intToBoolVec(NumOfArgs, Args);
    ASSERT_TRUE(CCNF(ArgsVec) == TruthTable(ArgsVec));
  }
}

TEST( truth_table_comparison, cdnf ) {
  boolean_function::signature Signature(NumOfArgs, SignatureInt);
  boolean_function::truth_table TruthTable(Signature);
  boolean_function::cdnf CDNF(Signature);

  for (int Args = 0; Args < (1 << NumOfArgs); Args++) {
    auto ArgsVec = boolean_function::intToBoolVec(NumOfArgs, Args);
    ASSERT_TRUE(CDNF(ArgsVec) == TruthTable(ArgsVec));
  }
}

TEST( truth_table_comparison, rdnf ) {
  boolean_function::signature Signature(NumOfArgs, SignatureInt);
  boolean_function::truth_table TruthTable(Signature);
  boolean_function::rdnf RDNF(Signature);

  for (int Args = 0; Args < (1 << NumOfArgs); Args++) {
    auto ArgsVec = boolean_function::intToBoolVec(NumOfArgs, Args);
    ASSERT_TRUE(RDNF(ArgsVec) == TruthTable(ArgsVec));
  }
}

int main( int argc, char **argv ) {
  boolean_function::signature Signature(NumOfArgs, SignatureInt);
  boolean_function::truth_table TruthTable(Signature);
  boolean_function::ccnf CCNF(Signature);
  boolean_function::cdnf CDNF(Signature);
  boolean_function::rdnf RDNF(Signature);
  //boolean_function Func(Signature);

  cout << Signature << endl << endl;
  cout << TruthTable << endl << endl;
  cout << CCNF << endl << endl;
  cout << CDNF << endl << endl;
  cout << RDNF << endl << endl;

  testing::InitGoogleTest(&argc, argv);
  RUN_ALL_TESTS();
  _getch();
  return 0;
}
