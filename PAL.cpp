# include <iostream>
# include <stdio.h>
# include <stdlib.h>
# include <vector>
# include <string>
# include <string.h>
# include <cstring>
# include <sstream>
using namespace std ;

int gfuckPL ;

enum TokenType
{
  LEFT, RIGHT, INT, STRING, DOT, FLOAT, NIL, T, QUOTE, SYMBOL, INITIAL
};

struct Tokeninfo {
  TokenType type;
  string token;
};

struct Node {
  Tokeninfo tokeninfo ;
  Node*left ;
  Node*right ;
};

struct Define {
  string symbol ;
  vector<Tokeninfo> binding ;
  string comefrom ;
};

struct Parameter { // one parameter's data .
  vector<Tokeninfo> para ;
};

Node*NewNode() { // just initial .
  Node* node = new Node() ;
  node->left = NULL ;
  node->right = NULL ;
  node->tokeninfo.token = "" ;
  node->tokeninfo.type = INITIAL ;
  return node ;
} // NewNode()
 
class Scaner { // It should give Parser a Token .

public:
  // some getToken function ...
  // GetToken : get a Token from "GetCommand", and return it ...

  string GetTempToken() ; // return a ensured token .
  string mtoken ; // a temp token, not check with grammer .

  void PeekToken() ; // just use to get TokenType !
  TokenType mtokentype ;

  int GetColumn() ; // get mindex ( column ) .
  int GetLine() ; // get mline ( line ) .
  string ReturnCommand() ;
  void SetLine() ; // set mline = 0 ;
  void SetLineToOne() ; // set mline = 0 ;
  void Skipcomment() ; // set mindex to last of command's char .
  int mClosingQuote ; // count no closing quote's line .
  // GetCommand : get a new command, save in a temp string ...

  void GetCommand() ;
  void Test() ;

  bool mIsEOF ; // if there a line contain EOF, should deal with token then EOF .

private:

  // command : This is a command .

  string mcommand ; // save command .
  int mindex ; // command ' s index
  int mline ; // line count .

  bool IsSymbol() ;
  bool IsInt() ;
  bool IsFloat() ;
  bool IsString() ;
  bool IsNIL() ;
  bool IsT() ;
  bool IsLeftParen() ;
  bool IsRightParen() ;
  bool IsDot() ;
  bool IsQuote() ;

};

void Scaner :: Test() {
  // string temp;
  // cin >> temp;
  GetCommand() ;
  while ( mindex != mcommand.length() - 1 ) {

    PeekToken() ;
    cout << "token type: " << mtokentype << ", " ;
    cout << "token: " << mtoken << endl ;
  } // while

} // Scaner::Test()

void Scaner :: PeekToken() {
  mtoken = GetTempToken() ;
  if ( IsInt() ) {
    mtokentype = INT ;
  } // if

  else if ( IsDot() ) {
    mtokentype = DOT ;
  } // else if

  else if ( IsFloat() ) {
    mtokentype = FLOAT ;
  } // else if

  else if ( IsT() ) {
    mtokentype = T ;
  } // else if

  else if ( IsNIL() ) {
    mtokentype = NIL ; 
  } // else if

  else if ( IsString() ) {
    mtokentype = STRING ;
  } // else if

  else if ( IsLeftParen() ) {
    mtokentype = LEFT ;
  } // else if

  else if ( IsRightParen() ) {
    mtokentype = RIGHT ;
  } // else if

  else if ( IsQuote() ) {
    mtokentype = QUOTE ;
  } // else if

  else {
    mtokentype = SYMBOL ;
  } // else
} // Scaner::PeekToken()

string Scaner :: GetTempToken() { // return a token .
  // read a command from GetToken, and get ( return ) one token from this command .
  // Token type : '(', ')', INT : '123', '-123', '+123', String, DOT, FLOAT : '123.567'
  // NIL : 'nil' or '#f', but not 'NIL' nor 'nIL', T : 't' or '#t', but not 'T' nor '#T'
  // QUOTE : ', SYMBOL

  string token = "" ;

  if ( mindex >= mcommand.length() ) { // it's the last letter, need to get a new command .
    GetCommand() ;
  } // if

  while ( mcommand[mindex] == ' ' || mcommand[mindex] == '\t' || mcommand[mindex] == '\n' ) {
    // skip white space .
    mindex++ ;
    if ( mindex >= mcommand.length() ) { // it's the last letter, need to get a new command .
      GetCommand() ;
    } // if
  } // while


    // below is to check the first letter is separator or not .
  if ( mcommand[mindex] == '(' ) { // "("
    token = token + mcommand[mindex] ;
    mindex++ ;
    while ( mcommand[mindex] == ' ' || mcommand[mindex] == '\t' || mcommand[mindex] == '\n' ) {
      // if there is '(' + '\n' + ')', it should be () !
      mindex++ ;
      if ( mindex >= mcommand.length() ) { // it's the last letter, need to get a new command .
        GetCommand() ;
      } // if
    } // while

    if ( mcommand[mindex] == ')' ) { // if the command contain "()", it should be nil.
      token = token + mcommand[mindex] ;
      mindex++ ;
    } // if

    return token ;
  } // if

  else if ( mcommand[mindex] == ')' ) { // ")"
    token = token + mcommand[mindex] ;
    mindex++ ;
    return token ; 
  } // else if

  else if ( mcommand[mindex] == ';' ) { // ";"
    token = token + mcommand[mindex] ;
    mindex++ ;
    return token ;
  } // else if

  else if ( mcommand[mindex] == '\'' ) { // '\''
    token = token + mcommand[mindex] ;
    mindex++ ;
    return token ;
  } // else if

  else if ( mcommand[mindex] == '\"' ) { // '\"', watch "string" type !
    token = token + mcommand[mindex] ;
    mindex++ ;
    while ( mcommand[mindex] != '\"' ) {
      if ( mcommand[mindex] == '\\' && mcommand[mindex + 1] == '\"' ) { // \" is allow in a string .
        // token = token + mcommand[mindex] ;
        mindex++ ;
        token = token + mcommand[mindex] ;
        mindex++ ;
      } // if

      if ( mcommand[mindex] == '\n' ) { // it should have another ' " ' .
        mindex ++ ; // The mindex in this error message should +1, because mindex start from 0 !
        throw "(no closing quote)" ;
        // cout << "String type error." << endl ;
        // return token ;
      } // if

      if ( mcommand[mindex] != '\"' ) {
        token = token + mcommand[mindex] ;
        mindex++ ;
      } // if
    } // while

    token = token + mcommand[mindex] ; //  this char should be ' " ' .
    mindex++ ;
    return token ;
  } // else if

  else {

  } // else

  while ( mcommand[mindex] != ' ' && mcommand[mindex] != '\t' && mcommand[mindex] != '\n' ) {

    if ( mcommand[mindex] == '(' ) { // "("
      return token ;
    } // if

    else if ( mcommand[mindex] == ')' ) { // ")"
      return token ;
    } // else if

    else if ( mcommand[mindex] == ';' ) { // ";"
      return token;
    } // else if

    else if ( mcommand[mindex] == '\'' ) { // '\''
      return token ;
    } // else if

    else if ( mcommand[mindex] == '\"' ) { // '\"'
      return token ;
    } // else if

    // ////////////////// above meaning : if the letter is separator, is should stop read .

    else {
      token = token + mcommand[mindex] ;
    } // else 

    mindex++ ;
  } // while 

  return token ;

} // Scaner::GetTempToken()

void Scaner :: GetCommand() {
  char tempchar = '\0' ;
  bool isAllWhiteSapce = true ; // check if command are all whitespace .
  string end = "(exit)" ;
  string end1 = end + '\n' ;
  // end = end + '\n' ;
  mcommand = "" ;
  mindex = 0 ;
  while ( tempchar != '\n' ) { // get a command .
    tempchar = cin.get() ;
    mcommand = mcommand + tempchar ;
    if ( tempchar == -1 && mcommand.length() == 1 ) { // whole line only EOF .
      throw "(no more input)" ;
    } // if

    else if ( tempchar == -1 ) {
      mIsEOF = true ; // there are other token should deal with, can't throw yet .
      tempchar = '\n' ;
    } // else if

  } // while

  mline = mline + 1 ;
  mClosingQuote = mClosingQuote + 1 ;
  if ( mcommand == end || mcommand == end1 ) {
    throw "(exit)" ;
  } // if
} // Scaner::GetCommand() 

bool Scaner :: IsSymbol() { // is Symbol or not .
  if ( !IsInt() && !IsFloat() && !IsString() && !IsNIL() &&
       !IsT() && !IsLeftParen() && !IsRightParen() ) {
    return true ;
  } // if

  else {
    return false ;
  } // else
} // Scaner::IsSymbol()

bool Scaner :: IsInt() {
  int i = 0;

  if ( mtoken == "+" || mtoken == "-" ) { // just + OR - is not int .
    return false ;
  } // if

  if ( mtoken[i] == '+' || mtoken[i] == '-' ) { // +1
    i++ ;
  } // if

  while ( i < mtoken.length() ) {
    if ( mtoken[i] >= '0' && mtoken[i] <= '9' ) {
      i++ ;
    } // if 

    else {
      return false ;
    } // else
  } // while

  return true ;
} // Scaner::IsInt()

bool Scaner :: IsFloat() {
  int i = 0 ;
  bool onlyonedot = true ; // if there are two, it's not float .
  bool nonumber = true ; // if just have "+ or -" + ".", but no number, it should be a symbol . 

  if ( mtoken == "." ) { // just ont dot is DOT .
    return false ;
  } // if

  if ( mtoken == "+" || mtoken == "-" ) { // just + or - is not float ;
    return false ;
  } // if

  if ( mtoken[i] == '+' || mtoken[i] == '-' ) { // +123.
    i++ ;
  } // if

  if ( mtoken[i] == '.' ) { // .123
    i++ ;
    while ( i < mtoken.length() ) {
      if ( mtoken[i] >= '0' && mtoken[i] <= '9' ) {
        nonumber = false ;
        i++ ;
      } // if

      else {
        return false ;
      } // else
    } // while

    if ( nonumber == false ) {
      return true ;
    } // if

    else {
      return false ;
    } // else 
  } // if

  else if ( mtoken[i] >= '0' && mtoken[i] <= '9' ) { // 123. OR 123.123
    while ( i < mtoken.length() ) {
      if ( mtoken[i] >= '0' && mtoken[i] <= '9' ) {
        i++ ;
      } // if

      else if ( mtoken[i] == '.' ) {
        i++;
        if ( onlyonedot == false ) { // there are two dot .
          return false ;
        } // if

        onlyonedot = false ;
      } // else if

      else {
        return false ;
      } // else 
    } // while

    mtokentype = FLOAT ;
    return true ;
  } // else if

  else {
    return false ;
  } // else
} // Scaner::IsFloat()

bool Scaner :: IsString() {
  int i = 0;

  if ( mtoken[0] != '\"' && mtoken[mtoken.length() - 1] != '\"' ) {
    return false ;
  } // if
  else {
    return true ;
  } // else
} // Scaner::IsString()

bool Scaner :: IsNIL() {
  if ( mtoken == "nil" || mtoken == "#f" || mtoken == "()" ) {
    return true ;
  } // if

  else {
    return false ;
  } // else
} // Scaner::IsNIL()

bool Scaner :: IsT() {
  if ( mtoken == "t" || mtoken == "#t" ) {
    return true ;
  } // if

  else {
    return false ;
  } // else
} // Scaner::IsT()

bool Scaner :: IsLeftParen() { // is Paren or not .
  if ( mtoken == "(" ) {
    return true ;
  } // if 

  else {
    return false ;
  } // else 
} // Scaner::IsLeftParen()

bool Scaner :: IsRightParen() {
  if ( mtoken == ")" ) {
    return true ;
  } // if 

  else {
    return false ;
  } // else 
} // Scaner::IsRightParen()

bool Scaner :: IsDot() {
  if ( mtoken == "." ) {
    return true ;
  } // if

  else {
    return false ;
  } // else
} // Scaner::IsDot()

bool Scaner :: IsQuote() {
  if ( mtoken == "\'" || mtoken == "quote" ) {
    return true ;
  } // if 

  else {
    return false ;
  } // else
} // Scaner::IsQuote()

int Scaner :: GetColumn() {
  return mindex ;
} // Scaner::GetColumn()

int Scaner :: GetLine() {
  return mline ;
} // Scaner::GetLine()

void Scaner :: SetLine() {
  mline = 0 ;
} // Scaner::SetLine()

void Scaner :: SetLineToOne() {
  mline = 1 ;
} // Scaner::SetLineToOne()

void Scaner :: Skipcomment() {
  mindex = mcommand.length() ;
} // Scaner::Skipcomment()

string Scaner :: ReturnCommand() {
  return mcommand ;
} // Scaner::ReturnCommand()


class Morefunction {
// When evaluating an S-exp, Scheme/Lisp treats the first token
// after '(' as a function call .
// OurScheme distinguishes between upper and lower cases .
// Whenever a function is called, its parameters are evaluated first.

public:
  vector<Tokeninfo> mOriginCommand ; // cdr, car, deal with DOT like : (cdr '(2 . ( 3 4 )))
  vector<Tokeninfo> mResultCommand ; // After evaluated command .
  vector<Tokeninfo> mByPassParent ; // use to pass parameter .
  vector<Define> mDefined ; // SYMBOL defined save here .
  vector<Parameter> mParaData ; // parameters of a function .
  int mindex ; // the index which must be a LEFT, to record function call behind LEFT .
  int mCounter ; // To check which function is top level, the last function be called .

  // if isDefined is true, set this int let caller know where the symbol is define in vector .
  int mDefinedIndex ; 
  bool mIsSymbol ; // use to deal with boolean function, when recursive happen .

  // check if orginanl command has dot been delete or not. EX:( list? '(3 . ( 4 5 ))) should be true .
  bool mHasDot ; 
  bool IsInternal( string token ) ; // check if internal function .

  int CountParent( int checkP ) ; // count how many parameter .

  bool IsDefined( string isDefined ) ; // the symbol is defined or not .

  int FindLimit() ; // To find the range of a function can modify .

  int FindLimit2( int index ) ;

  void Cons() ;
  
  void List() ; 

  int ByPass( int check ) ;

  void Defined() ;

  void Car() ; 

  void Cdr() ; 

  bool IsPair() ;

  bool IsNull() ;
  
  bool IsInteger() ;

  bool IsReal() ;
  
  bool IsNumber() ;  

  bool IsString() ;
  
  bool IsBoolean() ;
  
  bool IsSymbol() ;
  
  bool IsEqv() ;

  bool IsEqual() ;
  
  bool IsAtom() ;
   
  bool IsList() ;  

  void Math() ; // Basic arithmetic, logical and string operations .

  void And() ;

  void Or() ;

  void Not() ;

  void StringOP() ; // Basic string operations .
  
  void If() ;

  void Cond() ;

  void Begin() ;
}; // Morefunction

int Morefunction :: FindLimit() {
  int countLeft = 1 ;
  int check = mindex + 2 ;
  int limit = 0 ;
  while ( countLeft != 0 ) {
    if ( mResultCommand[check].type == LEFT ) {
      countLeft ++ ;
    } // if

    else if ( mResultCommand[check].type == RIGHT ) {
      countLeft -- ;
      if ( countLeft == 0 ) {
        limit = check ;
      } // if
    } // else if

    check ++ ;
  } // while

  return limit ;
} // Morefunction::FindLimit()

int Morefunction :: FindLimit2( int index ) {
  int countLeft = 1 ;
  int limit = 0 ;

  while ( countLeft != 0 ) {
    if ( mResultCommand[index].type == LEFT ) {
      countLeft ++ ;
    } // if

    else if ( mResultCommand[index].type == RIGHT ) {
      countLeft -- ;
    } // else if

    if ( countLeft == 0 ) {
      limit = index ;
    } // if

    index ++ ;
  } // while

  return limit ;
} // Morefunction::FindLimit2()

