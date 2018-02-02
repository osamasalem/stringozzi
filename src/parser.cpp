// parser.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#define DLLEXPORT
#include "parser.h"
#include <stdlib.h>  

using namespace Stringozzi;
using namespace Stringozzi::Rules;
using namespace Stringozzi::Rules::Manipulators;
using namespace Stringozzi::Rules::Filters;
using namespace Stringozzi::Rules::Utils;

LIB_REFERENCE Stringozzi::Rules::Filters::Any Stringozzi::Rules::Filters::ANY;
LIB_REFERENCE Stringozzi::Rules::Filters::Alphabet Stringozzi::Rules::Filters::ALPHABET;
LIB_REFERENCE Stringozzi::Rules::Filters::AlphaNumeric Stringozzi::Rules::Filters::ALPHANUMERIC;
LIB_REFERENCE Stringozzi::Rules::Filters::Digit Stringozzi::Rules::Filters::DIGIT;
LIB_REFERENCE Stringozzi::Rules::Filters::Decimal Stringozzi::Rules::Filters::DECIMAL;
LIB_REFERENCE Stringozzi::Rules::Filters::EndOfLine Stringozzi::Rules::Filters::EOL;
LIB_REFERENCE Stringozzi::Rules::Filters::EndOfText Stringozzi::Rules::Filters::EOT;
LIB_REFERENCE Stringozzi::Rules::Filters::Hex Stringozzi::Rules::Filters::HEX;
LIB_REFERENCE Stringozzi::Rules::Filters::WhiteSpace Stringozzi::Rules::Filters::WHITESPACE;
LIB_REFERENCE Stringozzi::Rules::Filters::Word Stringozzi::Rules::Filters::WORD;
LIB_REFERENCE Stringozzi::Rules::Utils::Email Stringozzi::Rules::Utils::EMAIL;
LIB_REFERENCE Stringozzi::Rules::Utils::Host Stringozzi::Rules::Utils::HOST;
LIB_REFERENCE Stringozzi::Rules::Utils::IPv4 Stringozzi::Rules::Utils::IPV4;
LIB_REFERENCE Stringozzi::Rules::Utils::Phone Stringozzi::Rules::Utils::PHONE;
LIB_REFERENCE Stringozzi::Rules::Utils::ServerAddress Stringozzi::Rules::Utils::SERVERADDRESS;
LIB_REFERENCE Stringozzi::Rules::Utils::Uri Stringozzi::Rules::Utils::URI;

namespace Stringozzi
{
	namespace Rules
	{
		/*********************************************************************************/
		//bool RulesSet::Check(const Char** _Stream) const 
		//{
		//	return tok.Check(_Stream);
		//}
		
		namespace Manipulators
		{
			bool Not::Check(const Char** _Stream)const 
			{
				const Char* _Start = *_Stream;
				if (!_Tokenizer.Check(&_Start))
				{
					(*_Stream)++;
					return true;
				}
				return false;
			}

			bool Or::Check(const Char** _Stream)const 
			{
				const Char* _Start = *_Stream;
				if (!_TokenizerA->Check(&_Start))
				{
					if (!_TokenizerB->Check(&_Start))
					{
						return false;
					}
				}
				*_Stream = _Start;
				return true;
			}

			bool And::Check(const Char** _Stream)const 
			{
				const Char *_Start1,*_Start2;

				_Start1 = *_Stream;
				if (!_TokenizerA->Check(&_Start1))
					return false;

				_Start2 = *_Stream;
				if (!_TokenizerB->Check(&_Start2))
					return false;

				_Start1 = ((_Start2 < _Start1) ? _Start2 : _Start1);

				*_Stream = _Start1;
				return true;
			}

			bool Sequence::Check(const Char** _Stream)const 
			{
			
				const Char* _Start = *_Stream;

				if (!_TokenizerA.Check(&_Start))
					return false;

				if (!_TokenizerB.Check(&_Start))
					return false;

				*_Stream = _Start;
				return true;

			}

			bool Within::Check(const Char** _Stream)const 
			{
				const Char* _Start = *_Stream;
				unsigned long i = 0;
				for (; i < max; i++)
				{
					if (!_Tokenizer.Check(&_Start))
						break;
				}

				if (i >= min && i <= max)
				{
					*_Stream = _Start;
					return true;
				}

				return false;
			}

			bool Times::Check(const Char** _Stream)const 
			{
				return Within(max, max, _Tokenizer).Check(_Stream);
			}

			bool OneOrMore::Check(const Char** _Stream)const 
			{
				return Within(1, -1, _Tokenizer).Check(_Stream);
			}

