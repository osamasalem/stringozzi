/**
 * @file Stringozzi.h
 * @author Osama Salem (usamamsalem@yahoo.com)
 * @brief  the main include file 
 * @version 2.0.0.0
 * @date 2020-10-25
 * 
 * @copyright Copyright (c) Osama Salem 2020
 * 
 */
/*
MIT License

Copyright (c) 2020 Osama Salem

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial
portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/**
 * @brief The main header file for Stringozzi
 * 
 */


#ifndef INCLUDE_STRINGOZZI_H_
#define INCLUDE_STRINGOZZI_H_


#include <string>
#include <map>
#include <vector>
#include <stack>

#ifdef __GNUC__
#include <string.h>
#include<cstring>
#endif

// TODO(osamamsalem): Greedy AND and OR validators

using namespace std;

#define CPPVER_1997 199711L
#define CPPVER_2003 CPPVER_1997
#define CPPVER_2011 201103L
#define CPPVER_2014 201402L
#define CPPVER_2017 201703L
#define CPPVER_2020 202002L

#if __cplusplus < CPPVER_2003
#error Stringozzi: this version of C++ is not supported
#endif

#if __cplusplus >= CPPVER_2003
#define CX03_SUPPORTED (1)
#endif

#if __cplusplus >= CPPVER_2011
#define CX11_SUPPORTED (1)
#endif

#if __cplusplus >= CPPVER_2014
#define CX14_SUPPORTED (1)
#endif

#if __cplusplus >= CPPVER_2017
#define CX17_SUPPORTED (1)
#endif

#if __cplusplus >= CPPVER_2020
#define CX20_SUPPORTED (1)
#endif


#ifndef CX20_SUPPORTED
typedef char char8_t;
#endif


/**
 * @brief Shared module attributes
 * 
 */
#if defined _WIN32 || defined __CYGWIN__
#ifdef BUILDING_DLL
#ifdef __GNUC__
#define DLL_PUBLIC __attribute__ ((dllexport))
#define DLL_PUBLIC_VAR __attribute__ ((dllexport))
#else  // !__GNUC__
#define DLL_PUBLIC __declspec(dllexport)
#define DLL_PUBLIC_VAR __declspec(dllexport)
#endif  // GNUC__
#else  // !BUILDING_DLL
#ifdef __GNUC__
#define DLL_PUBLIC __attribute__ ((dllimport))
#define DLL_PUBLIC_VAR __attribute__ ((dllimport))
#else  // !__GNUC__
#define DLL_PUBLIC __declspec(dllimport)
#define DLL_PUBLIC_VAR __declspec(dllimport)
#endif  // __GNUC__
#endif  // BUILDING_DLL
#define DLL_LOCAL
#else  // !defined _WIN32 && !defined __CYGWIN__
#if __GNUC__ >= 4
#define DLL_PUBLIC __attribute__ ((visibility ("default")))
#define DLL_PUBLIC_VAR
#define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#else
#define DLL_PUBLIC
#define DLL_PUBLIC_VAR
#define DLL_LOCAL
#endif
#endif


#define MAXIMUM(a, b) ( ( (a) > (b) ) ? (a) : (b))



#ifndef MAX_ITER
#define MAX_ITER 5000
#endif

#define SPEG_CASEINSENSITIVE (1 << 0)
#define SPEG_MATCHNAMED (1 << 1)
#define SPEG_MATCHUNNAMED (1 << 2)
#define SPEG_IGNORESPACES (1 << 3)

#define NORMALIZE(__X) ( ((__X) > 0)?(1):( ( (__X) < 0) ?(-1):0))
#define MATCHES_TOKEN "<MATCHES>"

/**
 * @brief The main namespace for Stringozzi
 * 
 */
namespace SPEG {
  /**
   * @brief UTF32 Data type
   * 
   */
  typedef unsigned long SChar;

/**
 * @brief Namespace for Core Stringozzi elements
 * 
 */
namespace Core {
class Rule;
typedef const void* Position;

}

/**
 * @brief Utilities namespace
 * 
 */
namespace Utils {
/**
 * @brief function conerts ASCII UTF32 characters to lower case
 * 
 * @param long 
 * @return SChar 
 */
DLL_PUBLIC SChar CharToLower(unsigned long);

/**
 * @brief Convert UTF8 char to UTF32 
 * 
 * @param ptr input string
 * @return SChar  char in UTF32
 */
DLL_PUBLIC SChar UTF8ToUTF32(const char* ptr);

/**
 * @brief get length of UTF8 token
 * 
 * @param ptr  input string
 * @return unsigned long size in bytes 
 */
DLL_PUBLIC unsigned long UTF8ToUTF32Length(const char* ptr);

#ifdef CX11_SUPPORTED
/**
 * @brief Convert UTF16 char to UTF32 
 * 
 * @param ptr input string
 * @return SChar  char in UTF32
 */
DLL_PUBLIC SChar UTF16ToUTF32(const char16_t * ptr);

/**
 * @brief get length of UTF16 token
 * 
 * @param ptr  input string
 * @return unsigned long size in chars 
 */
DLL_PUBLIC unsigned long UTF16ToUTF32Length(const char16_t* ptr);
#endif

/**
 * @brief Increment (move forward) the pointer one step 
 * 
 * @tparam __T string pointer type
 * @param pointer  string pointer
 */
template <typename __T>
inline void Increment(const __T** pointer) {
  (*pointer)++;
}

template<>
inline void Increment<char>(const char** pointer) {
  (*pointer) += UTF8ToUTF32Length(*pointer);
}

/**
 * @brief Decrement (move backward) the pointer one step 
 * 
 * @tparam __T string pointer type
 * @param pointer  string pointer
 */
template <typename __T>
void Decrement(const __T** pointer) {
  (*pointer)--;
}

template<>
DLL_PUBLIC void Decrement<char>(const char** pointer);


/**
 * @brief Get the UTF32 Char from different char types
 * 
 * @tparam __T  chare type
 * @param pointer string pointer
 * @return SChar the output UTF32 char   
 */
template <typename __T>
inline SChar GetChar(const __T* pointer) {
  return *pointer;
}

template<>
inline SChar GetChar<char>(const char* pointer) {
  return UTF8ToUTF32(pointer);
}

/**
 * @brief Cross platform atomic increment the passed variable
 * 
 * @param pnum a pointer to double word string  
 */
DLL_PUBLIC void SafeIncrement(unsigned long* pnum);
/**
 * @brief Cross platform atomic decrement the passed variable
 * 
 * @param pnum a pointer to double word string  
 */
DLL_PUBLIC void SafeDecrement(unsigned long* pnum);

/**
 * @brief The data structure that hold matches result
 * 
 * @tparam __CHARTYPE string pointer header
 */
template<typename __CHARTYPE>
class Matches {
 public:
  /**
   * @brief Represent a single expression Match
   * 
   * @tparam __T 
   */
  template<typename __T>
  struct Match {
    Match(Core::Position start, Core::Position end)
    : Start(start)
    , End(end)
    , Index(-1) {}
    Core::Position Start;
    Core::Position End;
    int Index;
  };


 private:
  typedef Match<__CHARTYPE> MATCH;
  typedef vector<MATCH>  VECTOR;
  typedef map<string, VECTOR>  MAP;
  typedef pair<string, VECTOR>  PAIR;
  typedef basic_string<__CHARTYPE> STRING;
  typedef vector<STRING> STRARRAY;