int Morefunction :: CountParent( int checkP ) { // count how many parameter .
  int parameter = 0 ;
  Tokeninfo temptoken ;
  Parameter temppara ;
  int countQuote = 0 ;
  int countLeft = 1 ;
  while ( countLeft != 0 ) {

    // if is number, count as one parameter .
    if ( mResultCommand[checkP].type == FLOAT || mResultCommand[checkP].type == INT ) {
      temptoken.token = mResultCommand[checkP].token ;
      temptoken.type = mResultCommand[checkP].type ;
      temppara.para.push_back( temptoken ) ;
      mParaData.push_back( temppara ) ;
      temppara.para.clear() ;
      parameter ++ ;
    } // if

    else if ( mResultCommand[checkP].type == SYMBOL ) {
      // check is defined or not .
      temptoken.token = mResultCommand[checkP].token ;
      temptoken.type = mResultCommand[checkP].type ;
      temppara.para.push_back( temptoken ) ;
      mParaData.push_back( temppara ) ;
      temppara.para.clear() ;
      parameter ++ ;
      /*
      if ( !IsDefined( mResultCommand[checkP].token ) ) {
        cout << "CountParent(), Error, symbol is not defined!" << endl ;
      } // if

      else {
        temptoken.token = mResultCommand[checkP].token ;
        temptoken.type = mResultCommand[checkP].type ;
        temppara.para.push_back( temptoken ) ;
        mParaData.push_back( temppara ) ;
        temppara.para.clear() ;
        parameter ++ ;
      } // else
      */
    } // else if

    else if ( mResultCommand[checkP].type == STRING ) {
      temptoken.token = mResultCommand[checkP].token ;
      temptoken.type = mResultCommand[checkP].type ;
      temppara.para.push_back( temptoken ) ;
      mParaData.push_back( temppara ) ;
      temppara.para.clear() ;
      parameter ++ ;
    } // else if

    else if  ( mResultCommand[checkP].type == NIL || mResultCommand[checkP].type == T ) {
      temptoken.token = mResultCommand[checkP].token ;
      temptoken.type = mResultCommand[checkP].type ;
      temppara.para.push_back( temptoken ) ;
      mParaData.push_back( temppara ) ;
      temppara.para.clear() ;
      parameter ++ ;
    } // else if

    else if ( mResultCommand[checkP].type == QUOTE || mResultCommand[checkP].token == "quote" ) {
      // do ByPass() .
      // if ByPass it's ok, then count as one parameter .
      countQuote ++ ;
      if ( countQuote > 1 ) {
        mByPassParent.clear() ;
      } // if

      checkP = ByPass( checkP ) ;
      temppara.para.assign( mByPassParent.begin(), mByPassParent.end() ) ;
      mParaData.push_back( temppara ) ;
      temppara.para.clear() ;
      
      checkP -- ;
      parameter ++ ;
    } // else if

    else if ( mResultCommand[checkP].type == LEFT ) {
      temptoken.token = mResultCommand[checkP].token ;
      temptoken.type = mResultCommand[checkP].type ;
      temppara.para.push_back( temptoken ) ;
      countLeft ++ ;
      parameter ++ ;

      int countLeft2 = 1 ;
      int tempindex = checkP + 1 ;
      while ( countLeft2 != 0 ) {
        if ( mResultCommand[tempindex].type == LEFT ) {
          countLeft2 ++ ;
        } // if

        else if ( mResultCommand[tempindex].type == RIGHT ) { 
          countLeft2 -- ;
        } // else if

        temptoken.token = mResultCommand[tempindex].token ;
        temptoken.type = mResultCommand[tempindex].type ;
        temppara.para.push_back( temptoken ) ;
        tempindex ++ ;
        checkP ++ ;
      } // while

      mParaData.push_back( temppara ) ;
      temppara.para.clear() ;
      checkP -- ;
    } // else if

    else if ( mResultCommand[checkP].type == RIGHT ) {
      countLeft -- ;
    } // else if

    checkP ++ ;
  } // while

  return parameter ;
} // Morefunction::CountParent()

bool Morefunction :: IsInternal( string token ) {

  if ( token == "cons" || token == "list" || token == "quote" || token == "'"
       || token == "define" || token == "car" || token == "cdr" ) {
    return true ;
  } // if

  else if ( token == "pair?" || token == "null?"
            || token == "integer?" || token == "real?"
            || token == "number?" || token == "string?"
            || token == "boolean?" || token == "symbol?" 
            || token == "eqv?" || token == "equal?"
            || token == "atom?" || token == "list?" ) {
    return true ;
  } // else if

  else if ( token == "if" || token == "cond" ) {
    return true ;
  } // else if

  else if ( token == "+" || token == "-"
            || token == "*" || token == "/"
            || token == "not" || token == "and"
            || token == "or" || token == ">"
            || token == ">=" || token == "<"
            || token == "<=" || token == "="
            || token == "string-append" || token == "begin"
            || token == "string>?" || token == "string<?" 
            || token == "string=?" ) {
    return true ;
  } // else if

  else {
    return false ;
  } // else

} // Morefunction::IsInternal()

bool Morefunction :: IsDefined( string isDefined ) {
  int index = 0 ; 
  if ( !mDefined.empty() ) {
    while ( index < mDefined.size() ) {
      if ( mDefined[index].symbol == isDefined ) {
        mDefinedIndex = index ;
        return true ;
      } // if

      index ++ ;
    } // while

    return false ;
  } // if
  
  else { // no symbol is defined .
    return false ;
  } // else
} // Morefunction::IsDefined()

void Morefunction :: Cons() {
  // first check parameter is find or not .
  // The index should be "LEFT" .
  Tokeninfo token ;
  token.token = "." ;
  token.type = DOT ;
  int parameter = 0 ; 
  int check = mindex + 2 ; // mindex should LEFT, mindex + 1 should be "cons", check next token mindex +2 .
  int record = 0 ; // record where dot should be insert .
  bool addDot  = true ; // record is the DOT should add or not ?
  // First check the parameter is current or not .
  // cons should have 2 parameter .

  int limit = 0 ;

  if ( CountParent( check ) != 2 ) {
    // cout << "Cons() ERROR, wrong parameter number !" << endl ;
    return ;
  } // if

  limit = FindLimit() ;
  int range = 0 ;
  // leave a piar of "("  ")" .
  mResultCommand.erase( mResultCommand.begin() + ( mindex + 1 ), mResultCommand.begin() + limit  ) ;

  // parameter 1 :

  if ( mParaData[0].para[0].type == SYMBOL ) { // restore it .
    if ( IsDefined( mParaData[0].para[0].token ) ) {
      for ( int i = mDefined[mDefinedIndex].binding.size() - 1 ; i >= 0 ; i-- ) {
        mResultCommand.insert( mResultCommand.begin() + ( mindex + 1 ), 
                               mDefined[mDefinedIndex].binding[i] ) ;
      } // for

      mResultCommand.insert( mResultCommand.begin() + // add a DOT .
                             ( mindex + 1 + mDefined[mDefinedIndex].binding.size() ), token ) ;
      range = mDefined[mDefinedIndex].binding.size() + 1 ;
    } // if

    else { // not a symbol, may be a list like '(xxx) .
      for ( int i = mParaData[0].para.size() - 1 ; i >= 0 ; i -- ) {
        mResultCommand.insert( mResultCommand.begin() + ( mindex + 1 ), mParaData[0].para[i] ) ;
      } // for

      mResultCommand.insert( mResultCommand.begin() + ( mindex + 1 + mParaData[0].para.size() ),
                             token ) ; // add a DOT .
      range = mParaData[0].para.size() + 1 ;
    } // else
  } // if

  else { // not a symbol, may be a list like '(xxx) .
    for ( int i = mParaData[0].para.size() - 1 ; i >= 0 ; i -- ) {
      mResultCommand.insert( mResultCommand.begin() + ( mindex + 1 ), mParaData[0].para[i] ) ;
    } // for

    mResultCommand.insert( mResultCommand.begin() + ( mindex + 1 + mParaData[0].para.size() ),
                           token ) ; // add a DOT .
    range = mParaData[0].para.size() + 1 ;
  } // else

  range ++ ;
  // parameter 2 :
  if ( mParaData[1].para[0].type == SYMBOL ) { // restore it .
    if ( IsDefined( mParaData[1].para[0].token ) ) {
      for ( int i = mDefined[mDefinedIndex].binding.size() - 1 ; i >= 0 ; i-- ) {
        mResultCommand.insert( mResultCommand.begin() + ( mindex + range ), 
                               mDefined[mDefinedIndex].binding[i] ) ;
      } // for
    } // if

    else {
      for ( int i = mParaData[1].para.size() - 1 ; i >= 0 ; i -- ) {
        mResultCommand.insert( mResultCommand.begin() + ( mindex + range ), mParaData[1].para[i] ) ;
      } // for
    } // else 
  } // if

  else { // not a symbol, may be a list like '(xxx) .
    for ( int i = mParaData[1].para.size() - 1 ; i >= 0 ; i -- ) {
      mResultCommand.insert( mResultCommand.begin() + ( mindex + range ), mParaData[1].para[i] ) ;
    } // for
  } // else

  // delete no need DOT !

  int k = 0 ; 
  int countLeft = 0 ;

  vector<int> checkRight ;
  while ( k < mResultCommand.size() ) {
    if ( mResultCommand[k].type == LEFT ) {
      countLeft = countLeft + 1 ;
    } // if

    else if ( mResultCommand[k].type == DOT ) {
      if ( mResultCommand[k+1].type == LEFT ) {
        checkRight.push_back( countLeft ) ;
        countLeft = countLeft + 1 ;
        mResultCommand.erase( mResultCommand.begin() + k ) ; // delete DOT .
        mResultCommand.erase( mResultCommand.begin() + k ) ; // delete LEFT .
        k = k - 1 ;
      } // if
    } // else if

    else if ( mResultCommand[k].type == RIGHT ) {
      countLeft = countLeft - 1 ;
      if ( !checkRight.empty() ) {
        if ( countLeft == checkRight.back() ) {
          mResultCommand.erase( mResultCommand.begin() + k ) ; // delete RIGHT .
          checkRight.pop_back() ;
          k = k - 1 ;
        } // if
      } // if
    } // else if

    k  =  k + 1 ;
  } // while

  // cout << "End of Cons --> " ;
  // for ( int g = 0 ; g <mResultCommand.size() ; g ++ ) {
    // cout <<mResultCommand[g].token ;
  // } // for
  // cout << endl ;
} // Morefunction::Cons()

void Morefunction :: List() {
  // first check parameter is find or not .
  // The index should be "LEFT" .
  int check = mindex + 2 ;

  // since parameter is find, then do what list should do .
  int countLeft = 1 ;

   

  while ( countLeft != 0 ) {

    if ( mResultCommand[check].type == RIGHT ) {
      countLeft -- ;
    } // if

    else if ( mResultCommand[check].type == QUOTE ) {
      int temp = 0 ;
      temp = ByPass( check ) ;
      mResultCommand.erase( mResultCommand.begin() + check, mResultCommand.begin() + 
                            ( check + mByPassParent.size() ) ) ;

      for ( int i = mByPassParent.size() - 1 ; i >= 0 ; i -- ) {
        mResultCommand.insert( mResultCommand.begin() + check, mByPassParent[i] ) ;
      } // for

      check = temp - 1 ;
    } // else if

    else if ( mResultCommand[check].type == SYMBOL ) {
      if ( IsDefined( mResultCommand[check].token ) ) {
        mResultCommand.erase( mResultCommand.begin() + check ) ;
        for ( int i = mDefined[mDefinedIndex].binding.size() - 1 ; i >= 0 ; i-- ) {
          mResultCommand.insert( mResultCommand.begin() + check, mDefined[mDefinedIndex].binding[i] ) ;
        } // for

        countLeft = 1 ;
        check = mindex + 1 ;
      } // if

      else {
      
      } // else
    } // else if

    else if ( mResultCommand[check].type == LEFT ) {
      countLeft ++ ;
    } // else if 

    check ++ ;
  } // while 

  mResultCommand.erase( mResultCommand.begin() + mindex + 1 ) ; // delete token "list" .
} // Morefunction::List()

int Morefunction :: ByPass( int check ) {

  int countLeft = 0 ;
  Tokeninfo token ;
  // initial
  mByPassParent.clear() ;

  // check should be "QUOTE"
  mResultCommand.erase( mResultCommand.begin() + check ) ; // delete token "quote" .

  // check next , token
  // if ATOM, that is not a LEFT
  if ( mResultCommand[check].type != LEFT ) {
    // index of mResultCommand should skip this "Not Left" token, and return the index after skip ,
    if ( mResultCommand[check].type == SYMBOL ) {
      token.token = mResultCommand[check].token ;
      token.type = INITIAL ; // if is a symbol behind quote, then it just a token, do not have binding !
      mByPassParent.push_back( token ) ;
    } // if

    else {
      mByPassParent.push_back( mResultCommand[check] ) ; // this token should return as a parameter .
    } // else
    
    check = check + 1 ; // skip the token .
  } // if

  else  { // is LEFT
    // count LEFT and RIGHT, to know how many token should deal with .
    
    mByPassParent.push_back( mResultCommand[check] ) ; // push back LEFT .
    check ++ ;
    countLeft ++ ;
    
    while ( countLeft != 0 ) {
      if ( mResultCommand[check].type == SYMBOL ) {
        // if is a symbol behind quote, then it just a token, do not have binding !
        token.token = mResultCommand[check].token ;
        token.type = INITIAL ;
        mByPassParent.push_back( token ) ;
      } // if

      else {
        mByPassParent.push_back( mResultCommand[check] ) ;
      } // else

      if ( mResultCommand[check].type == LEFT ) {
        countLeft ++ ;
      } // if

      else if ( mResultCommand[check].type == RIGHT ) {
        countLeft -- ;
      } // else if

      check ++ ; // shuold go to next token behind "Right" .
    } // while
  } // else 

  
  return check ;
} // Morefunction::ByPass()

