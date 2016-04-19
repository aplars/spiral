#pragma once
#include <fstream>
#include <istream>
#include "token.h"

namespace sa
{
	class Lexer
	{
	public:
		Lexer(const std::string& file);
    bool GetNextToken(Token& outToken) const;
	private:
		char GetNextChar() const;
		static bool IsValidVariableStartChar(char c);
		static bool IsValidVariableChar(char c);
		static bool IsValidReservedWordStartChar(char c);
		static bool IsValidReservedWordChar(char c);
		mutable unsigned int m_line;
		mutable std::ifstream m_stream;
	};
}