  MAP _matchesMap;
  STRARRAY _matchesStrings;

 public:
  /**
  * @brief Construct a new Matches object
  * 
  */
  Matches() {}

  /**
   * @brief Add a new match 
   * 
   * @param key   Match key 
   * @param start Token's start pointer
   * @param end   Token's end pointer
   */
  inline void Add(const char* key
        , const Core::Position start
        , const Core::Position end) {
    _matchesMap[key].push_back(MATCH(start, end));
  }


  /**
   * @brief Get a single Match
   * 
   * @param key Match name
   * @param index Match index for multiple occurances (default 0)
   * @return const __CHARTYPE* a pointer to null terminated string 
   *                           containing the mtached token
   */
  const __CHARTYPE* Get(const char* key, const unsigned int index = 0) {
    typename MAP::iterator it = _matchesMap.find(key);
    if (it == _matchesMap.end())
      return NULL;


    VECTOR& v = it->second;
    if (index >= v.size())
      return NULL;


    MATCH& m = v.at(index);
    if (m.Index == -1) {
      unsigned int size = static_cast<const __CHARTYPE*>(m.End)
                - static_cast<const __CHARTYPE*>(m.Start);
      _matchesStrings.push_back(STRING((__CHARTYPE*)m.Start, size));
      m.Index = _matchesStrings.size() - 1;
    }

    return _matchesStrings[m.Index].c_str();
  }

  /**
   * @brief return the number of keys in the matches table
   * 
   * @return unsigned int number of keys in the matches table
   */
  unsigned int NumberOfMatches() {
    return _matchesMap.size();
  }

  /**
 * @brief return the number of entries for a specific token key
 * 
 * @param key the input token key  
 * @return int  the number of entries 
 */
  int NumberOfMatches(const char* key) {
    typename MAP::iterator it = _matchesMap.find(key);
    if (it == _matchesMap.end())
      return 0;

    return it->second.size();
  }

  /**
   * @brief Clear all entries in the match table
   *  
   */
  void Clear() {
    _matchesMap.clear();
    _matchesStrings.clear();
  }
};

typedef Matches<char> MatchesA;
typedef Matches<wchar_t> MatchesW;

/**
 * @brief Represent a range of numbers used usaully with operators
 * 
 */
struct Range {
  /**
   * @brief Construct a new Range object
   * 
   * @param min range's lower bound
   * @param max range's upper bound
   */
  Range(const unsigned int min, const unsigned int max) : MIN(min), MAX(max) {}
  const unsigned int MIN;
  const unsigned int MAX;
};

/**
 * @brief Represent configuration flag set
 * 
 */
class Flags {
  typedef unsigned long FlagSet;

 public:
  FlagSet _flags;

  explicit Flags(FlagSet init) : _flags(init) {}

  inline void SetFlag(FlagSet flag) {
    _flags |= flag;
  }

  inline void ClearFlag(FlagSet flag) {
    _flags &= ~flag;
  }

  inline void ToggleFlag(FlagSet flag) {
    _flags ^= flag;
  }


  inline void SetFlag(FlagSet flag, bool value) {
    if (value)
      SetFlag(flag);
    else
      ClearFlag(flag);
  }

  inline void SetAllFlags(FlagSet flags) {
    _flags = flags;
  }



  inline bool IsFlagSet(FlagSet flag) {
    return ((_flags & (flag)) == flag);
  }
};
}  // namespace Utils

namespace Core {
/**
 * @brief This is a major interface in parsing process, 
 * this interface class holds the internal parsing context
 * and the state of underlying string and in the same time  
 * interats with all parsing elements to do their work 
 * 
 */
class ContextInterface {
 public:
 /**
  * @brief Move parsing cursor one step ahead
  * 
  * @return true if not end of text 
  * @return false otherwise 
  */
  virtual bool Forward() = 0;

  /**
   * @brief moves the parsing cursor one step backward
   * 
   * @return true if not beginning of text
   * @return false otherwises
   */
  virtual bool Backward() = 0;

  /**
   * @brief Check whether parsing cursor reached end of text
   * 
   * @return true BOT reached 
   * @return false otherwise
   */
  virtual bool BOT() = 0;

  /**
   * @brief Check whether parsing cursor reached end of text ('\0')
   * 
   * @return true EOT reached
   * @return false otherwise
   */
  virtual bool EOT() = 0;

  /**
   * @brief returns configuration flag set
   * 
   * @return Utils::Flags& the returned flags
   */
  virtual Utils::Flags& Flags() = 0;

  /**
   * @brief Compare a character with the the character under parsing cursor
   * 
   * @param c the input character
   * @return const int 1 if current char greater than the input character 
   *                   -1 if current char lesser than the input character
   *                    0 if they are equal
   */
  virtual const int Compare(SChar c) = 0;

  /**
   * @brief Get the character under parsing cursor in UTF32
   * 
   * @return const SChar returns the character under parsing cursor in UTF32
   */
  virtual const SChar Get() = 0;

  /**
   * @brief Get the current cursor position
   * 
   * @return Position cursor position
   */
  virtual Position GetPosition() = 0;

  /**
   * @brief Set the current cursor position
   * 
   * @param pos the new position 
   */
  virtual void SetPosition(Position pos) = 0;

  /**
   * @brief extract match to the unnamed matches table, from the specified 
   *        start position to the current cursor position
   * 
   * @param start  start position where the extraction start from
   */
  virtual void AddMatch(Position start) = 0;

  /**
   * @brief extract match to the named matches table, from the specified 
   *        start position to the current cursor position
   * @param key the key where the match will stored
   * @param start start position where the extraction start from
   */
  virtual void AddMatch(const char* key, Position start) = 0;

  /**
   * @brief Proxy to Matches.NumberOfMatches
   * 
   * @param key 
   * @return unsigned int 
   */
  virtual unsigned int NumberOfMatches(const char* key) = 0;

  /**
   * @brief Set the context variable
   * 
   * @param vname variable name
   * @param vval variable value 
   */
  virtual void SetVar(const char* vname, const char* vval) = 0;

  /**
   * @brief Get the context variable
   * 
   * @param vname variable name
   * @param vval  variable value
   * 
   * @return true if the variable exist 
   * @return false otherwise
   */
  virtual bool GetVar(const char* vname, char const ** vval) = 0;

  /**
   * @brief Delete(unset) context variable 
   * 
   * @param vname variable name
   * @return true if variable exist
   * @return false  otherwise
   */
  virtual bool DelVar(const char* vname) = 0;

  /**
   * @brief Adjust pointer position.. if SPEG_IGNOESPACES is set ,it will
   *  adjust the pointer to the first non space character
   * 
   * @return Position the new posistion
   */
  virtual Position AdjustPosition() = 0;
};


template<typename __CHARTYPE>
class Context : public ContextInterface {
  typedef basic_string<__CHARTYPE> STRING;
  typedef map<string, string>  MAP;
  typedef pair<string, string>  PAIR;

  const __CHARTYPE* _pointer;
  const __CHARTYPE* _string;
  Utils::Flags _flags;
  Utils::Matches<__CHARTYPE> _matches;
  MAP _vars;

  inline const SChar _Get() {
    SChar chr = Utils::GetChar(_pointer);
    return _Get(chr);
  }