void Morefunction :: Defined() {
  int check = mindex + 2 ;
  int parameter = 0 ;
  int limit = 0 ;
  int countParent = 1 ; // it same thing like countLeft .
  bool isQuote = false ;
  bool isUpdate = false ;
  bool isSymbol = false ;
  bool isLeft = false ;
  string temp = "" ; // temp string save symbol .
  string comefrom = "" ; // where this symbol come from ?
  Tokeninfo temptoken ;
  Define tempdefine ;
  limit = FindLimit() ;

  while ( countParent != 0 ) {
  // if is number, count as one parameter .
    if ( mResultCommand[check].type == SYMBOL && parameter == 0 ) {
      if ( IsInternal( mResultCommand[check].token ) ) {
        // error, can't define internal function
        // cout << "Error, can't define internal function !" << endl ;
      } // if

      temp = mResultCommand[check].token ;
      parameter ++ ;
    } // if

    else if ( mResultCommand[check].type == SYMBOL && parameter == 1 ) {
      // use a defined symbol to define a symbol, check this symbol is defined or not .
      if ( IsDefined( mResultCommand[check].token ) ) {
        comefrom = mDefined[mDefinedIndex].comefrom ;
        isSymbol = true ;
        tempdefine.binding.assign( mDefined[mDefinedIndex].binding.begin(), 
                                   mDefined[mDefinedIndex].binding.end() ) ;

        parameter ++ ;
      } // if

      else {
        // cout << "Defined() Error, symbol not defined !" << endl ;
      } // else
    } // else if

    else if ( ( mResultCommand[check].type == QUOTE || mResultCommand[check].token == "quote" )
              && parameter == 1 ) {
      // do ByPass() .
      isQuote = true ;
      check = ByPass( check ) ;
      parameter ++ ;
      // if ByPass it's ok, then count as one parameter .
    } // else if

    else if ( parameter == 1 && mResultCommand[check].type == INT ) {
      temptoken.token = mResultCommand[check].token ;
      temptoken.type = mResultCommand[check].type ;
      parameter ++ ;
    } // else if

    else if ( parameter == 1 && mResultCommand[check].type == FLOAT ) {
      temptoken.token = mResultCommand[check].token ;
      temptoken.type = mResultCommand[check].type ;
      parameter ++ ;
    } // else if

    else if ( parameter == 1 && mResultCommand[check].type == STRING ) {
      temptoken.token = mResultCommand[check].token ;
      temptoken.type = mResultCommand[check].type ;
      parameter ++ ;
    } // else if

    else if ( parameter == 1 && mResultCommand[check].type == NIL ) {
      temptoken.token = mResultCommand[check].token ;
      temptoken.type = mResultCommand[check].type ;
      parameter ++ ;
    } // else if

    else if ( parameter == 1 && mResultCommand[check].type == T ) {
      temptoken.token = mResultCommand[check].token ;
      temptoken.type = mResultCommand[check].type ;
      parameter ++ ;      
    } // else if

    else if ( parameter == 1 && mResultCommand[check].type == LEFT ) {
      countParent ++ ;
      int countLeft = 1 ;
      int k = check + 1 ;
      isLeft = true ;
      tempdefine.binding.push_back( mResultCommand[check] ) ;
      while ( countLeft != 0 ) {
        if ( mResultCommand[k].type == LEFT ) {
          countLeft ++ ;
        } // if

        else if ( mResultCommand[k].type == RIGHT ) {
          countLeft -- ;
        } // else if

        tempdefine.binding.push_back( mResultCommand[k] ) ;
        k++ ;
      } // while

      parameter ++ ;
    } // else if

    else if ( mResultCommand[check].type == RIGHT ) {
      countParent -- ;
    } // else if

    if ( check < mResultCommand.size() -1 ) {
      check ++ ;
    } // if

    
  } // while

  if ( parameter == 2 ) {
    check = mindex + 2 ;
    if ( temp != "" ) { 
      if ( ! mDefined.empty() ) {
        int k = 0 ;
        
        while ( k < mDefined.size() ) {
          if ( temp == mDefined[k].symbol ) { // this SYMBOL is defined .
            isUpdate = true ;
            mDefined[k].binding.clear() ; // clear old data .
            if ( isQuote ) { // paramter is quote .
              // update data in mDefined .
              mDefined[k].binding.assign( mByPassParent.begin(), mByPassParent.end() ) ;
              mByPassParent.clear() ;
            } // if

            else {
              if ( isSymbol ) {
                mDefined[k].binding.assign( mDefined[mDefinedIndex].binding.begin(), 
                                            mDefined[mDefinedIndex].binding.end() ) ;
                mDefined[k].comefrom = comefrom ;
              } // if

              else if ( isLeft ) {
                mDefined[k].binding.assign( tempdefine.binding.begin(), tempdefine.binding.end() ) ;
              } // else if

              else {
                mDefined[k].binding.push_back( temptoken ) ;
              } // else
            } // else
          } // if

          k ++ ;
        } // while
      } // if

      if ( ! isUpdate ) { // there are no symbol defined in mDefined, creat a new one .
        if ( isQuote ) {
          tempdefine.symbol = temp ;
          tempdefine.comefrom = temp ;
          tempdefine.binding.assign( mByPassParent.begin(), mByPassParent.end() ) ;
          mDefined.push_back( tempdefine ) ;
          mByPassParent.clear() ;
        } // if
        
        else {
          if ( isSymbol ) {
            tempdefine.symbol = temp ;
            tempdefine.binding.assign( mDefined[mDefinedIndex].binding.begin(), 
                                       mDefined[mDefinedIndex].binding.end() ) ;
            tempdefine.comefrom = comefrom ;
            mDefined.push_back( tempdefine ) ;
          } // if

          else if ( isLeft ) {
            tempdefine.symbol = temp ;
            tempdefine.comefrom = temp ;
            mDefined.push_back( tempdefine ) ;
          } // else if 

          else {
            tempdefine.symbol = temp ;
            tempdefine.comefrom = temp ;
            tempdefine.binding.push_back( temptoken ) ;
            mDefined.push_back( tempdefine ) ;
          } // else
        } // else
      } // if
    } // if

  } // if 

  else {
    // cout << "error message : defined ." << parameter << endl ;
  } // else 

  mResultCommand.clear() ;
  string output = temp ;
  temp = temp + " defined" ;
  temptoken.token = temp ;
  temptoken.type = STRING ;
  mResultCommand.push_back( temptoken ) ;
} // Morefunction::Defined()

void Morefunction :: Car() {
  // int check = mindex ;
  int check = mindex + 2 ;
  int limit = 0 ; // the range of car can touch, the last token that car can touch, limit + 1 is illegal .
  // count which Right is "car" 's Right .
  int countLeft = 1 ;
  bool isOneSymbol = false ;
  Tokeninfo token ;
  Parameter para ;

  if ( CountParent( check ) != 1 ) {
    // cout << "Car() ERROR, parameter wrong !" << endl ;
    return ;
  } // if

  int index = 0 ;

  if ( mParaData[0].para[index].type == SYMBOL ) { // start with symbol,delete it and restore it .
    if ( IsDefined( mParaData[0].para[index].token ) ) {

      mParaData[0].para.assign( mDefined[mDefinedIndex].binding.begin(), 
                                mDefined[mDefinedIndex].binding.end() ) ;
    } // if
  } // if

  limit = FindLimit() ;

  // "check" must be a LEFT,
  if ( mParaData[0].para[index].type == LEFT ) {
    // check next is a LEFT tor not .
    if ( mParaData[0].para[index + 1].type == LEFT ) {

      int tempindex = index + 1 ; // should be LEFT .

      token.token = mParaData[0].para[tempindex].token ;
      token.type = mParaData[0].para[tempindex].type ;
      para.para.push_back( token ) ;

      tempindex ++ ;

      while ( countLeft != 0 ) {
        if ( mParaData[0].para[tempindex].type == LEFT ) {
          countLeft ++ ;
        } // if

        else if ( mParaData[0].para[tempindex].type == RIGHT ) {
          countLeft -- ;
        } // else if

        token.token = mParaData[0].para[tempindex].token ;
        token.type = mParaData[0].para[tempindex].type ;
        para.para.push_back( token ) ;

        tempindex ++ ;
      } // while

      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      for ( int i = para.para.size() - 1 ; i >= 0 ; i -- ) {
        mResultCommand.insert( mResultCommand.begin() + mindex, para.para[i] ) ;
      } // for
    } // if 

    else { // just return this "Not LEFT" token, cause it must be the first one, left part .
      token.token = mParaData[0].para[index + 1].token ;
      token.type = mParaData[0].para[index + 1].type ;
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    } // else
  } // if

  else {
    // cout << "Car() Error, not a LEFT !" << endl ;
  } // else
} // Morefunction::Car()

void Morefunction :: Cdr() {
  // use Car code to modify, to know where is LEFT part, delete it !
  // int check = mindex ;
  int check = mindex + 2 ;
  int limit = 0 ; // the range of car can touch, the last token that car can touch, limit + 1 is illegal .
  // count which Right is "car" 's Right .
  int countLeft = 1 ;
  int isOneSymbol = false ;
  Tokeninfo token ;
  Parameter para ;

  if ( CountParent( check ) != 1 ) {
    // cout << "Cdr() ERROR, parameter wrong !" << endl ;
    return ;
  } // if

  int index = 0 ;

  if ( mParaData[0].para[index].type == SYMBOL ) { // start with symbol, restore it .
    if ( IsDefined( mParaData[0].para[index].token ) ) {
      mParaData[0].para.assign( mDefined[mDefinedIndex].binding.begin(), 
                                mDefined[mDefinedIndex].binding.end() ) ;
    } // if
  } // if

  limit = FindLimit() ;

  // "check" must be a LEFT,
  if ( mParaData[0].para[index].type == LEFT ) {
    // Find where is the end of "left part" .
    if ( mParaData[0].para[index + 1].type == LEFT ) {

      int tempindex = index + 1 ; // should be LEFT .

      tempindex ++ ;

      while ( countLeft != 0 ) {
        if ( mParaData[0].para[tempindex].type == LEFT ) {
          countLeft ++ ;
        } // if

        else if ( mParaData[0].para[tempindex].type == RIGHT ) {
          countLeft -- ;
        } // else if

        tempindex ++ ;
      } // while

      // tempindex should be after RIGHT, also means the start of "right part" !
      index = tempindex ;
    } // if 

    else {
      index = index + 2 ;
    } // else

 
    if ( mParaData[0].para[index].type == RIGHT ) { // like : (cdr '( 3 ) ), there isn't have any right part.
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "nil" ;
      token.type = NIL ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    } // if

    else if ( mParaData[0].para[index].type == DOT ) { // maybe a DOT, skip it .
      index ++ ;
      token.token = mParaData[0].para[index].token ;
      token.type = mParaData[0].para[index].type ;
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    } // else if

    else {
      while ( index < mParaData[0].para.size() ) {
        token.token = mParaData[0].para[index].token ;
        token.type = mParaData[0].para[index].type ;
        para.para.push_back( token ) ;
        index ++ ;
      } // while

      // leave a LEFT, this condition need a pair of LEFT RIGHT .
      mResultCommand.erase( mResultCommand.begin() + ( mindex + 1 ), 
                            mResultCommand.begin() + ( limit + 1 ) ) ;
      for ( int i = para.para.size() - 1 ; i >= 0 ; i -- ) {
        mResultCommand.insert( mResultCommand.begin() + ( mindex + 1 ), para.para[i] ) ;
      } // for
    } // else
  } // if

  else {
    // cout << "Cdr() Error, not a LEFT !" << endl ;
  } // else
} // Morefunction::Cdr()

bool Morefunction :: IsPair() {
  int check = mindex + 2 ;
  Tokeninfo token ;
  int limit = 0 ;

  limit = FindLimit() ;

  if ( mResultCommand[check].type == LEFT ) {
    mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
    token.token = "true" ;
    token.type = T ;
    mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    return true ;
  } // if

  else if ( mResultCommand[check].type == SYMBOL ) { // check is this symbol defined .
    if ( IsDefined( mResultCommand[check].token ) ) { // if so, restore it's binding, and recursive .
      mIsSymbol = true ;
      mResultCommand.erase( mResultCommand.begin() + check ) ;

      int k = 0 ;
      for ( k = mDefined[mDefinedIndex].binding.size() - 1 ; k >= 0 ; k -- ) {
        mResultCommand.insert( mResultCommand.begin() + check, mDefined[mDefinedIndex].binding[k] ) ;
      } // for

      IsPair() ;
      return true ;
    } // if

    else {
      // cout << "IsPair() Error, not defined symbol !" << endl ;
      return false ;
    } // else
  } // else if

  else if ( mResultCommand[check].type == QUOTE ) {
    ByPass( check ) ;
    limit = FindLimit() ;
    if ( mByPassParent[0].type == LEFT ) {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "true" ;
      token.type = T ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
      return true ;
    } // if

    else {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "false" ;
      token.type = NIL ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
      return false ;
    } // else
  } // else if

  else {
    mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
    token.token = "false" ;
    token.type = NIL ;
    mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;

    return false ;
  } // else
} // Morefunction::IsPair()

bool Morefunction :: IsNull() {
  int check = mindex + 2 ;
  Tokeninfo token ;
  int limit = 0 ;

  limit = FindLimit() ;

  if ( mResultCommand[check].type == NIL ) {
    mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
    token.token = "true" ;
    token.type = T ;
    mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    return true ;
  } // if

  else if ( mResultCommand[check].type == SYMBOL ) { // check is this symbol defined .
    if ( IsDefined( mResultCommand[check].token ) ) { // if so, restore it's binding, and recursive .
      mIsSymbol = true ;
      mResultCommand.erase( mResultCommand.begin() + check ) ;

      int k = 0 ;
      for ( k = mDefined[mDefinedIndex].binding.size() - 1 ; k >= 0 ; k -- ) {
        mResultCommand.insert( mResultCommand.begin() + check, mDefined[mDefinedIndex].binding[k] ) ;
      } // for

      IsNull() ;
      return true ;
    } // if

    else {
      // cout << "IsNull() Error, not defined symbol !" << endl ;
      return false ;
    } // else
  } // else if

  else if ( mResultCommand[check].type == QUOTE ) {
    ByPass( check ) ;
    limit = FindLimit() ;
    if ( mByPassParent[0].type == NIL ) {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "true" ;
      token.type = T ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
      return true ;
    } // if

    else {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "false" ;
      token.type = NIL ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
      return false ;
    } // else
  } // else if

  else {
    mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
    token.token = "false" ;
    token.type = NIL ;
    mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    return false ;
  } // else
} // Morefunction::IsNull()
  
bool Morefunction :: IsInteger() {
  int check = mindex + 2 ;
  Tokeninfo token ;
  int limit = 0 ;

  limit = FindLimit() ;

  if ( mResultCommand[check].type == INT ) {
    mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
    token.token = "true" ;
    token.type = T ;
    mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    return true ;
  } // if

  else if ( mResultCommand[check].type == SYMBOL ) { // check is this symbol defined .
    if ( IsDefined( mResultCommand[check].token ) ) { // if so, restore it's binding, and recursive .
      mIsSymbol = true ;
      mResultCommand.erase( mResultCommand.begin() + check ) ;

      int k = 0 ;
      for ( k = mDefined[mDefinedIndex].binding.size() - 1 ; k >= 0 ; k -- ) {
        mResultCommand.insert( mResultCommand.begin() + check, mDefined[mDefinedIndex].binding[k] ) ;
      } // for

      IsInteger() ;
      return true ;
    } // if

    else {
      // cout << "IsInteger() Error, not defined symbol !" << endl ;
      return false ;
    } // else
  } // else if

  else if ( mResultCommand[check].type == QUOTE ) {
    ByPass( check ) ;
    limit = FindLimit() ;
    if ( mByPassParent[0].type == INT ) {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "true" ;
      token.type = T ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
      return true ;
    } // if

    else {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "false" ;
      token.type = NIL ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
      return false ;
    } // else
  } // else if

  else {
    mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
    token.token = "false" ;
    token.type = NIL ;
    mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;

    return false ;
  } // else
} // Morefunction::IsInteger()

bool Morefunction :: IsReal() {
  int check = mindex + 2 ;
  Tokeninfo token ;
  int limit = 0 ;

  limit = FindLimit() ;

  if ( mResultCommand[check].type == INT || mResultCommand[check].type == FLOAT ) {
    mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
    token.token = "true" ;
    token.type = T ;
    mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;

    return true ;
  } // if

  else if ( mResultCommand[check].type == SYMBOL ) { // check is this symbol defined .
    if ( IsDefined( mResultCommand[check].token ) ) { // if so, restore it's binding, and recursive .
      mIsSymbol = true ;
      mResultCommand.erase( mResultCommand.begin() + check ) ;

      int k = 0 ;
      for ( k = mDefined[mDefinedIndex].binding.size() - 1 ; k >= 0 ; k -- ) {
        mResultCommand.insert( mResultCommand.begin() + check, mDefined[mDefinedIndex].binding[k] ) ;
      } // for

      IsReal() ;
      return true ;
    } // if

    else {
      // cout << "IsReal() Error, not defined symbol !" << endl ;
      return false ;
    } // else
  } // else if

  else if ( mResultCommand[check].type == QUOTE ) {
    ByPass( check ) ;
    limit = FindLimit() ;
    if ( mByPassParent[0].type == INT || mByPassParent[0].type == FLOAT ) {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "true" ;
      token.type = T ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
      return true ;
    } // if

    else {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "false" ;
      token.type = NIL ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
      return false ;
    } // else
  } // else if

  else {
    mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
    token.token = "false" ;
    token.type = NIL ;
    mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    return false ;
  } // else
} // Morefunction::IsReal()
  
bool Morefunction :: IsNumber() {
  int check = mindex + 2 ;
  Tokeninfo token ;
  int limit = 0 ;

  limit = FindLimit() ;

  if ( mResultCommand[check].type == INT || mResultCommand[check].type == FLOAT ) {
    mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
    token.token = "true" ;
    token.type = T ;
    mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;

    return true ;
  } // if

  else if ( mResultCommand[check].type == SYMBOL ) { // check is this symbol defined .
    if ( IsDefined( mResultCommand[check].token ) ) { // if so, restore it's binding, and recursive .
      mIsSymbol = true ;
      mResultCommand.erase( mResultCommand.begin() + check ) ;

      int k = 0 ;
      for ( k = mDefined[mDefinedIndex].binding.size() - 1 ; k >= 0 ; k -- ) {
        mResultCommand.insert( mResultCommand.begin() + check, mDefined[mDefinedIndex].binding[k] ) ;
      } // for

      IsNumber() ;
      return true ;
    } // if

    else {
      // cout << "IsNumber() Error, not defined symbol !" << endl ;
      return false ;
    } // else
  } // else if

  else if ( mResultCommand[check].type == QUOTE ) {
    ByPass( check ) ;
    limit = FindLimit() ;
    if ( mByPassParent[0].type == INT || mByPassParent[0].type == FLOAT ) {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "true" ;
      token.type = T ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
      return true ;
    } // if

    else {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "false" ;
      token.type = NIL ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
      return false ;
    } // else
  } // else if

  else {
    mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
    token.token = "false" ;
    token.type = NIL ;
    mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    return false ;
  } // else
} // Morefunction::IsNumber()

