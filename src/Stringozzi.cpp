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


#include "Stringozzi.h"
#ifdef _MSC_VER
#include <Windows.h>
#endif

using namespace std;

namespace SPEG
{
	namespace Utils
	{

		template <>
		void Decrement<char>(const char **p)
		{
			do
			{
				(*p)--;
			} while (((**p) & 0xC0) == 0x80);
		}

		unsigned long CharToLower(unsigned long in)
		{
			if (in >= 'A' && in <= 'Z')
				return in - 'A' + 'a';
			return in;
		}

		unsigned long UTF8ToUTF32(const char *p)
		{
			const char *c = p;
			unsigned long out = 0;
			if ((*c & 0x80) == 0)
			{
				return *c;
			}
			else if ((*c & 0xE0) == 0xC0)
			{
				out = ((*c) & 0x1F) << 6;
				c++;
				out |= ((*c) & 0x3F) << 0;
			}
			else if ((*c & 0xF0) == 0xE0)
			{
				out = ((*c) & 0xF) << 12;
				c++;
				out |= ((*c) & 0x3F) << 6;
				c++;
				out |= ((*c) & 0x3F) << 0;
				c++;
			}
			else if ((*c & 0xF8) == 0xF0)
			{
				out = ((*c) & 0x7) << 18;
				c++;
				out |= ((*c) & 0x3F) << 12;
				c++;
				out |= ((*c) & 0x3F) << 6;
				c++;
				out |= ((*c) & 0x3F) << 0;
				c++;
			}
			return out;
		}

		unsigned long UTF8ToUTF32Length(const char *p)
		{
			const char *c = p;
			unsigned long out = 0;
			if ((*c & 0x80) == 0)
			{
				return 1;
			}
			else if ((*c & 0xE0) == 0xC0)
			{
				return 2;
			}
			else if ((*c & 0xF0) == 0xE0)
			{
				return 3;
			}
			else if ((*c & 0xF8) == 0xF0)
			{
				return 4;
			}
			return 0;
		}

#ifdef _MSC_VER

		void SafeIncrement(unsigned long *num)
		{
			InterlockedIncrement(num);
		}

		void SafeDecrement(unsigned long *num)
		{
			InterlockedDecrement(num);
		}

#elif defined __GNUC__

		void SafeIncrement(unsigned long *num)
		{
			__sync_fetch_and_add(num, 1);
		}

		void SafeDecrement(unsigned long *num)
		{
			__sync_fetch_and_sub(num, 1);
		}

#else

		void SafeIncrement(unsigned long *num)
		{
			(*num)++;
		}

		void SafeDecrement(unsigned long *num)
		{
			(*num)--;
		}

#endif

	} // namespace Utils

	namespace Core
	{
		void NormalValidator::AddReference()
		{
			Utils::SafeIncrement(&_referenceCount);
		}

		void NormalValidator::Release()
		{
			Utils::SafeDecrement(&_referenceCount);
			if (!_referenceCount)
			{
				this->Dispose();
				delete this;
			}
		}

		void UnaryValidator::Dispose()
		{
			Operand->Release();
		}

		void BinaryValidator::Dispose() 
		{
			FirstOperand->Release();
			SecondOperand->Release();
		}
	} // namespace Core

	namespace Manipulators
	{

		bool SeqValidator::Check(Core::ContextInterface &context) const
		{
			Core::Position start = context.GetPosition();

			if (FirstOperand->Check(context))
			{
				if (SecondOperand->Check(context))
				{
					context.AddMatch(start);
					return true;
				}
			}
			context.SetPosition(start);
			return false;
		}

		bool AndValidator::Check(Core::ContextInterface &context) const
		{
			Core::Position start = context.GetPosition();
			if (FirstOperand->Check(context))
			{
				Core::Position frst = context.GetPosition();
				context.SetPosition(start);

				if (SecondOperand->Check(context))
				{
					if (frst > context.GetPosition())
						context.SetPosition(frst);
					context.AddMatch(start);
					return true;
				}
			}

			return false;
		}