  inline const SChar _Get(SChar chr) {
    if (_flags.IsFlagSet(SPEG_CASEINSENSITIVE))
      return Utils::CharToLower(chr);
    return chr;
  }

 public:
  virtual const SChar Get() {
    SChar chr = Utils::GetChar(_pointer);
    return _Get(chr);
  }

  Utils::Matches<__CHARTYPE>  Matches() {
    return _matches;
  }

  explicit Context(const __CHARTYPE* str, unsigned long flags)
    : _pointer(str)
    , _flags(flags) {
    AdjustPosition();
    _string = _pointer;
  }


  virtual Position AdjustPosition() {
    if (_flags.IsFlagSet(SPEG_IGNORESPACES)) {
      while (Compare(' ') == 0) {
        Forward();
      }
    }
    return _pointer;
  }

  inline bool Forward() {
    if (!EOT()) {
      Utils::Increment(&_pointer);
      return true;
    }
    return false;
  }


  inline bool Backward() {
    if (!BOT()) {
      Utils::Decrement(&_pointer);
      return true;
    }
    return false;
  }

  inline bool BOT() {
    return _string == _pointer;
  }

  inline bool EOT() {
    return !*_pointer;
  }


  virtual const int Compare(SChar c) {
    SChar local = _Get();
    SChar other = _Get(c);
    if (local > other)
      return 1;
    else if (local < other)
      return -1;
    return 0;
  }


  virtual Utils::Flags& Flags() {
    return _flags;
  }

  virtual unsigned int NumberOfMatches(const char* key) {
    return _matches.NumberOfMatches(key);
  }

  inline Position GetPosition() {
    return _pointer;
  }

  inline void SetPosition(Position position) {
    _pointer = static_cast<const __CHARTYPE*>(position);
  }

  inline void AddMatch(const char* key, Position start) {
    if (_flags.IsFlagSet(SPEG_MATCHNAMED)) {
      _matches.Add(key, start, GetPosition());
    }
  }

  inline void AddMatch(Position start) {
    if (_flags.IsFlagSet(SPEG_MATCHUNNAMED)) {
      Core::Position end = GetPosition();
      if (end > start) {
        _matches.Add(MATCHES_TOKEN, start, end);
      }
    }
  }

  inline void SetVar(const char* vname, const char* vval) {
    _vars[vname] = vval;
  }

  inline bool GetVar(const char* vname, char const ** vval) {
    if (_vars.find(vname) != _vars.end()) {
      *vval = _vars[vname].c_str();
      return true;
    }
    return false;
  }

  inline bool DelVar(const char* vname) {
    return _vars.erase(vname) == 1;
  }
};

typedef Context<char> ContextA;
typedef Context<wchar_t> ContextW;
/**
 * @brief the interface every parsing component should implement..
 * all children of this interface is used to build the parsing engine 
 * tree
 * 
 */
class StringValidator {
 public:
  /**
   * @brief the main function that perform parsing/validation
   * 
   * @param context a pointer to conext object that the parsing use 
   * @return true if parsing is successful.. the parsing cursor may move
   * @return false otherwise
   */
  virtual bool Check(ContextInterface* context) const = 0;
  /**
   * @brief Destroy the String Validator object
   * 
   */
  virtual ~StringValidator() {}
  /**
   * @brief Decrement the reference count of object .. if reference count 
   * reaches zero .. it will delete the object
   * 
   */
  virtual void Release() = 0;

  /**
   * @brief Increment the reference count.. meaning that there is another 
   * variable/object referring to this object
   * 
   */
  virtual void AddReference() = 0;
  /**
   * @brief Destructor like function called before finally
   * deleting the object
   * 
   */
  virtual void Dispose() = 0;
};

/**
 * @brief The father of all validation/parsing objects,
 * it implements the reference counting mechanism 
 * 
 */
class NormalValidator : public StringValidator {
  unsigned long _referenceCount;
 public:
  NormalValidator() : _referenceCount(0) {}

  DLL_PUBLIC virtual void AddReference();
  DLL_PUBLIC virtual void Release();
  virtual void Dispose() {}
};

/**
 * @brief it represents a single argument operation that receives
 * another operation as an argument  
 * 
 */
class UnaryValidator : public NormalValidator {
 protected:
 /**
  * @brief the operand the operation receives to execute it later
  * 
  */
  StringValidator* const Operand;
 public:
  explicit UnaryValidator(StringValidator* op) : Operand(op) {
    Operand->AddReference();
  }
  virtual void Dispose();
};

/**
 * @brief it represents a dual argument operation that receives
 * another two operations as arguments  
 */
class BinaryValidator : public NormalValidator {
 protected:
 /**
  * @brief the first operand
  * 
  */
  StringValidator* const FirstOperand;
  /**
   * @brief the second one
   * 
   */
  StringValidator* const SecondOperand;

 public:
  BinaryValidator(StringValidator* op1, StringValidator* op2)
    : FirstOperand(op1), SecondOperand(op2) {
    op1->AddReference();
    op2->AddReference();
  }

  virtual void Dispose();
};

}  // namespace Core

/**
 * @brief the namespace of all real parsing validators that checks the 
 * string and make decision based on content
 * 
 */
namespace Primitives {
/**
 * @brief receives a character .. if the character under cursor 
 * equals this character it returns true and move farward ... 
 * otherwise it returns false 
 * 
 * @tparam __CHARTYPE character type
 */
template<typename __CHARTYPE>
class IsValidator : public Core::NormalValidator {
  const __CHARTYPE _character;
 public:
 /**
  * @brief Construct a new Is Validator object
  * 
  * @param chr the charcter which will compared to 
  */
  explicit IsValidator(__CHARTYPE chr) : _character(chr) {}
  virtual bool Check(Core::ContextInterface* context)const {
    context->AdjustPosition();
    Core::Position start = context->GetPosition();
    if (!context->Compare(_character)) {
      context->Forward();
      context->AddMatch(start);
      return true;
    }
    return false;
  }
};

typedef IsValidator<char>  IsValidatorA;
typedef IsValidator<wchar_t>  IsValidatorW;
typedef IsValidator<char8_t>  IsValidatorU8;
#ifdef  CX11_SUPPORTED
typedef IsValidator<char16_t>  IsValidatorU16;
typedef IsValidator<char32_t>  IsValidatorU32;
#endif

/**
 * @brief receives a string containing set .. if the character under cursor 
 * equals one of the set's characters it returns true and move farward ... 
 * otherwise it returns false 
 * @tparam __CHARTYPE 
 */
template<typename __CHARTYPE>
class InValidator : public Core::NormalValidator {
  const __CHARTYPE* _set;
 public:
 /**
  * @brief Construct a new In Validator object
  * 
  * @param set the string that contains character set
  */
  explicit InValidator(const __CHARTYPE* set) : _set(set) {}
  virtual bool Check(Core::ContextInterface* context)const {
    context->AdjustPosition();
    Core::Position start = context->GetPosition();
    const __CHARTYPE* setPtr = _set;
    while (*setPtr) {
      SChar chr = Utils::GetChar(setPtr);
      if (!context->Compare(chr)) {
        context->Forward();
        context->AddMatch(start);
        return true;
      }
      Utils::Increment(&setPtr);
    }
    return false;
  }
};

typedef InValidator<char>  InValidatorA;
typedef InValidator<wchar_t>  InValidatorW;
typedef InValidator<char8_t>  InValidatorU8;
#ifdef  CX11_SUPPORTED
typedef InValidator<char16_t>  InValidatorU16;
typedef InValidator<char32_t>  InValidatorU32;
#endif

/**
 * @brief if the character under the cursor equals to the previous
 * one + 1 it returns true.. it detects chains like "ABC" "123" ..etc
 * so if the character is "B" and previous one is "A" .. it is considered
 * in chain
 */
class InChainValidator : public Core::NormalValidator {
 public:
  InChainValidator() {}
  virtual bool Check(Core::ContextInterface* context) const;
};

/**
 * @brief returns true if this is the beginning of the string.. this 
 * does not move parsing cursor
 * 
 */
class BOTValidator : public Core::NormalValidator {
 public:
  BOTValidator() {}
  virtual bool Check(Core::ContextInterface* context) const;
};

/**
 * @brief it matches any character but EOT ('\0') and rturns true
 * 
 */
class AnyValidator : public Core::NormalValidator {
 public:
  AnyValidator() {}
  virtual bool Check(Core::ContextInterface* context) const;
};

/**
 * @brief it receives character range .. and returns true if the character under 
 * the parsing cursor is between the specified range 
 * 
 * @tparam __CHARTYPE character range 
 */
template<typename __CHARTYPE>
class BetweenValidator : public Core::NormalValidator {
  __CHARTYPE _min;
  __CHARTYPE _max;