			bool ZeroOrOne::Check(const Char** _Stream)const 
			{
				return Within(0, 1, _Tokenizer).Check(_Stream);
			}

			bool While::Check(const Char** _Stream)const 
			{
				return Within(0, -1, _Tokenizer).Check(_Stream);
			}

			bool Until::Check(const Char** _Stream)const 
			{
				return (While(Not(_Tokenizer))).Check(_Stream);
			}

			bool Extract::Check(const Char** _Stream)const 
			{
				const Char* _Start = *_Stream;
				bool _Result = _Tokenizer.Check(&_Start);
				*_Input = 0;
				STRCAT(_Input,*_Stream,(_Start - *_Stream));
				*_Stream = _Start;
				return _Result;
			}

			bool ReturnToCallback::Check(const Char** _Stream)const 
			{
				const Char* _Start = *_Stream;
				STDSTRING _Output;
				bool _Result = _Tokenizer.Check(&_Start);
				_Output.assign(*_Stream,_Start);
				cb(_Output.c_str());
				*_Stream = _Start;
				return _Result;
			}

			bool Enclosed::Check(const Char** _Stream)const 
			{
				return (Filters::Exact(open) + _Tokenizer + Filters::Exact(close)).Check(_Stream);
			}


		}

		namespace Filters
		{
			bool Any::Check(const Char** _Stream)const 
			{
				(*_Stream)++;
				return true;
			}

			bool In::Check(const Char** _Stream)const 
			{
				if (!_Input || !*_Input )
					return false;
				
				if(!_Stream || !*_Stream || !**_Stream)
					return false;

				if (STRCHR(_Input, **_Stream) != 0)
				{
					(*_Stream)++;
					return true;
				}
				return false;
			}

			bool Between::Check(const Char** _Stream)const 
			{
				if (**_Stream >= _Lower && **_Stream <= _Upper)
				{
					(*_Stream)++;
					return true;
				}
				return false;
			}


			bool Alphabet::Check(const Char** _Stream)const 
			{
				return (Between(_C('a'), _C('z')).Check(_Stream) | Between(_C('A'), _C('Z')).Check(_Stream));
			}


			bool Digit::Check(const Char** _Stream)const 
			{
				return Between(_C('0'), _C('9')).Check(_Stream);
			}


			bool Hex::Check(const Char** _Stream)const 
			{
				return (Between(_C('0'), _C('9')) 
					| Between(_C('a'), _C('f')) 
					| Between(_C('A'), _C('F'))).Check(_Stream);
			}

			bool AlphaNumeric::Check(const Char** _Stream)const 
			{
				return (ALPHABET.Check(_Stream) | DIGIT.Check(_Stream));
			}

			bool Exact::Check(const Char** _Stream)const 
			{
				const Char* _StreamCursor = *_Stream;
				const Char* _InputCursor = _Input;
				if (!*_InputCursor)
					return false;

				while (*_InputCursor)
				{
					if (*_StreamCursor != *_InputCursor)
						return false;
					_StreamCursor++;
					_InputCursor++;
				}
				*_Stream = _StreamCursor;
				return true;

			}


			bool Like::Check(const Char** _Stream)const 
			{
				const Char* _StreamCursor = *_Stream;
				const Char* _InputCursor = _Input;
				while (*_InputCursor)
				{
					if (tolower(*_StreamCursor) != tolower(*_InputCursor))
						return false;
					_StreamCursor++; 
					_InputCursor++;
				}
				*_Stream = _StreamCursor;
				return true;

			}

			bool WhiteSpace::Check(const Char** _Stream)const 
			{
				return In(_C(" \t\n\r")).Check(_Stream);
			}

			bool Is::Check(const Char** _Stream)const 
			{
				if (**_Stream == _Letter)
				{
					(*_Stream)++;
					return true;
				}
				return false;
			}

			bool Decimal::Check(const Char** _Stream)const 
			{
				return OneOrMore(DIGIT).Check(_Stream);
			}

			bool Word::Check(const Char** _Stream)const 
			{
				return OneOrMore(ALPHABET).Check(_Stream);
			}

			bool EndOfText::Check(const Char** _Stream)const 
			{
				return (**_Stream == _C('\0'));
			}


			bool EndOfLine::Check(const Char** _Stream)const 
			{
				return Exact(_C("\r\n")).Check(_Stream);
			}

