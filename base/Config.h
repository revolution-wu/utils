#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <map>
#include <string>

class Config
{
 public:
  Config(const char* name);
  ~Config();
  
  //std::string tripSpace(const char* line);
  char* tripSpace(char* line);
  void addContent(const char* key, const char* value);
  void writeFile(const char* name = NULL);

  std::string getValue (const std::string& key) const;
  int readConfig(std::string* key);
  
  bool parseLine(char* line);
  const std::map<std::string, std::string>& getContent() const { return lineMaps_;}
 private:
  std::map<std::string, std::string> lineMaps_;  
  std::string configPath_;
};


#endif   /* __CONFIG_H__ */
