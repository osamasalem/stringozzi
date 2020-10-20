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


#include "gtest/gtest.h"
#include "Stringozzi.h"

using namespace SPEG;
using namespace SPEG::Utils;
using namespace SPEG::Operators;
using namespace SPEG::Actions;

TEST(Operators, TestNumbers)
{
	ASSERT_TRUE(StringozziA(Rational).Test("2"));
	ASSERT_TRUE(StringozziA(Rational).Test("2.0"));
	ASSERT_TRUE(StringozziA(Rational).Test("+2"));
	ASSERT_FALSE(StringozziA(Rational).Test(""));
	ASSERT_TRUE(StringozziA(Scientific).Test("4.5557e+12"));
}

TEST(Operators, TestSymbol)
{
	ASSERT_TRUE(StringozziA(Symbol).Test("/"));
	ASSERT_FALSE(StringozziA(Symbol).Test("9"));
	ASSERT_FALSE(StringozziA(Symbol).Test("A"));
	ASSERT_TRUE(StringozziA(Symbol).Test("#"));
	ASSERT_FALSE(StringozziA(Symbol).Test(""));
}

TEST(Manipulators, TestLookBack)
{
	ASSERT_STREQ(StringozziA(Is("aaa") > (Is(':') & LookBack(Is("aaa")))).SearchAndGetPtr("aaa:ccc bbb dddd"), "aaa:ccc bbb dddd");
	ASSERT_STREQ(StringozziA((Is(':') & LookBack(Is("aaa")))).SearchAndGetPtr("aaa:ccc bbb dddd"), ":ccc bbb dddd");
}

TEST(Manipulators, TestRecursive)
{
	Rule rule1 = Is('(') > *( Out("()") | Ref(rule1) ) > Is(')');
	ASSERT_TRUE(StringozziA(rule1).Test("(fdkjfd(fdj(d))jds(xx))"));
}


TEST(Manipulators, TestRecursiveGoingOutOfScope)
{
	Rule rule2 = Any;
	{
		Rule rule1 = Rule(Is('(') > *(Out("()") | Ref(rule1)) > Is(')'));
		rule2 = rule1;

	}
	ASSERT_TRUE(StringozziA(rule2).Test("(fdkjfd(fdj(d))jds(xx))"));
}

TEST(Primitives, TestInChain)
{
	ASSERT_TRUE(Actions::Test(Any > InChain > InChain > End, "ABC"));
	ASSERT_FALSE(Actions::Test(Any > InChain > InChain > End ,"AKL"));
	ASSERT_TRUE(Actions::Test(Any > 2 * InChain > End, "123"));
}

TEST(Actions, TestActionsTest)
{
	Actions::Test(Is("A") > Is("B"),  "AB", 0);
}

TEST(Manipulators, TestifMatched)
{
	MatchesA m;
	//ASSERT_TRUE(Actions::Match((*(Is("o") >> "O")) > (Is("s") >> "S") > End > IfMatched("O"),"   OOO  S    ", m,   FLAG_CASEINSENSITIVE | FLAG_IGNORESPACES));
	ASSERT_TRUE(StringozziA((*((Is('O') >> "O") | (Is('K') >> "K") | Is('S')))> End > IfMatched("O")).Match("OOOS", m, FLAG_CASEINSENSITIVE | FLAG_IGNORESPACES));
	m.Clear();
	ASSERT_TRUE(StringozziA(( *( (Is('O') >> "O") | (Is('K') >> "K") | Is('S')) )> End > IfMatched("O") ).Match("OOOS", m,   FLAG_CASEINSENSITIVE | FLAG_IGNORESPACES));
	m.Clear();
	ASSERT_FALSE(StringozziA((*( (Is('O') >> "O") | (Is('K') >> "K") | Is('S')) )> End > IfMatched("O") ).Match("KKKS", m,   FLAG_CASEINSENSITIVE | FLAG_IGNORESPACES));
	m.Clear();
	ASSERT_TRUE(StringozziA((*((Is('O') >> "O") | (Is('K') >> "K") | Is('S')))> End > IfMatched("O", 2)).Match("OOOS", m,   FLAG_CASEINSENSITIVE | FLAG_IGNORESPACES));
	m.Clear();
	ASSERT_TRUE(StringozziA((*((Is('O') >> "O") | (Is('K') >> "K") | Is('S')))> End > IfMatched("O", 3)).Match("OOOS", m,   FLAG_CASEINSENSITIVE | FLAG_IGNORESPACES));
	m.Clear();
	ASSERT_FALSE(StringozziA((*((Is('O') >> "O") | (Is('K') >> "K") | Is('S')))> End > IfMatched("O", 4)).Match("OOOS", m,   FLAG_CASEINSENSITIVE | FLAG_IGNORESPACES));
	m.Clear();
	ASSERT_TRUE(StringozziA((*((Is('O') >> "O") | (Is('K') >> "K") | (Is('S') >> "S" )))> End > (IfMatched("O", 3) & IfMatched("S", 1))).Match("OOOS", m,   FLAG_CASEINSENSITIVE | FLAG_IGNORESPACES));

}

