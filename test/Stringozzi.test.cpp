﻿/**
 * @file Stringozzi.test.cpp
 * @author Osama Salem (usamamsalem@yahoo.com)
 * @brief  Main E2E Tests source file
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

#ifdef _MSC_VER
#pragma warning(disable :4566)
#endif

#include "gtest/gtest.h"
#define EMBEDDED_SOURCE
#include "Stringozzi.h"
#include <string>

using namespace SPEG;
using namespace SPEG::Utils;
using namespace SPEG::Operators;
using namespace SPEG::Actions;

Rule somerule = Any() > Alphanumeric();

TEST(Utils, TestCounters) {
  unsigned long num = 0;
  ASSERT_TRUE(Utils::SafeIfZero(&num));
  Utils::SafeIncrement(&num);
  ASSERT_FALSE(Utils::SafeIfZero(&num));
  Utils::SafeIncrement(&num);
  ASSERT_FALSE(Utils::SafeIfZero(&num));
  Utils::SafeDecrement(&num);
  ASSERT_FALSE(Utils::SafeIfZero(&num));
  Utils::SafeDecrement(&num);
  ASSERT_TRUE(Utils::SafeIfZero(&num));
  
}

TEST(Operators, TestIs) {
  ASSERT_TRUE(Actions::Test(Is("TEST") > End() , "TEST"));
  ASSERT_FALSE(Actions::Test(Is(NULL) > End() , "TEST"));
  ASSERT_FALSE(Actions::Test(Is<char>(NULL) > End() , "TEST"));
  ASSERT_FALSE(Actions::Test(Is("KKKK") > End() , "TEST"));
  ASSERT_FALSE(Actions::Test(Is("Test") > End() , "TEST"));
  ASSERT_TRUE(Actions::Test(Is("Test") > End() , "TEST",SPEG_CASEINSENSITIVE));
}

TEST(Operators, TestIn) {
  ASSERT_TRUE(Actions::Test(+In("XYZ") > End() , "ZXYZ"));
  ASSERT_FALSE(Actions::Test(+In<char>(NULL) > End() , "ZXYZ"));
  ASSERT_FALSE(Actions::Test(+In("XYZ") > End() , "TEST"));
  ASSERT_FALSE(Actions::Test(+In("XYZ") > End() , "zxyz"));
  ASSERT_TRUE(Actions::Test(+In("XYZ") > End() , "zxyz" , SPEG_CASEINSENSITIVE));
}

TEST(Operators, TestSkip) {
  ASSERT_TRUE(Actions::Test(SkipTo(Is("B")) > Is("BBB") > End(), "AAABBB"));
  ASSERT_TRUE(Actions::Test(SkipTo(Is("B")) > End(), "AAA"));
  ASSERT_TRUE(Actions::Test(SkipTo(2) > Is("ABBB") > End(), "AAABBB"));
  ASSERT_TRUE(Actions::Test(SkipTo(3) > Is("BBB") > End(), "AAABBB"));
  ASSERT_FALSE(Actions::Test(SkipTo(10) > End(), "AAABBB"));
}

TEST(Operators, TestEnclosed) {
  ASSERT_TRUE(Actions::Test(Enclosed(Is("ABC"), "'") > End(), "'ABC'"));
  ASSERT_FALSE(Actions::Test(Enclosed(Is("ABC"), "'") > End(), "ABC"));
  ASSERT_FALSE(Actions::Test(Enclosed(Is("ABC"), "'") > End(), "'ABC"));
  ASSERT_FALSE(Actions::Test(Enclosed(Is("ABC"), "'") > End(), "ABC'"));
  ASSERT_FALSE(Actions::Test(Enclosed(Is("ABC"), "'") > End(), "<ABC>"));
  ASSERT_TRUE(Actions::Test(Enclosed(Is("ABC"), "#", "$") > End(), "#ABC$"));
  ASSERT_FALSE(Actions::Test(Enclosed(Is("ABC"), "#", "$") > End(), "#ABC"));
  ASSERT_FALSE(Actions::Test(Enclosed(Is("ABC"), "#", "$") > End(), "ABC$"));
  ASSERT_FALSE(Actions::Test(Enclosed(Is("ABC"), "#", "$") > End(), "$ABC#"));
  ASSERT_TRUE(Actions::Test(Enclosed(Is("ABC"), "#$", "$#") > End(), "#$ABC$#"));

}

TEST(Manipulators, TestLookAhead) {
  ASSERT_FALSE(Actions::Test(LookAhead(Is("ABC")) > End(), "ABC"));
  ASSERT_TRUE(Actions::Test(LookAhead(Is("ABC")) > Is("ABC") > End(), "ABC"));
  ASSERT_FALSE(Actions::Test(LookAhead(Is("ABC")) > Is("DEF") > End(), "DEF"));
}

TEST(StateKeepers, TestVars) {
  ASSERT_TRUE(Actions::Test(Set("VAR") > If("VAR") > Is('O'), "O"));
  ASSERT_FALSE(Actions::Test(Set("VAR") > If("FAR") > Is('O'), "O"));
  ASSERT_FALSE(Actions::Test(Set("VAR") > Del("VAR") >
              If("VAR") > Is('O'), "O"));
  ASSERT_FALSE(Actions::Test(Set("VAR", "2") > If("VAR") > Is('O'), "O"));
  ASSERT_TRUE(Actions::Test(Set("VAR", "2") > If("VAR", "2") > Is('O'), "O"));
}

TEST(StateKeepers, TestCaseModifier) {
  ASSERT_FALSE(Actions::Test(Is('O') > Is('O') > Is('O'), "Ooo"));
  ASSERT_TRUE(Actions::Test(Is('O') > CaseInsensitive()
            > Is('O') > Is('O'), "Ooo"));
  ASSERT_FALSE(Actions::Test(Is('O') > CaseInsensitive()
            > Is('O') > CaseSensitive() > Is('O'), "Ooo"));
  ASSERT_TRUE(Actions::Test(Is('O') > CaseInsensitive()
            > Is('O') > CaseSensitive() > Is('O'), "OoO"));
  ASSERT_TRUE(Actions::Test(Is('O') > CaseInsensitive()
            > Is('O') > CaseSensitive() > Is('O'), "OOO"));
  ASSERT_TRUE(Actions::Test(Is('O') > CaseInsensitive()
            > Is('o') > CaseSensitive() > Is('O'), "OOO"));
}

TEST(Primitives, TestBOT) {
  ASSERT_TRUE(Actions::Search(Beginning() > Is("B") > End(), "B"));
  ASSERT_FALSE(Actions::Search(Beginning() > Is("B") > End(), "     B"));
  ASSERT_TRUE(Actions::Search(Beginning() > Is("B") > End(), "     B"
                  , SPEG_IGNORESPACES));
}

TEST(Primitives, TestIgnoreSpaces) {
  ASSERT_TRUE(Actions::Search(Beginning() > Is("B") > Is("B") > End(), "BB", SPEG_IGNORESPACES));
  ASSERT_TRUE(Actions::Search(Beginning() > Is("B") > Is("B") > End(), "B B", SPEG_IGNORESPACES));
  ASSERT_TRUE(Actions::Search(Beginning() > Is("B") > Is("B") > End(), "   B B", SPEG_IGNORESPACES));
  ASSERT_TRUE(Actions::Search(Beginning() > Is("B") > Is("B") > End(), "     B B    ", SPEG_IGNORESPACES));
  ASSERT_TRUE(Actions::Search(Beginning() > Is("B") > Is("B") > End(), "BB", SPEG_IGNORESPACES));
  ASSERT_TRUE(Actions::Search(Beginning() > Is("B") > Is("B") > End(), "B\tB", SPEG_IGNORESPACES));
  ASSERT_TRUE(Actions::Search(Beginning() > Is("B") > Is("B") > End(), " \n \t B \nB", SPEG_IGNORESPACES));
  ASSERT_TRUE(Actions::Search(Beginning() > Is("B") > Is("B") > End(), "  \t \r  B B   \n ", SPEG_IGNORESPACES));

}

TEST(Utils, TestFlags) {
  Flags f(0x3);
  ASSERT_TRUE(f.IsFlagSet(0x1));
  ASSERT_TRUE(f.IsFlagSet(0x2));
  ASSERT_FALSE(f.IsFlagSet(0x4));

  f.ClearFlag(0x1);
  ASSERT_FALSE(f.IsFlagSet(0x1));
  ASSERT_TRUE(f.IsFlagSet(0x2));
  ASSERT_FALSE(f.IsFlagSet(0x4));

  f.SetFlag(0x4);
  ASSERT_FALSE(f.IsFlagSet(0x1));
  ASSERT_TRUE(f.IsFlagSet(0x2));
  ASSERT_TRUE(f.IsFlagSet(0x4));

  f.SetAllFlags(0x1);
  ASSERT_TRUE(f.IsFlagSet(0x1));
  ASSERT_FALSE(f.IsFlagSet(0x2));
  ASSERT_FALSE(f.IsFlagSet(0x4));

  f.SetFlag(0x4, true);
  ASSERT_TRUE(f.IsFlagSet(0x1));
  ASSERT_FALSE(f.IsFlagSet(0x2));
  ASSERT_TRUE(f.IsFlagSet(0x4));

  f.SetFlag(0x1, false);
  ASSERT_FALSE(f.IsFlagSet(0x1));
  ASSERT_FALSE(f.IsFlagSet(0x2));
  ASSERT_TRUE(f.IsFlagSet(0x4));

  f.ToggleFlag(0x1);
  ASSERT_TRUE(f.IsFlagSet(0x1));
  ASSERT_FALSE(f.IsFlagSet(0x2));
  ASSERT_TRUE(f.IsFlagSet(0x4));

  f.ToggleFlag(0x4);
  ASSERT_TRUE(f.IsFlagSet(0x1));
  ASSERT_FALSE(f.IsFlagSet(0x2));
  ASSERT_FALSE(f.IsFlagSet(0x4));
}


TEST(Utils, TestUTF8ToUTF32) {
  ASSERT_EQ(Utils::UTF8ToUTF32("\x24"), 0x24);
  ASSERT_EQ(Utils::UTF8ToUTF32("\xC2\xA2"), 0xA2);
  ASSERT_EQ(Utils::UTF8ToUTF32("\xE0\xA4\xB9"), 0x939);
  ASSERT_EQ(Utils::UTF8ToUTF32("\xE2\x82\xAC"), 0x20AC);
  ASSERT_EQ(Utils::UTF8ToUTF32("\xED\x95\x9C"), 0xD55C);
  ASSERT_EQ(Utils::UTF8ToUTF32("\xF0\x90\x8D\x88"), 0x10348);
  ASSERT_EQ(Utils::UTF8ToUTF32(""), 0);
}

TEST(Utils, TestUTF8ToUTF32Length) {
  ASSERT_EQ(Utils::UTF8ToUTF32Length("\x24"), 1);
  ASSERT_EQ(Utils::UTF8ToUTF32Length("\xC2\xA2"), 2);
  ASSERT_EQ(Utils::UTF8ToUTF32Length("\xE0\xA4\xB9"), 3);
  ASSERT_EQ(Utils::UTF8ToUTF32Length("\xE2\x82\xAC"), 3);
  ASSERT_EQ(Utils::UTF8ToUTF32Length("\xED\x95\x9C"), 3);
  ASSERT_EQ(Utils::UTF8ToUTF32Length("\xF0\x90\x8D\x88"), 4);
  ASSERT_EQ(Utils::UTF8ToUTF32Length(""), 1);
}

#ifdef CX11_SUPPORTED

TEST(Utils, TestUTF16ToUTF32) {
  ASSERT_EQ(Utils::UTF16ToUTF32(u"$"), 0x24);
  ASSERT_EQ(Utils::UTF16ToUTF32(u"¢"), 0xA2);
  ASSERT_EQ(Utils::UTF16ToUTF32(u"ह"), 0x939);
  ASSERT_EQ(Utils::UTF16ToUTF32(u"€"), 0x20AC);
  ASSERT_EQ(Utils::UTF16ToUTF32(u"한"), 0xD55C);
  ASSERT_EQ(Utils::UTF16ToUTF32(u"𐍈"), 0x10348);
  ASSERT_EQ(Utils::UTF16ToUTF32(u""), 0);
}

TEST(Utils, TestUTF16ToUTF32Length) {
  ASSERT_EQ(Utils::UTF16ToUTF32Length(u"$"), 1);
  ASSERT_EQ(Utils::UTF16ToUTF32Length(u"¢"), 1);
  ASSERT_EQ(Utils::UTF16ToUTF32Length(u"ह"), 1);
  ASSERT_EQ(Utils::UTF16ToUTF32Length(u"€"), 1);
  ASSERT_EQ(Utils::UTF16ToUTF32Length(u"한"), 1);
  ASSERT_EQ(Utils::UTF16ToUTF32Length(u"𐍈"), 2);
  ASSERT_EQ(Utils::UTF16ToUTF32Length(u""), 1);
}
#endif

TEST(Operators, TestNumbers) {
  ASSERT_TRUE(StringozziA(Rational()).Test("2"));
  ASSERT_TRUE(StringozziA(Rational()).Test("2.0"));
  ASSERT_TRUE(StringozziA(Rational()).Test("+2"));
  ASSERT_FALSE(StringozziA(Rational()).Test(""));
  ASSERT_TRUE(StringozziA(Scientific()).Test("4.5557e+12"));
}

TEST(Operators, TestSymbol) {
  ASSERT_TRUE(StringozziA(Symbol()).Test("/"));
  ASSERT_FALSE(StringozziA(Symbol()).Test("9"));
  ASSERT_FALSE(StringozziA(Symbol()).Test("A"));
  ASSERT_TRUE(StringozziA(Symbol()).Test("#"));
  ASSERT_FALSE(StringozziA(Symbol()).Test(""));
}

TEST(Manipulators, TestLookBack) {
  ASSERT_STREQ(StringozziA(Is("aaa") > (Is(':') & LookBack(Is("aaa"))))
          .SearchAndGetPtr("aaa:ccc bbb dddd"), "aaa:ccc bbb dddd");
  ASSERT_STREQ(StringozziA((Is(':') & LookBack(Is("aaa"))))
          .SearchAndGetPtr("aaa:ccc bbb dddd"), ":ccc bbb dddd");
  ASSERT_STREQ(StringozziA((Is(':') & LookBack(Is("qqq"))))
          .SearchAndGetPtr("aaa:ccc bbb dddd"), NULL);
  ASSERT_STREQ(StringozziA((Is('a') & LookBack(Is("aa"))))
          .SearchAndGetPtr("aaa:ccc bbb dddd"), "a:ccc bbb dddd");
  ASSERT_STREQ(StringozziW((Is('a') & LookBack(Is("aa"))))
          .SearchAndGetPtr(L"aaa:ccc bbb dddd"), L"a:ccc bbb dddd");

}

TEST(Manipulators, TestRecursiveByRef) {
  Rule rule1 = Is('(') > *(Out("()") | Ref(rule1)) > Is(')');
  ASSERT_TRUE(StringozziA(rule1).Test("(fdkjfd(fdj(d))jds(xx))"));
}


TEST(Manipulators, TestRecursiveWithPlaceHolder) {
  PlaceHolder ph;
  Rule rule1 = Is('(') > *(Out("()") | Ref(ph)) > Is(')');
  ph.Inject(rule1);
  ASSERT_TRUE(StringozziA(rule1).Test("(fdkjfd(fdj(d))jds(xx))"));
}


TEST(Manipulators, TestRecursiveGoingOutOfScope) {
  Rule rule2 = Any();
  {
    Rule rule1 = Rule(Is('(') > *(Out("()") | Ref(rule1)) > Is(')'));
    rule2 = rule1;
  }
  ASSERT_TRUE(StringozziA(rule2).Test("(fdkjfd(fdj(d))jds(xx))"));
}

TEST(Primitives, TestInChain) {
  ASSERT_TRUE(Actions::Test(Any() > InChain() > InChain() > End(), "ABC"));
  ASSERT_FALSE(Actions::Test(Any() > InChain() > InChain() > End(), "AKL"));
  ASSERT_TRUE(Actions::Test(Any() > 2 * InChain() > End(), "123"));
}

TEST(Actions, TestActionsTest) {
  Actions::Test(Is("A") > Is("B"), "AB", 0);
}

TEST(Manipulators, TestifMatched) {
  MatchesA m;
  ASSERT_TRUE(StringozziA((*((Is('O') >> "O")
            | (Is('K') >> "K") | Is('S'))) > End() > IfMatched("O"))
            .Match("OOOS", m, SPEG_CASEINSENSITIVE | SPEG_IGNORESPACES));
  m.Clear();
  ASSERT_TRUE(StringozziA((*((Is('O') >> "O") | (Is('K') >> "K")
            | Is('S'))) > End() > IfMatched("O"))
            .Match("OOOS", m, SPEG_CASEINSENSITIVE | SPEG_IGNORESPACES));
  m.Clear();
  ASSERT_FALSE(StringozziA((*((Is('O') >> "O") | (Is('K') >> "K")
            | Is('S'))) > End() > IfMatched("O"))
            .Match("KKKS", m, SPEG_CASEINSENSITIVE | SPEG_IGNORESPACES));
  m.Clear();
  ASSERT_TRUE(StringozziA((*((Is('O') >> "O") | (Is('K') >> "K")
            | Is('S'))) > End() > IfMatched("O", 2))
            .Match("OOOS", m, SPEG_CASEINSENSITIVE | SPEG_IGNORESPACES));
  m.Clear();
  ASSERT_TRUE(StringozziA((*((Is('O') >> "O") | (Is('K') >> "K")
            | Is('S'))) > End() > IfMatched("O", 3))
            .Match("OOOS", m, SPEG_CASEINSENSITIVE | SPEG_IGNORESPACES));
  m.Clear();
  ASSERT_FALSE(StringozziA((*((Is('O') >> "O") | (Is('K') >> "K")
            | Is('S'))) > End() > IfMatched("O", 4))
            .Match("OOOS", m, SPEG_CASEINSENSITIVE | SPEG_IGNORESPACES));
  m.Clear();
  ASSERT_TRUE(StringozziA((*((Is('O') >> "O") | (Is('K') >> "K")
            | Is('S'))) > End() > IfMatched("O", 2, 5))
            .Match("OOOS", m, SPEG_CASEINSENSITIVE | SPEG_IGNORESPACES));
  m.Clear();
  ASSERT_TRUE(StringozziA((*((Is('O') >> "O") | (Is('K') >> "K")
            | (Is('S') >> "S"))) > End() >
            (IfMatched("O", 3) & IfMatched("S", 1)))
            .Match("OOOS", m, SPEG_CASEINSENSITIVE | SPEG_IGNORESPACES));
}

TEST(Manipulators, TestExtract) {
  MatchesA m;
  ASSERT_TRUE(StringozziA((*(Is("o") >> "O")) > (Extract(Is("s"), "S"))
          > (Extract(Is("j"))) > End()).Match("   OOO  SJ    "
          , m, SPEG_CASEINSENSITIVE | SPEG_IGNORESPACES));
  ASSERT_EQ(m.NumberOfMatches(), 4);
  ASSERT_EQ(m.NumberOfMatches("O"), 3);
  ASSERT_EQ(m.NumberOfMatches("S"), 1);
  ASSERT_EQ(m.NumberOfMatches("<UNNAMED>"), 1);
  ASSERT_EQ(m.NumberOfMatches("X"), 0);
  ASSERT_STREQ(m.Get("O", 0), "O");
  ASSERT_STREQ(m.Get("O", 0), "O");
  ASSERT_STREQ(m.Get("O", 1), "O");
  ASSERT_STREQ(m.Get("S", 0), "S");
  ASSERT_STREQ(m.Get("S", 1), NULL);
  ASSERT_STREQ(m.Get("X", 1), NULL);
}


TEST(Utils, TestIPv4) {
  ASSERT_FALSE(StringozziA(IPv4()).Test(""));
  ASSERT_FALSE(StringozziA(IPv4()).Test("111111"));
  ASSERT_FALSE(StringozziA(IPv4()).Test("aaaaaaa"));
  ASSERT_FALSE(StringozziA(IPv4()).Test("12.45"));
  ASSERT_FALSE(StringozziA(IPv4()).Test("12.jfk"));
  ASSERT_FALSE(StringozziA(IPv4()).Test("12..56.78"));
  ASSERT_FALSE(StringozziA(IPv4()).Test("-12.5.56.78"));
  ASSERT_TRUE(StringozziA(IPv4()).Test("0.0.0.0"));
  ASSERT_FALSE(StringozziA(IPv4()).Test("757.466.223.55"));
  ASSERT_FALSE(StringozziA(IPv4()).Test("172.a.22.67"));
  ASSERT_TRUE(StringozziA(IPv4()).Test("172.76.22.67"));
}

TEST(Utils, TestIPv6) {
  ASSERT_FALSE(StringozziA(IPv6() > End()).Test(""));
  ASSERT_FALSE(StringozziA(IPv6() > End()).Test("111111"));
  ASSERT_FALSE(StringozziA(IPv6() > End()).Test("aaaaaaa"));
  ASSERT_TRUE(StringozziA(IPv6() > End()).Test("2001:db8:3:4:f2::192.0.2.33"));
  ASSERT_TRUE(StringozziA(IPv6() > End()).Test("::1"));
  ASSERT_TRUE(StringozziA(IPv6() > End()).Test("::ffff:192.0.2.128"));
  ASSERT_TRUE(StringozziA(IPv6() > End()).Test("::ffff:c000:280"));
  ASSERT_FALSE(StringozziA(IPv6() > End()).Test(":ffff:c000:280"));
  ASSERT_TRUE(StringozziA(IPv6() > End()).Test("0:0:0:0:0:FFFF:129.144.52.38"));
  ASSERT_TRUE(StringozziA(IPv6() > End()).Test("::FFFF:129.144.52.38"));
}


TEST(Manipulators, TestOr) {
  ASSERT_FALSE(StringozziA(Is('S')  | Is('O')).Test(""));
  ASSERT_FALSE(StringozziA(Is('S')  | Is('O')).Test("K"));
  ASSERT_TRUE(StringozziA(Is('S')   | Is('O')).Test("O"));
  ASSERT_TRUE(StringozziA(Is('S')   | Is('O')).Test("S"));
  ASSERT_FALSE(StringozziA((Is('V') | Is("Via")) > End()).Test("Via"));
  ASSERT_TRUE(StringozziA((Is("Via") | Is('V')) > End()).Test("Via"));
}

TEST(Manipulators, TestGreedyOr) {
  ASSERT_FALSE(StringozziA(Is('S') || Is('O')).Test(""));
  ASSERT_FALSE(StringozziA(Is('S') || Is('O')).Test("K"));
  ASSERT_TRUE(StringozziA(Is('S') || Is('O')).Test("O"));
  ASSERT_TRUE(StringozziA(Is('S') || Is('O')).Test("S"));
  ASSERT_TRUE(StringozziA((Is('V') || Is("Via")) > End()).Test("Via"));
  ASSERT_TRUE(StringozziA((Is("Via") || Is('V')) > End()).Test("Via"));
}

TEST(Manipulators, TestSequence) {
  ASSERT_FALSE(StringozziA(Is('S') > Is('O')).Test(""));
  ASSERT_FALSE(StringozziA(Is('S') > Is('O')).Test("S"));
  ASSERT_FALSE(StringozziA(Is('S') > Is('O')).Test("SK"));
  ASSERT_TRUE(StringozziA(Is('S') > Is('O')).Test("SO"));
  ASSERT_FALSE(StringozziA(Is('S') > Is('O') > End()).Test("SOO"));
}

TEST(Manipulators, TestZeroOrMore) {
  ASSERT_TRUE(StringozziA(*Is('L') > End()).Test(""));
  ASSERT_TRUE(StringozziA(*Is('L') > End()).Test("LLLLLL"));
  ASSERT_FALSE(StringozziA(ZeroOrMore(Is('L')) > End()).Test("LLLLKKK"));
  ASSERT_TRUE(StringozziA(ZeroOrMore(Is('L'), 5) > End()).Test("LLLL"));
  ASSERT_FALSE(StringozziA(ZeroOrMore(Is('L'), 2) > End()).Test("LLLLKKK"));
}

TEST(Manipulators, TestOneOrMore) {
  ASSERT_FALSE(StringozziA(+Is('K') > End()).Test(""));
  ASSERT_FALSE(StringozziA(+Is('K') > End()).Test("LLLLLL"));
  ASSERT_TRUE(StringozziA(+Is('K') > End()).Test("K"));
  ASSERT_TRUE(StringozziA(OneOrMore(Is('K')) > End()).Test("KKKKK"));
  ASSERT_TRUE(StringozziA(3 + Is("ABC") > End()).Test("ABCABCABC"));
  ASSERT_TRUE(StringozziA(Is("ABC") + 4 > End()).Test("ABCABCABC"));
  ASSERT_FALSE(StringozziA(OneOrMore(Is("ABC"), 2) > End()).Test("ABCABCABC"));
  ASSERT_FALSE(StringozziA(+Is("ABC") > End()).Test("ABCABCAB"));
  ASSERT_FALSE(StringozziA(+Is("ABC") > End()).Test("ABCABABC"));
  ASSERT_TRUE(StringozziA(+Is("ABC")).Test("ABCABCAB"));
}


TEST(Operators, TestHost) {
  ASSERT_FALSE(StringozziA(Host() > End()).Test(NULL));
  ASSERT_FALSE(StringozziA(Host() > End()).Test(""));
  ASSERT_TRUE(StringozziA(Host() > End()).Test("osama"));
  ASSERT_TRUE(StringozziA(Host() > End()).Test("osama.net"));
  ASSERT_TRUE(StringozziA(Host() > End()).Test(".dddddd"));
  ASSERT_TRUE(StringozziA(Host() > End()).Test("-jfkjdfdk"));
  ASSERT_TRUE(StringozziA(Host() > End()).Test("a-jfkjdfdk"));
  ASSERT_TRUE(StringozziA(Host() > End()).Test("a--jfkjdfdk"));
  ASSERT_TRUE(StringozziA(Host() > End()).Test("www.google.com"));
  ASSERT_TRUE(StringozziA(Host() > End()).Test("111.222.111.222"));
  ASSERT_TRUE(StringozziA(Host() > End()).Test("ABCABABC"));
}


TEST(Manipulators, TestUntil) {
  ASSERT_TRUE(StringozziA(Until(End())).Test(""));
  ASSERT_FALSE(StringozziA(Until(Is("k"))).Test("AAAAAA"));
  ASSERT_TRUE(StringozziA(Until(Is("K"))).Test("AAAKAA"));
  ASSERT_TRUE(StringozziA(Until(Is('K')) > Is('K') > End()).Test("K"));
  ASSERT_FALSE(StringozziA(Until(Is('K')) > Is('K') > End()).Test("NNNNNN"));
  ASSERT_TRUE(StringozziA(Until(Is('K')) > Is('K') > End()).Test("NNNNNNK"));
}


TEST(Manipulators, TestTimes) {
  ASSERT_FALSE(StringozziA(1 * WhiteSpace() > Is("Text") > End()).Test(""));
  ASSERT_TRUE(StringozziA(0 * WhiteSpace() > Is("Text") > End()).Test("Text"));
  ASSERT_TRUE(StringozziA(6 * WhiteSpace() > Is("Text") > End())
              .Test("      Text"));
  ASSERT_FALSE(StringozziA(6 * WhiteSpace() > Is("Text") > End())
              .Test("     Text"));
  ASSERT_FALSE(StringozziA(6 * WhiteSpace() > Is("Text") > End())
              .Test("       Text"));
  ASSERT_TRUE(StringozziA(Times(Is("AB"), 4) > End()).Test("ABABABAB"));
  ASSERT_FALSE(StringozziA(Range(1, 3) * Is("A") > End()).Test(""));
  ASSERT_TRUE(StringozziA(Range(1, 3) * Is("A") > End()).Test("A"));
  ASSERT_TRUE(StringozziA(Range(1, 3) * Is("A") > End()).Test("AA"));
  ASSERT_TRUE(StringozziA(Range(1, 3) * Is("A") > End()).Test("AAA"));
  ASSERT_TRUE(StringozziA(Is("A") * Range(1, 3) > End()).Test("AAA"));
  ASSERT_FALSE(StringozziA(Range(1, 3) * Is("A") > End()).Test("AAAA"));
}

TEST(Actions, TestReplace) {
  ASSERT_STREQ(StringozziA(Is("Osama"))
            .Replace(NULL, "l").c_str(), "");
  ASSERT_STREQ(StringozziA(Is("Osama"))
            .Replace("1234567Osamadddd", NULL).c_str(), "");

  ASSERT_STREQ(StringozziA(Is("Osama"))
            .Replace("1234567Osamadddd", "l").c_str(), "1234567ldddd");
  ASSERT_STREQ(StringozziA(Is("Osama"))
            .Replace("1234567Osamadddd", "lol").c_str(), "1234567loldddd");
  ASSERT_STREQ(StringozziA(Is("Osama"))
            .Replace("1234567OsamaOsamadddd", "lol").c_str()
            , "1234567lolOsamadddd");
  ASSERT_STREQ(StringozziA(Is("Osama"))
            .Replace("1234567OsamaOsamadddd"
            , "lol", 0, 2).c_str()
            , "1234567lolloldddd");
  ASSERT_STREQ(StringozziA(Is("Osama"))
            .Replace("1234567OsamaOsamadddd"
            , "lol", 0, 10).c_str()
            , "1234567lolloldddd");
  ASSERT_STREQ(StringozziA(Is("Osama"))
            .Replace("1234567OsamOsamadddd"
            , "lol").c_str()
            , "1234567Osamloldddd");
}

TEST(Actions, TestReplaceInPlace) {
  std::wstring wstr = L"ABCDEFG";
  wstr.reserve(50);
  StringozziW(Is("CDE")).Replace(&wstr[0], 50, L"XYZ");
  ASSERT_STREQ(wstr.c_str(), L"ABXYZFG");

  std::string str = "ABCDEFG";
  str.reserve(50);
  StringozziA(Is("CDE")).Replace(&str[0], 50, "XYZ");
  ASSERT_STREQ(str.c_str(), "ABXYZFG");
}

TEST(Actions, TestSearch) {
  const char* str = "osaama";
  str = Actions::SearchAndGetPtr(Is('a'), str);
  ASSERT_STRCASEEQ(str, "aama");
  str++;
  str = Actions::SearchAndGetPtr(Is('a'), str);
  ASSERT_STRCASEEQ(str, "ama");
  str++;
  str = Actions::SearchAndGetPtr(Is('a'), str);
  ASSERT_STRCASEEQ(str, "a");
  str++;
  str = Actions::SearchAndGetPtr(Is('a'), str);
  ASSERT_STRCASEEQ(str, NULL);

}

TEST(Manipulators, TestZeroOrOne) {
  ASSERT_TRUE(Actions::Test(Is('A') > Optional(Is('B')) > Is('C')
              > End(), "AC"));
  ASSERT_TRUE(Actions::Test(Is('A') > Optional(Is('B')) > Is('C')
              > End(), "ABC"));
  ASSERT_FALSE(Actions::Test(Is('A') > Optional(Is('B')) > Is('C')
              > End(), "AKC"));
  ASSERT_TRUE(Actions::Test((~Is('B')) > End(), "B"));
  ASSERT_TRUE(Actions::Test((~Is('B')) > End(), ""));
}

TEST(Primitives, TestIn) {
  ASSERT_TRUE(Actions::Test(In("ABC"), "A"));
  ASSERT_TRUE(Actions::Test(In("ABC"), "B"));
  ASSERT_TRUE(Actions::Test(In("ABC"), "C"));
  ASSERT_FALSE(Actions::Test(In("ABC"), ""));
  ASSERT_FALSE(Actions::Test(In("ABC"), "Z"));
  ASSERT_FALSE(Actions::Test(In("ABC"), "a"));
  ASSERT_TRUE(Actions::Test(In("ABC"), "a", SPEG_CASEINSENSITIVE));
  ASSERT_FALSE(Actions::Test(In(""), "A"));
}

TEST(Primitives, TestAny) {

  ASSERT_TRUE(Actions::Test(Any(), "A"));
  ASSERT_TRUE(Actions::Test(Any(), "a"));
  ASSERT_TRUE(Actions::Test(Any(), "1"));
  ASSERT_TRUE(Actions::Test(Any(), "#"));
  ASSERT_TRUE(Actions::Test(Any(), "\x1"));
  ASSERT_FALSE(Actions::Test(Any(), ""));
}

TEST(Primitives, TestRuleConstructor) {
  ASSERT_FALSE(Actions::Test(Rule(), "A"));
  ASSERT_FALSE(Actions::Test(Rule(), "a"));
  ASSERT_FALSE(Actions::Test(Rule(), "1"));
  ASSERT_FALSE(Actions::Test(Rule(), "#"));
  ASSERT_FALSE(Actions::Test(Rule(), "\x1"));
  ASSERT_TRUE(Actions::Test(Rule(), ""));
}

void CallBackFunction(Core::Position start
	, Core::Position end
	, void* context)
{
	ASSERT_TRUE(*(char*)start == 'A');
	ASSERT_TRUE(*(char*)end == 'B');
	ASSERT_TRUE(context == NULL);
}

TEST(Manipulators, TestCallbackValidator) {
	ASSERT_TRUE(Actions::Test(CallBack(Is("A"), CallBackFunction, NULL ), "AB"));
}



int main(int argc, char** argv) {
	
	::testing::InitGoogleTest(&argc, argv);
	return  RUN_ALL_TESTS();
}

