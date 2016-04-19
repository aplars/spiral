#include "config.h"
#include "directoryhelpers.h"
#include <QHostInfo>

namespace sa
{
  bool ConfigurationManager::init(const std::string& configFile)
  {
    bool isOk = true;
    DirectoryHelper dirH;
    std::string fullPath =  dirH.FindFile(configFile);
    if(fullPath.size() <= 0)
    {
      isOk = false;
    }
    else
    {
      PathString pathStr(fullPath);

      Parser parser(pathStr.GetDirPart(), pathStr.GetFilePart(), getMyHostName());
      if(!parser.Parse())
        isOk = false;
      else
        m_symbols = parser.GetSymbolTable();
    }
    return isOk;
  }

  std::string ConfigurationManager::getParam(const std::string& key) const
	{
		Parser::SymbolTable::const_iterator it = m_symbols.find(key);
	
		if(it != m_symbols.end())
		{
			return (it->second);
		}

		return "";
	}

  std::string ConfigurationManager::getMyHostName()
	{
    QString qHostName = QHostInfo::localHostName();
    std::string hostName = "";//qHostName.toStdString();
    return hostName;
	}
}
