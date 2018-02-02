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
#include "parser.h"

using namespace Stringozzi;
using namespace Stringozzi::Rules;
using namespace Stringozzi::Rules::Manipulators;
using namespace Stringozzi::Rules::Filters;
using namespace Stringozzi::Rules::Utils;



TEST(Utils, TestIPv4)
{
	ASSERT_FALSE(StringProcessor("").Parse( IPV4 ));
	ASSERT_FALSE(StringProcessor("111111").Parse(IPV4));
	ASSERT_FALSE(StringProcessor("aaaaaaa").Parse(IPV4));
	ASSERT_FALSE(StringProcessor("12.45").Parse(IPV4));
	ASSERT_FALSE(StringProcessor("12.jfk").Parse(IPV4));
	ASSERT_FALSE(StringProcessor("12..56.78").Parse(IPV4));
	ASSERT_FALSE(StringProcessor("-12.5.56.78").Parse(IPV4));
	ASSERT_TRUE (StringProcessor("0.0.0.0").Parse(IPV4));
	ASSERT_FALSE(StringProcessor("757.466.223.55").Parse(IPV4));
	ASSERT_FALSE(StringProcessor("172.a.22.67").Parse(IPV4));
	ASSERT_TRUE (StringProcessor("172.76.22.67").Parse(IPV4));

	

}


TEST(Manipulators, TestOr)
{
	ASSERT_FALSE(StringProcessor("").Parse(Is('S') | Is('O')));
	ASSERT_FALSE(StringProcessor("K").Parse(Is('S') | Is('O')));
	ASSERT_TRUE (StringProcessor("O").Parse(Is('S') | Is('O')));
	ASSERT_TRUE (StringProcessor("S").Parse(Is('S') | Is('O')));

}

TEST(Manipulators, TestSequence)
{
	ASSERT_FALSE(StringProcessor("").Parse(Is('S') + Is('O')));
	ASSERT_FALSE(StringProcessor("S").Parse(Is('S') + Is('O')));
	ASSERT_FALSE(StringProcessor("SK").Parse(Is('S') + Is('O')));
	ASSERT_TRUE (StringProcessor("SO").Parse(Is('S') + Is('O')));
	ASSERT_FALSE(StringProcessor("SOO").Parse(Is('S') + Is('O') + EOT ));

}

TEST(Manipulators, TestZeroOrMore)
{
	ASSERT_TRUE (StringProcessor("").Parse(ZeroOrMore(Is('L')) + EOT));
	ASSERT_TRUE (StringProcessor("LLLLLLLL").Parse(ZeroOrMore(Is('L')) + EOT));
	ASSERT_FALSE(StringProcessor("LLLLKKKK").Parse(ZeroOrMore(Is('L')) + EOT));

}

TEST(Manipulators, TestOneOrMore)
{
	ASSERT_FALSE(StringProcessor("").Parse(OneOrMore( Is('K') ) + EOT));
	ASSERT_FALSE(StringProcessor("LLLLL").Parse(OneOrMore( Is('K') ) + EndOfText()));
	ASSERT_TRUE (StringProcessor("K").Parse(OneOrMore( Is('K') ) + EndOfText()));
	ASSERT_TRUE (StringProcessor("LLLLLLLL").Parse(OneOrMore( Is('L') ) + EndOfText()));
	ASSERT_TRUE (StringProcessor("ABCABCABC").Parse(OneOrMore( Exact("ABC") ) + EOT));
	ASSERT_FALSE(StringProcessor("ABCABCAB").Parse(OneOrMore( Exact("ABC") ) + EOT));
}

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
}


TEST(Manipulators, TestUntil)
{
	ASSERT_TRUE (StringProcessor("").Parse(Until(EOT)));
	ASSERT_TRUE (StringProcessor("K").Parse(Until(Is('K')) + Is('K') + EOT));
	ASSERT_TRUE (StringProcessor("NNNNNK").Parse(Until(Is('K')) + Is('K') + EOT));
	ASSERT_TRUE (StringProcessor("......osama").Parse(Until(Exact("osama")) + Exact("osama") + EOT));
}


TEST(Manipulators, TestTimes)
{
	ASSERT_TRUE (StringProcessor("osama").Parse(0 * WhiteSpace() + Exact("osama") + EOT));
	ASSERT_TRUE (StringProcessor("......osama").Parse(6 * Is('.') + Exact("osama") + EOT));
	ASSERT_FALSE(StringProcessor(".....osama").Parse(6 * Is('.') + Exact("osama") + EOT));
	ASSERT_FALSE(StringProcessor(".......osama").Parse(6 * Is('.') + Exact("osama") + EOT));

}


TEST(Manipulators, TestReturn)
{
	std::string ret ;
	ret.reserve(128);

	ASSERT_TRUE (StringProcessor("osamasssss").Parse( Extract( &ret[0] , Exact("osama")) ));
	ASSERT_STREQ( ret.c_str() , "osama" );
}

TEST(StringProcessor, TestSearch)
{
	Char* string = "osaama";
	StringProcessor obj(string);
	ASSERT_EQ(obj.Search(Exact("a")),&string[2]);
	ASSERT_EQ(obj.Search(Exact("a")), &string[3]);
	ASSERT_EQ(obj.Search(Exact("a")), &string[5]);
	ASSERT_EQ(obj.Search(Exact("a")),(Char*)0);

}

TEST(Manipulators, TestZeroOrOne)
{
	ASSERT_TRUE(StringProcessor("AC").Parse(Is('A') + Optional(Is('B')) + Is('C') + EOT));
	ASSERT_TRUE(StringProcessor("ABC").Parse(Is('A') + Optional(Is('B')) + Is('C') + EOT));
	ASSERT_FALSE(StringProcessor("ADC").Parse(Is('A') + Optional(Is('B')) + Is('C') + EOT));

}


TEST(Filters, TestAny)
{
	StringProcessor str("A1#\1\122");
	ASSERT_TRUE(str.Parse(ANY));
	ASSERT_TRUE(str.Parse(ANY));
	ASSERT_TRUE(str.Parse(ANY));
	ASSERT_TRUE(str.Parse(ANY));
	ASSERT_TRUE(str.Parse(ANY));
	ASSERT_TRUE(str.Parse(ANY));
}

TEST(Filters, TestIn)
{
	ASSERT_TRUE (StringProcessor("A") .Parse(In("ABC")));
	ASSERT_TRUE (StringProcessor("B") .Parse(In("ABC")));
	ASSERT_TRUE (StringProcessor("C") .Parse(In("ABC")));
	ASSERT_FALSE (StringProcessor("") .Parse(In("")));

	ASSERT_FALSE(StringProcessor("Z").Parse(In("ABC")));
	ASSERT_FALSE(StringProcessor("a").Parse(In("ABC")));
	ASSERT_FALSE(StringProcessor("") .Parse(In("ABC")));//<-----
	ASSERT_FALSE(StringProcessor("A") .Parse(In("")));
}


TEST(Filters, TestEmail)
{
	ASSERT_TRUE(StringProcessor("osama@abc.com").Parse(EMAIL));
}
int main(int argc , char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return  RUN_ALL_TESTS();
	
}