TEST(Manipulators, TestExtract)
{
	MatchesA m;
	ASSERT_TRUE(StringozziA(( *(Is("o") >> "O")) > (Is("s") >> "S") > End ).Match("   OOO  S    ", m,   FLAG_CASEINSENSITIVE | FLAG_IGNORESPACES));
	ASSERT_EQ(m.NumberOfMatches(), 3);
	ASSERT_EQ(m.NumberOfMatches("O"), 3);
	ASSERT_EQ(m.NumberOfMatches("S"), 1);
	ASSERT_EQ(m.NumberOfMatches("X"), 0);
	ASSERT_STREQ(m.Get("O", 0), "O");
	ASSERT_STREQ(m.Get("O", 0), "O");
	ASSERT_STREQ(m.Get("O", 1), "O");
	ASSERT_STREQ(m.Get("S", 0), "S");
	ASSERT_STREQ(m.Get("S", 1), NULL);
	ASSERT_STREQ(m.Get("X", 1), NULL);

}


TEST(Utils, TestIPv4)
{
	ASSERT_FALSE(StringozziA(IPv4).Test(""));
	ASSERT_FALSE(StringozziA(IPv4).Test("111111"));
	ASSERT_FALSE(StringozziA(IPv4).Test("aaaaaaa"));
	ASSERT_FALSE(StringozziA(IPv4).Test("12.45"));
	ASSERT_FALSE(StringozziA(IPv4).Test("12.jfk"));
	ASSERT_FALSE(StringozziA(IPv4).Test("12..56.78"));
	ASSERT_FALSE(StringozziA(IPv4).Test("-12.5.56.78"));
	ASSERT_TRUE (StringozziA(IPv4).Test("0.0.0.0"));
	ASSERT_FALSE(StringozziA(IPv4).Test("757.466.223.55"));
	ASSERT_FALSE(StringozziA(IPv4).Test("172.a.22.67"));
	ASSERT_TRUE (StringozziA(IPv4).Test("172.76.22.67"));
}


TEST(Manipulators, TestOr)
{
	ASSERT_FALSE(StringozziA(Is('S') | Is('O')).Test(""));
	ASSERT_FALSE(StringozziA(Is('S') | Is('O')).Test("K"));
	ASSERT_TRUE(StringozziA(Is('S') | Is('O')).Test("O"));
	ASSERT_TRUE(StringozziA(Is('S') | Is('O')).Test("S"));
	ASSERT_FALSE(StringozziA((Is('V') | Is("Via")) > End).Test("Via"));
	ASSERT_TRUE(StringozziA((Is("Via") | Is('V')) > End).Test("Via"));
}

