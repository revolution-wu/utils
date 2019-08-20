/**************************************************************************
 *      FileName: Config_test.cpp
 * 
 *        Author: 
 *   Description: ---
 *  Created Time: 2019年08月13日 星期二 17时25分27秒
 * Last Modified: 2019-08-13 17:25:27
 * 
 **************************************************************************/
#include "../Config.h"
#include <iostream>
#include <time.h>
using namespace std;
using namespace utils;
int main(int argc, char *argv[])
{
  Config conf("2.txt");
  conf.addContent("listen", "80");
  conf.addContent("requesttype", "GET");
  conf.addContent("length", "100");
  conf.addContent("version", "HTTP1.1");
  conf.addContent("message", "ok");

  conf.writeFile();
  std::string result;
  conf.readConfig(&result);
  std::string value = conf.getValue("listen");
  cout << " vlaue " << value << endl;

  time_t now = time(NULL);
  printf("%lu\n", now);
  time_t start = now / 86400 * 86400;
  printf("%lu\n", start);
  return 0;
}