			bool Integer::Check(const Char** _Stream)const 
			{
				const Char* _Temp = *_Stream;
				if (!*_Temp)
					return false;
				int _Result = STRTOL(_Temp, (Char**) &_Temp, 10);
				if (_Temp == *_Stream)
					return false;
				if (_Result >= _Min && _Result <= _Max)
				{
					*_Stream = _Temp;
					return true;
				}
				return false;
			}

			bool Float::Check(const Char** _Stream)const 
			{
				const Char* temp = *_Stream;
				if (!*temp) 
					return false;
				float ret = STRTOF(temp, (Char**) &temp);
				if (temp == *_Stream)	
					return false;
				if (ret >= _Min && ret <= _Max)
				{
					*_Stream = temp;
					return true;
				}
				return false;
			}
		}

	
		namespace Utils
		{

			bool IPv4::Check(const Char** _Stream)const 
			{
				return (3 * ((Within(1,3,DIGIT) & Integer(255)) + Is(_C('.'))) + (Within(1,3,DIGIT) 
						& Filters::Integer(255))).Check(_Stream);
			}
			/*********************************************************************************/

			bool Host::Check(const Char** _Stream)const 
			{
				return ( IPV4 | (OneOrMore(ALPHANUMERIC) + ZeroOrMore(Is(_C('-')) + OneOrMore(ALPHANUMERIC))
					+ ZeroOrMore( Is(_C('.')) + OneOrMore(ALPHANUMERIC) + ZeroOrMore(Is(_C('-')) 
						+ OneOrMore(ALPHANUMERIC))) ) ) .Check(_Stream);
			}
			/*********************************************************************************/

			bool Email::Check(const Char** _Stream)const 
			{
				return (OneOrMore(ALPHANUMERIC | In(_C("!#$%&'*+/=?^_`{|}~-"))) + ZeroOrMore(Is('.') + (ALPHANUMERIC
					| In(_C("!#$%&'*+/=?^_`{|}~-")))) + Is('@') + HOST).Check(_Stream);
			}
			/*********************************************************************************/

			bool Phone::Check(const Char** _Stream)const 
			{
				return (ZeroOrMore(Is(_C('+'))) + (ZeroOrMore(Is(_C('(')) + OneOrMore(DIGIT) + Is(_C(')')))
					+ ZeroOrMore(WHITESPACE)) + OneOrMore(DIGIT) + ZeroOrMore(In(_C(" -"))
						+ OneOrMore(DIGIT))).Check(_Stream);
			}

			/*********************************************************************************/

			bool ServerAddress::Check(const Char** _Stream)const 
			{
				return (HOST + ZeroOrOne(Is(_C(':')) + Integer(1, 65535))).Check(_Stream);
			}
			/*********************************************************************************/
			bool Uri::Check(const Char** _Stream)const 
			{
				return (OneOrMore(ALPHANUMERIC) + Is(_C(':')) + (OneOrMore(ALPHANUMERIC | In(_C("!#$%&'*+/=?^_`{|}~-"))) 
					+ ZeroOrMore(Is('.') + (ALPHANUMERIC | In(_C("!#$%&'*+/=?^_`{|}~-"))))) + Is(_C('@'))
					+ SERVERADDRESS ).Check(_Stream);
			}


		}

	}

	/*********************************************************************************/

	StringProcessor::StringProcessor(const Char* str)	
	{
		string=str;
		cursor=str;
		lasttokenized.resize(512);
		lasttokenized.clear();
	}

	bool StringProcessor::Parse(const Rules::TokenizerInterface& tok )
	{
		const Char* _Start = cursor;
		bool _Result = tok.Check(&cursor);
		if(_Result)	lasttokenized.assign(_Start, cursor);
		return  _Result	;
	}

	bool StringProcessor::Validate(const Rules::TokenizerInterface& tok)
	{
		return (tok + EndOfText()).Check(&cursor);
	}

	const Char* StringProcessor::Search(const Rules::TokenizerInterface& tok)
	{
		bool _Result = (Until(EOT | tok)).Check(&cursor);
		if (_Result && *cursor!=0)
		{
			const Char* _Start = cursor;
			(tok).Check(&cursor);
			return _Start;
		}
		return 0;
	}


	int StringProcessor::GetLastParserPosition()
	{
		return ((int)(cursor - string));
	}
	
	void StringProcessor::Push()
	{
		savedpositions.push(cursor);
	}

	void StringProcessor::Pop()
	{
		if (!savedpositions.empty())
		{
			cursor = savedpositions.top();
			savedpositions.pop();
		}
	}

	bool StringProcessor::IsEOT()
	{
		return ((*cursor) == _C('\0'));
	}

	void StringProcessor::Reset()
	{
		lasttokenized.clear();
		cursor = string;
	}

}


