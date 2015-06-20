/*
 * cbible.cpp
 */
#include <readline/readline.h>
#include <readline/history.h>
#include <iostream>
#include "SwordFuncs.hpp"

using namespace::sword;
using namespace std;

int main(int argc, char *argv[])
{
  char *buf;
  SwordFuncs *sw = new SwordFuncs();
  rl_bind_key('\t',rl_abort);  //disable auto-complete
  while((buf = readline("\n >> "))!=NULL)
  {
    if (strcmp(buf,"quit")==0)
      break;

    string ref = buf;
    cout << sw->lookup(ref) << endl;
    if (buf[0]!=0)
      add_history(buf);
  }
  free(buf);
  free(sw);
  return 0;
}
