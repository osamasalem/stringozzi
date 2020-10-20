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

## Why Stringozzi
1. Wide cross C++ standard compliance (this version is compliant to C++97 and upward)
2. 3x-10x* faster than std::regex VS/GCC
3. Lean and mean .. use only feature that you need to ( Validate / Search distinction and disable matching results ) 
4. the grammer is checked by C++ compiler rather than wasting time debugging regex expressions
5. new features not in Regex like Support for recursive expressions, character chains detection , conditional parsing and (A)BNF like rules 
6. Seamless support for UTF and Unicode string with mix and match
7. support for long expressions and text
8. Packed with useful built-in expressions (IPv4,URI, ServerName) 

## Why not Stringozzi
1. You don't like pasta or Italian couisine 
2. The expressions you use are too short
3. The program loads validation expressions from remote source(like DB or text files)
  
 ## Example for What you can Do
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

  StringozziA(r).Test("Student No#: 434");
}
```
### Rules and Operators

| Rule | Description |
|-|-|
| Any | Any Character |
| Digit | Any Character between '0' and '9'|
| Alphabet | Any Character between 'A' and 'Z'|
| Whitespace | Any space Character |
| _a_>_b_ | Perform parsing using rule _a_ then rule _b_ |
| !_a_ | Negate parsing rule _a_, this action does not move parsing pointer |
| _a_&_b_ | Greedy and: Rule _a_ and Rule _b_, the token must comply with both rules, if they are both true it will apply the  |
| _a_\|_b_ | Rule a or Rule b, the token may comply with either rules, **short circuit applies here, so start always with most specific rule to more general ones OR use Greedy OR operator** |
| _a_\|\|_b_ | Rule _a_ or Rule _b_, the token may comply with either rules, it always checks all rules and takes the most relevant one in expense for performance |
| *_a_ | Zero Or More  Mtahces a single instance or multiiple instances **Take care when using this rule it can match any thing**|
|+_a_| One Or More (at least rule), Matches a single instance or multiiple instances |
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

## Use Cases
**- _Or_ Vs. _Greedy Or_**
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

**-Recursive rules**
Sometimes we want to use the rule inside itself to check some recursive behaviour

For instance .. we want to have some sort of brackets structured data like this

```
( Data ( String ( ABC ) ) Count ( Max( 1 ) ))
```

we will face matching the parantheses properly.. the proble in such situations is we have to use an expression which is not even initialized

we have three ways to do it
**1- Unrecommended way: direct reference**
The problem with this method is the r is not initialized so the library check for some magic number for object validation to distinguish this scenario ... which is not reliable enough .. this is just for fallback situations

```cpp
Rule r = Is('(') > (*Any | r ) Is(')');
```

**2-Better: ```Ref``` operator**
this is a better way .. ```Ref``` operator receives rule reference and wait for parsing to evaluate it .. the down side here is that the developer must maintain the lifetime of the reference while parsing takes place

```cpp
Rule r = Is('(') > (*Any | Ref(r) ) Is(')');
```

**3-Best: ```Ref``` operator with PlaceHolder**

Placeholder is a special object that fill the reference gap in expression .. and then we redirect this holder to the right object after initialization phase

Take a look
```cpp
Placeholder ph;
Rule r = Is('(') > (*Any | Ref(ph) ) Is(')');
ph.Inject(r);
```

```Inject()``` will adjust itself to point to the rule's internal object pointer which is independent of Rule lifetime

> **NOTE:**
> PlaceHolder object does not touch reference counting of internal objects, so avoid using it with other rules objects.. this may leave to dangling pointer

## Perfromance 
This is tested on MS VS C++ 2015/GCC 8.3 