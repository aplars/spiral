#pragma once
#include <string>
#include <map>
#include "lexer.h"
#include "token.h"


namespace sa
{
	class Parser
	{
	public:	
		typedef std::map<std::string, std::string> SymbolTable; 
		typedef std::pair<std::string, std::string> Symbol;
		typedef std::map<std::string, SymbolTable> SymbolTableOverrides;

		///Creates parser. 
		Parser(const std::string& directory, const std::string& file, const std::string& hostName);

		///Run the parser.
    bool Parse();

		///Return the symbol table.
		SymbolTable GetSymbolTable() const { return m_symboltable; }
	private:
		///Return the next token.
		Token GetLookAhead() const { return m_lookahead; }

		///Matches a token.
    bool Match(Token::Type t);
		//Parses the left side in an expression.
    bool Lft(std::string& lft);

		//Parses the right side in an expression.
    bool Rth(std::string& rth);
		
		//Parses a full expression. Eg. $A = $B"/Kalle";
    bool Exp(Symbol& symbol);

		//Parses a list of expressions.
    bool Exprs(bool setSymTable, SymbolTable& symTable);

		//Parses the ovverides.
    bool Overrides(SymbolTableOverrides& symTableOverrides);
		
		//Parses the variables
    bool Vars(std::string& vars);

		//Parses the strings
    bool Strings(std::string& strs);

		Lexer m_lexer;
		Token m_lookahead;
		std::string m_hostName;
		SymbolTable m_symboltable;
		
	};
}
