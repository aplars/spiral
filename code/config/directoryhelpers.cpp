#include "directoryhelpers.h"
#include <iostream>
#include <functional>
#include <algorithm>
#include <QDir>
#include <QDirIterator>

namespace sa
{
std::string DirectoryHelper::FindFile(const std::string& file)
{ 
  std::string s = FindFile(QDir::currentPath().toStdString(), file);
  return s;
}

std::string DirectoryHelper::FindFile(const std::string& currentDir, const std::string& file)
{
  bool tryAgain = true;
  QDir curPath(currentDir.c_str());

  while(tryAgain)
  {
    QDirIterator end(curPath.absolutePath(), QDirIterator::Subdirectories);

    for(; end.hasNext(); )
    {
      end.next();
      if(end.fileInfo().isFile())
      {
        QString recPath = end.filePath();
        std::string recPathFileName = end.fileName().toStdString();
        if(recPathFileName == file)
        {
          return recPath.toStdString();
        }
      }
    }

    if(!curPath.isRoot())
      curPath.cdUp();
    else
      tryAgain = false;
  }
  return "";
}

char fu(char c)
{
  if(c == '\\')
    return '/';
  return c;
}

PathString::PathString(const std::string& path)
{
  m_path = path;

  std::transform(path.begin(), path.end(), m_path.begin(), fu);
  std::string::size_type index = m_path.find_last_of('/');

  if(index != std::string::npos)
  {
    char* filePart = new char[1 + m_path.size() - index -1];
    filePart[m_path.size() - index - 1] = '\0';
    m_path.copy(filePart, m_path.size() - index -1 , index+1);
    m_filePart = filePart;
    delete []filePart; filePart = NULL;

    char* dirPart = new char[1 + m_path.size() - m_filePart.size()];
    dirPart[m_path.size() - m_filePart.size()] = '\0';
    m_path.copy(dirPart, m_path.size() - m_filePart.size(), 0);
    m_dirPart = dirPart;
    delete []dirPart; dirPart = NULL;
  }
}


}