bool Morefunction :: IsString() {
  int check = mindex + 2 ;
  Tokeninfo token ;
  int limit = 0 ;

  limit = FindLimit() ;

  if ( mResultCommand[check].type == STRING ) {
    mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
    token.token = "true" ;
    token.type = T ;
    mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;

    return true ;
  } // if

  else if ( mResultCommand[check].type == SYMBOL ) { // check is this symbol defined .
    if ( IsDefined( mResultCommand[check].token ) ) { // if so, restore it's binding, and recursive .
      mIsSymbol = true ;
      mResultCommand.erase( mResultCommand.begin() + check ) ;

      int k = 0 ;
      for ( k = mDefined[mDefinedIndex].binding.size() - 1 ; k >= 0 ; k -- ) {
        mResultCommand.insert( mResultCommand.begin() + check, mDefined[mDefinedIndex].binding[k] ) ;
      } // for

      IsString() ;
      return true ;
    } // if

    else {
      // cout << "IsString() Error, not defined symbol !" << endl ;
      return false ;
    } // else
  } // else if

  else if ( mResultCommand[check].type == QUOTE ) {
    ByPass( check ) ;
    limit = FindLimit() ;
    if ( mByPassParent[0].type == STRING ) {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "true" ;
      token.type = T ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
      return true ;
    } // if

    else {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "false" ;
      token.type = NIL ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
      return false ;
    } // else
  } // else if

  else {
    mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
    token.token = "false" ;
    token.type = NIL ;
    mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    return false ;
  } // else
} // Morefunction::IsString()

bool Morefunction :: IsBoolean() {
  int check = mindex + 2 ;
  Tokeninfo token ;
  int limit = 0 ;

  limit = FindLimit() ;

  if ( mResultCommand[check].type == NIL || mResultCommand[check].type == T ) {
    mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
    token.token = "true" ;
    token.type = T ;
    mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;

    return true ;
  } // if

  else if ( mResultCommand[check].type == SYMBOL ) { // check is this symbol defined .
    if ( IsDefined( mResultCommand[check].token ) ) { // if so, restore it's binding, and recursive .
      mIsSymbol = true ;
      mResultCommand.erase( mResultCommand.begin() + check ) ;

      int k = 0 ;
      for ( k = mDefined[mDefinedIndex].binding.size() - 1 ; k >= 0 ; k -- ) {
        mResultCommand.insert( mResultCommand.begin() + check, mDefined[mDefinedIndex].binding[k] ) ;
      } // for

      IsBoolean() ;
      return true ;
    } // if

    else {
      // cout << "IsBoolean() Error, not defined symbol !" << endl ;
      return false ;
    } // else
  } // else if

  else if ( mResultCommand[check].type == QUOTE ) {
    ByPass( check ) ;
    limit = FindLimit() ;
    if ( mByPassParent[0].type == T || mByPassParent[0].type == NIL ) {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "true" ;
      token.type = T ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
      return true ;
    } // if

    else {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "false" ;
      token.type = NIL ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
      return false ;
    } // else
  } // else if

  else {
    mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
    token.token = "false" ;
    token.type = NIL ;
    mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    return false ;
  } // else
} // Morefunction::IsBoolean()
  
bool Morefunction :: IsSymbol() {
  int check = mindex + 2 ;
  Tokeninfo token ;
  int limit = 0 ;
  int parameter = 0 ;

  limit = FindLimit() ;

  if ( mResultCommand[check].type == SYMBOL ) { // check is this symbol defined .
    if ( IsDefined( mResultCommand[check].token ) ) { // if so, check is this symbol "means" a real symbol ?

      if ( mDefined[mDefinedIndex].binding[0].type == SYMBOL ) {
        mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
        token.token = "true" ;
        token.type = T ;
        mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
        return true ;
      } // if

      else {
        mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
        token.token = "false" ;
        token.type = NIL ;
        mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
        return false ;
      } // else
    } // if

    else if ( IsInternal( mResultCommand[check].token ) ) {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "false" ;
      token.type = NIL ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
      return false ;
    } // else if

    else {
      // cout << "IsSymbol() Error, not defined symbol !" << endl ;
      return false ;
    } // else
  } // if

  else if ( mResultCommand[check].type == QUOTE ) {
    ByPass( check ) ;
    limit = FindLimit() ;
    if ( mByPassParent[0].type == INITIAL ) { // initial behind the quote is a symbol .
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "true" ;
      token.type = T ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
      return true ;
    } // if

    else {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "false" ;
      token.type = NIL ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
      return false ;
    } // else
  } // else if

  else {
    mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
    token.token = "false" ;
    token.type = NIL ;
    mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    return false ;
  } // else
} // Morefunction::IsSymbol()
  
bool Morefunction :: IsEqv() {
  // eqv? returns "true" only when the two being compared
  // objects are atoms (except in the case of strings)
  // or when the two being compared objects "occupy the
  // same memory space".

  int check = mindex + 2 ;
  Tokeninfo token ; // first para .
  Tokeninfo token2 ; // second para .
  token2.token = "" ;

  string comefrom1 = "-1" ;
  string comefrom2 = "-1" ;
  int limit = 0 ;

  limit = FindLimit() ;

  // parameter 1 :
  if ( mResultCommand[check].type == INT || mResultCommand[check].type == FLOAT
       || mResultCommand[check].type == NIL || mResultCommand[check].type == T ) {
    token.token = mResultCommand[check].token ;
    token.type = mResultCommand[check].type ;
  } // if

  else if ( mResultCommand[check].type == SYMBOL ) { // check is this symbol defined .

    if ( IsDefined( mResultCommand[check].token ) ) { // if so, restore it's binding, and recursive .
      mIsSymbol = true ;
      comefrom1 = mDefined[mDefinedIndex].comefrom ;

      for ( int k = 0 ; k < mDefined[mDefinedIndex].binding.size() ; k++ ) {
        token.token = token.token + mDefined[mDefinedIndex].binding[k].token ;
      } // for

      token.type = mDefined[mDefinedIndex].binding[0].type ;
    } // if

    else {
      // cout << "IsEqv() Error, not defined symbol !" << endl ;
      return false ;
    } // else
  } // else if

  else if ( mResultCommand[check].type == QUOTE ) {
    ByPass( check ) ;
    limit = FindLimit() ;
    if ( mByPassParent.size() > 1 ) { // can just compare atom .
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "false" ;
      token.type = NIL ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
      return false ;
    } // if

    else if ( mByPassParent[0].type == STRING ) {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "false" ;
      token.type = NIL ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
      return false ;
    } // else if

    token.token = mByPassParent[0].token ;
    token.type = mByPassParent[0].type ;
    mByPassParent.clear() ;

  } // else if

  else { // other type is not compatible.
    mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
    token.token = "false" ;
    token.type = NIL ;
    mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    return false ;
  } // else

  // parameter 2 .

  if ( mResultCommand[check+1].type == INT || mResultCommand[check+1].type == FLOAT
       || mResultCommand[check+1].type == NIL || mResultCommand[check+1].type == T ) {
    token2.token = mResultCommand[check+1].token ;
    token2.type = mResultCommand[check+1].type ;
  } // if

  else if ( mResultCommand[check+1].type == SYMBOL ) { // check is this symbol defined .
    if ( IsDefined( mResultCommand[check+1].token ) ) { // if so, restore it's binding, and recursive .
      comefrom2 = mDefined[mDefinedIndex].comefrom ;

      for ( int k = 0 ; k < mDefined[mDefinedIndex].binding.size() ; k++ ) {
        token2.token = token2.token + mDefined[mDefinedIndex].binding[k].token ;
      } // for

      token2.type = mDefined[mDefinedIndex].binding[0].type ;
    } // if

    else {

      // cout << "IsEqv() Error, not defined symbol !" << endl ;
      return false ;
    } // else
  } // else if

  else if ( mResultCommand[check+1].type == QUOTE ) {

    ByPass( check + 1 ) ;
    limit = FindLimit() ;
    if ( mByPassParent.size() > 1 ) { // can just compare atom .
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "false" ;
      token.type = NIL ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
      return false ;
    } // if

    else if ( mByPassParent[0].type == STRING ) {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "false" ;
      token.type = NIL ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
      return false ;
    } // else if

    token2.token = mByPassParent[0].token ;
    token2.type = mByPassParent[0].type ;
    mByPassParent.clear() ;

  } // else if

  else { // other type is not compatible.
    mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
    token.token = "false" ;
    token.type = NIL ;
    mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    return false ;
  } // else


  // compare :

  if ( comefrom1 != "-1" && comefrom2 != "-1" ) {
    if ( comefrom1 == comefrom2 ) {
      if ( token.token == token2.token ) {
        mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
        token.token = "true" ;
        token.type = T ;
        mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
        return true ;
      } // if

      else {
        mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
        token.token = "false" ;
        token.type = NIL ;
        mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
        return false ;
      } // else
    } // if

    else {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "false" ;
      token.type = NIL ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
      return false ;
    } // else
  } // if

  if ( token.type == token2.type && token.token == token2.token ) {
    mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
    token.token = "true" ;
    token.type = T ;
    mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    return true ;
  } // if

  else {
    mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
    token.token = "false" ;
    token.type = NIL ;
    mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    return false ;
  } // else
} // Morefunction::IsEqv()

bool Morefunction :: IsEqual() {
  int check = mindex + 2 ;
  Tokeninfo token ; // first para .
  Tokeninfo token2 ; // second para .
  int checkpara = 0 ; // check where is second para is .
  token2.token = "" ;
  string temp1 = "" ;
  string temp2 = "" ;
  int limit = 0 ;

  limit = FindLimit() ;

  // parameter 1 :
  if ( mResultCommand[check].type == INT || mResultCommand[check].type == FLOAT
       || mResultCommand[check].type == NIL || mResultCommand[check].type == T
       || mResultCommand[check].type == STRING ) {
    temp1 = temp1 + mResultCommand[check].token ;
  } // if

  else if ( mResultCommand[check].type == SYMBOL ) { // check is this symbol defined .

    if ( IsDefined( mResultCommand[check].token ) ) { // if so, restore it's binding, and recursive .
      mIsSymbol = true ;
      for ( int k = 0 ; k < mDefined[mDefinedIndex].binding.size() ; k++ ) {
        temp1 = temp1 + mDefined[mDefinedIndex].binding[k].token ;
      } // for
    } // if

    else {
      // cout << "IsEqv() Error, not defined symbol !" << endl ;
      return false ;
    } // else
  } // else if

  else if ( mResultCommand[check].type == QUOTE ) {
    check = ByPass( check ) ;
    limit = FindLimit() ;

    for ( int i = 0 ; i < mByPassParent.size() ; i ++ ) {
      temp1 = temp1 + mByPassParent[i].token ;
    } // for

    mByPassParent.clear() ;
    check -- ;
  } // else if

  else if ( mResultCommand[check].type == LEFT ) {
    int countLeft = 1 ;
    int tempindex = check + 1 ;
    temp1 = temp1 + mResultCommand[check].token ;
    while ( countLeft != 0 ) {
      if ( mResultCommand[tempindex].type == LEFT ) {
        countLeft ++ ;
      } // if

      else if ( mResultCommand[tempindex].type == RIGHT ) {
        countLeft -- ;
      } // else if

      temp1 = temp1 + mResultCommand[tempindex].token ;
      tempindex ++ ;
    } // while

    check = tempindex - 1 ;
  } // else if

  // parameter 2 :

  if ( mResultCommand[check + 1].type == INT || mResultCommand[check + 1].type == FLOAT
       || mResultCommand[check + 1].type == NIL || mResultCommand[check + 1].type == T
       || mResultCommand[check + 1].type == STRING ) {
    temp2 = temp2 + mResultCommand[check + 1].token ;
  } // if

  else if ( mResultCommand[check + 1].type == SYMBOL ) { // check is this symbol defined .

    if ( IsDefined( mResultCommand[check + 1].token ) ) { // if so, restore it's binding, and recursive .
      mIsSymbol = true ;
      for ( int k = 0 ; k < mDefined[mDefinedIndex].binding.size() ; k++ ) {
        temp2 = temp2 + mDefined[mDefinedIndex].binding[k].token ;
      } // for
    } // if

    else {
      // cout << "IsEqv() Error, not defined symbol !" << endl ;
      return false ;
    } // else
  } // else if

  else if ( mResultCommand[check + 1].type == QUOTE ) {
    check = ByPass( check + 1 ) ;
    limit = FindLimit() ;

    for ( int i = 0 ; i < mByPassParent.size() ; i ++ ) {
      temp2 = temp2 + mByPassParent[i].token ;
    } // for

    mByPassParent.clear() ;
    check -- ;
  } // else if

  else if ( mResultCommand[check + 1].type == LEFT ) {
    int countLeft2 = 1 ;
    int tempindex2 = check + 1 ;
    temp1 = temp1 + mResultCommand[check + 1].token ;

    while ( countLeft2 != 0 ) {
      if ( mResultCommand[tempindex2].type == LEFT ) {
        countLeft2 ++ ;
      } // if

      else if ( mResultCommand[tempindex2].type == RIGHT ) {
        countLeft2 -- ;
      } // else if

      temp2 = temp2 + mResultCommand[tempindex2].token ;
      tempindex2 ++ ;
    } // while

    check = tempindex2 - 1 ;
  } // else if

  // compare :

  if ( temp1 == temp2 ) {
    mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
    token.token = "true" ;
    token.type = T ;
    mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    return true ;
  } // if

  else {
    mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
    token.token = "false" ;
    token.type = NIL ;
    mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    return true ;
  } // else
} // Morefunction::IsEqual()
  
bool Morefunction :: IsAtom() {
  int check = mindex + 2 ;
  Tokeninfo token ;
  int limit = 0 ;

  limit = FindLimit() ;

  if ( mResultCommand[check].type == INT || mResultCommand[check].type == FLOAT 
       || mResultCommand[check].type == STRING || mResultCommand[check].type == NIL
       || mResultCommand[check].type == T ) {
    mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
    token.token = "true" ;
    token.type = T ;
    mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;

    return true ;
  } // if

  else if ( mResultCommand[check].type == SYMBOL ) { // check is this symbol defined .
    if ( IsDefined( mResultCommand[check].token ) ) { // if so, restore it's binding, and recursive .
      mIsSymbol = true ;
      mResultCommand.erase( mResultCommand.begin() + check ) ;

      int k = 0 ;
      for ( k = mDefined[mDefinedIndex].binding.size() - 1 ; k >= 0 ; k -- ) {
        mResultCommand.insert( mResultCommand.begin() + check, mDefined[mDefinedIndex].binding[k] ) ;
      } // for

      IsAtom() ;
      return true ;
    } // if

    else if ( IsInternal( mResultCommand[check].token ) ) {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "true" ;
      token.type = T ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
      return true ;
    } // else if

    else {
      return false ;
    } // else
  } // else if

  else if ( mResultCommand[check].type == QUOTE ) {
    ByPass( check ) ;
    limit = FindLimit() ;
    if ( mByPassParent[0].type == INT || mByPassParent[0].type == FLOAT
         || mByPassParent[0].type == NIL || mByPassParent[0].type == T
         || mByPassParent[0].type == STRING || mByPassParent[0].type == INITIAL
         || mByPassParent[0].type == SYMBOL ) {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "true" ;
      token.type = T ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
      return true ;
    } // if

    else {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "false" ;
      token.type = NIL ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
      return false ;
    } // else
  } // else if

  else {
    mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
    token.token = "false" ;
    token.type = NIL ;
    mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    return false ;
  } // else
} // Morefunction::IsAtom()