 public:
 /**
  * @brief Construct a new Between Validator object
  * 
  * @param min lower bound
  * @param max upper bound
  */
  BetweenValidator(const __CHARTYPE min, const __CHARTYPE max)
    : _min(min)
    , _max(max) {}

  explicit BetweenValidator(const __CHARTYPE* range)
    : _min(range[0])
    , _max(range[1]) { }

  virtual bool Check(Core::ContextInterface* context) const {
    context->AdjustPosition();
    Core::Position start = context->GetPosition();
    if (context->Compare(_min) >= 0
      && context->Compare(_max) <= 0
      && context->Forward()) {
      context->AddMatch(start);
      return true;
    }
    return false;
  }
};


typedef BetweenValidator<char>  BetweenValidatorA;
typedef BetweenValidator<wchar_t>  BetweenValidatorW;
typedef BetweenValidator<char8_t>  BetweenValidatorU8;
#ifdef  CX11_SUPPORTED
typedef BetweenValidator<char16_t>  BetweenValidatorU16;
typedef BetweenValidator<char32_t>  BetweenValidatorU32;
#endif

/**
 * @brief it recieves a string containing a sequence of characters 
 * if the character under the cursor follows the same context it returns 
 * true.. false otherwise 
 * @tparam __T characted type
 */
template<typename __T>
class ExactValidator : public Core::NormalValidator {
  const __T* _phrase;

 public:
 /**
  * @brief Construct a new Exact Validator object
  * 
  * @param phrase the phrase which the text will be compared with
  */
  explicit ExactValidator(const __T* phrase) : _phrase(phrase) {}

  virtual bool Check(Core::ContextInterface* context) const {
    context->AdjustPosition();
    const __T* phrasePointer = _phrase;
    Core::Position start = context->GetPosition();
    while (*phrasePointer) {
      SChar chr = Utils::GetChar(phrasePointer);
      if (context->Compare(chr)) {
        context->SetPosition(start);
        return false;
      }
      Utils::Increment(&phrasePointer);
      context->Forward();
    }
    context->AddMatch(start);
    return true;
  }
};

typedef ExactValidator<char> ExactValidatorA;
typedef ExactValidator<wchar_t> ExactValidatorW;
typedef ExactValidator<char8_t> ExactValidatorU8;
#ifdef  CX11_SUPPORTED
typedef ExactValidator<char16_t> ExactValidatorU16;
typedef ExactValidator<char32_t> ExactValidatorU32;
#endif
}  // namespace Primitives


/**
 * @brief Manipulators is special validators that receives and executes other
 * child validator and take decisions based on their results
 * 
 */
namespace Manipulators {

/**
 * @brief Executes the operands sequentially the first then second
 * 
 */
class SeqValidator : public Core::BinaryValidator {
 public:
  explicit SeqValidator(Core::StringValidator* s1, Core::StringValidator* s2) :
    Core::BinaryValidator(s1, s2) {}
  virtual bool Check(Core::ContextInterface* context) const;
};

/**
 * @brief perform And operation between the two operations results . so if one
 * of them failed it will fail this operation and restore cursor positions 
 * 
 */
class AndValidator : public Core::BinaryValidator {
 public:
  explicit AndValidator(Core::StringValidator* s1, Core::StringValidator* s2) :
    Core::BinaryValidator(s1, s2) {}

  virtual bool Check(Core::ContextInterface* context) const;
};

/**
 * @brief perform Or operation between the two operations results . it applies 
 * the first successful operation.. if both are failed it will fail this 
 * operation and restore cursor positions
 * 
 */
class OrValidator : public Core::BinaryValidator {
 public:
  explicit OrValidator(Core::StringValidator* s1, Core::StringValidator* s2) :
    BinaryValidator(s1, s2) {}

  virtual bool Check(Core::ContextInterface* context) const;
};

/**
 * @brief perform Or operation between the two operations results . it applies 
 * the the most relevant match.. if both are failed it will fail this 
 * operation and restore cursor positions
 * 
 */
class GreedyOrValidator : public Core::BinaryValidator {
 public:
  explicit GreedyOrValidator(Core::StringValidator* op1
      , Core::StringValidator* op2) :
    BinaryValidator(op1, op2) {}
  virtual bool Check(Core::ContextInterface* context) const;
};

/**
 * @brief it negates the operation result ... this operation does not move
 * parsing cursor  
 * 
 */
class NotValidator : public Core::UnaryValidator {
 public:
  explicit NotValidator(Core::StringValidator* op)
        : Core::UnaryValidator(op) {}
  virtual bool Check(Core::ContextInterface* context) const;
};

/**
 * @brief it peeks the next token and checks if it matches the input 
 * rule, it does not move parsing pointer
 * 
 */
class LookAheadValidator : public Core::UnaryValidator {
 public:
  explicit LookAheadValidator(StringValidator* op)
                : UnaryValidator(op) {}
  virtual bool Check(Core::ContextInterface* context) const;
};

/**
 * @brief it peeks the previous token and checks if it matches the input 
 * rule, it does not move parsing pointer
 * 
 */
class LookBackValidator : public Core::UnaryValidator {
 public:
  explicit LookBackValidator(Core::StringValidator* op)
        : Core::UnaryValidator(op) {}
  virtual bool Check(Core::ContextInterface* context) const;
};

/**
 * @brief Used in searches ... skips successive character till 
 * the input rule matches
 * 
 */
class UntilValidator : public Core::UnaryValidator {
 public:
  explicit UntilValidator(Core::StringValidator* op)
        : UnaryValidator(op) {}

  virtual bool Check(Core::ContextInterface* context) const;
};
/**
 * @brief Repeat the input rule for a number of times .. and validate
 * if it is matched for a specified times 
 * 
 */
class RepeatValidator : public Core::UnaryValidator {
  unsigned int _maxIter;
  unsigned int _minIter;
 public:
  explicit RepeatValidator(Core::StringValidator* op)
    : Core::UnaryValidator(op)
    , _maxIter(-1)
    , _minIter(0)
  {}