TEST(Manipulators, TestGreedyOr)
{
	ASSERT_FALSE(StringozziA(Is('S') || Is('O')).Test(""));
	ASSERT_FALSE(StringozziA(Is('S') || Is('O')).Test("K"));
	ASSERT_TRUE(StringozziA(Is('S') || Is('O')).Test("O"));
	ASSERT_TRUE(StringozziA(Is('S') || Is('O')).Test("S"));
	ASSERT_TRUE(StringozziA((Is('V') || Is("Via")) > End).Test("Via"));
	ASSERT_TRUE(StringozziA((Is("Via") || Is('V')) > End).Test("Via"));
}

TEST(Manipulators, TestSequence)
{
	ASSERT_FALSE(StringozziA(Is('S') > Is('O')).Test(""));
	ASSERT_FALSE(StringozziA(Is('S') > Is('O')).Test("S"));
	ASSERT_FALSE(StringozziA(Is('S') > Is('O')).Test("SK"));
	ASSERT_TRUE(StringozziA(Is('S') > Is('O')).Test("SO"));
	ASSERT_FALSE(StringozziA(Is('S') > Is('O') > End).Test("SOO"));

}

TEST(Manipulators, TestZeroOrMore)
{
	ASSERT_TRUE(StringozziA(*Is('L') > End).Test(""));
	ASSERT_TRUE(StringozziA(*Is('L') > End).Test("LLLLLL"));
	ASSERT_FALSE(StringozziA(*Is('L') > End).Test("LLLLKKK"));
}

TEST(Manipulators, TestOneOrMore)
{

	ASSERT_FALSE(StringozziA(+Is('K') > End).Test(""));
	ASSERT_FALSE(StringozziA(+Is('K') > End).Test("LLLLLL"));
	ASSERT_TRUE(StringozziA(+Is('K') > End).Test("K"));
	ASSERT_TRUE(StringozziA(+Is('K') > End).Test("KKKKK"));
	ASSERT_TRUE(StringozziA(+Is("ABC") > End).Test("ABCABCABC"));
	ASSERT_FALSE(StringozziA(+Is("ABC") > End).Test("ABCABCAB"));
	ASSERT_FALSE(StringozziA(+Is("ABC") > End).Test("ABCABABC"));
	ASSERT_TRUE(StringozziA(+Is("ABC")).Test("ABCABCAB"));

}

/*
TEST(Utils, TestHost)
{
	ASSERT_FALSE(StringProcessor("").Parse(HOST + EOT));
	ASSERT_TRUE (StringProcessor("osama").Parse(HOST + EOT));
	ASSERT_TRUE (StringProcessor("osama.net").Parse(HOST + EOT));
	ASSERT_FALSE(StringProcessor(".dddddd").Parse(HOST + EOT));
	ASSERT_FALSE(StringProcessor("-jfkjdfdk").Parse(HOST + EOT));
	ASSERT_TRUE (StringProcessor("a-jfkjdfdk").Parse(HOST + EOT));
	ASSERT_FALSE(StringProcessor("a--jfkjdfdk").Parse(HOST + EOT));
	ASSERT_TRUE (StringProcessor("www.google.com").Parse(HOST + EOT));
	ASSERT_TRUE (StringProcessor("111.222.111.222").Parse(HOST + EOT));

	ASSERT_FALSE(StringozziA(HostName > End).Test("ABCABABC"));



}
*/

TEST(Manipulators, TestUntil)
{
	ASSERT_TRUE(StringozziA(Until(End)).Test(""));
	ASSERT_TRUE(StringozziA(Until(Is('K')) > Is('K') > End).Test("K"));
	ASSERT_FALSE(StringozziA(Until(Is('K')) > Is('K') > End).Test("NNNNNN"));
	ASSERT_TRUE(StringozziA(Until(Is('K')) > Is('K') > End).Test("NNNNNNK"));
}