bool Morefunction :: IsList() {
  int check = mindex + 2 ;
  Tokeninfo token ;
  int limit = 0 ;

  limit = FindLimit() ;

  if ( mResultCommand[check].type == SYMBOL ) { // check is this symbol defined .
    if ( IsDefined( mResultCommand[check].token ) ) { // if so, restore it's binding, and recursive .
      mIsSymbol = true ;
      mResultCommand.erase( mResultCommand.begin() + check ) ;

      int k = 0 ;
      for ( k = mDefined[mDefinedIndex].binding.size() - 1 ; k >= 0 ; k -- ) {
        mResultCommand.insert( mResultCommand.begin() + check, mDefined[mDefinedIndex].binding[k] ) ;
      } // for

      IsList() ;
      return true ;
    } // if

    else {
      // cout << "IsList() Error, not defined symbol !" << endl ;
      return false ;
    } // else
  } // if

  else if ( mResultCommand[check].type == LEFT ) { // means it must come from ByPass,
    // check this pair LEFT RIGHT is a list or not .
    int countLeft = 1 ;
    int j = check + 1 ; // it't a token behind LEFT .

    while ( countLeft != 0 ) {
      if ( mResultCommand[j].type == LEFT ) {
        countLeft ++ ;
      } // if

      else if ( mResultCommand[j].type == RIGHT ) {
        countLeft -- ;
      } // else if

      else if ( mResultCommand[j].type == DOT ) { // it's a list !
        mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
        token.token = "true" ;
        token.type = T ;
        mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;

        return true ;
      } // else if

      j ++ ;
    } // while

    // if while is done, means it isn't a list !
    mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
    token.token = "false" ;
    token.type = NIL ;
    mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    return false ;
  } // else if

  else if ( mResultCommand[check].type == QUOTE ) {
    if ( mHasDot ) { // original command has a DOT .
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "true" ;
      token.type = T ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
      return true ;
    } // if

    ByPass( check ) ;
    limit = FindLimit() ;
    for ( int i = 0 ; i < mByPassParent.size() ; i ++ ) {
      if ( mByPassParent[i].type == DOT ) {
        mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
        token.token = "true" ;
        token.type = T ;
        mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
        return true ;
      } // if
    } // for

    // if for is done, means it isn't a list !
    mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
    token.token = "false" ;
    token.type = NIL ;
    mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    return false ;

  } // else if

  else {
    mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
    token.token = "false" ;
    token.type = NIL ;
    mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    return false ;
  } // else
} // Morefunction::IsList()

void Morefunction :: Math() {
  int check = mindex + 2 ;
  Tokeninfo token ;
  int limit = 0 ; // should be the last RIGHT that current function can modify .
  int temp = mindex + 2 ;
  int parameter = 0 ;
  double result = 0 ; // ans of float .
  string ans = "" ; // ans of string .
  int numberindex = 0 ;
  bool isfloat = false ; // ans is int or float .
  bool isZero = false ; // check for divide Zero .
  bool isTrue = true ; // to deal with > >= < <= = .
  stringstream ss ;
  double p2 = 0.0 ;
  vector<double> number ;
  limit = FindLimit() ;

  while ( temp < limit ) {
    if ( mResultCommand[temp].type == INT || mResultCommand[temp].type == FLOAT ) {
      if ( mResultCommand[temp].type == FLOAT ) {
        isfloat = true ;
      } // if

      p2 = atof( mResultCommand[temp].token.c_str() ) ;
      if ( p2 == 0 ) {
        isZero = true ;
      } // if

      number.push_back( p2 ) ;
      parameter ++ ;
    } // if

    else if ( mResultCommand[temp].type == SYMBOL ) {
      if ( IsDefined( mResultCommand[temp].token ) ) {
        if ( mDefined[mDefinedIndex].binding[0].type == INT 
             || mDefined[mDefinedIndex].binding[0].type == FLOAT ) { // must a INT or a FLOAT !

          if ( mDefined[mDefinedIndex].binding[0].type == FLOAT ) {
            isfloat = true ;
          } // if

          p2 = atof( mDefined[mDefinedIndex].binding[0].token.c_str() ) ;
          if ( p2 == 0 ) {
            isZero = true ;
          } // if

          number.push_back( p2 ) ;
          parameter ++ ;
        } // if

        else {
          // cout << "Math() ERROR, wrong type parameter !" << endl ;
          return ;
        } // else

      } // if

      else {
        // cout << "Math() ERROR, not defined symbol !" << endl ;
        return ;
      } // else
    } // else if

    else {
      // cout << "Math() ERROR, wrong type parameter !" << endl ;
      return ;
    } // else

    temp ++ ;
  } // while

  if ( parameter < 2 ) {
    // cout << "Math() ERROR, wrong number of parameter !" << endl ;
    number.clear() ;
    return ;
  } // if

  if ( mResultCommand[mindex + 1].token == "+" ) {
    while ( numberindex < number.size() ) {
      result = result + number[numberindex] ;
      numberindex ++ ;
    } // while

    numberindex = 0 ;
    ss << result ;
    ss >> ans ;

    if ( isfloat ) {

      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = ans ;
      token.type = FLOAT ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    } // if

    else {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = ans ;
      token.type = INT ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    } // else 
  } // if 

  else if ( mResultCommand[mindex + 1].token == "-" ) {
    result = number[0] ;
    numberindex = 1 ;
    while ( numberindex < number.size() ) {
      result = result - number[numberindex] ;
      numberindex ++ ;
    } // while

    numberindex = 0 ;
    ss << result ;
    ss >> ans ;

    if ( isfloat ) {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = ans ;
      token.type = FLOAT ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    } // if

    else {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = ans ;
      token.type = INT ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    } // else   
  } // else if
 
  else if ( mResultCommand[mindex + 1].token == "*" ) {
    result = number[0] ;
    numberindex = 1 ;
    while ( numberindex < number.size() ) {
      result = result * number[numberindex] ;
      numberindex ++ ;
    } // while

    numberindex = 0 ;
    ss << result ;
    ss >> ans ;

    if ( isfloat ) {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = ans ;
      token.type = FLOAT ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    } // if

    else {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = ans ;
      token.type = INT ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    } // else   
  } // else if

  else if ( mResultCommand[mindex + 1].token == "/" ) {

    if ( isZero ) {
      // cout << "Math() ERROR, divide by 0 !" << endl ;
      // return ;
    } // if

    result = number[0] ;
    numberindex = 1 ;
    while ( numberindex < number.size() ) {
      result = result / number[numberindex] ;
      numberindex ++ ;
    } // while

    numberindex = 0 ;
    ss << result ;
    ss >> ans ;

    if ( isfloat ) {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = ans ;
      token.type = FLOAT ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    } // if
    
    else {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = ans ;
      token.type = INT ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    } // else
  } // else if

  else if ( mResultCommand[mindex + 1].token == ">" ) { // boolean
    result = number[0] ;
    numberindex = 1 ;
    isTrue = true ; // if there is one "<=", then false .
    while ( numberindex < number.size() ) {
      if ( result <= number[numberindex] ) { // "<=" means is not ">" !
        isTrue = false ;
      } // if

      numberindex ++ ;
    } // while

    if ( isTrue ) {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "true" ;
      token.type = T ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    } // if

    else {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "false" ;
      token.type = NIL ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    } // else   
  } // else if

  else if ( mResultCommand[mindex + 1].token == ">=" ) { // boolean
    result = number[0] ;
    numberindex = 1 ;
    isTrue = true ; // if there is one "<", then false .
    while ( numberindex < number.size() ) {
      if ( result < number[numberindex] ) { // "<" means is not ">=" !
        isTrue = false ;
      } // if

      numberindex ++ ;
    } // while

    if ( isTrue ) {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "true" ;
      token.type = T ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    } // if

    else {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "false" ;
      token.type = NIL ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    } // else   
  } // else if

  else if ( mResultCommand[mindex + 1].token == "<" ) { // boolean
    result = number[0] ;
    numberindex = 1 ;
    isTrue = true ; // if there is one ">=", then false .
    while ( numberindex < number.size() ) {
      if ( result >= number[numberindex] ) { // ">=" means is not "<" !
        isTrue = false ;
      } // if

      numberindex ++ ;
    } // while

    if ( isTrue ) {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "true" ;
      token.type = T ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    } // if

    else {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "false" ;
      token.type = NIL ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    } // else   
  } // else if

  else if ( mResultCommand[mindex + 1].token == "<=" ) { // boolean
    result = number[0] ;
    numberindex = 1 ;
    isTrue = true ; // if there is one ">", then false .
    while ( numberindex < number.size() ) {
      if ( result > number[numberindex] ) { // ">" means is not "<=" !
        isTrue = false ;
      } // if

      numberindex ++ ;
    } // while

    if ( isTrue ) {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "true" ;
      token.type = T ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    } // if

    else {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "false" ;
      token.type = NIL ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    } // else    
  } // else if

  else if ( mResultCommand[mindex + 1].token == "=" ) { // boolean
    result = number[0] ;
    numberindex = 1 ;
    isTrue = true ; // if there is one "!=", then false .
    while ( numberindex < number.size() ) {
      if ( result != number[numberindex] ) { // "!=" means is not "=" !
        isTrue = false ;
      } // if

      numberindex ++ ;
    } // while

    if ( isTrue ) {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "true" ;
      token.type = T ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    } // if

    else {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "false" ;
      token.type = NIL ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    } // else    
  } // else if
} // Morefunction::Math()

void Morefunction :: And() { 
  int check = mindex + 2 ;
  int limit = 0 ;
  int countLeft = 1 ;
  int tempindex = check + 1 ;
  Tokeninfo token ;
  Parameter para ;
  vector<Parameter> tempPara ;

  limit = FindLimit() ;
  int t = 0 ;

  while ( check < limit ) {
    if ( mResultCommand[check].type == LEFT ) { // NOT NIL !
      token.token = mResultCommand[check].token ;
      token.type = mResultCommand[check].type ;
      para.para.push_back( token ) ;
      countLeft = 1 ;
      tempindex = check + 1 ;

      while ( countLeft != 0 ) {
        if ( mResultCommand[tempindex].type == LEFT ) {
          countLeft ++ ;
        } // if

        else if ( mResultCommand[tempindex].type == RIGHT ) {
          countLeft -- ;
        } // else if

        token.token = mResultCommand[tempindex].token ;
        token.type = mResultCommand[tempindex].type ;
        para.para.push_back( token ) ; 

        tempindex ++ ;
      } // while

      check = tempindex - 1 ;
      tempPara.push_back( para ) ;
      para.para.clear() ;
    } // if

    else if ( mResultCommand[check].type == SYMBOL ) { // Check is NIL or NOT .
      IsDefined( mResultCommand[check].token ) ;
      if ( mDefined[mDefinedIndex].binding[0].type == NIL ) { 
        // this symbol's binding is NIL, return this symbol's binding .
        token.token =  mDefined[mDefinedIndex].binding[0].token ;
        token.type = NIL ;
        mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
        mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
        return ;
      } // if

      else { // record the data, use to check is last parameter ?
        for ( int i = 0 ; i < mDefined[mDefinedIndex].binding.size() ; i ++ ) {
          token.token = mDefined[mDefinedIndex].binding[i].token ;
          token.type = mDefined[mDefinedIndex].binding[i].type ;
          para.para.push_back( token ) ;
        } // for

        tempPara.push_back( para ) ;
        para.para.clear() ;
      } // else
    } // else if

    else if ( mResultCommand[check].type == QUOTE ) { // Check is NIL or NOT
      check = ByPass( check ) ;
      check -- ;
      limit = FindLimit() ;
      if ( mByPassParent[0].type == NIL ) {
        token.token = mByPassParent[0].token ;
        token.type = NIL ;
        mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
        mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
        return ;
      } // ifr

      else {
        for ( int i = 0 ; i < mByPassParent.size() ; i ++ ) {
          token.token = mByPassParent[i].token ;
          token.type = mByPassParent[i].type ;
          para.para.push_back( token ) ;
        } // for

        tempPara.push_back( para ) ;
        para.para.clear() ;
      } // else
    } // else if

    else if ( mResultCommand[check].type == NIL ) { // if is a nil, return it .
      token.token = mResultCommand[check].token ;
      token.type = NIL ;
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
      return ;
    } // else if

    else {
      token.token = mResultCommand[check].token ;
      token.type = mResultCommand[check].type ;
      para.para.push_back( token ) ;
      tempPara.push_back( para ) ;
      para.para.clear() ;
    } // else

    check ++ ;
  } // while

  // if while is done, return tempPara's last element .
  mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
  for ( int i = tempPara[tempPara.size() - 1].para.size() - 1 ; i >= 0 ; i -- ) {
    mResultCommand.insert( mResultCommand.begin() + mindex, tempPara[tempPara.size() - 1].para[i] ) ;
  } // for

} // Morefunction::And()

void Morefunction :: Or() { 

  int check = mindex + 2 ;
  int limit = 0 ;
  int countLeft = 1 ;
  int tempindex = check + 1 ;
  Tokeninfo token ;
  Parameter para ;
  vector<Parameter> tempPara ;

  limit = FindLimit() ;
  int t = 0 ;

  while ( check < limit ) {
    if ( mResultCommand[check].type == LEFT ) { // must not a NIL, return it !
      token.token = mResultCommand[check].token ;
      token.type = mResultCommand[check].type ;
      para.para.push_back( token ) ;
      countLeft = 1 ;
      tempindex = check + 1 ;

      while ( countLeft != 0 ) {
        if ( mResultCommand[tempindex].type == LEFT ) {
          countLeft ++ ;
        } // if

        else if ( mResultCommand[tempindex].type == RIGHT ) {
          countLeft -- ;
        } // else if

        token.token = mResultCommand[tempindex].token ;
        token.type = mResultCommand[tempindex].type ;
        para.para.push_back( token ) ; 

        tempindex ++ ;
      } // while
      
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      for ( int i = para.para.size() - 1 ; i >= 0 ; i -- ) {
        mResultCommand.insert( mResultCommand.begin() + mindex, para.para[i] ) ;
      } // for

      return ;
    } // if

    else if ( mResultCommand[check].type == SYMBOL ) { // Check is NIL or NOT .
      IsDefined( mResultCommand[check].token ) ;
      if ( mDefined[mDefinedIndex].binding[0].type != NIL ) { // Not a NIL, return it !
        mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
        for ( int i = mDefined[mDefinedIndex].binding.size() - 1 ; i >= 0 ; i -- ) {
          mResultCommand.insert( mResultCommand.begin() + mindex, mDefined[mDefinedIndex].binding[i] ) ;
        } // for

        return ;
      } // if

      else { // record the data, use to check is last parameter ?
        token.token = mResultCommand[check].token ;
        token.type = mResultCommand[check].type ;
        para.para.push_back( token ) ;
        tempPara.push_back( para ) ;
        para.para.clear() ;
      } // else
    } // else if

    else if ( mResultCommand[check].type == QUOTE ) { // Check is NIL or NOT
      check = ByPass( check ) ;
      check -- ;
      limit = FindLimit() ;
      if ( mByPassParent[0].type != NIL ) {
        mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;

        for ( int i = mByPassParent.size() - 1 ; i >= 0 ; i -- ) {
          mResultCommand.insert( mResultCommand.begin() + mindex, mByPassParent[i] ) ;
        } // for

        return ;
      } // ifr

      else {
        for ( int i = 0 ; i < mByPassParent.size() ; i ++ ) {
          token.token = mByPassParent[i].token ;
          token.type = mByPassParent[i].type ;
          para.para.push_back( token ) ;
        } // for

        tempPara.push_back( para ) ;
        para.para.clear() ;
      } // else
    } // else if

    else if ( mResultCommand[check].type != NIL ) { 
      token.token = mResultCommand[check].token ;
      token.type = mResultCommand[check].type ;
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
      return ;
    } // else if

    else {
      token.token = mResultCommand[check].token ;
      token.type = mResultCommand[check].type ;
      para.para.push_back( token ) ;
      tempPara.push_back( para ) ;
      para.para.clear() ;
    } // else

    check ++ ;
  } // while

  // if while is done, return last para .
  mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
  for ( int i = tempPara[tempPara.size() - 1].para.size() - 1 ; i >= 0 ; i -- ) {
    mResultCommand.insert( mResultCommand.begin() + mindex, tempPara[tempPara.size() - 1].para[i] ) ;
  } // for

} // Morefunction::Or()