  RepeatValidator(Core::StringValidator* op, int max)
    : Core::UnaryValidator(op)
    , _maxIter(max)
    , _minIter(0)
  {}

  RepeatValidator(Core::StringValidator* op, int min, int max)
    : Core::UnaryValidator(op)
    , _maxIter(max)
    , _minIter(min)
  {}

  virtual bool Check(Core::ContextInterface* context) const;
};

/**
 * @brief if the input rule matches ... the matched token is added 
 * to the matches table
 * 
 */
class ExtractValidator : public Core::UnaryValidator {
  const char* _key;
 public:
  explicit ExtractValidator(Core::StringValidator* op)
    : Core::UnaryValidator(op)
    , _key("<UNNAMED>") {}

  ExtractValidator(Core::StringValidator* op, const char* key)
    : Core::UnaryValidator(op)
    , _key(key) {}


  virtual bool Check(Core::ContextInterface* context) const;
};


/**
 * @brief used in circular recursive rules .. it points to uninitialized rule
 * 
 */
class RefValidator : public Core::NormalValidator {
  Core::StringValidator* _validator;
  const Core::Rule* _rule;
 public:
  RefValidator() : _rule(NULL), _validator(NULL) {}
  explicit RefValidator(const Core::Rule& r) : _rule(&r)
          , _validator(NULL) {}

  virtual bool Check(Core::ContextInterface* context) const;
  DLL_PUBLIC void Set(const Core::Rule& rule);
};
}  // namespace Manipulators

/**
 * @brief this namespace includes all classes that relates to parsing status,
 * all classes in this namespace do not move cursor position 
 * 
 */
namespace StateKeepers {
  /**
   * @brief Alter the case sensitivity modes in the parsing process
   * 
   */
class CaseModifier : public Core::NormalValidator {
  bool _caseSensitive;

 public:
  explicit CaseModifier(bool cs) : _caseSensitive(cs) {}
  virtual bool Check(Core::ContextInterface* context) const;
};

/**
 * @brief Validator Set context variable 
 * 
 */
class SetFlagModifier : public Core::NormalValidator {
  const char* _flag;
  const char* _value;

 public:
  SetFlagModifier(const char* flag, const char* val)
    : _flag(flag)
    , _value(val) {}

  explicit SetFlagModifier(const char* flag)
    : _flag(flag)
    , _value("1") {}

  virtual bool Check(Core::ContextInterface* context) const;
};

/**
 * @brief Delete (unset) context variable
 * 
 */
class DelFlagModifier : public Core::NormalValidator {
  const char* _flag;

 public:
  explicit DelFlagModifier(const char* flag)
    : _flag(flag) {}


  virtual bool Check(Core::ContextInterface* context) const;
};

/**
 * @brief checks if context variable equals value
 * 
 */
class IfValidator : public Core::NormalValidator {
  const char* _flag;
  const char* _value;

 public:
  IfValidator(const char* flag, const char* val)
    : _flag(flag)
    , _value(val)
  {}

  explicit IfValidator(const char* flag)
    : _flag(flag)
    , _value("1")
  {}

  virtual bool Check(Core::ContextInterface* context) const;
};

/**
 * @brief checks if named match is already matched
 * 
 */
class IfMatchedValidator : public Core::NormalValidator {
  const char* _key;
  const unsigned long _min;
  const unsigned long _max;


 public:
  IfMatchedValidator(const char* key
      , const unsigned long min
      , const unsigned long max)
    : _key(key)
    , _min(min)
    , _max(max)
  {}

  IfMatchedValidator(const char* key, const unsigned long max)
    : _key(key)
    , _min(1)
    , _max(max)
  {}

  explicit IfMatchedValidator(const char* key)
    : _key(key)
    , _min(1)
    , _max(-1)
  {}

  virtual bool Check(Core::ContextInterface* context) const;
};

}  // namespace StateKeepers


namespace Core {
  /**
   * @brief Rule is handler of a parsing tree and handles the reference couting
   * of its elements 
   * 
   */
class Rule {
  Core::StringValidator* _strValid;

 public:
 /**
  * @brief equal operator 
  * 
  * @param other RHS Rule 
  * @return DLL_PUBLIC LHS Rule
  */
  DLL_PUBLIC Rule operator=(const Rule& other);
  Rule() : _strValid(
    new Manipulators::NotValidator(
      new Primitives::AnyValidator())) {}

  Rule(StringValidator* obj) {
    _strValid = obj;
    _strValid->AddReference();
  }


  Rule(const Rule& other)  {
    _strValid = other._strValid;
    if (_strValid)
      _strValid->AddReference();
  }


  StringValidator* Get() const {
    return _strValid;
  }


  bool Check(Core::ContextInterface* context) const {
    return _strValid->Check(context);
  }

