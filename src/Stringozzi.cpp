/**
 * @file Stringozzi.cpp
 * @author Osama Salem (usamamsalem@yahoo.com)
 * @brief the main implementation file 
 * @version 2.0.0.0
 * @date 2020-10-25
 * 
 * @copyright Copyright (c) 2020
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


#define BUILDING_DLL

#include "Stringozzi.h"
#ifdef _MSC_VER
#include <Windows.h>
#endif

using namespace std;

namespace SPEG {
namespace Utils {

template <>
DLL_PUBLIC void Decrement<char>(const char **p) {
  do {
    (*p)--;
  } while (((**p) & 0xC0) == 0x80);
}

DLL_PUBLIC unsigned long CharToLower(unsigned long in) {
  if (in >= 'A' && in <= 'Z')
    return in - 'A' + 'a';
  return in;
}

DLL_PUBLIC unsigned long UTF8ToUTF32(const char *ptr) {
  unsigned long out = 0;
  if ((*ptr & 0x80) == 0) {
    return *ptr;
  } else if ((*ptr & 0xE0) == 0xC0) {
    out = ((*ptr) & 0x1F) << 6;
    ptr++;
    out |= ((*ptr) & 0x3F) << 0;
  } else if ((*ptr & 0xF0) == 0xE0) {
    out = ((*ptr) & 0xF) << 12;
    ptr++;
    out |= ((*ptr) & 0x3F) << 6;
    ptr++;
    out |= ((*ptr) & 0x3F) << 0;
    ptr++;
  } else if ((*ptr & 0xF8) == 0xF0) {
    out = ((*ptr) & 0x7) << 18;
    ptr++;
    out |= ((*ptr) & 0x3F) << 12;
    ptr++;
    out |= ((*ptr) & 0x3F) << 6;
    ptr++;
    out |= ((*ptr) & 0x3F) << 0;
    ptr++;
  }
  return out;
}

DLL_PUBLIC unsigned long UTF8ToUTF32Length(const char *ptr) {
  unsigned long out = 0;
  if ((*ptr & 0xE0) == 0xC0) {
    return 2;
  } else if ((*ptr & 0xF0) == 0xE0) {
    return 3;
  } else if ((*ptr & 0xF8) == 0xF0) {
    return 4;
  }

  return 1;
}

#ifdef CX11_SUPPORTED

DLL_PUBLIC unsigned long UTF16ToUTF32(const char16_t * ptr) {
  unsigned long out = 0;
  if ((*ptr & 0xFC00) == 0xD800) {
    out = ((*ptr) & 0x3FF) << 10;
    ptr++;

    out |= ((*ptr) & 0x3FF) << 0;
    out += 0x10000;
  } else {
    return *ptr;
  }
  return out;
}

DLL_PUBLIC unsigned long UTF16ToUTF32Length(const char16_t* ptr) {
  const char16_t* chr = ptr;
  unsigned long out = 0;
  if ((*chr & 0xFC00) == 0xD800) {
    return 2;
  }
  return 1;
}
#endif

#ifdef _MSC_VER

DLL_PUBLIC void SafeIncrement(unsigned long *num) {
  InterlockedIncrement(num);
}

DLL_PUBLIC void SafeDecrement(unsigned long *num) {
  InterlockedDecrement(num);
}

DLL_PUBLIC bool SafeIfZero(unsigned long* pnum) {
  return InterlockedCompareExchange(pnum, *pnum, 0) == 0;
}

#elif defined __GNUC__

DLL_PUBLIC void SafeIncrement(unsigned long *num) {
  __sync_fetch_and_add(num, 1);
}

DLL_PUBLIC void SafeDecrement(unsigned long *num) {
  __sync_fetch_and_sub(num, 1);
}

DLL_PUBLIC bool SafeIfZero(unsigned long* pnum) {
  return __sync_bool_compare_and_swap (pnum, 0, *pnum);
}

#else

DLL_PUBLIC void SafeIncrement(unsigned long *num) {
  (*num)++;
}

DLL_PUBLIC void SafeDecrement(unsigned long *num) {
  (*num)--;
}

DLL_PUBLIC bool SafeIfZero(unsigned long* pnum) {
  return *pnum == 0;
}

#endif

}  // namespace Utils

namespace Core {
DLL_PUBLIC void NormalValidator::AddReference() {
  Utils::SafeIncrement(&_referenceCount);
}

DLL_PUBLIC void NormalValidator::Release() {
  Utils::SafeDecrement(&_referenceCount);
  if (Utils::SafeIfZero(&_referenceCount)) {
    this->Dispose();
    delete this;
  }
}

void UnaryValidator::Dispose() {
  Operand->Release();
}

void BinaryValidator::Dispose() {
  FirstOperand->Release();
  SecondOperand->Release();
}
}  // namespace Core

namespace Manipulators {
bool SeqValidator::Check(Core::ContextInterface* context) const {
  Core::Position start = context->GetPosition();

  if (FirstOperand->Check(context)) {
	  context->AdjustPosition();
    if (SecondOperand->Check(context)) {
      context->AddMatch(start);
      return true;
    }
  }
  context->SetPosition(start);
  return false;
}

bool UntilValidator::Check(Core::ContextInterface* context) const {
  Core::Position start = context->GetPosition();
  do {
    Core::Position before = context->GetPosition();
    if (Operand->Check(context)) {
      context->SetPosition(before);
      context->AddMatch(start);
      return true;
    }
  } while (context->Forward());

  context->SetPosition(start);
  return false;
}

bool AndValidator::Check(Core::ContextInterface* context) const {
  Core::Position start = context->GetPosition();
  if (FirstOperand->Check(context)) {
    Core::Position frst = context->GetPosition();
    context->SetPosition(start);

    if (SecondOperand->Check(context)) {
      if (frst > context->GetPosition())
      context->SetPosition(frst);
      context->AddMatch(start);
      return true;
    }
  }

  return false;
}

bool OrValidator::Check(Core::ContextInterface* context) const {
  Core::Position start = context->GetPosition();
  if (!FirstOperand->Check(context)) {
    if (!SecondOperand->Check(context))
      return false;
  }
  context->AddMatch(start);
  return true;
}

bool GreedyOrValidator::Check(Core::ContextInterface* context) const {
  Core::Position start = context->GetPosition();
  bool success = false;
  if (FirstOperand->Check(context)) {
    success = true;
  }

  Core::Position first = context->GetPosition();

  context->SetPosition(start);
  if (SecondOperand->Check(context)) {
    success = true;
  }

  Core::Position second = context->GetPosition();

  if (success) {
    context->SetPosition(MAXIMUM(first, second));
    context->AddMatch(start);
    return true;
  }
  return false;
}

bool NotValidator::Check(Core::ContextInterface* context) const {
  Core::Position start = context->GetPosition();
  if (Operand->Check(context)) {
    context->SetPosition(start);
    return false;
  }
  return true;
}

bool LookAheadValidator::Check(Core::ContextInterface* context) const {
  Core::Position start = context->GetPosition();
  if (!Operand->Check(context)) {
    context->SetPosition(start);
    return false;
  }
  context->SetPosition(start);
  return true;
}

bool LookBackValidator::Check(Core::ContextInterface* context) const {
  Core::Position start = context->GetPosition();
  while (context->Backward()) {
    Core::Position newStart = context->GetPosition();
    if (Operand->Check(context)) {
      if (context->GetPosition() == start)
        return true;
      context->SetPosition(newStart);
    }
  }
  context->SetPosition(start);
  return false;
}

bool ExtractValidator::Check(Core::ContextInterface* context) const {
  Core::Position pos = context->GetPosition();
  if (Operand->Check(context)) {
    context->AddMatch(_key, pos);
    return true;
  }
  return false;
}

bool CallBackValidator::Check(Core::ContextInterface* context) const {
	Core::Position pos = context->GetPosition();
	if (Operand->Check(context)) {
		_func(pos, context->GetPosition(), _cbcontext);
		return true;
	}
	return false;
}


}  // namespace Manipulators

namespace Primitives {
bool BOTValidator::Check(Core::ContextInterface* context) const {
  return context->BOT();
}

bool AnyValidator::Check(Core::ContextInterface* context) const {
  Core::Position start = context->GetPosition();
  if (context->Forward()) {
    context->AddMatch(start);
    return true;
  }
  return false;
}

bool InChainValidator::Check(Core::ContextInterface* context) const {
  Core::Position start = context->GetPosition();
  SChar curr = context->Get();
  if (context->Backward()) {
    SChar prev = context->Get();
    context->Forward();
    if (prev == curr - 1 && context->Forward()) {
      context->AddMatch(start);
      return true;
    }
  }
  return false;
}

}  // namespace Primitives

namespace StateKeepers {
bool CaseModifier::Check(Core::ContextInterface* context) const {
  context->Flags().SetFlag(SPEG_CASEINSENSITIVE, _caseSensitive);
  return true;
}

bool SetFlagModifier::Check(Core::ContextInterface* context) const {
  context->SetVar(_flag, _value);
  return true;
}

bool DelFlagModifier::Check(Core::ContextInterface* context) const {
  context->DelVar(_flag);
  return true;
}

bool IfValidator::Check(Core::ContextInterface* context) const {
  const char *val = NULL;
  if (context->GetVar(_flag, &val)) {
    return strcmp(_value, val) == 0;
  }
  return false;
}

bool IfMatchedValidator::Check(Core::ContextInterface* context) const {
  unsigned int num = context->NumberOfMatches(_key);
  return (num >= _min && num <= _max);
}

}  // namespace StateKeepers

namespace Manipulators {
bool RepeatValidator::Check(Core::ContextInterface* context) const {
  Core::Position start = context->GetPosition();

  for (unsigned int counter = 0; counter < _maxIter; counter++) {
    Core::Position startOp = context->GetPosition();
    if (!Operand->Check(context)) {
      if (counter >= _minIter) {
        break;
      } else {
        context->SetPosition(start);
        return false;
      }
    }
  }
  context->AddMatch(start);
  return true;
}

bool RefValidator::Check(Core::ContextInterface* context) const {
  Core::Position start = context->GetPosition();
  if (!_validator) {
    if (_rule->Get()->Check(context)) {
      context->AddMatch(start);
      return true;
    } else {
      return false;
    }
  }

  if (_validator && _validator->Check(context)) {
    context->AddMatch(start);
    return true;
  }
  return false;
}

DLL_PUBLIC void RefValidator::Set(const Core::Rule &rule) {
  _validator = rule.Get();
}

}  // namespace Manipulators

namespace Core {
DLL_PUBLIC Rule& Rule::operator=(const Rule &other) {
  if (_strValid)
    _strValid->Release();
  _strValid = other._strValid;
  _strValid->AddReference();
  return ((*this));
}
}  // namespace Core
}  // namespace SPEG

DLL_PUBLIC SPEG::Core::Rule operator > (const SPEG::Core::Rule& first
            , const SPEG::Core::Rule& second) {
  return SPEG::Operators::Sequence(first, second);
}


DLL_PUBLIC SPEG::Core::Rule operator & (const SPEG::Core::Rule& first
            , const SPEG::Core::Rule& second) {
  return new SPEG::Manipulators::AndValidator(first.Get(), second.Get());
}

DLL_PUBLIC SPEG::Core::Rule operator | (const SPEG::Core::Rule& first
              , const SPEG::Core::Rule& second) {
    return new SPEG::Manipulators::OrValidator(first.Get(), second.Get());
}
DLL_PUBLIC SPEG::Core::Rule operator ||(const SPEG::Core::Rule &first
            , const SPEG::Core::Rule &second) {
  return new SPEG::Manipulators::GreedyOrValidator(first.Get(), second.Get());
}

DLL_PUBLIC SPEG::Core::Rule operator! (const SPEG::Core::Rule& rule) {
    return new SPEG::Manipulators::NotValidator(rule.Get());
}

DLL_PUBLIC SPEG::Core::Rule operator* (const SPEG::Core::Rule& rule) {
  return new SPEG::Manipulators::RepeatValidator(rule.Get(), 0, -1);
}

DLL_PUBLIC SPEG::Core::Rule operator* (const unsigned int num
              , const SPEG::Core::Rule& rule) {
  return new SPEG::Manipulators::RepeatValidator(rule.Get(), num, num);
}

DLL_PUBLIC SPEG::Core::Rule operator* (const SPEG::Core::Rule& rule
              , const unsigned int num) {
  return new SPEG::Manipulators::RepeatValidator(rule.Get(), num, num);
}

DLL_PUBLIC SPEG::Core::Rule operator+ (const unsigned int num
            , const SPEG::Core::Rule& rule) {
  return new SPEG::Manipulators::RepeatValidator(rule.Get(), 1, num);
}

DLL_PUBLIC SPEG::Core::Rule operator+ (const SPEG::Core::Rule& rule
            , const unsigned int num) {
  return new SPEG::Manipulators::RepeatValidator(rule.Get(), 1, num);
}
DLL_PUBLIC SPEG::Core::Rule operator* (const SPEG::Utils::Range& rng
          , const SPEG::Core::Rule& rule) {
  return new SPEG::Manipulators::RepeatValidator(rule.Get(), rng.MIN, rng.MAX);
}

DLL_PUBLIC SPEG::Core::Rule operator* (const SPEG::Core::Rule& rule
          , const SPEG::Utils::Range& rng) {
  return new SPEG::Manipulators::RepeatValidator(rule.Get(), rng.MIN, rng.MAX);
}

DLL_PUBLIC SPEG::Core::Rule operator+ (const SPEG::Core::Rule& rule) {
  return new SPEG::Manipulators::RepeatValidator(rule.Get(), 1, -1);
}

DLL_PUBLIC SPEG::Core::Rule operator~ (const SPEG::Core::Rule& rule) {
    return new SPEG::Manipulators::RepeatValidator(rule.Get(), 0, 1);
}

DLL_PUBLIC SPEG::Core::Rule operator >> (const SPEG::Core::Rule& rule
            , const char* key) {
  ADJUST_NULL_STR(key);
  return new SPEG::Manipulators::ExtractValidator(rule.Get(), key);
}

namespace SPEG {
namespace Operators {
DLL_PUBLIC Rule Sequence(const Rule &first, const Rule &second) {
  return new Manipulators::SeqValidator(first.Get(), second.Get());
}

DLL_PUBLIC Rule Not(const Rule &rule) {
  return new Manipulators::NotValidator(rule.Get());
}

DLL_PUBLIC Rule ZeroOrOne(const Rule &rule) {
  return new Manipulators::RepeatValidator(rule.Get(), 0, 1);
}

DLL_PUBLIC Rule Optional(const Rule &rule) {
  return ZeroOrOne(rule);
}

DLL_PUBLIC Rule ZeroOrMore(const Rule &rule, const unsigned int num) {
  return new Manipulators::RepeatValidator(rule.Get(), 0, num);
}

DLL_PUBLIC Rule ZeroOrMore(const Rule &rule) {
  return new Manipulators::RepeatValidator(rule.Get(), 0, -1);
}

DLL_PUBLIC Rule Times(const Rule &rule, const unsigned int num) {
  return new Manipulators::RepeatValidator(rule.Get(), num, num);
}

DLL_PUBLIC Rule OneOrMore(const Rule &rule, const unsigned int num) {
  return new Manipulators::RepeatValidator(rule.Get(), 1, num);
}

DLL_PUBLIC Rule OneOrMore(const Rule &rule) {
  return new Manipulators::RepeatValidator(rule.Get(), 1, -1);
}

DLL_PUBLIC Rule LookAhead(const Rule &rule) {
  return new Manipulators::LookAheadValidator(rule.Get());
}

DLL_PUBLIC Rule LookBack(const Rule &rule) {
  return new Manipulators::LookBackValidator(rule.Get());
}

DLL_PUBLIC const Rule Any() 
{ 
	static Rule rule = new Primitives::AnyValidator();
	return rule;
}

DLL_PUBLIC const Rule Digit() { return Between("09"); }

DLL_PUBLIC const Rule SmallAlphabet() { return  Between("az"); }

DLL_PUBLIC const Rule CapitalAlphabet() { return  Between("AZ"); }

DLL_PUBLIC const Rule Alphabet() { return CapitalAlphabet() | SmallAlphabet(); }

DLL_PUBLIC const Rule Alphanumeric() { return Digit() | Alphabet(); }

DLL_PUBLIC const Rule End() { return Is('\0'); }

DLL_PUBLIC const Rule Beginning() { 
	static Rule rule = new Primitives::BOTValidator();
	return rule;
}

DLL_PUBLIC const Rule Symbol() { return Any() & !Alphanumeric(); }

DLL_PUBLIC const Rule Hex() { return Digit() | Between("af") | Between("AF"); }

DLL_PUBLIC const Rule Octet() { return Between("07"); }

DLL_PUBLIC const Rule EndOfLine() { return Is("\r\n") | In("\n\r"); }

DLL_PUBLIC const Rule BeginningOfLine() { return Beginning() | LookBack(EndOfLine()); }

DLL_PUBLIC const Rule WhiteSpace() { return In(" \t\r\n"); }

DLL_PUBLIC const Rule WhiteSpaces() { return +WhiteSpace(); }

DLL_PUBLIC const Rule Binary() { return In("01"); }

DLL_PUBLIC const Rule WordEnd() { return LookAhead(!Alphanumeric()); }

DLL_PUBLIC const Rule WordStart() { return LookBack(!Alphanumeric()); }

DLL_PUBLIC const Rule Natural() { return +Digit(); }

DLL_PUBLIC const Rule Integer() { return  ~In("+-") > Natural(); }

DLL_PUBLIC const Rule Rational() { return Integer() > ~(Is('.') > Natural()); }

DLL_PUBLIC const Rule Scientific() {
	return Rational() > ~(In("Ee") > In("+-") > Natural());
}

DLL_PUBLIC const Rule InChain() { 
	static Rule rule = new Primitives::InChainValidator(); 
	return rule;
}


DLL_PUBLIC const Rule IPv4() {

	const Rule DecOctet = (Is("25") > Between("05")) |
		(Is('2') > Between("04") > Digit()) |
		(Is('1') > (2 * Digit())) |
		(Between("19") > Digit()) |
		Digit();

	return DecOctet > ((Is('.') > DecOctet) * 3);
}



DLL_PUBLIC const Rule IPv6() {
	const Rule h16 = Utils::Range(1, 4) * Hex();

	const Rule COLON = Is(':');

	const Rule DBLCOLON = Is("::");

	const Rule h16Colon = h16 > COLON;

	const Rule ls32 = (h16Colon > h16) | IPv4();

	return ((6 * (h16Colon)) > ls32)
		|| (DBLCOLON > (5 * (h16Colon)) > ls32)
		|| (~h16 > DBLCOLON > (4 * (h16Colon)) > ls32)
		|| (~((1 * (h16Colon)) > h16) > DBLCOLON > (3 * (h16Colon)) > ls32)
		|| (~((2 * (h16Colon)) > h16) > DBLCOLON > (2 * (h16Colon)) > ls32)
		|| (~((3 * (h16Colon)) > h16) > DBLCOLON > (1 * (h16Colon)) > ls32)
		|| (~((4 * (h16Colon)) > h16) > DBLCOLON > ls32)
		|| (~((5 * (h16Colon)) > h16) > DBLCOLON > h16)
		|| (~((6 * (h16Colon)) > h16) > DBLCOLON);
}




DLL_PUBLIC const Rule Host() {
	return (+((Is('%') > Hex() > Hex())
		| Alphanumeric()
		| In("-_.~!$&'()*+,;=")))
		|| IPv4()
		|| IPv6();
}


DLL_PUBLIC Rule SkipTo(const Rule &rule) {
  return ZeroOrMore(Any() & Not(rule));
}

DLL_PUBLIC Rule SkipTo(const unsigned long count) {
  return count * Any();
}

DLL_PUBLIC Rule Until(const Rule &rule) {
  return new Manipulators::UntilValidator(rule.Get());
}

DLL_PUBLIC Rule Extract(const Rule &rule, const char *key) {
  ADJUST_NULL_STR(key);
  return new Manipulators::ExtractValidator(rule.Get(), key);
}

DLL_PUBLIC Rule Extract(const Rule &rule) {
  return new Manipulators::ExtractValidator(rule.Get());
}

DLL_PUBLIC Rule CallBack(const Rule &rule
	, Manipulators::CALLBACKFUNCTION func
	, void* ctx) {
	return new Manipulators::CallBackValidator(rule.Get(), func, ctx);
}

DLL_PUBLIC Rule Enclosed(const Rule &rule, const char *quote) {
  ADJUST_NULL_STR(quote);
  Rule quoteRule = Is(quote);
  return quoteRule  > rule > quoteRule;
}

DLL_PUBLIC Rule Enclosed(const Rule &rule, const char *open
                            , const char *close) {
  ADJUST_NULL_STR(open);
  ADJUST_NULL_STR(close);

  return Is(open) > rule > Is(close);
}

DLL_PUBLIC const Rule CaseSensitive() {
	return new StateKeepers::CaseModifier(false);
}

DLL_PUBLIC const Rule CaseInsensitive() {
	return new StateKeepers::CaseModifier(true);
}

DLL_PUBLIC Rule Set(const char *flag) {
  RETURN_IF_NULL(flag, Rule());
  return new StateKeepers::SetFlagModifier(flag);
}

DLL_PUBLIC Rule Set(const char *flag, const char *value) {
  RETURN_IF_NULL(flag, Rule());
  RETURN_IF_NULL(value, Rule());
  return new StateKeepers::SetFlagModifier(flag, value);
}

DLL_PUBLIC Rule Del(const char *flag) {
  RETURN_IF_NULL(flag, Rule());
  return new StateKeepers::DelFlagModifier(flag);
}

DLL_PUBLIC Rule If(const char *flag) {
  RETURN_IF_NULL(flag, Rule());
  return new StateKeepers::IfValidator(flag);
}

DLL_PUBLIC Rule If(const char *flag, const char *value) {
  RETURN_IF_NULL(flag, Rule());
  RETURN_IF_NULL(value, Rule());
  return new StateKeepers::IfValidator(flag, value);
}

DLL_PUBLIC Rule Ref(const Rule &rule) {
  return new Manipulators::RefValidator(rule);
}

DLL_PUBLIC Rule Ref(Utils::PlaceHolder& ph) {
  Manipulators::RefValidator *ref = new Manipulators::RefValidator();
  ph.Set(ref);
  return ref;
}

DLL_PUBLIC Rule IfMatched(const char* key) {
  RETURN_IF_NULL(key, Rule());
  return new StateKeepers::IfMatchedValidator(key, 1, -1);
}

DLL_PUBLIC Rule IfMatched(const char* key, unsigned long min) {
  RETURN_IF_NULL(key, Rule());
  return new StateKeepers::IfMatchedValidator(key, min, -1);
}

DLL_PUBLIC Rule IfMatched(const char* key, unsigned long min
                            , unsigned long max) {
  RETURN_IF_NULL(key, Rule());
  return new StateKeepers::IfMatchedValidator(key, min, max);
}

}  // namespace Operators

}  // namespace SPEG