void Morefunction :: Not() { // if parameter is a "nil", return True .
  int check = mindex + 2 ;
  Tokeninfo token ;
  int limit = 0 ;

  limit = FindLimit() ;

  if ( mResultCommand[check].type == NIL ) { // if is nil, return true .
    mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
    token.token = "true" ;
    token.type = T ;
    mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;

  } // if

  else if ( mResultCommand[check].type == SYMBOL ) { 

    if ( IsDefined( mResultCommand[check].token ) ) {
      if ( mDefined[mDefinedIndex].binding[0].type == NIL ) {
        mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
        token.token = "true" ;
        token.type = T ;
        mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
      } // if

      else {
        mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
        token.token = "false" ;
        token.type = NIL ;
        mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
      } // else

    } // if

  } // else if

  else if ( mResultCommand[check].type == QUOTE ) {
    ByPass( check ) ;
    limit = FindLimit() ;
    if ( mByPassParent[0].type == NIL ) {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "true" ;
      token.type = T ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
      return ;
    } // if

    else {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "false" ;
      token.type = NIL ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
      return ;
    } // else
  } // else if
  
  else {
    mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
    token.token = "false" ;
    token.type = NIL ;
    mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    return ;
  } // else
} // Morefunction::Not()

void Morefunction :: StringOP() {
  int check = mindex + 2 ;
  Tokeninfo token ;
  int limit = 0 ; // should be the last RIGHT that current function can modify .
  int temp = mindex + 2 ;
  int parameter = 0 ;
  string result = "" ;
  string ans = "" ;
  int numberindex = 0 ;
  bool isTrue = true ; // to deal with > >= < <= = .
  vector<string> stringlist ;

  limit = FindLimit() ;

  while ( temp < limit ) {
    if ( mResultCommand[temp].type == STRING ) {
      stringlist.push_back( mResultCommand[temp].token ) ;
      parameter ++ ;
    } // if

    else if ( mResultCommand[temp].type == SYMBOL ) {
      if ( IsDefined( mResultCommand[temp].token ) ) {
        if ( mDefined[mDefinedIndex].binding[0].type == STRING ) { // must be a string !

          stringlist.push_back( mDefined[mDefinedIndex].binding[0].token ) ;
          parameter ++ ;
        } // if

        else {
          // cout << "StringOP() ERROR, wrong type parameter !" << endl ;
          return ;
        } // else

      } // if

      else {
        // cout << "StringOP() ERROR, not defined symbol !" << endl ;
        return ;
      } // else
    } // else if

    else if ( mResultCommand[temp].type == QUOTE ) {
      temp = ByPass( temp ) ;
      limit = FindLimit() ;
      if ( mByPassParent[0].type == STRING ) {
        stringlist.push_back( mByPassParent[0].token ) ;
        parameter ++ ;
      } // if

      temp -- ;
    } // else if

    else {
      // cout << "StringOP() ERROR, wrong type parameter !" << endl ;
      return ;
    } // else

    temp ++ ;
  } // while

  if ( parameter < 2 ) {
    // cout << "StringOP() ERROR, wrong number of parameter !" << endl ;
    stringlist.clear() ;
    return ;
  } // if

  if ( mResultCommand[mindex + 1].token == "string-append" ) {
    // delete first string's last cahr: ", delete second string's frist char: ", then append .
    result = stringlist[0] ;
    result.erase( result.size() - 1 ) ; // delete first string's : " .
    numberindex = 1 ;
    while ( numberindex < stringlist.size() ) { 
      stringlist[numberindex].erase( stringlist[numberindex].begin() ) ;
      stringlist[numberindex].erase( stringlist[numberindex].size() - 1 ) ;
      result = result + stringlist[numberindex] ;
      
      numberindex ++ ;
    } // while

    result.insert( result.size(), "\"" ) ;

    mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
    token.token = result ;
    token.type = STRING ;
    mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
  } // if 

  else if ( mResultCommand[mindex + 1].token == "string>?" ) {
    result = stringlist[0] ;
    numberindex = 1 ;
    isTrue = true ; // if there is one "<=", then false .
    while ( numberindex < stringlist.size() ) {
      if ( result <= stringlist[numberindex] ) { // "<=" means is not ">" !
        isTrue = false ;
      } // if

      result = stringlist[numberindex] ;
      numberindex ++ ;
    } // while

    if ( isTrue ) {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "true" ;
      token.type = T ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    } // if

    else {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "false" ;
      token.type = NIL ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    } // else   
  } // else if

  else if ( mResultCommand[mindex + 1].token == "string<?" ) {
    result = stringlist[0] ;
    numberindex = 1 ;
    isTrue = true ; // if there is one ">=", then false .
    while ( numberindex < stringlist.size() ) {
      if ( result >= stringlist[numberindex] ) { // ">=" means is not "<" !
        isTrue = false ;
      } // if

      result = stringlist[numberindex] ;
      numberindex ++ ;
    } // while

    if ( isTrue ) {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "true" ;
      token.type = T ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    } // if

    else {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "false" ;
      token.type = NIL ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    } // else    
  } // else if

  else if ( mResultCommand[mindex + 1].token == "string=?" ) {
    result = stringlist[0] ;
    numberindex = 1 ;
    isTrue = true ; // if there is one "!=", then false .
    while ( numberindex < stringlist.size() ) {
      if ( result != stringlist[numberindex] ) { // "!=" means is not "==" !
        isTrue = false ;
      } // if

      result = stringlist[numberindex] ;
      numberindex ++ ;
    } // while

    if ( isTrue ) {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "true" ;
      token.type = T ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    } // if

    else {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
      token.token = "false" ;
      token.type = NIL ;
      mResultCommand.insert( mResultCommand.begin() + mindex, token ) ;
    } // else   
  } // else if
  

} // Morefunction::StringOP()

void Morefunction :: If() {
  int check = mindex + 2 ;
  Tokeninfo token ;
  int limit = 0 ;
  int paranum =  CountParent( check ) ;
  if ( paranum != 2 && paranum != 3 ) {
    // cout << "If() Error, wrong parameter: " << paranum << endl ;
    return ;
  } // if

  if ( mResultCommand[check].type == SYMBOL ) {
    if ( IsDefined( mResultCommand[check].token ) ) {
      mResultCommand.erase( mResultCommand.begin() + check ) ;
      for ( int i = mDefined[mDefinedIndex].binding.size() - 1 ; i >= 0 ; i -- ) {
        mResultCommand.insert( mResultCommand.begin() + check, mDefined[mDefinedIndex].binding[i] ) ;
      } // for
    } // if 
  } // if

  limit = FindLimit() ;

  if ( mResultCommand[check].type != NIL ) { // not a nil then is true .
    mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;
    for ( int i = mParaData[1].para.size() - 1 ; i >= 0 ; i -- ) {
      mResultCommand.insert( mResultCommand.begin() + mindex, mParaData[1].para[i] ) ;
    } // for
  } // if

  else {
    if ( mParaData.size() < 3 ) {
      // cout << " If() ERROR, paranumber wrong, need 3:  " << mParaData.size() ; 
      return ;
    } // if

    else {
      mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;

      for ( int i = mParaData[2].para.size() - 1 ; i >= 0 ; i -- ) {
        mResultCommand.insert( mResultCommand.begin() + mindex, mParaData[2].para[i] ) ;
      } // for
    } // else
  } // else
} // Morefunction::If()

void Morefunction :: Cond() {
  int check = mindex + 2 ; // should be a LEFT .
  int limit = FindLimit() ;
  int countLeft = 1 ;
  int right = 0 ;
  int findlastpara = 0 ; // find where is the last parameter .
  int index = mindex + 2 ; // should be a LEFT .
  int k = index ;
  Tokeninfo token ;
  Parameter para ;

  while ( index < limit ) {
    if ( mResultCommand[index].type == LEFT ) { // check behind is a "non nil", "nil" or "else" .
      index ++ ;
      findlastpara = index ;

      if ( mResultCommand[index].token == "else" ) { // check if a last?
        right = FindLimit2( index ) ;
        // if the token behind "( else XXX )" is a RIGHT, means this "else" is a key word !
        if ( mResultCommand[right + 1].type == RIGHT ) {

          CountParent( findlastpara ) ;
          limit = FindLimit() ;
          mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit - 1 ) ) ;
          for ( int i = mParaData[mParaData.size() - 1].para.size() - 1 ; i >= 0 ; i -- ) {
            mResultCommand.insert( mResultCommand.begin() + mindex, 
                                   mParaData[mParaData.size() - 1].para[i] ) ;
          } // for

          return ;
        } // if

        else { // is a normal symbol .
          // index -- ;
        } // else
      } // if

      if ( mResultCommand[index].type == SYMBOL ) {
        if ( IsDefined( mResultCommand[index].token ) ) {
          if ( mDefined[mDefinedIndex].binding[0].type != NIL ) {
            CountParent( findlastpara ) ;
            limit = FindLimit() ;
            mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit - 1 ) ) ;
            for ( int i = mParaData[mParaData.size() - 1].para.size() - 1 ; i >= 0 ; i -- ) {
              mResultCommand.insert( mResultCommand.begin() + mindex, 
                                     mParaData[mParaData.size() - 1].para[i] ) ;
            } // for

            return ;
          } // if

          else {
            index = FindLimit2( index ) ; // skip this pair of () .
          } // else
        } // if
      } // if

      else if ( mResultCommand[index].type != NIL ) { // every thing is non nil means True!

        CountParent( findlastpara ) ;
        limit = FindLimit() ;

        mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit - 1 ) ) ;

        for ( int i = mParaData[mParaData.size() - 1].para.size() - 1 ; i >= 0 ; i -- ) {
          mResultCommand.insert( mResultCommand.begin() + mindex, 
                                 mParaData[mParaData.size() - 1].para[i] ) ;
        } // for

        return ;
      } // else if

      else if ( mResultCommand[index].type == NIL ) {
        index = FindLimit2( index ) ;
      } // else if
    } // if

    index ++ ;
  } // while
} // Morefunction::Cond()

void Morefunction :: Begin() {
  int check = mindex + 2 ;
  Tokeninfo token ; // first para .
  Tokeninfo token2 ; // second para .
  int checkpara = 0 ; // check where is second para is .

  int limit = 0 ;

  if ( CountParent( check ) < 1 ) {
    // cout << "Begin() Error, wrong parameter !" << endl ;
    return ;
  } // if

  limit = FindLimit() ;
  mResultCommand.erase( mResultCommand.begin() + mindex, mResultCommand.begin() + ( limit + 1 ) ) ;

  // return last parameter .
  
  if ( mParaData[mParaData.size() - 1].para[0].type == SYMBOL ) {
    if ( IsDefined( mParaData[mParaData.size() - 1].para[0].token ) ) {
      for ( int i = mDefined[mDefinedIndex].binding.size() - 1 ; i >= 0 ; i -- ) {
        mResultCommand.insert( mResultCommand.begin() + mindex, mDefined[mDefinedIndex].binding[i] ) ;
      } // for
    } // if

    else {
      for ( int i = mParaData[mParaData.size() - 1].para.size() - 1 ; i >= 0 ; i -- ) {
        mResultCommand.insert( mResultCommand.begin() + mindex, mParaData[mParaData.size() - 1].para[i] ) ;
      } // for
    } // else
  } // if

  else {
    for ( int i = mParaData[mParaData.size() - 1].para.size() - 1 ; i >= 0 ; i -- ) {
      mResultCommand.insert( mResultCommand.begin() + mindex, mParaData[mParaData.size() - 1].para[i] ) ;
    } // for
  } // else
} // Morefunction::Begin()

class Parser { // It should create a parser tree using token from Scarner .
public:
  // some grammer function ...
  Scaner mscaner ;
  Morefunction morefunction ;
  void Parsermain() ;
  bool S_exp() ;
  bool Atom() ;
  
private:
  Tokeninfo mtokeninfo ; // a struct to save token info .
  vector<Tokeninfo> mTokenlist ; // currect grammer token save in this vector .
  bool mIsCommentLine ; // check if the whole line is comment line .
  bool mIsUnexpectedRight ; // error message
  bool mIsUnexpectedLeft ; // error message
  bool mIsNotS_exp ; // error message, basically same usage as mIsUnexpectedLeft .
  void PrintNode( int index ) ;
  void InitialTokenlist() ; // if a command line is done, initial vector tokenlist .
  bool CheckWhiteSpace() ; // check are there all white space after current token,
                             // if true, skip then all .

  // maybe need to create a Parser tree ...
  Node* mroot ;
  Node* mcurrent ;
  Node* mparent ;
  
  void BuildTree( int & index, Node* parent ) ;
  void CreatRoot() ;
  void PrettyPrint( int & index, int & currentCharNum ) ;
  bool IsAtom( TokenType type ) ; // check token type is Atom or not .
  void DeleteDot() ; // DeleteDot when .( happend . mTokenlist
  void DeleteDot2() ; // DeleteDot when .( happend . mRebuildCommand
  void DeleteNil() ; // DeleteNIL when . nil happend .
  void RestoreSymbol() ; // restore symbol binding .
  vector<Tokeninfo> mRebuildCommand ;
  vector<int> mQuoteNum ; // save quote number when print .
  vector<int> mLeftNum ; // use to  deal with "Dot + LEFT" ;
  bool mprintSymbol ; // use to check how many enter should be print ;
  // Here should have a "Evaluate()", used to chose which function should call,
  // and formal should like : EVAL( in_sexp, result_sexp ) 
  void EvalSexp( int & tempindex2 ) ;
  void Eval( int tempindex ) ;
 
}; // Parser