		bool OrValidator::Check(Core::ContextInterface &context) const
		{
			Core::Position start = context.GetPosition();
			if (!FirstOperand->Check(context))
			{
				if (!SecondOperand->Check(context))
					return false;
			}
			context.AddMatch(start);
			return true;
		}

		bool GreedyOrValidator::Check(Core::ContextInterface &context) const
		{
			Core::Position start = context.GetPosition();
			Core::Position first = start, second = start;
			bool success = false;
			if (FirstOperand->Check(context))
			{
				first = context.GetPosition();
				success = true;
			}

			context.SetPosition(start);
			if (SecondOperand->Check(context))
			{
				second = context.GetPosition();
				success = true;
			}
			
			if(success)
			{
				context.SetPosition(MAXIMUM(first, second));
				context.AddMatch(start);
				return true;
			}
			return false;

		}

		bool NotValidator::Check(Core::ContextInterface &context) const
		{
			Core::Position start = context.GetPosition();
			if (Operand->Check(context))
			{
				context.SetPosition(start);
				return false;
			}
			context.AddMatch(start);
			return true;
		}

		bool LookAheadValidator::Check(Core::ContextInterface &context) const
		{
			Core::Position start = context.GetPosition();
			if (!Operand->Check(context))
			{
				context.SetPosition(start);
				return false;
			}
			context.SetPosition(start);
			return true;
		}

		bool LookBackValidator::Check(Core::ContextInterface &context) const
		{
			Core::Position start = context.GetPosition();
			while (!context.BOT())
			{
				context.Backward();
				Core::Position newStart = context.GetPosition();
				if (Operand->Check(context))
				{
					if (context.GetPosition() == start)
						return true;
					context.SetPosition(newStart);
				}
			}
			context.SetPosition(start);
			return false;
		}

		bool ExtractValidator::Check(Core::ContextInterface &context) const
		{

			Core::Position pos = context.GetPosition();
			if (Operand->Check(context))
			{
				context.AddMatch(_key, pos, context.GetPosition());
				return true;
			}
			return false;
		}

	} // namespace Manipulators

	namespace Primitives
	{
		bool BOTValidator::Check(Core::ContextInterface &context) const
		{
			return context.BOT();
		}

		bool AnyValidator::Check(Core::ContextInterface &context) const
		{
			context.AdjustPosition();
			Core::Position start = context.GetPosition();
			if (context.Forward())
			{
				context.AddMatch(start);
				return true;
			}
			return false;
		}

		bool InChainValidator::Check(Core::ContextInterface &context) const
		{
			context.AdjustPosition();
			Core::Position start = context.GetPosition();
			SChar curr = context.Get();
			if (context.Backward())
			{
				SChar prev = context.Get();
				context.Forward();
				if (prev == curr - 1 && context.Forward())
				{
					context.AddMatch(start);
					return true;
				}
			}
			return false;
		}

	} // namespace Primitives

	namespace StateKeepers
	{
		bool CaseModifier::Check(Core::ContextInterface &context) const
		{
			context.Flags().SetFlag(FLAG_CASEINSENSITIVE, _caseSensitive);
			return true;
		}

		bool SetFlagModifier::Check(Core::ContextInterface &context) const
		{
			context.SetVar(_flag, _value);
			return true;
		}

		bool DelFlagModifier::Check(Core::ContextInterface &context) const
		{
			context.DelVar(_flag);
			return true;
		}

		bool IfValidator::Check(Core::ContextInterface &context) const
		{
			const char *val = NULL;
			if (context.GetVar(_flag, &val))
			{
				return strcmp(_value, val) == 0;
			}
			return false;
		}

		bool IfMatchedValidator::Check(Core::ContextInterface &context) const
		{
			unsigned int num = context.NumberOfMatches(_key);
			return (num >= _min && num <= _max);
		}

	} // namespace StateKeepers

	namespace Manipulators
	{
		bool RepeatValidator::Check(Core::ContextInterface &context) const
		{
			Core::Position start = context.GetPosition();

			for (unsigned int i = 0; i < _maxIter; i++)
			{
				Core::Position startOp = context.GetPosition();
				if (!Operand->Check(context))
				{
					if (i >= _minIter)
					{
						break;
					}
					else
					{
						context.SetPosition(start);
						return false;
					}
				}

				if (context.GetPosition() == startOp)
					break;
			}
			context.AddMatch(start);
			return true;
		}

