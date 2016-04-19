#pragma once
#include <string>

namespace sa
{
	///Represents a token in a configuration file.
	class Token
	{
	public:
		enum Type
		{
			NO_Type,
			EOF_Type,
			Variable_Type,
			String_Type,
			Assign_Type,
			SemiColon_Type,
			LeftBrace_Type,
			RightBrace_Type,
			Override_Type
		};
		///Creates a token with no type.
		/**
		Creates a token with no type. 
		Just here because we want it to be storable in stl containers.
		*/
		Token();

		///Creates a token.
		Token(Type type);

		///Creates a token with a value.
		Token(Type type, const std::string& value);
		
		///Returna the token's type.
		Type GetType() const { return m_type; }

		///Returns the token's value.
		const std::string& GetValue() const { return m_value; }
	private:
		Type m_type;
		std::string m_value;
	};
}