void Parser :: Parsermain() {
  bool isEOF = false ;
  bool check = false ;
  int errortype = 0 ;
  int errorcolumn = 0 ;
  int errorline = 0 ;
  int tokenIndex = 0 ;
  
  mtokeninfo.token = "(" ;
  mtokeninfo.type = LEFT ;
  mRebuildCommand.push_back( mtokeninfo ) ;
  
  Node*tree ;
  string command ;
  mIsCommentLine = false ;
  mscaner.SetLine() ; // initial mline ;
  mscaner.GetCommand() ;
  while ( mscaner.ReturnCommand()[0] == '\n' ) { // if it's just a enter, get another command .
    // cout << "check" ;
    mscaner.GetCommand() ;
  } // while

  while ( !isEOF ) { // if isEOF, exit the Scheme .
    try {
      while ( mscaner.ReturnCommand()[0] == '\n' ) { // if it's just a enter, get another command .
        // cout << "check" ;
        mscaner.GetCommand() ;
      } // while

      while ( mscaner.GetColumn() < mscaner.ReturnCommand().length() - 1 ) { 
        // cout << mscaner.GetColumn() << "\t" << mscaner.ReturnCommand().length() << endl ;
        // this "while" is  same line ! should go through whole line 
        mIsUnexpectedRight = false ;
        mIsUnexpectedLeft = false ;
        mIsNotS_exp = false ;

        if ( S_exp() ) {
          mscaner.SetLineToOne() ;
          if ( mTokenlist.size() >= 3 ) {
            if ( mTokenlist[0].token == "(" && mTokenlist[1].token == "exit" 
                 && mTokenlist[2].token == ")" ) {
              throw "(exit)" ;
            } // if
          } // if

          if ( CheckWhiteSpace() ) { 
            // if there are all white space, skip all, no need to deal with it .
            mscaner.Skipcomment() ;
          } // if

          if ( mIsCommentLine == true ) { 
            // whole line is a comment line, count a line used to print error message .
            errorline = errorline + 1 ;
            check = true ;
            mIsCommentLine = false ;
          } // if

          else { 
            // if not whole line is comment line, and it's grammer is fine, 
            // errorline should initial . 
            mscaner.mClosingQuote = 0 ;
            errorline = 0 ;
          } // else

          errorcolumn = mscaner.GetColumn() ;
          
          // morefunction.mOriginCommand.assign( mTokenlist.begin(), mTokenlist.end() ) ;
          morefunction.mHasDot = false ; // is the origin command has dot?
          DeleteDot() ;
          CreatRoot() ;
          Node*buildtree = mroot ;
          BuildTree( tokenIndex, buildtree ) ;

          // mRebuildCommand.assign( mTokenlist.begin(), mTokenlist.end() ) ;

          if ( mRebuildCommand.size() == 3 ) {
            if ( mRebuildCommand[0].type == LEFT ) {
              if ( mRebuildCommand[1].token == "exit" ) {
                if ( mRebuildCommand[2].type == RIGHT ) {
                  throw "(exit)" ;
                } // if 
              } // if
            } // if
          } // if

          tokenIndex = 0 ;
          int temp = 0 ; // use to reset index in EavlSexp()
          

          if ( mRebuildCommand.size() > 1 ) {
            morefunction.mResultCommand.assign( mRebuildCommand.begin(), mRebuildCommand.end() ) ;
            morefunction.mCounter = 0 ;

            EvalSexp( temp ) ;
            morefunction.mHasDot = false ;
            morefunction.mCounter = 0 ;
            mRebuildCommand.clear() ;
            mRebuildCommand.assign( morefunction.mResultCommand.begin(), 
                                    morefunction.mResultCommand.end() ) ;

            DeleteNil() ;
            DeleteDot2() ;
            
            morefunction.mResultCommand.clear() ;
            morefunction.mindex = 0 ;
          } // if
          
         
          // for ( int q = 0 ; q < mRebuildCommand.size() ; q ++ ) {
            // cout << mRebuildCommand[q].token ;
          // } // for
          

          if ( ! mTokenlist.empty() ) {
            if ( mTokenlist[0].type != LEFT && mTokenlist[0].type != QUOTE  ) { 
              // just one leaf tree, must a ATOM .
              // Print ATOM .
              // cout << "HERE" ;
              mprintSymbol = false ;
              cout << "> " ;
              PrintNode( 1 ) ;

              if ( mprintSymbol ) {
                cout << endl ;
              } // if

              else {
                cout << endl << endl ;
              } // else
            } // if

            else {
              int printindex = 0 ; // index of command .
              int currentnum = 0 ; // how many LEFT .

              RestoreSymbol() ; // if this command is like (a a), restore symbols binding .

              cout << "> " ;
              PrettyPrint( printindex, currentnum ) ;
              cout << endl ; // Because it's end up with Right, it do'nt have to print two  \n .
            } // else

            mRebuildCommand.clear() ;
            mtokeninfo.token = "(" ;
            mtokeninfo.type = LEFT ;
            mRebuildCommand.push_back( mtokeninfo ) ;
            // cout << "FK" << endl ;
          } // if

          else {
            // mRebuildCommand.clear() ;
          } // else
        } // if
         
        else {
          // cout << "test : " << mIsUnexpectedLeft << mIsNotS_exp << mIsUnexpectedRight << endl ;
          if ( !mIsUnexpectedRight && ( mIsUnexpectedLeft == true || mIsNotS_exp == true ) ) {
            cout << "> ERROR (unexpected token) : atom or '(' expected when token at " ;
            cout << "Line " << mscaner.GetLine() + errorline << " Column " ;

            if ( errorcolumn < mscaner.GetColumn() ) {
              cout << mscaner.GetColumn() - errorcolumn ;
            } // if
              
            else {
              cout << mscaner.GetColumn() ;
            } // else

            cout << " is " << ">>" << mscaner.mtoken << "<<" << endl << endl ;
            // cout << "test :" << errorcolumn << endl ;
            mscaner.Skipcomment() ;
            mscaner.SetLine() ; // initial mline ;
            errorline = 0 ;
            mIsCommentLine = false ;
          } // if

          else if ( mIsUnexpectedRight == true ) {
            cout << "> ERROR (unexpected token) : ')' expected when token at " ;
            cout << "Line " << mscaner.GetLine() + errorline << " Column " ;

            if ( errorcolumn < mscaner.GetColumn() ) {
              cout << mscaner.GetColumn() - errorcolumn ;
            } // if
              
            else {
              cout << mscaner.GetColumn() ;
            } // else

            cout << " is " << ">>" << mscaner.mtoken << "<<" << endl << endl ;
            mscaner.Skipcomment() ;
            mscaner.SetLine() ; // initial mline ;
            errorline = 0 ;
            mIsCommentLine = false ;
          } // else if
        } // else

        InitialTokenlist() ;
      } // while

      if ( mscaner.mIsEOF == true ) {
        throw "(no more input)" ;
      } // if

      mIsUnexpectedRight = false ;
      mIsUnexpectedLeft = false ;
      mIsNotS_exp = false ;
      // cout << check ;
      if ( !check ) {
        errorline = 0 ;
        mscaner.mClosingQuote = 0 ;
      } // if
      

      errorcolumn = 0 ;
      
      check = false ;
      mscaner.SetLine() ; // initial mline ;
      mscaner.GetCommand() ;
    } // try 

    catch ( const char* errorMessage ) {
      errortype = strcmp( errorMessage, "(no closing quote)" ) ;
      if ( errortype == 0 ) {
        cout << "> ERROR (no closing quote) : END-OF-LINE encountered at " ;
        if ( mscaner.mClosingQuote == 0 ) mscaner.mClosingQuote = 1 ;
        cout << "Line " << mscaner.mClosingQuote << " Column " ;
        // cout << mscaner.GetColumn() - errorcolumn << endl ;
        mscaner.mClosingQuote = 0 ;
        if ( errorcolumn < mscaner.GetColumn() ) {
          cout << mscaner.GetColumn() - errorcolumn << endl ;
        } // if
              
        else {
          cout << mscaner.GetColumn() << endl ;
        } // else

        errorcolumn = 0 ;
        cout << endl ;
        // cout << mscaner.ReturnCommand().size() << "ddd"<<endl ;
        mscaner.SetLine() ; // initial mline = 0 ;
        InitialTokenlist() ;
        mscaner.GetCommand() ;
        errorline = 0 ;
        errorcolumn = 0 ;
        mIsCommentLine = false ;
      } // if

      errortype = strcmp( errorMessage, "(no more input)" ) ;
      if ( errortype == 0 ) { // eof encountter should exit Scheme .
        cout << "> ERROR (no more input) : END-OF-FILE encountered" << endl ; 
        isEOF = true ;
      } // if 

      errortype = strcmp( errorMessage, "(exit)" ) ;
      if ( errortype == 0 ) {
        isEOF = true ; 
        cout << "> " << endl ;
      } // if

    } // catch
  } // while

} // Parser::Parsermain()

void Parser :: PrintNode( int index ) {
  int i = 0 ;
  int k = 0 ;
  int temp1 ;
  double temp ;

  if ( mRebuildCommand[index].type == FLOAT ) {
    // printFLOAT()
    temp = atof( mRebuildCommand[index].token.c_str() ) ;
    printf( "%.3f", temp ) ;
  } // if

  else if ( mRebuildCommand[index].type == INT ) {
    temp1 = atoi( mRebuildCommand[index].token.c_str() ) ;
    cout << temp1 ;
  } // else if

  else if ( mRebuildCommand[index].type == T ) {
    cout << "#t" ;
  } // else if

  else if ( mRebuildCommand[index].type == NIL ) {
    cout << "nil" ;
  } // else if

  else if ( mRebuildCommand[index].type == QUOTE ) {
    cout << "quote" ;
  } // else if

  else if ( mRebuildCommand[index].type == STRING ) {
    while ( k < mRebuildCommand[index].token.length() ) {
      if ( mRebuildCommand[index].token[k] == '\\' ) { // check if after '\' is some special char
        if ( mRebuildCommand[index].token[k + 1] == 'n' ) { // '\n' should cout \n
          k = k + 1 ; // '\n' should not cout .
          cout << endl ;
        } // if

        else if ( mRebuildCommand[index].token[k + 1] == 't' ) {
          k = k + 1 ;
          cout << '\t' ;
        } // else if

        else if ( mRebuildCommand[index].token[k + 1] == '\\' ) { // means this '\' is just a '\',
                                                         // so it should be printed .
          k = k + 1 ;
          cout << '\\' ;
        } // else if

        else {
          cout << mRebuildCommand[index].token[k] ;
        } // else
      } // if

      else {
        cout << mRebuildCommand[index].token[k] ;
      } // else

      k++ ;
    } // while
  } // else if

  else { // symbol or dot .
 
    if ( mRebuildCommand[index].type == SYMBOL && mRebuildCommand.size() == 2 ) {
      if ( morefunction.IsDefined( mRebuildCommand[index].token ) ) {
    
        if ( morefunction.mDefined[morefunction.mDefinedIndex].binding.size() > 1 ) {
          mRebuildCommand.clear() ;
          mRebuildCommand.assign( morefunction.mDefined[morefunction.mDefinedIndex].binding.begin(),
                                  morefunction.mDefined[morefunction.mDefinedIndex].binding.end() ) ;
          mprintSymbol = true ;
          int printindex = 0 ; // index of command .
          int currentnum = 0 ; // how many LEFT .
          PrettyPrint( printindex, currentnum ) ;
            // cout << endl ; // Because it's end up with Right, it do'nt have to print two  \n .
        } // if


        else {
          cout << morefunction.mDefined[morefunction.mDefinedIndex].binding[0].token ;
        } // else

      } // if

      else {
        cout << mRebuildCommand[index].token ;
      } // else
    } // if

    else {
      cout << mRebuildCommand[index].token ;
    } // else

    // cout << mRebuildCommand[index].token ;

  } // else
} // Parser::PrintNode()

bool Parser :: S_exp() { // The grammer of S_exp .
  static int count = 0 ;
  mscaner.PeekToken() ; // get a tokentype .
  if ( count < mscaner.GetLine() ) {
    mIsCommentLine = false ;
  } // if
  // if ( mscaner.mtoken == ";" ) { // skip whole command line after ";", and get a new command line .
    // mscaner.Skipcomment() ;
    // mscaner.PeekToken() ; // get a tokentype .
  // } // if

  if ( Atom() ) {

    if ( mscaner.mtoken == ";" ) { // skip whole command line after ";", and get a new command line .
      mscaner.Skipcomment() ;
      // if ( mTokenlist.empty() ) { // whole line is comment line .
        // mIsCommentLine = true ;
      // } // if 

      int p = 0 ;
      int k = 0 ;
      while ( p < mscaner.ReturnCommand().length() && mscaner.ReturnCommand()[p] != ';' ) {
        if ( mscaner.ReturnCommand()[p] != ' ' && mscaner.ReturnCommand()[p] != '\t' ) {
          k = 1 ;
        } // if

        p ++ ;
      } // while

      if ( k == 0 ) {
        mIsCommentLine = true ;
        count = mscaner.GetLine() ;
      } // if mIsCommentLine = true ;
      // mscaner.PeekToken() ; // get a tokentype .
    } // if

    else {
      mtokeninfo.type = mscaner.mtokentype ; // currect grammer, save in vector .
      mtokeninfo.token = mscaner.mtoken ;
      mTokenlist.push_back( mtokeninfo ) ;
    } // else

    return true ;
  } // if

  else if ( mscaner.mtokentype == LEFT ) {
    mtokeninfo.type = mscaner.mtokentype ; // currect grammer, save in vector .
    mtokeninfo.token = mscaner.mtoken ;
    mTokenlist.push_back( mtokeninfo ) ;

    if ( S_exp() ) {

      while ( S_exp() ) {

      } // while

      if ( mIsUnexpectedLeft || mIsUnexpectedRight ) {
        return false ;
      } // if

      if ( mscaner.mtokentype == DOT ) { // if not, it's ok .
        mtokeninfo.type = mscaner.mtokentype ; // currect grammer, save in vector .
        mtokeninfo.token = mscaner.mtoken ;
        mTokenlist.push_back( mtokeninfo ) ;

        if ( S_exp() ) {
          int k = 0 ;
          while ( mscaner.mtoken == ";" ) { // get rid of comment line .
            if ( S_exp() ) {
              mscaner.PeekToken() ; // get a tokentype .
              k = 1 ;
            } // if

            else {
              // mIsUnexpectedLeft = true ;
              return false ;
            } // else
          } // while

          if ( k == 0 ) {
            mscaner.PeekToken() ; // get a tokentype .
          } // if  

          else {
            k = 0 ;
          } // else
        } // if

        else {
          // mIsUnexpectedLeft = true ;
          return false ;
        } // else
      } // if

      if ( mscaner.mtoken == ";" ) { // skip whole command line after ";", and get a new command line .
        mscaner.Skipcomment() ;
        if ( mTokenlist.empty() ) { // whole line is comment line .
          mIsCommentLine = true ;
        } // if 

        mscaner.PeekToken() ;
      } // if

      if ( mscaner.mtokentype == RIGHT ) {
        mtokeninfo.type = mscaner.mtokentype ; // currect grammer, save in vector .
        mtokeninfo.token = mscaner.mtoken ;
        mTokenlist.push_back( mtokeninfo ) ;

        return true ;
      } // if

      else {
        mIsUnexpectedRight = true ;
        return false ;
      } // else

    } // if

    else {
      mIsUnexpectedLeft = true ;
      return false ;
    } // else
  } // else if

  else if ( mscaner.mtokentype == QUOTE ) {
    mtokeninfo.type = mscaner.mtokentype ; // currect grammer, save in vector .
    mtokeninfo.token = mscaner.mtoken ;
    mTokenlist.push_back( mtokeninfo ) ;

    if ( S_exp() ) {
      while ( mscaner.mtoken == ";" ) {
        if ( S_exp() ) {
        
        } // if

        else {
          return false ;
        } // else
      } // while

      return true ; 
    } // if

    else {
      return false ;
    } // else
  } // else if

  else {
    mIsNotS_exp = true ;
    return false ;
  } // else
} // Parser::S_exp()


bool Parser :: Atom() { // The grmmer of Atom .

  if ( mscaner.mtokentype == INT ) {
    return true ;
  } // if

  else if ( mscaner.mtokentype == NIL ) {
    return true ;
  } // else if

  else if ( mscaner.mtokentype == T ) {
    return true ;
  } // else if

  else if ( mscaner.mtokentype == STRING ) {
    return true ;
  } // else if

  else if ( mscaner.mtokentype == FLOAT ) {
    return true ; 
  } // else if

  else if ( mscaner.mtokentype == SYMBOL ) {
    return true ;
  } // else if

  else {
    return false ;
  } // else
} // Parser::Atom()

void Parser :: InitialTokenlist() {
  mTokenlist.clear() ;
} // Parser::InitialTokenlist()

bool Parser :: CheckWhiteSpace() {
  string command = mscaner.ReturnCommand() ;
  int index = mscaner.GetColumn() ;

  while ( command[index] != '\n' ) {
    if ( command[index] != ' ' && command[index] != '\t' && command[index] != '\n' ) {
      return false ; // There are not all white space, can not skip !
    } // if

    index++ ;
  } // while

  return true ;
} // Parser::CheckWhiteSpace()

void Parser :: DeleteDot() {
  int k = 0 ; 
  int countLeft = 0 ;

  vector<int> checkRight ;
  while ( k < mTokenlist.size() ) {
    if ( mTokenlist[k].type == LEFT ) {
      countLeft = countLeft + 1 ;
    } // if

    else if ( mTokenlist[k].type == DOT ) {
      if ( mTokenlist[k+1].type == LEFT ) {
        checkRight.push_back( countLeft ) ;
        countLeft = countLeft + 1 ;
        morefunction.mHasDot = true ;
        mTokenlist.erase( mTokenlist.begin() + k ) ; // delete DOT .
        mTokenlist.erase( mTokenlist.begin() + k ) ; // delete LEFT .
        k = k - 1 ;
      } // if
    } // else if

    else if ( mTokenlist[k].type == RIGHT ) {
      countLeft = countLeft - 1 ;
      if ( !checkRight.empty() ) {
        if ( countLeft == checkRight.back() ) {
          mTokenlist.erase( mTokenlist.begin() + k ) ; // delete RIGHT .
          checkRight.pop_back() ;
          k = k - 1 ;
        } // if
      } // if
    } // else if

    k  =  k + 1 ;
  } // while
} // Parser::DeleteDot()

