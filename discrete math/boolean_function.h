#pragma once

#include <vector>
#include <iostream>

namespace discr_math {

class boolean_function {
public:
  static std::vector<bool> intToBoolVec( int NoofArgs, int InArgs );
  static int binToGray( int Bin );
  static void binToGray( std::vector<bool> &Binary );

  class signature : public std::vector<bool> {
  private:
    int NoofArgs;
    friend std::ostream & operator<<( std::ostream &, const signature & );
  public:
    signature( int NoofArgs, int InSign );
    int getNumArgs( void ) const;
  };

  class truth_table {
  private:
    signature Signature;
    friend std::ostream & operator<<( std::ostream &, const truth_table & );
  public:
    truth_table( const signature &InSign );
    bool operator()( const std::vector<bool> &Args ) const;
    bool signBit( int Num ) const;
    bool argsBit( int Num, int Col ) const;
    int getNumArgs( void ) const;
    operator signature( void ) const;
  };

  class ccnf {
  private:
    signature Signature;
    friend std::ostream & operator<<( std::ostream &, const ccnf & );
    using args = std::vector<bool>;
    std::vector<args> Conjunctions;

  public:
    ccnf( const truth_table &Table );
    bool operator()( const std::vector<bool> &Args ) const;
  };

  class cdnf {
  private:
    signature Signature;
    using args = std::vector<bool>;
    std::vector<args> Disjunctions;

    friend std::ostream & operator<<( std::ostream &, const cdnf & );
  public:
    cdnf( const truth_table &Table );
    bool operator()( const std::vector<bool> &Args ) const;
  };

  class rdnf {
  private:
    class operand {
    public:
      bool Value;
      int Num;
    };
    signature Signature;
    using args = std::vector<operand>;
    std::vector<args> Disjunctions;

    friend std::ostream & operator<<( std::ostream &, const rdnf & );
  public:
    rdnf( const truth_table &Table );
    bool operator()( const std::vector<bool> &Args ) const;
  };

private:
  signature Signature;
public:
  boolean_function( const signature &Signature );
};

std::ostream & operator<<( std::ostream &Stream, const boolean_function::signature &Signature );
std::ostream & operator<<( std::ostream &Stream, const boolean_function::truth_table &Table );
std::ostream & operator<<( std::ostream &Stream, const boolean_function::ccnf &Form );
std::ostream & operator<<( std::ostream &Stream, const boolean_function::cdnf &Form );
std::ostream & operator<<( std::ostream &Stream, const boolean_function::rdnf &Form );

} // End of 'discr_math' namespace
