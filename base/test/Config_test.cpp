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

using namespace std;
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
  return 0;
}