  virtual ~Rule() {
    _strValid->Release();
  }
};
}  // namespace Core

/**
 * @brief  the operators that construct Stringozzi Rule
 * 
 */
namespace Operators {
using namespace Core;
/** 
 * @brief A then B 
 */
DLL_PUBLIC Rule Sequence(const Rule& first, const Rule& second);
/**
 * @brief A then B 
 * 
 * @param first 
 * @param second 
 * @return DLL_PUBLIC 
 */
DLL_PUBLIC Rule operator > (const Rule& first, const Rule& second);
/**
 * @brief A and B
 * 
 * @param first 
 * @param second 
 * @return DLL_PUBLIC 
 */
DLL_PUBLIC Rule operator & (const Rule& first, const Rule& second);
/**
 * @brief A or B
 * 
 * @param first 
 * @param second 
 * @return DLL_PUBLIC 
 */
DLL_PUBLIC Rule operator | (const Rule& first, const Rule& second);
/**
 * @brief  A or B with best relevance
 * 
 * @param first 
 * @param second 
 * @return DLL_PUBLIC 
 */
DLL_PUBLIC Rule operator ||(const Rule &first, const Rule &second);

/**
 * @brief Is Exact char c 
 * 
 * @tparam __CHARTYPE 
 * @param chr 
 * @return Rule 
 */
template<typename __CHARTYPE>
Rule Is(const __CHARTYPE chr) {
  return new Primitives::IsValidator<__CHARTYPE>(chr);
}

/**
 * @brief Not A
 * 
 * @param rule 
 * @return DLL_PUBLIC 
 */
DLL_PUBLIC Rule Not(const Rule& rule);
/**
 * @brief Not A
 * 
 * @param rule 
 * @return DLL_PUBLIC 
 */
DLL_PUBLIC Rule operator! (const Rule& rule);

/**
 * @brief (0->1) * A
 * 
 * @param rule 
 * @return DLL_PUBLIC 
 */
DLL_PUBLIC Rule ZeroOrOne(const Rule& rule);

/**
 * @brief A valid either matched or not
 * 
 * @param rule 
 * @return DLL_PUBLIC 
 */
DLL_PUBLIC Rule Optional(const Rule& rule);

/**
 * @brief (0->num)* A 
 * 
 * @param rule 
 * @param num 
 * @return DLL_PUBLIC 
 */
DLL_PUBLIC Rule ZeroOrMore(const Rule& rule, const unsigned int num);
/**
 * @brief (0->INFINITY) * A 
 * 
 * @param rule 
 * @return DLL_PUBLIC 
 */
DLL_PUBLIC Rule ZeroOrMore(const Rule& rule);

/**
 * @brief Exact instances of  A
 * 
 * @param rule 
 * @return DLL_PUBLIC 
 */
DLL_PUBLIC Rule operator* (const Rule& rule);

/**
 * @brief n times of A 
 * 
 * @param rule 
 * @param num 
 * @return DLL_PUBLIC 
 */
DLL_PUBLIC Rule Times(const Rule& rule, const unsigned int num);

/**
 * @brief n times of A 
 * 
 * @param num 
 * @param rule 
 * @return DLL_PUBLIC 
 */
DLL_PUBLIC Rule operator* (const unsigned int num, const Rule& rule);

/**
 * @brief n times of A 
 * 
 * @param rule 
 * @param num 
 * @return DLL_PUBLIC 
 */
DLL_PUBLIC Rule operator* (const Rule& rule, const unsigned int num);

/**
 * @brief (1->num) times of A 
 * 
 * @param rule 
 * @param num 
 * @return DLL_PUBLIC 
 */
DLL_PUBLIC Rule OneOrMore(const Rule& rule, const unsigned int num);

/**
 * @brief (1->num) times of A 
 * 
 * @param rule 
 * @return DLL_PUBLIC 
 */
DLL_PUBLIC Rule OneOrMore(const Rule& rule);

/**
 * @brief (1->num) times of A 
 * 
 * @param num 
 * @param rule 
 * @return DLL_PUBLIC 
 */
DLL_PUBLIC Rule operator+ (const unsigned int num, const Rule& rule);

/**
 * @brief (1->num) times of A
 * 
 * @param rule 
 * @param num 
 * @return DLL_PUBLIC 
 */
DLL_PUBLIC Rule operator+ (const Rule& rule, const unsigned int num);

/**
 * @brief (min->max) times of A 
 * 
 * @param rng 
 * @param rule 
 * @return DLL_PUBLIC 
 */
DLL_PUBLIC Rule operator* (const Utils::Range& rng, const Rule& rule);

/**
 * @brief (min->max) times of A 
 * 
 * @param rule 
 * @param rng 
 * @return DLL_PUBLIC 
 */
DLL_PUBLIC Rule operator* (const Rule& rule, const Utils::Range& rng);

/**
 * @brief (1->INFINITY) times of A 
 * 
 * @param rule 
 * @return DLL_PUBLIC 
 */
DLL_PUBLIC Rule operator+ (const Rule& rule);

/**
 * @brief char between (min < c < max)
 * 
 * @tparam __CHARTYPE 
 * @param min 
 * @param max 
 * @return Rule 
 */
template<typename __CHARTYPE>
Rule Between(const __CHARTYPE min, const __CHARTYPE max) {
  return new Primitives::BetweenValidator<__CHARTYPE>(min, max);
}

/**
 * @brief char between (min < c < max)
 * 
 * @tparam __CHARTYPE 
 * @param range 
 * @return Rule 
 */
template<typename __CHARTYPE>
Rule Between(const __CHARTYPE* range) {
  return new Primitives::BetweenValidator<__CHARTYPE>(range);
}

/**
 * @brief A in { a,b,c...} 
 * 
 * @tparam __CHARTYPE 
 * @param set 
 * @return Rule 
 */
template<typename __CHARTYPE>
Rule In(const __CHARTYPE* set) {
  return new Primitives::InValidator<__CHARTYPE>(set);
}


/**
 * @brief Exact A == "phrase"
 * 
 * @tparam __CHARTYPE 
 * @param phrase 
 * @return Rule 
 */
template<typename __CHARTYPE>
Rule Is(const __CHARTYPE* phrase) {
  return new Primitives::ExactValidator<__CHARTYPE>(phrase);
}

/**
 * @brief Peek next rule
 * 
 * @param rule 
 * @return DLL_PUBLIC 
 */
DLL_PUBLIC Rule LookAhead(const Rule& rule);

/**
 * @brief Peek previous rule
 * 
 * @param rule 
 * @return DLL_PUBLIC 
 */
DLL_PUBLIC Rule LookBack(const Rule& rule);

/**
 * @brief Optional A
 * 
 * @param rule 
 * @return DLL_PUBLIC 
 */
DLL_PUBLIC Rule operator~ (const Rule& rule);
/**
 * @brief Any
 * 
 */
DLL_PUBLIC_VAR extern const Rule Any;

/**
 * @brief 0->9 
 * 
 */
DLL_PUBLIC_VAR extern const Rule Digit;

/**
 * @brief a->z
 * 
 */
DLL_PUBLIC_VAR extern const Rule SmallAlphabet;

/**
 * @brief A->Z
 * 
 */
DLL_PUBLIC_VAR extern const Rule CapitalAlphabet;

/**
 * @brief a->z or A->Z
 * 
 */
DLL_PUBLIC_VAR extern const Rule Alphabet;

/**
 * @brief a->z or A->Z or 0->9
 * 
 */
DLL_PUBLIC_VAR extern const Rule Alphanumeric;

/**
 * @brief End of text : lorem ipsum ... EOT.
 * 
 */
DLL_PUBLIC_VAR extern const Rule End;

/**
 * @brief Beginning of Text : BOT ...lorem ipsum
 * 
 */
DLL_PUBLIC_VAR extern const Rule Beginning;

/**
 * @brief Not Alphanumeric
 * 
 */
DLL_PUBLIC_VAR extern const Rule Symbol;

/**
 * @brief 0->9 or A-F 
 * 
 */
DLL_PUBLIC_VAR extern const Rule Hex;

/**
 * @brief 0->7
 * 
 */
DLL_PUBLIC_VAR extern const Rule Octet;

/**
 * @brief Beginning of line .....\n 
 * 
 */
DLL_PUBLIC_VAR extern const Rule BeginningOfLine;

/**
 * @brief CRLF or LF or CR 
 * 
 */
DLL_PUBLIC_VAR extern const Rule EndOfLine;

/**
 * @brief SPACE or TAB or NEWLINE 
 * 
 */
DLL_PUBLIC_VAR extern const Rule WhiteSpace;

/**
 * @brief One or more Whitespaces 
 * 
 */
DLL_PUBLIC_VAR extern const Rule WhiteSpaces;

/**
 * @brief 0 or 1 !! 
 * 
 */
DLL_PUBLIC_VAR extern const Rule Binary;
/**
 * @brief last letter in word (word<<)
 * 
 */
DLL_PUBLIC_VAR extern const Rule WordEnd;

/**
 * @brief first lteer in word (>>word) 
 * 
 */
DLL_PUBLIC_VAR extern const Rule WordStart;

/**
 * @brief Natural: {1, 2, 3, 4,...} 
 * 
 */
DLL_PUBLIC_VAR extern const Rule Natural;

/**
 * @brief Integer {..,-2,-1,0,1,2,...} 
 * 
 */
DLL_PUBLIC_VAR extern const Rule Integer;

/**
 * @brief Rational Number {-2.0,... ,1.0 ,1.5,1.53} 
 * 
 */
DLL_PUBLIC_VAR extern const Rule Rational;

/**
 * @brief Scenific Number like +2.034e+12
 * 
 */
DLL_PUBLIC_VAR extern const Rule Scientific;

/**
 * @brief IP v4 Address 11.22.33.44 
 * 
 */
DLL_PUBLIC_VAR extern const Rule IPv4;

/**
 * @brief IP v6 Address  
 * 
 */
DLL_PUBLIC_VAR extern const Rule IPv6;

/**
 * @brief Server host
 * 
 */
DLL_PUBLIC_VAR extern const Rule Host;

/**
 * @brief To case sensitive
 * 
 */
DLL_PUBLIC_VAR extern const Rule CaseSensitive;

/**
 * @brief To case insensitive 
 * 
 */
DLL_PUBLIC_VAR extern const Rule CaseInsensitive;

/**
 * @brief In chain like A..B..C 
 * 
 */
DLL_PUBLIC_VAR extern const Rule InChain;


/**
 * @brief Not in set
 * 
 * @tparam __CHARTYPE 
 * @param set 
 * @return Rule 
 */
template<typename __CHARTYPE>
Rule Out(const __CHARTYPE* set) {
  return Any & Not(In(set));
}

/**
 * @brief skip letters till the rule applies 
 * 
 * @param r 
 * @return DLL_PUBLIC 
 */
DLL_PUBLIC Rule SkipTo(const Rule& r);

/**
 * @brief skip fixed number of letters 
 * 
 * @param count 
 * @return DLL_PUBLIC 
 */
DLL_PUBLIC Rule SkipTo(const unsigned long count);

/**
 * @brief Stop when rule applies ..return false if it does not exist 
 * 
 * @param r 
 * @return DLL_PUBLIC 
 */
DLL_PUBLIC Rule Until(const Rule& r);

/**
 * @brief extract to matach named key 
 * 
 * @param r 
 * @param key 
 * @return DLL_PUBLIC 
 */
DLL_PUBLIC Rule Extract(const Rule& r, const char* key);

/**
 * @brief extract but unnamed 
 * 
 * @param r 
 * @return DLL_PUBLIC 
 */
DLL_PUBLIC Rule Extract(const Rule& r);

/**
 * @brief extract to matach named key 
 * 
 * @param r 
 * @param key 
 * @return DLL_PUBLIC 
 */
DLL_PUBLIC Rule operator >> (const Rule& r, const char* key);

/**
 * @brief the rule is enclosed between quote symbol 
 * 
 * @param r 
 * @param quote 
 * @return DLL_PUBLIC 
 */
DLL_PUBLIC Rule Enclosed(const Rule& r, const char* quote);

/**
 * @brief enclosed between open/close marks 
 * 
 * @param r 
 * @param open 
 * @param close 
 * @return DLL_PUBLIC 
 */
DLL_PUBLIC Rule Enclosed(const Rule& r, const char* open
                            , const char* close);

/**
 * @brief Set context variable with name 
 * 
 * @param f 
 * @return DLL_PUBLIC 
 */
DLL_PUBLIC Rule Set(const char* f);

/**
 * @brief set context variable with name and value 
 * 
 * @param f 
 * @param v 
 * @return DLL_PUBLIC 
 */
DLL_PUBLIC Rule Set(const char* f, const char* v);

/**
 * @brief Delete/unset context variable 
 * 
 * @param f 
 * @return DLL_PUBLIC 
 */
DLL_PUBLIC Rule Del(const char* f);

/**
 * @brief if context variable equals "1" 
 * 
 * @param f 
 * @return DLL_PUBLIC 
 */
DLL_PUBLIC Rule If(const char* f);

/**
 * @brief if context variable equals value 
 * 
 * @param f 
 * @param v 
 * @return DLL_PUBLIC 
 */
DLL_PUBLIC Rule If(const char* f, const char* v);

/**
 * @brief reference the rule recursively 
 * 
 * @param r 
 * @return DLL_PUBLIC 
 */
DLL_PUBLIC Rule Ref(const Rule& r);

/**
 * @brief if named rule already matched at least once 
 * 
 * @param key 
 * @return DLL_PUBLIC 
 */
DLL_PUBLIC Rule IfMatched(const char* key);

/**
 * @brief if named rule already matched at least number of times
 * 
 * @param key 
 * @param min 
 * @return DLL_PUBLIC 
 */
DLL_PUBLIC Rule IfMatched(const char* key, unsigned long min);

/**
 * @brief if named rule already matched number of times between
 *  min and max 
 * 
 * @param key 
 * @param min 
 * @param max 
 * @return DLL_PUBLIC 
 */
DLL_PUBLIC Rule IfMatched(const char* key
      , unsigned long min
      , unsigned long max);
}  // namespace Operators

/**
 * @brief The main processor class, it is similar to regex
 * 
 * @tparam __CHARTYPE 
 */
template <typename __CHARTYPE>
class Stringozzi {
  typedef basic_string<__CHARTYPE> STRING;
  Core::Rule _rule;

