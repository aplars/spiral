#include "lexer.h"
#include <string>

namespace sa
{

	Lexer::Lexer(const std::string& file)
		: m_line(0)
    , m_stream(file.c_str(), std::ios::in)
	{ }
	
  bool Lexer::GetNextToken(Token& outToken) const
	{
    bool isOk = true;
    outToken = Token(Token::NO_Type);
    while(isOk && outToken.GetType() == Token::NO_Type)
		{
			char c = GetNextChar();
			
			if(m_stream.eof())
			{
        outToken = Token(Token::EOF_Type);
			}

      else if(c == '\n')
			{
				++m_line;
				continue;
			}
			else if(c == ' ')
			{
				continue;
			}
			else if(c == '\t')
			{
				continue;
			}
			else if(c == '=')
			{
        outToken = Token(Token::Assign_Type);
			}
			else if(c == '{')
			{
        outToken = Token(Token::LeftBrace_Type);
			}
			else if(c == '}')
			{
        outToken = Token(Token::RightBrace_Type);
			}
			else if(c == ';')
			{
        outToken = Token(Token::SemiColon_Type);
			}
			else if(c == '$')
			{
				std::string value;
				char cc = GetNextChar();
				if(IsValidVariableStartChar(cc))
				{
					while(IsValidVariableChar(cc) && !m_stream.eof())
					{
						value+=cc;
						cc = GetNextChar();
					}
				}
				else
				{
          //throw SyntaxErrorException("Syntax error when reading variable", "Lexer::GetNextToken", __LINE__, __FILE__);
          isOk = false;
        }
				//Jump back one char
				m_stream.unget();
        outToken = Token(Token::Variable_Type, value);
			}
			else if (c == '\"')
			{
				char cc = GetNextChar();
				std::string value;
				while(cc != '\"' && !m_stream.eof())
				{
					value += cc;
					cc = GetNextChar();
				}
				if(m_stream.eof())
				{
          //throw SyntaxErrorException("Syntax error when reading variable content", "Lexer::GetNextToken", __LINE__, __FILE__);
          isOk = false;
        }

        outToken = Token(Token::String_Type, value);
			}
			else //Check for reserved words
			{
				std::string value;
				char cc = c;
				if(IsValidReservedWordStartChar(cc))
				{

					while(IsValidReservedWordChar(cc) && !m_stream.eof())
					{
						value += cc;
						cc = GetNextChar();
					}
					m_stream.unget();
					if(value == "OVERRIDE")
					{
            outToken = Token(Token::Override_Type, value);
					}
					else
					{
						//Error
            isOk = false;
          }
				}
				else
				{
					//Error
          isOk = false;
        }
			}
      //if(outToken.GetType() != Token::NO_Type)
        //return isOk;
		}
    return isOk;
	}

	char Lexer::GetNextChar() const
	{
		return static_cast<char>(m_stream.get());
	}
	
	bool Lexer::IsValidVariableStartChar(char c)
	{
		return isalpha(c) || c == '_';
	}

	bool Lexer::IsValidVariableChar(char c)
	{
		return isalnum(c) || c == '_';
	}

	bool Lexer::IsValidReservedWordStartChar(char c)
	{
		return isalpha(c) || c == '_';
	}

	bool Lexer::IsValidReservedWordChar(char c)
	{
		return isalnum(c) || c == '_';
	}
}
