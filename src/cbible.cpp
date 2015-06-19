/*
 * cbible.cpp
 */
#include <readline/readline.h>
#include <readline/history.h>
#include <cstdlib>
#include <iostream>
#include <swmgr.h>
#include <swmodule.h>
#include <markupfiltmgr.h>

using namespace::sword;
using namespace std;

int main(int argc, char *argv[])
{
  char *buf;
  rl_bind_key('\t',rl_abort);  //disable auto-complete
  while((buf = readline("\n >> "))!=NULL)
  {
    if (strcmp(buf,"quit")==0)
      break;

    cout << buf << endl;
    if (buf[0]!=0)
      add_history(buf);
  }
  free(buf);
  return 0;
}