		bool RefValidator::Check(Core::ContextInterface &context) const
		{
			Core::Position start = context.GetPosition();
			if (!_validator)
			{
				if (_rule->Get()->Check(context))
				{
					context.AddMatch(start);
					return true;
				}
				else
				{
					return false;
				}
			}

			if (_validator->Check(context))
			{
				context.AddMatch(start);
				return true;
			}
			return false;
		}

		void RefValidator::Set(const Core::Rule &r)
		{
			_validator = r.Get();
		}

	} // namespace Manipulators

	namespace Core
	{
		Rule Rule::operator=(const Rule &other)
		{
			if (_strValid)
				_strValid->Release();
			_strValid = other._strValid;
			_strValid->AddReference();
			return ((*this));
		}
	} // namespace Core

	namespace Operators
	{
		Rule Sequence(const Rule &a, const Rule &b)
		{
			return new Manipulators::SeqValidator(a.Get(), b.Get());
		}

		Rule operator>(const Rule &a, const Rule b)
		{
			return Sequence(a, b);
		}

		Rule operator&(const Rule &a, const Rule &b)
		{
			return new Manipulators::AndValidator(a.Get(), b.Get());
		}

		Rule operator|(const Rule &a, const Rule &b)
		{
			return new Manipulators::OrValidator(a.Get(), b.Get());
		}

		Rule operator||(const Rule &a, const Rule &b)
		{
			return new Manipulators::GreedyOrValidator(a.Get(), b.Get());
		}


		Rule Not(const Rule &r)
		{
			return new Manipulators::NotValidator(r.Get());
		}

		Rule operator!(const Rule &r)
		{
			return Not(r);
		}

		Rule ZeroOrOne(const Rule &r)
		{
			return new Manipulators::RepeatValidator(r.Get(), 0, 1);
		}

		Rule Optional(const Rule &r)
		{
			return ZeroOrOne(r);
		}

		Rule ZeroOrMore(const Rule &r, const unsigned int num)
		{
			return new Manipulators::RepeatValidator(r.Get(), 0, num);
		}

		Rule ZeroOrMore(const Rule &r)
		{
			return new Manipulators::RepeatValidator(r.Get(), 0, -1);
		}

		Rule operator*(const Rule &r)
		{
			return ZeroOrMore(r);
		}

		Rule Times(const Rule &r, const unsigned int num)
		{
			return new Manipulators::RepeatValidator(r.Get(), num, num);
		}

		Rule operator*(const unsigned int num, const Rule &r)
		{
			return Times(r, num);
		}

		Rule operator*(const Rule &r, const unsigned int num)
		{
			return Times(r, num);
		}

		Rule OneOrMore(const Rule &r, const unsigned int num)
		{
			return new Manipulators::RepeatValidator(r.Get(), 1, num);
		}

		Rule OneOrMore(const Rule &r)
		{
			return new Manipulators::RepeatValidator(r.Get(), 1, -1);
		}

		Rule operator+(const unsigned int num, const Rule &r)
		{
			return OneOrMore(r, num);
		}

		Rule operator+(const Rule &r, const unsigned int num)
		{
			return OneOrMore(r, num);
		}

		Rule operator*(const Utils::Range &rng, const Rule &r)
		{
			return new Manipulators::RepeatValidator(r.Get(), rng.MIN, rng.MAX);
		}

		Rule operator*(const Rule &r, const Utils::Range &rng)
		{
			return new Manipulators::RepeatValidator(r.Get(), rng.MIN, rng.MAX);
		}

		Rule operator+(const Rule &r)
		{
			return OneOrMore(r);
		}

		Rule LookAhead(const Rule &r)
		{
			return new Manipulators::LookAheadValidator(r.Get());
		}

		Rule LookBack(const Rule &r)
		{
			return new Manipulators::LookBackValidator(r.Get());
		}

