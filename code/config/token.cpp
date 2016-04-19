#include "token.h"

namespace sa
{
	Token::Token()
		: m_type(Token::NO_Type) { }

	Token::Token(Type type)
		: m_type(type) { }

	Token::Token(Type type, const std::string& value)
		: m_type(type)
		, m_value(value) { }			
}