void Parser :: DeleteDot2() {
  int k = 0 ; 
  int countLeft = 0 ;

  vector<int> checkRight ;
  while ( k < mRebuildCommand.size() ) {
    if ( mRebuildCommand[k].type == LEFT ) {
      countLeft = countLeft + 1 ;
    } // if

    else if ( mRebuildCommand[k].type == DOT ) {
      if ( mRebuildCommand[k+1].type == LEFT ) {
        checkRight.push_back( countLeft ) ;
        countLeft = countLeft + 1 ;
        mRebuildCommand.erase( mRebuildCommand.begin() + k ) ; // delete DOT .
        mRebuildCommand.erase( mRebuildCommand.begin() + k ) ; // delete LEFT .
        k = k - 1 ;
      } // if
    } // else if

    else if ( mRebuildCommand[k].type == RIGHT ) {
      countLeft = countLeft - 1 ;
      if ( !checkRight.empty() ) {
        if ( countLeft == checkRight.back() ) {
          mRebuildCommand.erase( mRebuildCommand.begin() + k ) ; // delete RIGHT .
          checkRight.pop_back() ;
          k = k - 1 ;
        } // if
      } // if
    } // else if

    k  =  k + 1 ;
  } // while
} // Parser::DeleteDot2()

void Parser :: DeleteNil() {
  int i = 0 ; 
  while ( i < mRebuildCommand.size() ) {
    if ( mRebuildCommand[i].type == DOT && mRebuildCommand[i+1].type == NIL ) {
      mRebuildCommand.erase( mRebuildCommand.begin() + i ) ;
      mRebuildCommand.erase( mRebuildCommand.begin() + i ) ;
    } // if

    i++ ;
  } // while
} // Parser::DeleteNil()

void Parser :: RestoreSymbol() {
  int index = 0 ;
  while ( index < mRebuildCommand.size() ) {
  
    if ( mRebuildCommand[index].type == SYMBOL ) {
      if ( morefunction.IsDefined( mRebuildCommand[index].token ) ) {

        mRebuildCommand.erase( mRebuildCommand.begin() + index ) ;

        int k = 0 ;
        for ( k = morefunction.mDefined[morefunction.mDefinedIndex].binding.size() - 1 ; k >= 0 ; k -- ) {
          mRebuildCommand.insert( mRebuildCommand.begin() + index,
                                  morefunction.mDefined[morefunction.mDefinedIndex].binding[k] ) ;
        } // for
      } // if

      else {
      
      } // else
    } // if

    index ++ ;
  } // while
} // Parser::RestoreSymbol()

void Parser :: BuildTree( int & index, Node*parent ) { // "index" is mTokenlist's index, start with 0 !
  static bool isDot = false ;

  if ( index >= mTokenlist.size() ) {
    return ;
  } // if

  // if '('
  if ( mTokenlist[index].type == LEFT ) { // creat a new node at left side .

 
    if ( parent->left == NULL ) {
      parent->left = NewNode() ;
      index++ ;
      if ( IsAtom( mTokenlist[index].type ) ) { // if ATOM
        // cout << "left" ;
        // cout << mTokenlist[index].token ;
        parent->left->tokeninfo = mTokenlist[index] ; 
        mRebuildCommand.push_back( mTokenlist[index] ) ;
        index++ ;
      } // if
  
      else { // LEFT again 

        // cout << "left " ;
        mtokeninfo.token = "(" ;
        mtokeninfo.type = LEFT ;
        mRebuildCommand.push_back( mtokeninfo ) ;
        BuildTree( index, parent->left ) ;
      } // else

      // cout << "P " ;
      BuildTree( index, parent ) ;
    } // if
    
    else {

      parent->right = NewNode() ;
      parent->right->left = NewNode() ;

      mtokeninfo.token = "(" ;
      mtokeninfo.type = LEFT ;
      mRebuildCommand.push_back( mtokeninfo ) ;
      // cout << "right left  " ;

      BuildTree( index, parent->right->left ) ;
      // cout << "right " ;
      
      BuildTree( index, parent->right ) ;
       
      // parent = parent->right ;
    } // else
  } // if

  // else if ATOM
  else if ( IsAtom( mTokenlist[index].type ) ) {
            // save token info in node, and go back to parent .

    if ( parent->left == NULL ) {
      parent->left = NewNode() ; 
      parent->left->tokeninfo = mTokenlist[index] ;
      // cout << "left " ;
      // cout << mTokenlist[index].token ;
      mRebuildCommand.push_back( mTokenlist[index] ) ;
      index++ ;
      // cout << "P " ;
      BuildTree( index, parent ) ;
      // cout << "dfsdfsdfsfsf" ;
    } // if

    else {
      // cout << "test " ;
      parent->right = NewNode() ;
      // cout << "right" ;
      BuildTree( index, parent->right ) ;
    } // else
    
  } // else if

  // else if '.'
  else if ( mTokenlist[index].type == DOT ) { // creat node at right side .
    
    index ++ ;

    if ( IsAtom( mTokenlist[index].type ) ) {
      // isDot = false ;
      parent->right = NewNode() ;
      // cout << "right " ;
      // cout << mTokenlist[index].token ;
      parent->right->tokeninfo = mTokenlist[index] ;
      if ( mTokenlist[index].type != NIL ) {
        mtokeninfo.token = "." ;
        mtokeninfo.type = DOT ;
        mRebuildCommand.push_back( mtokeninfo ) ;
        mRebuildCommand.push_back( mTokenlist[index] ) ;
      } // if
      
      index ++ ;
      // cout << "P " ;

      mtokeninfo.token = ")" ;
      mtokeninfo.type = RIGHT ;
      mRebuildCommand.push_back( mtokeninfo ) ;

      BuildTree( index, parent ) ;
    } // if

    else { // means it's DOT + "(", skip "("

    } // else
  } // else if

  // else if ')'
  else if ( mTokenlist[index].type == RIGHT ) {

    if ( parent->right == NULL ) {
      parent->right = NewNode() ;
      parent->right->tokeninfo.type = NIL ;
      parent->right->tokeninfo.token = "NIL" ;
      // parent = special ;
        
      mtokeninfo.token = ")" ;
      mtokeninfo.type = RIGHT ;
      mRebuildCommand.push_back( mtokeninfo ) ;

    } // if


    index++ ;
    // cout << "P " ;
    // BuildTree( index, parent ) ;

  } // else if
} // Parser::BuildTree()

void Parser :: CreatRoot() {
  mroot = NewNode() ; 
} // Parser::CreatRoot()

bool Parser :: IsAtom( TokenType type ) {
  if ( type == SYMBOL || type == INT || type == FLOAT || type == STRING 
       || type == NIL || type == T || type == QUOTE ) {
    return true ;
  } // if

  else {
    return false ;
  } // else
} // Parser::IsAtom()

void Parser :: PrettyPrint( int & index, int & currentCharNum ) {

  if ( mRebuildCommand[index].type == LEFT ) {
    for ( int j = 0 ; j < currentCharNum * 2 ; j ++ ) {
      cout << " " ;
    } // for

    while ( mRebuildCommand[index].type == LEFT ) {
      currentCharNum = currentCharNum + 1 ;
      cout << "( " ;
      index ++ ;
    } // while

    PrintNode( index ) ;
    cout << endl ;

    index ++ ;
  } // if

  else if ( mRebuildCommand[index].type == RIGHT ) {
    currentCharNum = currentCharNum - 1 ;

    for ( int i = 0 ; i < currentCharNum * 2 ; i ++ ) {
      cout << " " ;
    } // for

    cout << ")" << endl ;
    index ++ ;
  } // else if

  else { // not LEFT neither RIGHT
    for ( int k = 0 ; k < currentCharNum * 2 ; k ++ ) {
      cout << " " ;
    } // for

    PrintNode( index ) ;
    cout << endl ;
    index ++ ;
  } // else 

  if ( currentCharNum != 0 ) {
    PrettyPrint( index, currentCharNum ) ;
  } // if

  else {

  } // else
} // Parser::PrettyPrint()

void Parser :: Eval( int tempindex ) {
  // if no evaluation error
  // means current token is any one of internal function call( class Morefunction ),

  morefunction.mIsSymbol = false ; // explain in Morefunction .

  if ( mRebuildCommand[tempindex].token == "cons" ) {
    morefunction.Cons() ;
  } // if

  else if ( mRebuildCommand[tempindex].token == "list" ) {
    morefunction.List() ;
  } // else if

  else if ( mRebuildCommand[tempindex].token == "quote" || mRebuildCommand[tempindex].token == "'" ) {
    // if this statement is called, means it's like : ('(3 . nil)), don't need the parent out side .
    if ( morefunction.mCounter == 0 ) {
    
      morefunction.ByPass( tempindex ) ;
      morefunction.mByPassParent.clear() ;
    
      int countLeft = 1 ;
      int k = tempindex - 1 ;
      morefunction.mResultCommand.erase( morefunction.mResultCommand.begin() + k ) ; // delete first LEFT .
      while ( countLeft != 0 ) {
        if ( morefunction.mResultCommand[k].type == LEFT ) {
          countLeft ++ ;
        } // if

        else if ( morefunction.mResultCommand[k].type == RIGHT ) {
          countLeft -- ;
        } // else if

        if ( countLeft == 0 ) {
          // delete last RIGHT .
          morefunction.mResultCommand.erase( morefunction.mResultCommand.begin() + k ) ;
        } // if

        k++ ;
      } // while
    } // if
  } // else if

  else if ( mRebuildCommand[tempindex].token == "define" ) {
    if ( morefunction.mCounter == 0 ) {
      morefunction.Defined() ;
    } // if
    
    else {
      // cout << "Defined() should be the top level !" << endl ;
    } // else 
  } // else if

  else if ( mRebuildCommand[tempindex].token == "car" ) {
    morefunction.Car() ;
  } // else if

  else if ( mRebuildCommand[tempindex].token == "cdr" ) {
    morefunction.Cdr() ;
  } // else if 

  else if ( mRebuildCommand[tempindex].token == "pair?" ) {
    morefunction.IsPair() ;
  } // else if

  else if ( mRebuildCommand[tempindex].token == "null?" ) {
    morefunction.IsNull() ;
  } // else if 

  else if ( mRebuildCommand[tempindex].token == "integer?" ) {
    morefunction.IsInteger() ;
  } // else if

  else if ( mRebuildCommand[tempindex].token == "real?" ) {
    morefunction.IsReal() ;
  } // else if

  else if ( mRebuildCommand[tempindex].token == "number?" ) {
    morefunction.IsNumber() ;
  } // else if

  else if ( mRebuildCommand[tempindex].token == "string?" ) {
    morefunction.IsString() ;
  } // else if 

  else if ( mRebuildCommand[tempindex].token == "boolean?" ) {
    morefunction.IsBoolean() ;
  } // else if

  else if ( mRebuildCommand[tempindex].token == "symbol?" ) {
    morefunction.IsSymbol() ;
  } // else if

  else if ( mRebuildCommand[tempindex].token == "eqv?" ) {
    morefunction.IsEqv() ;
  } // else if 

  else if ( mRebuildCommand[tempindex].token == "equal?" ) {
    morefunction.IsEqual() ;
  } // else if

  else if ( mRebuildCommand[tempindex].token == "atom?" ) {
    morefunction.IsAtom() ;
  } // else if

  else if ( mRebuildCommand[tempindex].token == "list?" ) {
    morefunction.IsList() ;
  } // else if

  else if ( mRebuildCommand[tempindex].token == "if" ) {
    morefunction.If() ;
  } // else if

  else if ( mRebuildCommand[tempindex].token == "cond" ) {
    morefunction.Cond() ;
  } // else if

  else if ( mRebuildCommand[tempindex].token == "+" || mRebuildCommand[tempindex].token == "-"
            || mRebuildCommand[tempindex].token == "*" || mRebuildCommand[tempindex].token == "/"
            || mRebuildCommand[tempindex].token == ">" || mRebuildCommand[tempindex].token == ">="
            || mRebuildCommand[tempindex].token == "<" || mRebuildCommand[tempindex].token == "<="
            || mRebuildCommand[tempindex].token == "=" ) {

    morefunction.Math() ;
  } // else if

  else if ( mRebuildCommand[tempindex].token == "not" ) {
    morefunction.Not() ;
  } // else if

  else if ( mRebuildCommand[tempindex].token == "and" ) {
    morefunction.And() ;
  } // else if

  else if ( mRebuildCommand[tempindex].token == "or" ) {
    morefunction.Or() ;
  } // else if

  else if ( mRebuildCommand[tempindex].token == "string-append" || 
            mRebuildCommand[tempindex].token == "string>?" || 
            mRebuildCommand[tempindex].token == "string<?" ||
            mRebuildCommand[tempindex].token == "string=?" ) {
    morefunction.StringOP() ;
  } // else if

  else if ( mRebuildCommand[tempindex].token == "begin" ) {
    morefunction.Begin() ;
  } // else if

  else {
    // errormessage here! .
  } // else
  // else evaluation error
    // then PrintEvaluationError(), in my case, may used "throw" to return error message .
  morefunction.mByPassParent.clear() ;
  morefunction.mParaData.clear() ;
  morefunction.mIsSymbol = false ;
} // Parser::Eval()

void Parser :: EvalSexp( int & tempindex2 ) {
  // static int index = 0 ;
  int index2 = 0 ;
  Tokeninfo token ;
  string output = "environment cleaned" ;

  // if the command is : (clean-environment), clean all defined symbol .
  if ( mRebuildCommand.size() == 3 ) {
    if ( mRebuildCommand[0].type == LEFT ) {
      if ( mRebuildCommand[1].token == "clean-environment" ) {
        if ( mRebuildCommand[2].type == RIGHT ) {
          morefunction.mDefined.clear() ;
          morefunction.mResultCommand.clear() ;
          token.token = output ;
          token.type = STRING ;
          morefunction.mResultCommand.push_back( token ) ;
          return ;
        } // if
      } // if
    } // if
  } // if

  if ( mTokenlist[0].type == QUOTE ) {
    morefunction.mResultCommand.erase( morefunction.mResultCommand.begin() ) ; // delete the addtion LEFT .
    morefunction.mResultCommand.erase( morefunction.mResultCommand.begin() ) ; // delete token "quote" .
    return ;
  } // if 

  if ( tempindex2 < mRebuildCommand.size() ) {
    if ( mRebuildCommand[tempindex2].type == QUOTE || mRebuildCommand[tempindex2].token == "quote" ) {
      if ( mRebuildCommand[tempindex2 + 1].type != LEFT ) { // it's a "quote + atom" .
        tempindex2 = tempindex2 + 2 ;
      } // if

      else {
        int countLeft = 1 ;
        tempindex2 = tempindex2 + 2 ;
        while ( countLeft != 0 ) {
          if ( mRebuildCommand[tempindex2].type == LEFT ) {
            countLeft ++ ;
          } // if

          else if ( mRebuildCommand[tempindex2].type == RIGHT ) {
            countLeft -- ;
          } // else if

          tempindex2 ++ ;
        } // while
      } // else

      EvalSexp( tempindex2 ) ;
    } // if

    else if ( mRebuildCommand[tempindex2].type == LEFT && mRebuildCommand[tempindex2 +1].type != LEFT ) {
      // if it may be a function call, check and recursive to check behind .
      if ( morefunction.IsInternal( mRebuildCommand[tempindex2 + 1].token ) ) {
        index2 = tempindex2 ;

        tempindex2 = tempindex2 + 2 ;
        morefunction.mCounter ++ ;
        
        EvalSexp( tempindex2 ) ; // find next function call .
        morefunction.mCounter -- ;
        morefunction.mindex = index2 ;

        Eval( index2 + 1 ) ;
      } // if

      else { // the token after a "(", should be a function call .
        // Errormessage Here !
        tempindex2 ++ ;
        EvalSexp( tempindex2 ) ; // find next function call .
      } // else
    } // else if

    else {
      tempindex2 ++ ;
      EvalSexp( tempindex2 ) ; // find next function call .
    } // else 
  } // if

  else {
    return ;
  } // else
} // Parser::EvalSexp()

int main()
{
  Parser parser ;
  cin >> gfuckPL ;
  cin.get() ;
  
  cout << "Welcome to OurScheme!" << endl << endl ;
  parser.Parsermain() ;

  cout << "Thanks for using OurScheme!" ;
  
} // main()