		Rule operator~(const Rule &r)
		{
			return ZeroOrOne(r);
		}

		///const Rule Any = Between(1, -1);
		const Rule Any = new Primitives::AnyValidator();

		const Rule Digit = Between("09");

		const Rule SmallAlphabet = Between("az");

		const Rule CapitalAlphabet = Between("AZ");

		const Rule Alphabet = CapitalAlphabet | SmallAlphabet;

		const Rule Alphanumeric = Digit | Alphabet;

		const Rule End = Is('\0');

		const Rule Beginning = !LookBack(Any);

		const Rule Symbol = Any & !Alphanumeric;

		const Rule Hex = Digit | Between("af") | Between("AF");

		const Rule Octet = Between("07");

		const Rule EndOfLine = Is("\r\n") | In("\n\r");

		const Rule BeginningOfLine = Beginning | LookBack(EndOfLine);

		const Rule WhiteSpace = In(" \t\r\n");

		const Rule WhiteSpaces = +WhiteSpace;

		const Rule Binary = In("01");

		const Rule WordEnd = LookAhead(!Alphanumeric);

		const Rule WordStart = LookBack(!Alphanumeric);

		const Rule Natural = +Digit;

		const Rule Integer = ~In("+-") > Natural;

		const Rule Rational = Integer > ~(Is('.') > Natural);

		const Rule Scientific = Rational > ~(In("Ee") > In("+-") > Natural);

		const Rule InChain = new Primitives::InChainValidator();

		const Rule DecOctet = (Is("25") > Between("05"))|
													(Is('2') > Between("04") > Digit) |
													(Is('1') > (2 * Digit)) |
													(Between("19") > Digit) |
													Digit ;

		const Rule IPv4 = DecOctet > ((Is('.') > DecOctet) * 3);

		Rule Skip(const Rule &r)
		{
			return ZeroOrMore(Any & Not(r));
		}

		Rule Skip(const unsigned long count)
		{
			return count * Any;
		}

		Rule Until(const Rule &r)
		{
			return Skip(r) > LookAhead(r);
		}

		Rule Extract(const Rule &r, const char *key)
		{
			return new Manipulators::ExtractValidator(r.Get(), key);
		}

		Rule Extract(const Rule &r)
		{
			return new Manipulators::ExtractValidator(r.Get());
		}

		Rule operator>>(const Rule &r, const char *key)
		{
			return Extract(r, key);
		}

		Rule Enclosed(const Rule &r, const char *quote)
		{
			Rule q = Is(quote);
			return q > r > q;
		}

		Rule Enclosed(const Rule &r, const char *open, const char *close)
		{
			return Is(open) > r > Is(close);
		}

		const Rule CaseSensitive = new StateKeepers::CaseModifier(true);
		const Rule CaseInsensitive = new StateKeepers::CaseModifier(false);

		Rule Set(const char *f)
		{
			return new StateKeepers::SetFlagModifier(f);
		}

		Rule Set(const char *f, const char *v)
		{
			return new StateKeepers::SetFlagModifier(f, v);
		}

		Rule Del(const char *f)
		{
			return new StateKeepers::DelFlagModifier(f);
		}

		Rule If(const char *f)
		{
			return new StateKeepers::IfValidator(f);
		}

		Rule If(const char *f, const char *v)
		{
			return new StateKeepers::IfValidator(f, v);
		}

		Rule Ref(const Rule &r)
		{
			return new Manipulators::RefValidator(r);
		}

		Rule Ref(Utils::PlaceHolder &ph)
		{
			Manipulators::RefValidator *r = new Manipulators::RefValidator();
			ph.Set(r);
			return r;
		}

		Rule IfMatched(const char* key)
		{
			return new StateKeepers::IfMatchedValidator(key, 1, -1);
		}
		Rule IfMatched(const char* key, unsigned long min)
		{
			return new StateKeepers::IfMatchedValidator(key, min, -1);
		}
		Rule IfMatched(const char* key, unsigned long min, unsigned long max)
		{
			return new StateKeepers::IfMatchedValidator(key, min, max);
		}

	} // namespace Operators

} // namespace SPEG
