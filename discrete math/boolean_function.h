#pragma once

#include <vector>
#include <iostream>

namespace discr_math {

class boolean_function {
public:
  static std::vector<bool> intToBoolVec( int NoofArgs, long long InArgs );

  class signature : public std::vector<bool> {
  private:
    int NoofArgs;
    friend std::ostream & operator<<( std::ostream &, const signature & );
  public:
    signature( int NoofArgs, long long InSign );
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

  class normal_form {
  public:
    class element {
    private:
      int OperandNum;
    public:
      enum class type {
        OPERAND, INVERSE_OPERAND, OPERATOR_OR, OPERATOR_AND, BRACKET_L, BRACKET_R
      } Type;
      element *Left, *Right;

      element( type Type, element *L = nullptr, element *R = nullptr );
      element( type Type, int OperandNum );
      bool eval( const std::vector<bool> &Args ) const;
    };
  private:
    void clear( element *&Node );
  protected:
    element *Root;
    void clear( void );
    bool operator()( const std::vector<bool> &Args ) const;
    normal_form( void );
    ~normal_form( void );
  };

  class ccnf : private normal_form {
  private:
    std::vector<element *> ArgsLinearized;
    signature Signature;
    friend std::ostream & operator<<( std::ostream &, const ccnf & );
  public:
    ccnf( const truth_table &Table );
    bool operator()( const std::vector<bool> &Args ) const;
  };

  class cdnf : private normal_form {
  private:
    std::vector<element *> ArgsLinearized;
    signature Signature;
    friend std::ostream & operator<<( std::ostream &, const cdnf & );
  public:
    cdnf( const truth_table &Table );
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

} // End of 'discr_math' namespace
