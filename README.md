# Stringozzi

### _Serving effeciently, Served with :sparkling_heart:_
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT) [![Build Status](https://travis-ci.org/osamasalem/stringozzi.svg?branch=master)](https://travis-ci.org/osamasalem/stringozzi)  [![codecov](https://codecov.io/gh/osamasalem/stringozzi/branch/master/graph/badge.svg)](https://codecov.io/gh/osamasalem/stringozzi)


**C++ Parsing Expression grammer library for parsing/searching/validating like Regular Expression**

## Whats new 
the new version comes with totally rewritten core to provide the balance between flexibility and performance 

## Motivation
The idea of this project is raised for the need to a library to parse text based network protocols eaily and effeciently.. and support for such structured expressions like ones in ABNF so you can build and structure complex parsing expressions

## Philosophy
1. To be open source and friendly available for commercial product
2. To be effecient specially in parsing  without loosing flexibility
3. To be cross platform (Windows and Linux for now !!)
4. To support wide C++ standards choices as much as possible

## Why Stringozzi ?
1. Wide cross C++ standard compliance (this version is compliant to C++97 and upward)
2. 3x-10x* faster than std::regex VS/GCC
3. Lean and mean .. use only feature that you need to ( Validate / Search distinction and disable matching results ) 
4. the grammer is checked by C++ compiler rather than wasting time debugging regex expressions
5. new features not in Regex like Support for recursive expressions, character chains detection , conditional parsing and (A)BNF like rules 
6. Seamless support for UTF and Unicode string with mix and match
7. support for long expressions and text
8. Packed with useful built-in expressions (IPv4,URI, ServerName) 

## Why _not_ ?
1. You don't like pasta or Italian couisine 
2. The expressions you use are too short
3. The program loads validation expressions from remote source(like DB or text files)
  
 ## The magic you can do
 Expression like that

  ```
  ^([XYZ]{5}X?\s+\<(AB){3}\>)*$
  ```

Is equivalent to 
 ```cpp
Rule z = *(5 * (In("XYZ")) > ~Is('X') > +WhiteSpace > Enclosed(3 * (Is("AB")), "<", ">")) > End;
...
StringozziA(z).Test("ZXYZZ <ABABAB>")
 ```
  Or sructured ones like these
```cpp
const Rule Verb = (Is("GET") | Is("POST"));
const Rule URI = Is("http://") > *(Any & !WhiteSpace);
const Rule RequestLine = Verb > URI > Is("HTTP/2.0") > EndOfLine;
const Rule Header =  *(Any & !WhiteSpaces) >  Is(":") > *(Any & !WhiteSpaces)
const Rule Request = RequestLine > *(Headers) > EndOfLine > Content;
...
```

## Getting Started 
### Cloning the repository
```
cd stringozzi
git.exe clone "https://github.com/osamasalem/stringozzi.git ."  
```

### Building 
For Windows
```
build.bat
```

For Linux
```
./build.sh
```
### First Steps
```cpp
#include <Stringozzi.h>
int main(int argc, char** argv) 
{
  const Rule r = Is("Student No#:") > (Range(1,3) * Digit);

  Actions::Test(r, "Student No#: 434");
}
```
### Rules and Operators

| Rule | Description |
|-|-|
| Any | Any Character |
| Digit | Any Character between '0' and '9'|
| Alphabet | Any Character between 'A' and 'Z'|
| Alphanumeric | ```Digit``` or ```Alphabet```|
| Whitespace | Any space Character |
| Beginning | Matches the beginning of text |
|End | Matches the end of text (i.e. ```'\0'```)
| _a_>_b_ | Perform parsing sequentially using rule _a_ first then rule _b_ |
| !_a_ | Negate parsing rule _a_, this action does not move parsing pointer |
| _a_&_b_ | Greedy and: Rule _a_ and Rule _b_, the token must comply with both rules, if they are both true it will apply the most relevant one |
| _a_\|_b_ | Rule a or Rule b, the token may comply with either rules, **short circuit applies here, so start always with most specific rule to more general ones OR use Greedy OR operator** |
| _a_\|\|_b_ | Rule _a_ or Rule _b_, the token may comply with either rules, it always checks all rules and takes the most relevant one in expense of performance |
| *_a_ | Zero Or More  Mtahces a single instance or multiiple instances **Take care when using this rule it can match any thing**|
| _[num]_*_a_ | exact ```num``` of mtahces |
|_[num]_+_a_| One Or More (optional maximum ```num```) (at least rule), Matches a single instance or multiiple instances |
| ~_a_ | Optional token. it parses tokens whenever possible |
| Between(_a_,_b_) | _a_ and _b_ are chars, this matches any character in the specified range|
| In(_str_) | _str_ is string pointer,Belongs to rule: matches the chracter with a set of characters |
| Out(_str_) | Any character out of set _str_ |
| Is(_tok_) | Equal operator,_tok_ is either a char or string pointer .. it matches the token with characters or a set of consecutive characters |
| _rule_ >> _str_ | put the matched by rule _rule_ string in matches table with the name speicifed in _str_| 
| Skip(_rule_) | Skip the characters till it matches the _rule_, always return true |
| Until(_rule_) | Skip the characters till it matches the _rule_, it requires the next token to match _rule_  |
| LookAhead(_rule_) | it peeks the next token and checks if it matches _rule_, it does not move parsing pointer |
| LookBack(_rule_) | it peeks the previous token and checks if it matches _rule_, it does not move parsing pointer |
| CaseSensitive | this will set case sensitive mode in parsing process |
| CaseSensitive | this will set case insensitive mode in parsing process |
| SetVar(varname,_[value]_) | this will  match all the time .. this sets a flag/variable with specified value.. if no value is supplied the default will be ```1``` |
| DelVar(varname) | this will match always .. this is remove flag/variable  |
| If(_varname_,[_value_]) | checks if the sored named var equals the specified value.. if no value speicified the default value will be ```1``` |



## Flags definition
| Flag | Description |
|-|-|
| SPEG_CASEINSENSITIVE | Specify if matching process is case (in)sensitive | 	
| SPEG_MATCHNAMED	| Match all named returns by ```Extract``` or ```>>``` operators. clearing this flag will bypass marking matches | 
| SPEG_MATCHUNNAMED	| Store all successful matches , clearing this flag will bypass marking matches |
| SPEG_IGNORESPACES	| Will match all successive tokens whether there are spaces between them or not, ```Whitespace``` match pattern will not work here in this mode | 


## Guides and Use Cases
### Use structured Rules
It is possible to use the Rules inside each other like this
```cpp
const Rule Digit = Between('0','9');

const Rule SmallAlphabet = Between('a','z');

const Rule CapitalAlphabet = Between('A','Z');

const Rule Alphabet = CapitalAlphabet | SmallAlphabet;

const Rule Alphanumeric = Digit | Alphabet;

```
## Basic operations
the basic operation you can do with Stringozzi
1. **Test**: It validate the input string against the set rule 
  ```cpp
  Actions::Test(In("ABC"), "A")
  ```
2. **FastMatch**: like test but returns the related matches
  ```cpp
  MatchesA m;
  Actions::FastMatch(In("ABC") >> "Match" , "A", m)
  ```
3. **Search**: it searches the string till the rule applies
```cpp
bool b = Actions::Search(Is('b'), "abc"); // true
char* ptr = Actions::SearchAndGetPtr(Is('b'), "abc"); // "bc"
int idx = Actions::SearchAndGetIndex(Is('b'), "abc"); // 1
```
4. **Match**: it is the combined ioperation of **Search** + **FastMatch** ... it searches the string till the rule applies 
```cpp
  MatchesA m;
  Actions::Match(In("ABC") >> "Match" , "-----A", m)
```
  
5. **Replace**:
   searches the string till the rule applies and then replace the string match with the specified text 
```cpp
  Actions::Replace(Is("ABC"), "1234567ABC890ABC", "X", 0, 1) // "1234567X890ABC";
```
6. **Split**:
   searches the string till the rule applies and then replace the string match with the specified text 
```cpp
	vector<string> vec;
	Actions::Split(Is("<=>"), "1234567<=>ABC", vec, 0, true, 1); // ["1234567","ABC"]
```

### **Using Matches.. (Not :fire: ones :wink:)**

There are two types of expression matches 
1. __Named__ : where you set the name of the match in the rule
2. __Anynomous__ : Any other tokens matched by expression elements

you can use extract that way and get the resulting Matches
```cpp
MatchesA m;
StringozziA str(Is('K') >> "MYMATCH"); //a rule to match letter K and store it as "MYMATCH"
str.Match("K", m); // Match against string "K"

//number of total matches entries 
m.NumberOfMatches(); // == 2 => "K" and "<UNNAMED>" 

// number of MYATCH entries
m.NumberOfMatches("MYMATCH"); // == 1
m.NumberOfMatches("NOTFOUND"); // == 0
m.NumberOfMatches("<UNNAMED>"); //  == 1

m.Get("MYMATCH",0); // "K"
m.Get("MYMATCH",1); // <NULL>

```
### Case sensitivity
The default mode for Stringozzin is case sensitive
You can specify the case insensitivity in two ways
either you can specify **SPEG_CASEINSENSITIVE** in the operation 
```cpp
Actions::Test(In("ABC"), "a", SPEG_CASEINSENSITIVE);
```

Or you can use mode change in the rule it self
```cpp
Actions::Test(CaseInsensitive > In("ABC") > CaseSensitive, "a") 
```
### **- _Or_ Vs. _Greedy Or_**
Consider this example 
```cpp
Rule r = (Is("V") | Is("Via")) > Is(':') ; // will not work for Via
```
This will work with ```V:``` but not for ```Via:``` because it will always match the first letter and in the other hand ```ia:``` and ```:``` will not match due to the fact that ```OR``` operator will be short circuited to the first option and will not check the other routes

So the expression should be written like that
```cpp
Rule r = (Is("Via") | Is("V")) > Is(':') ; // Works !!
```
or using Greedy OR which seek the most benefitial match in a set of OR
```cpp
Rule r = (Is("Via") || Is("V")) > Is(':') ; // Works too !!
```

### **Recursive rules**
Sometimes we want to use the rule inside itself to check some recursive behaviour

For instance .. we want to have some sort of brackets structured data like this

```
( Data ( String ( ABC ) ) Count ( Max( 1 ) ))
```

we will face matching the parantheses properly.. the proble in such situations is we have to use an expression which is not even initialized

> **WARNING:**
> Generally take care to adjust the stopping condition in recursive rules to avoid stack overflow situation 


#### **1- ```Ref``` operator**
this is a better way .. ```Ref``` operator receives rule reference and wait for parsing to evaluate it .. the down side here is that the developer must maintain the lifetime of the reference while parsing takes place

```cpp
Rule r = Is('(') > (*Any | Ref(r) ) Is(')');
```

#### **2- Better: ```Ref``` operator with PlaceHolder**

Placeholder is a special object that fill the reference gap in expression .. and then we redirect this holder to the right object after initialization phase

Take a look
```cpp
Placeholder ph;
Rule r = Is('(') > (*Any | Ref(ph) ) Is(')');
ph.Inject(r);
```

```Inject()``` will adjust itself to point to the rule's internal object pointer which is independent of Rule lifetime

> **NOTE:**
> PlaceHolder object does not touch reference counting of internal objects, so avoid using it with other rules objects.. this may leave to dangling pointer.. instead you can directly reference it in the rule 

## Q&A
**Q. Is the library thread-safe** ?
A. Ammm..Yes and no ... the parsing expression tree is relying on reference counting which is atomic operation .. so using the same rule in multiple threads is Ok.. 
However.. the parsing process is somehow dependent on each other so the internal context and matches table are not thread safe (and it should not be )..this is a design choice to not impact the speed of parsing 
The rule of the thumb here ... keep the whole parsing cycle in the same thread.. 

**Q. why case insensitivity is not working on European languages**
A. Yes the internal 'to lower' function is ASCII only for now
it is probable to add ICU lib to support such a function in near futrue ... ( just I dont like to overcomplicate things)