TEST(Manipulators, TestTimes)
{

	ASSERT_FALSE(StringozziA(1 * WhiteSpace > Is("Text") > End).Test(""));
	ASSERT_TRUE (StringozziA(0 * WhiteSpace > Is("Text") > End).Test("Text"));
	ASSERT_TRUE (StringozziA(6 * WhiteSpace > Is("Text") > End).Test("      Text"));
	ASSERT_FALSE(StringozziA(6 * WhiteSpace > Is("Text") > End).Test("     Text"));
	ASSERT_FALSE(StringozziA(6 * WhiteSpace > Is("Text") > End).Test("       Text"));
	ASSERT_TRUE(StringozziA(4 * Is("AB") > End).Test("ABABABAB"));

}

TEST(Actions, TestReplace)
{
	ASSERT_STREQ(StringozziA(Is("Osama")).Replace("1234567Osamadddd", "l").c_str(), "1234567ldddd");
	ASSERT_STREQ(StringozziA(Is("Osama")).Replace("1234567Osamadddd", "lol").c_str(), "1234567loldddd");
	ASSERT_STREQ(StringozziA(Is("Osama")).Replace("1234567OsamaOsamadddd", "lol").c_str(), "1234567lolOsamadddd");
	ASSERT_STREQ(StringozziA(Is("Osama")).Replace("1234567OsamaOsamadddd", "lol",0,2).c_str(), "1234567lolloldddd");
	ASSERT_STREQ(StringozziA(Is("Osama")).Replace("1234567OsamaOsamadddd", "lol", 0, 10).c_str(), "1234567lolloldddd");
	ASSERT_STREQ(StringozziA(Is("Osama")).Replace("1234567OsamOsamadddd", "lol").c_str(), "1234567Osamloldddd");
}

TEST(Actions, TestSearch)
{
	const char* str = "osaama";
	/*
	ASSERT_EQ(obj.Search(Exact("a")), &string[2]);
	ASSERT_EQ(obj.Search(Exact("a")), &string[3]);
	ASSERT_EQ(obj.Search(Exact("a")), &string[5]);
	ASSERT_EQ(obj.Search(Exact("a")), (Char*)0);
	*/


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

TEST(Manipulators, TestZeroOrOne)
{

	ASSERT_TRUE(Actions::Test(Is('A') > Optional(Is('B')) > Is('C') > End, "AC"));
	ASSERT_TRUE(Actions::Test(Is('A') > Optional(Is('B')) > Is('C') > End, "ABC"));
	ASSERT_FALSE(Actions::Test(Is('A') > Optional(Is('B')) > Is('C') > End, "AKC"));
	ASSERT_TRUE(Actions::Test((~Is('B')) > End, "B"));
	ASSERT_TRUE(Actions::Test((~Is('B')) > End, ""));

}

TEST(Primitives, TestIn)
{
	ASSERT_TRUE(Actions::Test(In("ABC"), "A" ));
	ASSERT_TRUE(Actions::Test(In("ABC"), "B"));
	ASSERT_TRUE(Actions::Test(In("ABC"), "C"));
	ASSERT_FALSE(Actions::Test(In("ABC"), ""));
	ASSERT_FALSE(Actions::Test(In("ABC"), "Z"));
	ASSERT_FALSE(Actions::Test(In("ABC"), "a"));
	ASSERT_TRUE(Actions::Test(In("ABC"), "a", FLAG_CASEINSENSITIVE));
	ASSERT_FALSE(Actions::Test(In(""), "A"));
}

TEST(Primitives, TestAny)
{
	ASSERT_TRUE(Actions::Test(Any, "A"));
	ASSERT_TRUE(Actions::Test(Any, "a"));
	ASSERT_TRUE(Actions::Test(Any, "1"));
	ASSERT_TRUE(Actions::Test(Any, "#"));
	ASSERT_TRUE(Actions::Test(Any, "\x1"));
	ASSERT_FALSE(Actions::Test(Any, ""));
}


/*



TEST(Filters, TestEmail)
{
	ASSERT_TRUE(StringProcessor("osama@abc.com").Parse(EMAIL));
}
*/

int main(int argc , char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return  RUN_ALL_TESTS();
	
}