 public:
 /**
  * @brief Construct a new Stringozzi object
  * 
  * @param r 
  */
  explicit Stringozzi(const Core::Rule& r) : _rule(r) {}

 /**
 * @brief direct testing the string versus the rule ..
 *  no search conducted 
 * 
 * @param str 
 * @param flags 
 * @return true 
 * @return false 
 */
  bool Test(const __CHARTYPE* str, unsigned long flags = 0) {
    Core::Context<__CHARTYPE> context(str, flags);
    return _rule.Check(&context);
  }

  bool FastMatch(const __CHARTYPE* str, unsigned long flags = 0) {
    Utils::Matches<__CHARTYPE> _matches;
    Core::Context<__CHARTYPE> context(str, flags, &_matches);
    return _rule.Check(&context);
  }

  /**
   * @brief Search the text for the specified rule 
   * 
   * @param str 
   * @param flags 
   * @return true if found
   * @return false otherwise
   */
  bool Search(const __CHARTYPE* str, unsigned long flags = 0) {
    Core::Context<__CHARTYPE> context(str, flags);
    Core::Rule rule = Operators::Until(_rule);
    return rule.Check(&context);
  }


  /**
   * @brief Search the text for the specified rule and return 
   * string pointer of the first match
   * 
   * @param str 
   * @param flags 
   * @return const __CHARTYPE* 
   */
  const __CHARTYPE* SearchAndGetPtr(const __CHARTYPE* str
          , unsigned long flags = 0) {
    Utils::Matches<__CHARTYPE> matches;
    Core::Context<__CHARTYPE> context(str, flags);
    Core::Rule r = Operators::Until(_rule);
    if (r.Check(&context))
      return static_cast<const __CHARTYPE*>(context.GetPosition());
    else
      return NULL;
  }

/**
 * @brief Search the text for the specified rule and return 
 * index of the first match
 * 
 * 
 * @param str 
 * @param flags 
 * @return size_t 
 */
  size_t SearchAndGetIndex(const __CHARTYPE* str
            , unsigned long flags = 0) {
    Utils::Matches<__CHARTYPE> _matches;
    const Core::Context<__CHARTYPE> context(str, flags, &_matches);
    Core::Rule r = Operators::Until(_rule);
    if (r.Check(&context))
      return static_cast<const __CHARTYPE*>(context->GetPosition()) - str;
    else
      return -1;
  }

/**
 * @brief search text for the rule, if found .. it returns all matches  
 * 
 * @param str 
 * @param matches 
 * @param flags 
 * @return true 
 * @return false 
 */
  bool Match(const __CHARTYPE* str
          , Utils::Matches<__CHARTYPE>& matches
          , unsigned long flags = 0) {
    flags = flags | SPEG_MATCHNAMED | SPEG_MATCHUNNAMED;
    const __CHARTYPE* ptr = SearchAndGetPtr(str, flags);
    if (!ptr)
      return false;

    Core::Context<__CHARTYPE> context(ptr, flags);
    bool ret =  _rule.Check(&context);
    matches = context.Matches();
    return ret;
  }


/**
 * @brief Search the text and replace the matched token with 
 * the specified string
 * 
 * @param str 
 * @param rep 
 * @param flags 
 * @param count 
 * @return STRING 
 */
  STRING Replace(const __CHARTYPE* str, const __CHARTYPE* rep
    , unsigned long flags = 0
    , unsigned int count = 1 ) {
    Core::Context<__CHARTYPE> context(str, flags);
    Core::Position last_start = str;
    Core::Rule r = Operators::Until(_rule);
    STRING s;

    for (unsigned int i = 0; i < count && r.Check(&context); i++) {
      Core::Position start = context.GetPosition();
      _rule.Check(&context);
      Core::Position end = context.GetPosition();
      s.append(static_cast<const __CHARTYPE*>(last_start)
            , static_cast<const __CHARTYPE*>(start)
            - static_cast<const __CHARTYPE*>(last_start));
      s.append(rep);
      last_start = static_cast<const __CHARTYPE*>(end);
    }
    s.append(static_cast<const __CHARTYPE*>(last_start));
    return s;
  }

/**
 * @brief Search the text and replace the matched token with 
 * the specified string in place
 * 
 * @param str 
 * @param size 
 * @param rep 
 * @param count 
 * @param casesensitive 
 */
  void Replace(__CHARTYPE* str, unsigned long size
        , const __CHARTYPE* rep
        , unsigned int count = 1
        , bool casesensitive = true) {
  STRING ret = Replace(str, rep, count, casesensitive);
  strncpy(str, ret.c_str(), size);
  }

/**
 * @brief Split the string base on separator specified in the rule  
 * 
 * @param str 
 * @param vector 
 * @param flags 
 * @param dropEmpty 
 * @param count 
 * @param caseSensitive 
 * @return true 
 * @return false 
 */
  bool Split(__CHARTYPE* str, vector<STRING>& vector
    , unsigned long flags = 0
    , bool dropEmpty = true
    , unsigned int count = 1) {
    Core::Context<__CHARTYPE> context(str, flags);
    Core::Position last_start = str;
    Core::Rule rule = Operators::Until(_rule);
    STRING result;
    for (int i = 0; i < count && rule.Check(context); i++) {
      Core::Position start = context.GetPosition();
      _rule.Check(context);
      Core::Position end = context.GetPosition();
      result.assign(static_cast<const __CHARTYPE*>(last_start)
            , static_cast<const __CHARTYPE*>(start)
            - static_cast<const __CHARTYPE*>(last_start));
      if (!result.empty() || !dropEmpty)
        vector.push_back(result);
      last_start = end;
    }
    result.assign(static_cast<const __CHARTYPE*>(last_start));
    vector.push_back(result);
    return true;
  }

