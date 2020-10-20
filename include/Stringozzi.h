/*
MIT License

Copyright (c) 2018 Osama Salem

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


#ifndef __STRINGOZZI_H__
#define __STRINGOZZI_H__


#include <string>
#include <map>
#include <vector>
#include <stack>

#ifdef __GNUC__  
#include <string.h>
#include<cstring>
#endif

//TODO: Greedy AND and OR validators

using namespace std;

#define CPPVER_1997 199711L
#define CPPVER_2003 CPPVER_1997
#define CPPVER_2011 201103L
#define CPPVER_2014 201402L
#define CPPVER_2017 201703L

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

#define MAXIMUM(a,b) 	( ( (a) > (b) ) ? (a) : (b))



#ifndef MAX_ITER	
#define MAX_ITER	5000
#endif

#define OBJECT_VALIDITY_MAGICNUMBER 0x900df00d
#define FLAG_CASEINSENSITIVE	(1 << 0)
#define FLAG_MATCHNAMED			(1 << 1)
#define FLAG_MATCHUNNAMED		(1 << 2)
#define FLAG_IGNORESPACES		(1 << 3)

#define NORMALIZE(__X) ( ((__X)>0)?(1):( ( (__X)<0) ?(-1):0))
#define	MATCHES_TOKEN "<MATCHES>"


namespace SPEG
{
	typedef unsigned long SChar;

	namespace Core
	{
		class Rule;
		typedef const void* Position;

	}

	namespace Utils
	{
		unsigned long CharToLower(unsigned long);
		unsigned long UTF8ToUTF32(const char*);
		unsigned long UTF8ToUTF32Length(const char*);



		template <typename __T>
		inline void Increment(const __T** pointer) {
			(*pointer)++;
		}

		template<>
		inline void Increment<char>(const char** pointer) {
			(*pointer) += UTF8ToUTF32Length(*pointer);
		}

		template <typename __T>
		void Decrement(const __T** pointer) {
			(*pointer)--;
		}

		template<>
		void Decrement<char>(const char** pointer);

		template <typename __T>
		inline unsigned int GetChar(const __T* pointer) {
			return *pointer;
		}

		template<>
		inline unsigned int GetChar<char>(const char* pointer) {
			return UTF8ToUTF32(pointer);
		}

		void SafeIncrement(unsigned long*);
		void SafeDecrement(unsigned long*);

		template<typename __CHARTYPE>
		class Matches
		{
		public:

			template<typename __T>
			struct Match
			{
				Match(Core::Position start, Core::Position end) : Start(start), End(end), Index(-1) {}
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

			Matches() {}

			inline void Add(const char* key, const Core::Position start, const Core::Position end) {
				_matchesMap[key].push_back(MATCH(start, end));
			}


			const __CHARTYPE* Get(const char* key, const unsigned int index) {
				typename MAP::iterator it = _matchesMap.find(key);
				if (it == _matchesMap.end())
					return NULL;


				VECTOR& v = it->second;
				if (index >= v.size())
					return NULL;


				MATCH& m = v.at(index);
				if (m.Index == -1)
				{
					unsigned int size = (__CHARTYPE*)m.End - (__CHARTYPE*)m.Start;
					_matchesStrings.push_back(STRING((__CHARTYPE*)m.Start, size));
					m.Index = _matchesStrings.size() - 1;
				}

				return _matchesStrings[m.Index].c_str();
			}

			unsigned int NumberOfMatches()
			{
				return _matchesMap.size();
			}

			int NumberOfMatches(const char* key)
			{
				typename MAP::iterator it = _matchesMap.find(key);
				if (it == _matchesMap.end())
					return 0;

				return it->second.size();
			}

			void Clear() {
				_matchesMap.clear();
				_matchesStrings.clear();
			}
		};

		typedef Matches<char> MatchesA;
		typedef Matches<wchar_t> MatchesW;

		struct Range {
			Range(const unsigned int min, const unsigned int max) : MIN(min), MAX(max) {}
			const unsigned int MIN;
			const unsigned int MAX;
		};


		class Flags
		{
			typedef unsigned long FlagSet;

		public:
			FlagSet _flags;

			Flags(unsigned long init) : _flags(init) {}

			inline void SetFlag(unsigned long flag) {
				_flags |= flag;
			}

			inline void ClearFlag(unsigned long flag) {
				_flags &= ~flag;
			}

			inline void ToggleFlag(unsigned long flag) {
				_flags ^= flag;
			}


			inline void SetFlag(unsigned long flag, bool value) {
				if (value)
					SetFlag(flag);
				else
					ClearFlag(flag);
			}

			inline void SetAllFlags(unsigned long flags) {
				_flags = flags;
			}



			inline bool IsFlagSet(unsigned long flag) {
				return ((_flags & (flag)) != 0);
			}


		};

	}

	namespace Core
	{

		class ContextInterface {
		public:
			virtual bool Forward() = 0;
			virtual bool Backward() = 0;
			virtual bool BOT() = 0;
			virtual bool EOT() = 0;
			virtual Utils::Flags& Flags() = 0;
			virtual const int Compare(SChar c) = 0;
			virtual const SChar Get() = 0;
			virtual const SChar Get(SChar c) = 0;
			virtual Position GetPosition() = 0;
			virtual void SetPosition(Position pos) = 0;
			virtual void AddMatch(Position start) = 0;
			virtual void AddMatch(const char* k, Position start, Position end) = 0;
			virtual unsigned long NumberOfMatches(const char* k) = 0;
			virtual void SetVar(const char* vname, const char* vval) = 0;
			virtual bool GetVar(const char* vname, char const ** vval) = 0;
			virtual bool DelVar(const char* vname) = 0;
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

			inline const SChar _Get(SChar c) {
				if (_flags.IsFlagSet(FLAG_CASEINSENSITIVE))
					return Utils::CharToLower(c);
				return c;
			}


		public:

			virtual const SChar Get() {
				unsigned long chr = Utils::GetChar(_pointer);
				return _Get(chr);
			}

			virtual const SChar Get(SChar chr) {
				return _Get(chr);
			}


			Utils::Matches<__CHARTYPE>  Matches() {
				return _matches;
			}

			explicit Context(const __CHARTYPE* str, unsigned long  flags)
				: _pointer(str)
				, _flags(flags)
			{
				AdjustPosition();
				_string = _pointer;
			}


			virtual Position AdjustPosition() {
				if (_flags.IsFlagSet(FLAG_IGNORESPACES))
				{
					while (Compare(' ') == 0)
					{
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

			virtual unsigned long NumberOfMatches(const char* key) {
				return _matches.NumberOfMatches(key) ;
			}

			inline Position GetPosition() {
				return _pointer;
			}

			inline void SetPosition(Position position) {
				_pointer = (__CHARTYPE*)position;
			}

			inline void AddMatch(const char* key, Position start, Position end) {
				if (_flags.IsFlagSet(FLAG_MATCHNAMED))
				{
					_matches.Add(key, start, end);
				}
			}
			
			inline void AddMatch(Position start) {
				if (_flags.IsFlagSet(FLAG_MATCHUNNAMED))
				{
					Core::Position end = GetPosition();
					if (end > start)
					{
						_matches.Add(MATCHES_TOKEN, start, end);
					}
				}
			}

			inline void SetVar(const char* vname, const char* vval) {
				_vars[vname] = vval;
			}

			inline bool GetVar(const char* vname, char const ** vval) {
				if (_vars.find(vname) != _vars.end())
				{
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

		class StringValidator {
		public:
			virtual bool Check(ContextInterface& context) const = 0;
			virtual ~StringValidator() {};
			virtual void Release() = 0;
			virtual void AddReference() = 0;
			virtual void Dispose() = 0;
		};

		class NormalValidator : public StringValidator {
			unsigned long _referenceCount;

		public:
			NormalValidator() : _referenceCount(1) {}

			virtual void AddReference();
			virtual void Release();
			virtual void Dispose() {}
		};

		class UnaryValidator : public NormalValidator {
		protected:
			StringValidator* const Operand;
		public:
			explicit UnaryValidator(StringValidator* op) : Operand(op) {
				Operand->AddReference();
			}
			virtual void Dispose();
		};


		class BinaryValidator : public NormalValidator {
		protected:
			StringValidator* const FirstOperand;
			StringValidator* const SecondOperand;

		public:
			BinaryValidator(StringValidator* op1, StringValidator* op2)
				: FirstOperand(op1), SecondOperand(op2) {
				op1->AddReference();
				op2->AddReference();
			}

			virtual void Dispose();
		};
	}

	namespace Primitives
	{

		template<typename __CHARTYPE>
		class IsValidator : public Core::NormalValidator {
			const __CHARTYPE _character;
		public:
			explicit IsValidator(__CHARTYPE chr) : _character(chr) {}
			virtual bool Check(Core::ContextInterface& context)const {
				context.AdjustPosition();
				Core::Position start = context.GetPosition();
				if (!context.Compare(_character))
				{
					context.Forward();
					context.AddMatch(start);
					return true;
				}
				return false;
			}
		};

		typedef IsValidator<char>  IsValidatorA;
		typedef IsValidator<wchar_t>  IsValidatorW;


		template<typename __CHARTYPE>
		class InValidator : public Core::NormalValidator {
			const __CHARTYPE* _set;
		public:
			explicit InValidator(const __CHARTYPE* set) : _set(set) {}
			virtual bool Check(Core::ContextInterface& context)const {
				context.AdjustPosition();
				Core::Position start = context.GetPosition();
				const __CHARTYPE* setPtr = _set;
				while (*setPtr)
				{
					SChar chr = Utils::GetChar(setPtr);
					if (!context.Compare(chr))
					{
						context.Forward();
						context.AddMatch(start);
						return true;
					}
					Utils::Increment(&setPtr);
				}
				return false;
			}
		};

		typedef InValidator<char>  InValidatorA;
		typedef InValidator<wchar_t>  InValidatorW;
		typedef InValidator<char16_t>  InValidatorU16;
		typedef InValidator<char32_t>  InValidatorU32;

		class InChainValidator : public Core::NormalValidator {
		public:
			InChainValidator() {}
			virtual bool Check(Core::ContextInterface& context)const;
		};


		class BOTValidator : public Core::NormalValidator {
		public:
			BOTValidator() {}
			virtual bool Check(Core::ContextInterface& context) const;
		};


		class AnyValidator : public Core::NormalValidator {
		public:
			AnyValidator() {}
			virtual bool Check(Core::ContextInterface& context) const;
		};


		template<typename __CHARTYPE>
		class BetweenValidator : public Core::NormalValidator {
			__CHARTYPE _min;
			__CHARTYPE _max;
		public:
			BetweenValidator(const __CHARTYPE min, const __CHARTYPE max)
				: _min(min)
				, _max(max) {}

			BetweenValidator(const __CHARTYPE* range)
				: _min(0)
				, _max(0)
			{
				_min = Utils::GetChar(range);
				Utils::Increment(&range);
				_max = Utils::GetChar(range);
			}

			virtual bool Check(Core::ContextInterface& context) const {
				context.AdjustPosition();
				Core::Position start = context.GetPosition();
				if (context.Compare(_min) >= 0
					&& context.Compare(_max) <= 0
					&& context.Forward())
				{
					context.AddMatch(start);
					return true;
				}
				return false;
			}
		};


		typedef BetweenValidator<char>  BetweenValidatorA;
		typedef BetweenValidator<wchar_t>  BetweenValidatorW;
		typedef BetweenValidator<char16_t>  BetweenValidatorU16;
		typedef BetweenValidator<char32_t>  BetweenValidatorU32;

		template<typename __T>
		class ExactValidator : public Core::NormalValidator {
			const __T* _phrase;
		public:
			explicit ExactValidator(const __T* phrase) : _phrase(phrase) {}

			virtual bool Check(Core::ContextInterface& context) const {
				context.AdjustPosition();
				const __T* phrasePointer = _phrase;
				Core::Position start = context.GetPosition();
				while (*phrasePointer)
				{
					SChar chr = Utils::GetChar(phrasePointer);
					if (context.Compare(chr)) {
						context.SetPosition(start);
						return false;
					}
					Utils::Increment(&phrasePointer);
					context.Forward();
				}
				context.AddMatch(start);
				return true;
			}
		};

		typedef ExactValidator<char> ExactValidatorA;
		typedef ExactValidator<wchar_t> ExactValidatorW;
		//typedef ExactValidator<char8_t> ExactValidatorU8;
		typedef ExactValidator<char16_t> ExactValidatorU16;
		typedef ExactValidator<char32_t> ExactValidatorU32;
	}


	namespace Manipulators
	{
		class SeqValidator : public Core::BinaryValidator {

		public:
			explicit SeqValidator(Core::StringValidator* s1, Core::StringValidator* s2) :
				Core::BinaryValidator(s1, s2) {}

			virtual bool Check(Core::ContextInterface& context) const;
		};

		class AndValidator : public Core::BinaryValidator {

		public:
			explicit AndValidator(Core::StringValidator* s1, Core::StringValidator* s2) :
				Core::BinaryValidator(s1, s2) {}

			virtual bool Check(Core::ContextInterface& context) const;

		};

		class OrValidator : public Core::BinaryValidator {

		public:
			explicit OrValidator(Core::StringValidator* s1, Core::StringValidator* s2) :
				BinaryValidator(s1, s2) {}

			virtual bool Check(Core::ContextInterface& context) const;
		};

		class GreedyOrValidator : public Core::BinaryValidator {

		public:
			explicit GreedyOrValidator(Core::StringValidator* s1, Core::StringValidator* s2) :
				BinaryValidator(s1, s2) {}

			virtual bool Check(Core::ContextInterface& context) const;
		};

		class NotValidator : public Core::UnaryValidator {
		public:
			explicit NotValidator(Core::StringValidator* op) : Core::UnaryValidator(op) {}
			virtual bool Check(Core::ContextInterface& context) const;
		};

		class LookAheadValidator : public Core::UnaryValidator {
		public:
			explicit LookAheadValidator(StringValidator* op) : UnaryValidator(op) {}
			virtual bool Check(Core::ContextInterface& context) const;
		};

		class LookBackValidator : public Core::UnaryValidator {
		public:
			explicit LookBackValidator(Core::StringValidator* op) : Core::UnaryValidator(op) {}
			virtual bool Check(Core::ContextInterface& context) const;
		};

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

			virtual bool Check(Core::ContextInterface& context) const;

		};

		class ExtractValidator : public Core::UnaryValidator {
			const char* _key;
		public:
			explicit ExtractValidator(Core::StringValidator* op)
				: Core::UnaryValidator(op)
				, _key("<UNNAMED>") {}

			ExtractValidator(Core::StringValidator* op, const char* key)
				: Core::UnaryValidator(op)
				, _key(key) {}


			virtual bool Check(Core::ContextInterface& context) const;
		};



		class RefValidator : public Core::NormalValidator {
			Core::StringValidator* _validator;
			const Core::Rule* _rule;
		public:

			explicit RefValidator() : _rule(NULL), _validator(NULL) {}
			explicit RefValidator(const Core::Rule& r) : _rule(&r), _validator(NULL) {}

			virtual bool Check(Core::ContextInterface& context) const;

			void Set(const Core::Rule& r);

		};

	}



	namespace StateKeepers
	{
		class CaseModifier : public Core::NormalValidator {
			bool _caseSensitive;
		public:
			explicit CaseModifier(bool cs) : _caseSensitive(cs) {}
			virtual bool Check(Core::ContextInterface& context) const;
		};

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

			virtual bool Check(Core::ContextInterface& context) const;
		};

		class DelFlagModifier : public Core::NormalValidator {
			const char* _flag;
		public:
			explicit DelFlagModifier(const char* flag)
				: _flag(flag) {}


			virtual bool Check(Core::ContextInterface& context) const;
		};

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

			virtual bool Check(Core::ContextInterface& context) const;
		};

		class IfMatchedValidator : public Core::NormalValidator {
			const char* _key;
			const unsigned long _min;
			const unsigned long _max;


		public:
			IfMatchedValidator(const char* key, const unsigned long min, const unsigned long max)
				: _key(key)
				, _min(min)
				, _max(max)
			{}

			IfMatchedValidator(const char* key, const unsigned long max)
				: _key(key)
				, _min(1)
				, _max(max)
			{}

			IfMatchedValidator(const char* key)
				: _key(key)
				, _min(1)
				, _max(-1)
			{}

			virtual bool Check(Core::ContextInterface& context) const;
		};

	}



	namespace Core
	{
		class Rule
		{
			Core::StringValidator* _strValid;
			unsigned long _magicNumber;


		public:

			Rule operator=(const Rule& other);
			Rule() : Core::Rule(new Manipulators::NotValidator(new Primitives::AnyValidator())) {}


			Rule(StringValidator* obj) : _magicNumber(OBJECT_VALIDITY_MAGICNUMBER) {
				_strValid = obj;
			}


			Rule(const Rule& other) : _magicNumber(OBJECT_VALIDITY_MAGICNUMBER) {
				if (other._magicNumber != OBJECT_VALIDITY_MAGICNUMBER)
				{
					_strValid = new Manipulators::RefValidator(*this);
					return;
				}
				_strValid = other._strValid;
				if (_strValid)
					_strValid->AddReference();
			}


			StringValidator* Get() const {
				return _strValid;
			}


			bool Check(Core::ContextInterface& context) const {
				return _strValid->Check(context);
			}

			virtual ~Rule() {
				_strValid->Release();
			}
		};
	}






	namespace Operators
	{
		using namespace Core;
		Rule Sequence(const Rule& a, const Rule& b);
		Rule operator > (const Rule& a, const Rule b);
		Rule operator & (const Rule& a, const Rule& b);
		Rule operator | (const Rule& a, const Rule& b);
		Rule operator||(const Rule &a, const Rule &b);

		template<typename __CHARTYPE>
		Rule Is(const __CHARTYPE c) {
			return new Primitives::IsValidator<__CHARTYPE>(c);
		}

		Rule Not(const Rule& r);
		Rule operator! (const Rule& r);
		Rule ZeroOrOne(const Rule& r);
		Rule Optional(const Rule& r);
		Rule ZeroOrMore(const Rule& r, const unsigned int num);
		Rule ZeroOrMore(const Rule& r);
		Rule operator* (const Rule& r);
		Rule Times(const Rule& r, const unsigned int num);
		Rule operator* (const unsigned int num, const Rule& r);
		Rule operator* (const Rule& r, const unsigned int num);
		Rule OneOrMore(const Rule& r, const unsigned int num);
		Rule OneOrMore(const Rule& r);
		Rule operator+ (const unsigned int num, const Rule& r);
		Rule operator+ (const Rule& r, const unsigned int num);
		Rule operator* (const Utils::Range& rng, const Rule& r);
		Rule operator* (const Rule& r, const Utils::Range& rng);
		Rule operator+ (const Rule& r);

		template<typename __CHARTYPE>
		Rule Between(const __CHARTYPE min, const __CHARTYPE max) {
			return new Primitives::BetweenValidator<__CHARTYPE>(min, max);
		}

		template<typename __CHARTYPE>
		Rule Between(const __CHARTYPE* range) {
			return new Primitives::BetweenValidator<__CHARTYPE>(range);
		}


		template<typename __CHARTYPE>
		Rule In(const __CHARTYPE* set) {
			return new Primitives::InValidator<__CHARTYPE>(set);
		}


		template<typename __CHARTYPE>
		Rule Is(const __CHARTYPE* phrase) {
			return new Primitives::ExactValidator<__CHARTYPE>(phrase);
		}

		Rule LookAhead(const Rule& r);
		Rule LookBack(const Rule& r);
		Rule operator~ (const Rule& r);

		extern const Rule Any;
		extern const Rule Digit;
		extern const Rule SmallAlphabet;
		extern const Rule CapitalAlphabet;
		extern const Rule Alphabet;
		extern const Rule Alphanumeric;
		extern const Rule End;
		extern const Rule Beginning;
		extern const Rule Symbol;
		extern const Rule Hex;
		extern const Rule Octet;
		extern const Rule BeginningOfLine;
		extern const Rule EndOfLine;
		extern const Rule WhiteSpace;
		extern const Rule WhiteSpaces;
		extern const Rule Binary;
		extern const Rule WordEnd;
		extern const Rule WordStart;
		extern const Rule Natural;
		extern const Rule Integer;
		extern const Rule Rational;
		extern const Rule Scientific;
		extern const Rule IPv4;
		extern const Rule CaseSensitive;
		extern const Rule CaseInsensitive;
		extern const Rule InChain;
		//extern const Rule HostName;
		template<typename __CHARTYPE>
		Rule Out(const __CHARTYPE* set) {
			return Any & Not(In(set));
		}

		Rule Skip(const Rule& r);
		Rule Skip(const unsigned long count);
		Rule Until(const Rule& r);
		Rule Extract(const Rule& r, const char* key);
		Rule Extract(const Rule& r);
		Rule operator >> (const Rule& r, const char* key);
		Rule Enclosed(const Rule& r, const char* quote);
		Rule Enclosed(const Rule& r, const char* open, const char* close);
		Rule Set(const char* f);
		Rule Set(const char* f, const char* v);
		Rule Del(const char* f);
		Rule If(const char* f);
		Rule If(const char* f, const char* v);
		Rule Ref(const Rule& r);
		Rule IfMatched(const char* key);
		Rule IfMatched(const char* key, unsigned long min);
		Rule IfMatched(const char* key, unsigned long min, unsigned long max);
	}


	template <typename __CHARTYPE>
	class Stringozzi {
		typedef basic_string<__CHARTYPE> STRING;
		Core::Rule _rule;

	public:
		explicit Stringozzi(const Core::Rule& r) : _rule(r) {}

		bool Test(const __CHARTYPE* str, unsigned long flags = 0) {
			Core::Context<__CHARTYPE> context(str, flags);
			return _rule.Check(context);
		}

		bool FastMatch(const __CHARTYPE* str, unsigned long flags = 0) {
			Utils::Matches<__CHARTYPE> _matches;
			Core::Context<__CHARTYPE> context(str, flags, &_matches);
			return _rule.Check(context);
		}

		bool Search(const __CHARTYPE* str, unsigned long flags = 0) {
			Core::Context<__CHARTYPE> context(str, flags, NULL);
			Core::Rule r = Operators::Until(_rule);
			return r.Check(context);
		}

		const __CHARTYPE* SearchAndGetPtr(const __CHARTYPE* str, unsigned long flags = 0) {
			Utils::Matches<__CHARTYPE> matches;
			Core::Context<__CHARTYPE> context(str, flags);
			Core::Rule r = Operators::Until(_rule);
			if (r.Check(context))
				return (char*)context.GetPosition();
			else
				return NULL;
		}

		size_t SearchAndGetIndex(const __CHARTYPE* str, unsigned long flags = 0) {
			Utils::Matches<__CHARTYPE> _matches;
			const Core::Context<__CHARTYPE> context(str, flags, &_matches);
			Core::Rule r = Operators::Until(_rule);
			if (r.Check(context))
				return (char*)context.GetPosition() - str;
			else
				return -1;
		}

		bool Match(const __CHARTYPE* str, Utils::Matches<__CHARTYPE>& matches, unsigned long flags = 0) {
			
			flags = flags | FLAG_MATCHNAMED | FLAG_MATCHUNNAMED; 
			const __CHARTYPE* ptr = SearchAndGetPtr(str, flags);
			if (!ptr)
				return false;

			Core::Context<__CHARTYPE> context(ptr, flags);
			bool ret =  _rule.Check(context);
			matches = context.Matches();
			return ret;
		}


		STRING Replace(const __CHARTYPE* str, const __CHARTYPE* rep, unsigned long flags = 0
			, unsigned int count = 1)
		{
			Core::Context<__CHARTYPE> context(str, flags);
			Core::Position last_start = str;
			Core::Rule r = Operators::Until(_rule);
			STRING s;

			for (unsigned int i = 0; i < count && r.Check(context); i++)
			{
				Core::Position start = context.GetPosition();
				_rule.Check(context);
				Core::Position end = context.GetPosition();
				s.append((__CHARTYPE*)last_start, (__CHARTYPE*)start - (__CHARTYPE*)last_start);
				s.append(rep);
				last_start = (__CHARTYPE*)end;
			}
			s.append((__CHARTYPE*)last_start);
			return s;
		}

		
		void Replace(__CHARTYPE* str, unsigned long size, const __CHARTYPE* rep, unsigned int count = 1, bool casesensitive = true)
		{
		STRING ret = Replace(str, rep, count, casesensitive);
		strncpy(str, ret.c_str(), size);
		}
		

		bool Split(__CHARTYPE* str, vector<STRING>& v
			, unsigned long flags = 0
			, bool dropEmpty = true
			, unsigned int count = 1
			, bool caseSensitive = true)
		{
			Core::Context<__CHARTYPE> context(str, flags);
			Core::Position last_start = str;
			Core::Rule r = Operators::Until(_rule);
			string s;
			for (int i = 0; i < count && r.Check(context); i++)
			{
				Core::Position start = context.GetPosition();
				_rule.Check(context);
				Core::Position end = context.GetPosition();
				s.assign((__CHARTYPE*)last_start, (__CHARTYPE*)start - (__CHARTYPE*)last_start);
				if (!s.empty() || !dropEmpty)
					v.push_back(s);
				last_start = end;
			}
			s.assign((__CHARTYPE*)last_start);
			v.push_back(s);
			return true;
		}

		vector<STRING> Split(__CHARTYPE* str)
		{
			vector<string> vec;
			SplitAll(str, vec);
			return vec;
		}

	};

	typedef Stringozzi<char> StringozziA;
	typedef Stringozzi<wchar_t> StringozziW;
	typedef Stringozzi<char16_t> StringozziU16;
	typedef Stringozzi<char32_t> StringozziU32;




	namespace Utils {
		class PlaceHolder
		{
			Manipulators::RefValidator* _ref;

		public:
			PlaceHolder() :_ref(NULL) {}
			void Set(Manipulators::RefValidator* ref) {
				_ref = ref;
			}


			void Inject(const Core::Rule& r);
		};
	}

	namespace Operators {
		Core::Rule Ref(Utils::PlaceHolder& ph);
	}

	namespace Actions
	{
		template<typename __CHARTYPE>
		bool Test(const Core::Rule& rule, const __CHARTYPE* text, unsigned long flags =0) {
			return Stringozzi<__CHARTYPE>(rule).Test(text, flags);
		}

		template<typename __CHARTYPE>
		bool FastMatch(const Core::Rule& rule,const __CHARTYPE* str, unsigned long flags = 0) {
			return Stringozzi<__CHARTYPE>(rule).FastMatch(str, flags);
		}

		template<typename __CHARTYPE>
		bool Search(const Core::Rule& rule,const __CHARTYPE* str, unsigned long flags = 0) {
			return Stringozzi<__CHARTYPE>(rule).Search(str,flags);
		}

		template<typename __CHARTYPE>
		const __CHARTYPE* SearchAndGetPtr(const Core::Rule& _rule,const __CHARTYPE* str, unsigned long flags = 0) {
			return Stringozzi<__CHARTYPE>(_rule).SearchAndGetPtr(str, flags);
		}

		template<typename __CHARTYPE>
		size_t SearchAndGetIndex(const Core::Rule& _rule, const __CHARTYPE* str, unsigned long flags = 0) {
			return Stringozzi<__CHARTYPE>(_rule).SearchAndGetIndex(str, flags);
		}

		template<typename __CHARTYPE>
		bool Match(const Core::Rule& rule,const __CHARTYPE* str, Utils::Matches<__CHARTYPE>& matches, unsigned long flags = 0) {
			return Stringozzi<__CHARTYPE>(rule).Match(str, matches, flags);
		}

		template<typename __CHARTYPE>
		basic_string<__CHARTYPE> Replace(const Core::Rule& _rule,__CHARTYPE* str, const __CHARTYPE* rep, unsigned long flags = 0
			, unsigned int count = 1)
		{
			return Stringozzi<__CHARTYPE>(_rule).Replace(str, rep, flags, count);
		}

		template<typename __CHARTYPE>
		bool Split(const Core::Rule& _rule, __CHARTYPE* str, vector<basic_string<__CHARTYPE>>& v
			, unsigned long flags = 0
			, bool dropEmpty = true
			, unsigned int count = 1)
		{
			return Stringozzi<__CHARTYPE>(_rule).Split(str, v, flags, dropEmpty, count);
		}


	}

}



#endif // __STRINGOZZI_H__
