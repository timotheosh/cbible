/*
 * Copyright 2015 Tim Hawes <tim@selfdidactic.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
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
