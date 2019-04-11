#include <iostream>
#include <conio.h>

#include <gtest/gtest.h>
#include "../discrete math/boolean_function.h"

using namespace discr_math;
using std::cout;
using std::endl;

static const int
  NumOfArgs = 10,
  SignatureInt = (16 + 192) * (1 << (NumOfArgs - 3));

int main( int argc, char **argv ) {
  testing::InitGoogleTest(&argc, argv);
  RUN_ALL_TESTS();

  //auto Sign = boolean_function::signature(std::vector<bool>({true, true, true, false}));
  auto Sign = boolean_function::signature(NumOfArgs, SignatureInt);
  boolean_function Func(Sign);
  boolean_function::truth_table Table(Sign);
  //boolean_function Func(Signature);

  cout << Func << endl << endl
       << Table << endl << endl
       << boolean_function::ccnf((boolean_function::signature)Func) << endl << endl
       << boolean_function::cdnf((boolean_function::signature)Func) << endl << endl
       << boolean_function::rdnf((boolean_function::signature)Func) << endl << endl
       << boolean_function::karnaugh_map((boolean_function::signature)Func) << endl << endl
       << boolean_function::zhegalkin_poly((boolean_function::signature)Func) << endl << endl;

  if (Func.isZeroSaving())
    cout << "Saves zero" << endl;
  if (Func.isOneSaving())
    cout << "Saves one" << endl;
  if (Func.isSelfDual())
    cout << "Self-Dual" << endl;
  if (Func.isMonotonous())
    cout << "Monotonous" << endl;
  if (Func.isSymmetric())
    cout << "Symmetric" << endl;

  _getch();

  return 0;
}

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

TEST( truth_table_comparison, karnaugh ) {
  boolean_function::signature Signature(NumOfArgs, SignatureInt);
  boolean_function::truth_table TruthTable(Signature);
  boolean_function::karnaugh_map Map(Signature);

  for (int Args = 0; Args < (1 << NumOfArgs); Args++) {
    auto ArgsVec = boolean_function::intToBoolVec(NumOfArgs, Args);
    ASSERT_TRUE(Map(ArgsVec) == TruthTable(ArgsVec));
  }
}

TEST( truth_table_comparison, zhegalkin ) {
  boolean_function::signature Signature(NumOfArgs, SignatureInt);
  boolean_function::truth_table TruthTable(Signature);
  boolean_function::zhegalkin_poly Poly(Signature);

  for (int Args = 0; Args < (1 << NumOfArgs); Args++) {
    auto ArgsVec = boolean_function::intToBoolVec(NumOfArgs, Args);
    ASSERT_TRUE(Poly(ArgsVec) == TruthTable(ArgsVec));
  }
}
