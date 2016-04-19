#include "parser.h"

namespace sa
{
Parser::Parser(const std::string& directory, const std::string& file, const std::string& hostName)
  : m_lexer(directory+file)
  ,  m_hostName(hostName)
{
  m_lexer.GetNextToken(m_lookahead);
  m_symboltable["PATH"] = directory;
}

bool Parser::Parse()
{
  bool isOk = true;

  SymbolTable symbolTable;
  if(!Exprs(true, symbolTable))
    isOk = false;

  SymbolTableOverrides overrides;

  if(isOk && !Overrides(overrides))
    isOk = false;
  if(isOk && !Match(Token::EOF_Type))
    isOk = false;

  return isOk;
}

bool Parser::Match(Token::Type t)
{
  bool isOk = true;
  if(t != m_lookahead.GetType())
  {
    isOk = false;
  }
  else
  {
    m_lexer.GetNextToken(m_lookahead);
  }
  return isOk;
}


bool Parser::Lft(std::string& lft)
{
  bool isOk = true;
  std::string var = m_lookahead.GetValue();
  if(Match(Token::Variable_Type))
    lft = var;
  else
    isOk = false;
  return isOk;
}

bool Parser::Rth(std::string& rth)
{
  bool isOk = true;
  if(m_lookahead.GetType() == Token::Variable_Type || m_lookahead.GetType() == Token::String_Type)
  {
    std::string all;
    std::string vars;
    if(!Vars(vars))
    {
      isOk = false;
    }
    else
    {
      std::string strs;
      if(!Strings(strs))
      {
        isOk = false;
      }
      else
      {
        all.append(vars);
        all.append(strs);
        std::string srth;
        if(!Rth(srth))
        {
          isOk = false;
        }
        else
        {
          all.append(srth);
          rth = all;
        }
      }
    }
  }
  else
  {
    rth = "";
  }
  return isOk;
}

bool Parser::Exp(Parser::Symbol& symbol)
{
  bool isOk = true;
  std::string varName;
  if(!Lft(varName))
    isOk = false;

  if(isOk && !Match(Token::Assign_Type))
  {
    isOk = false;
  }

  std::string varValue;
  if(isOk && !Rth(varValue))
    isOk = false;
  if(isOk && !Match(Token::SemiColon_Type))
    isOk = false;
  if(isOk)
    symbol = Symbol(varName, varValue);
  return isOk;
}

bool Parser::Exprs(bool setSymTable, Parser::SymbolTable& symTable)
{
  bool isOk = true;
  SymbolTable allExprs;

  if(m_lookahead.GetType() == Token::Variable_Type)
  {
    Symbol exp;
    if(!Exp(exp))
      isOk = false;

    if(isOk)
    {
      if(setSymTable)
      {
        m_symboltable[exp.first] =  exp.second;
      }
      allExprs.insert(exp);
      SymbolTable exprs;
      if(!Exprs(setSymTable, exprs))
      {
        isOk = false;
      }
      else
      {
        allExprs.insert(exprs.begin(), exprs.end());
        symTable = allExprs;
      }
    }
  }
  return isOk;
}

bool Parser::Overrides(Parser::SymbolTableOverrides& symTableOverrides)
{
  bool isOk = true;
  SymbolTableOverrides allOverrides;
  if(m_lookahead.GetType() == Token::Override_Type)
  {
    Match(Token::Override_Type);
    std::string overrideName = m_lookahead.GetValue();

    Match(Token::String_Type);
    Match(Token::LeftBrace_Type);

    if(!Exprs(overrideName==m_hostName, allOverrides[overrideName]))
      isOk = false;

    if(isOk && !Match(Token::RightBrace_Type))
      isOk = false;

    SymbolTableOverrides subOverrides;
    if(isOk && !Overrides(subOverrides))
      isOk = false;
    else
      allOverrides.insert(subOverrides.begin(), subOverrides.end());
  }
  symTableOverrides = allOverrides;
  return isOk;
}

bool Parser::Vars(std::string& vars)
{
  bool isOk = true;
  if(m_lookahead.GetType() == Token::Variable_Type)
  {
    std::string value;
    std::string variable = m_lookahead.GetValue();
    if(!Match(Token::Variable_Type))
    {
      isOk = false;
    }
    else
    {
      SymbolTable::const_iterator it = m_symboltable.find(variable);
      if(it != m_symboltable.end())
      {
        value = it->second;
      }
      std::string svars;
      Vars(svars);
      value.append(vars);
      vars = value;
    }
  }
  else
    vars = "";

  return isOk;
}

bool Parser::Strings(std::string& strs)
{
  bool isOk = true;
  if(m_lookahead.GetType() == Token::String_Type)
  {
    std::string str = m_lookahead.GetValue();
    if(!Match(Token::String_Type))
      isOk = false;
    else
    {
      std::string sstrings;
      Strings(sstrings);
      str.append(sstrings);
      strs = str;

      //std::cout << strs;
    }
  }
  else
    strs = "";

  return isOk;
}

}
