#include "Config.h"
#include <stdio.h>
#include <string.h>
Config::Config(const char* name)
  :configPath_(name)
{

}

Config::~Config()
{}
void Config::addContent(const char* key, const char* value)
{
  
  if(key == NULL || value == NULL)
  {
    return;
  }

  
  std::map<std::string, std::string>::iterator it = lineMaps_.find(key);
  if(it == lineMaps_.end())
  {
    lineMaps_.insert(std::make_pair(key, value));
  }
  else
  {
    it->second = value;
  }
 
}

void Config::writeFile(const char* name)
{
  FILE* fp = NULL;
  if(!name)
  {
    fp = fopen(configPath_.c_str(), "w");
  }
  else
  {
    fp = fopen(name, "w");
  }

  if(fp == NULL)
  {
    return;
  }
  std::string line;
  line.reserve(256);
  for(const auto& it : lineMaps_)
  {
    line = it.first;
    line += ": " ;
    line += it.second;
    line += '\n';
    int ret = fwrite(line.c_str(), line.size(), 1, fp);
    if(ret != 1)
    {
      return;
    }
  }
  fclose(fp);
}

std::string Config::getValue(const std::string& key) const
{
  std::string result;
  std::map<std::string, std::string>::const_iterator it = lineMaps_.find(key);
  if(it != lineMaps_.end())
  {
    result = it->second;
  }
  return result;
}

int Config::readConfig(std::string* key)
{


  FILE* fp = fopen(configPath_.c_str(), "r");
  if(!fp)
  {
    return -1;
  }
  
  char buf[256];
  while(true)
  {
    memset(buf, 0, sizeof buf);
    char* line = fgets(buf, sizeof buf, fp);
    if(line == NULL)
    {
      break;
    }
    else
    {
      int len = strlen(buf);
      if(buf[len - 1] == '\n')  //remove '\n';
      {
        buf[len - 1] = '\0';
      }
      if(buf[0] == '#')  //remove begin #
      {
        buf[0] = '\0';
      }
    }
    if(strlen(buf) == 0)
    {
      continue;
    }
    if(parseLine(buf))
    {
      key->append(line);
    }
  }

  fclose(fp);
  return 0;
}

char* Config::tripSpace(char* line)
{
  if(line == NULL)
  {
    return NULL;
  }

  char* start = line;
  char* end = line + strlen(line) - 1;
  while(isspace(*start))
  {
    start++;
  }
  
  
  while(isspace(*end))
  {
    end--;
  }
  
  int len = end + 1 - start; 
  end += 1;
  *end = 0;
  if(len < 0)
  {
    return NULL;
  }
  //这样效率不高，频繁创建销毁
  //std::string s (start, start + len);
  return start ;
}

bool Config::parseLine(char* line)
{
  if(line == NULL)
  {
    return false;
  }
  
  char* start = line;
  char* space = strchr(start, ':');
  *space = 0;
  const char* key = tripSpace(start);
  const char* value = tripSpace(space + 1);
  
  if(key && value)
  {
    lineMaps_.insert(std::make_pair(key, value));
  }

  return true;
}
