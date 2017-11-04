#pragma once

#include "parser.h"
#include <string>

namespace sa
{
  ///configuration manager used for lookup of parameters stored in file.
	/**

	*/
  class Config
	{
	public:
		///Starts the configuration manager looking for the file 'configFile'.
    bool init(const std::string& configFile);

		///Returns a parameter. On fail empty string is returned.
    std::string getParam(const std::string& key) const;

		///Returns the path to the configuration file.
    std::string getPathToConfigFile() const { return getParam("PATH"); }

	private:
		static std::string getMyHostName();

		Parser::SymbolTable m_symbols;
	};
}