  vector<STRING> Split(__CHARTYPE* str) {
    vector<string> vec;
    SplitAll(str, vec);
    return vec;
  }
};

typedef Stringozzi<char> StringozziA;
typedef Stringozzi<wchar_t> StringozziW;
typedef Stringozzi<char8_t> StringozziU8;
#ifdef  CX11_SUPPORTED
typedef Stringozzi<char16_t> StringozziU16;
typedef Stringozzi<char32_t> StringozziU32;
#endif



namespace Utils {
/**
 * @brief placehoder object to inject rule into recursive rule
 * 
 */
class PlaceHolder {
  Manipulators::RefValidator* _ref;

 public:
  PlaceHolder() :_ref(NULL) {}
  void Set(Manipulators::RefValidator* ref) {
    _ref = ref;
  }


  void Inject(const Core::Rule& r) {
    _ref->Set(r);
  }
};
}  // namespace Utils

namespace Operators {
  DLL_PUBLIC Core::Rule Ref(Utils::PlaceHolder& ph);
}
/**
 * @brief Direct parsing actions functions
 * 
 */
namespace Actions {
  /**
   * @brief Proxy to Stringozzi.Test
   * 
   * @tparam __CHARTYPE 
   * @param rule 
   * @param text 
   * @param flags 
   * @return true 
   * @return false 
   */
template<typename __CHARTYPE>
bool Test(const Core::Rule& rule, const __CHARTYPE* text
            , unsigned long flags = 0) {
  return Stringozzi<__CHARTYPE>(rule).Test(text, flags);
}

/**
 * @brief Proxy to Stringozzi.FastMatch
 * 
 * @tparam __CHARTYPE 
 * @param rule 
 * @param str 
 * @param flags 
 * @return true 
 * @return false 
 */
template<typename __CHARTYPE>
bool FastMatch(const Core::Rule& rule, const __CHARTYPE* str
            , unsigned long flags = 0) {
  return Stringozzi<__CHARTYPE>(rule).FastMatch(str, flags);
}

/**
 * @brief Proxy to Stringozzi.Search
 * 
 * @tparam __CHARTYPE 
 * @param rule 
 * @param str 
 * @param flags 
 * @return true 
 * @return false 
 */
template<typename __CHARTYPE>
bool Search(const Core::Rule& rule, const __CHARTYPE* str
            , unsigned long flags = 0) {
  return Stringozzi<__CHARTYPE>(rule).Search(str, flags);
}


/**
 * @brief Proxy to StringozziA::SearchAndGetPtr
 * 
 * @tparam __CHARTYPE 
 * @param _rule 
 * @param str 
 * @param flags 
 * @return const __CHARTYPE* 
 */
template<typename __CHARTYPE>
const __CHARTYPE* SearchAndGetPtr(const Core::Rule& _rule
            , const __CHARTYPE* str, unsigned long flags = 0) {
  return Stringozzi<__CHARTYPE>(_rule).SearchAndGetPtr(str, flags);
}

/**
 * @brief Proxy to Stringozzi:: SearchAndGetIndex
 * 
 * @tparam __CHARTYPE 
 * @param _rule 
 * @param str 
 * @param flags 
 * @return size_t 
 */
template<typename __CHARTYPE>
size_t SearchAndGetIndex(const Core::Rule& _rule
            , const __CHARTYPE* str, unsigned long flags = 0) {
  return Stringozzi<__CHARTYPE>(_rule).SearchAndGetIndex(str, flags);
}

template<typename __CHARTYPE>
bool Match(const Core::Rule& rule, const __CHARTYPE* str
            , Utils::Matches<__CHARTYPE>& matches
            , unsigned long flags = 0) {
  return Stringozzi<__CHARTYPE>(rule).Match(str, matches, flags);
}

template<typename __CHARTYPE>
basic_string<__CHARTYPE> Replace(const Core::Rule& _rule
            , __CHARTYPE* str, const __CHARTYPE* rep, unsigned long flags = 0
  , unsigned int count = 1) {
  return Stringozzi<__CHARTYPE>(_rule).Replace(str, rep, flags, count);
}

template<typename __CHARTYPE>
bool Split(const Core::Rule& _rule, __CHARTYPE* str
  , vector<basic_string<__CHARTYPE> >& vec
  , unsigned long flags = 0
  , bool dropEmpty = true
  , unsigned int count = 1) {
  return Stringozzi<__CHARTYPE>(_rule).Split(str, vec, flags, dropEmpty, count);
}

}  // namespace Actions
}  // namespace SPEG

#endif  // INCLUDE_STRINGOZZI_H_
