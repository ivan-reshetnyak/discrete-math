#pragma once

#include <vector>
#include <iostream>

namespace discr_math {

class boolean_function {
public:
  static std::vector<bool> intToBoolVec( int NoofArgs, int InArgs );
  static int boolVecToInt( const std::vector<bool> &Vec );
  static int binToGray( int Bin );
  static void binToGray( std::vector<bool> &Binary );

  class signature : public std::vector<bool> {
  private:
    int NoofArgs;
    friend std::ostream & operator<<( std::ostream &, const signature & );
  public:
    signature( int NoofArgs, int InSign );
    signature( const std::vector<bool> & );
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
    operator signature( void ) const;
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
    operator signature( void ) const;
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
    operator signature( void ) const;
  };

  class karnaugh_map {
  private:
    truth_table Table;

    friend std::ostream & operator<<( std::ostream &, const karnaugh_map & );
  public:
    operator signature( void ) const;
    bool operator()( const std::vector<bool> &Args ) const;
    //std::function<bool(const std::vector<bool> &)> operator[]( const std::vector<bool> &Front ) const;
    karnaugh_map( const truth_table &Table );
  };

  class zhegalkin_poly {
  private:
    signature Signature;
    using args = std::vector<bool>;
    std::vector<args> Xors;

    friend std::ostream & operator<<( std::ostream &, const zhegalkin_poly & );
  public:
    operator signature( void ) const;
    bool operator()( const std::vector<bool> &Args ) const;
    bool operator[]( const std::vector<bool> &Args ) const;  // Check polynomial coefficient
    zhegalkin_poly( const truth_table &Table );
  };

private:
  signature Signature;
public:
  std::vector<bool> one( void ) const { return std::vector<bool>(Signature.getNumArgs(), 1); }
  std::vector<bool> zero( void ) const { return std::vector<bool>(Signature.getNumArgs(), 0); }
  std::vector<bool> intToBoolVec( int InArgs ) { return intToBoolVec(Signature.getNumArgs(), InArgs); }
  operator signature( void ) const;
  std::vector<bool>::reference operator[]( const std::vector<bool> &Args );
  boolean_function( const signature &Signature );

  // Properties check funcs
  bool isSelfDual( void );
  bool isOneSaving( void );
  bool isZeroSaving( void );
  bool isMonotonous( void );
  bool isLinear( void );
  bool isSymmetric( void );
};

std::ostream & operator<<( std::ostream &Stream, const boolean_function::signature &Signature );
std::ostream & operator<<( std::ostream &Stream, const boolean_function::truth_table &Table );
std::ostream & operator<<( std::ostream &Stream, const boolean_function::ccnf &Form );
std::ostream & operator<<( std::ostream &Stream, const boolean_function::cdnf &Form );
std::ostream & operator<<( std::ostream &Stream, const boolean_function::rdnf &Form );
std::ostream & operator<<( std::ostream &Stream, const boolean_function::karnaugh_map &Map );
std::ostream & operator<<( std::ostream &Stream, const boolean_function::zhegalkin_poly &Map );

} // End of 'discr_math' namespace
