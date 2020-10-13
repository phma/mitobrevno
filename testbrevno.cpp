/******************************************************/
/*                                                    */
/* testbrevno.cpp - main program for testing          */
/*                                                    */
/******************************************************/
/* Copyright 2020 Pierre Abbat.
 * This file is part of Mitobrevno.
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
 */

#include <iostream>
#include <fstream>
#include "mitobrevno.h"
#include "roundup.h"

#define tassert(x) testfail|=(!(x))

using namespace std;

bool testfail=false;
vector<string> args;

void testroundUp()
{
  array<unsigned long,2> result;
  result=roundUp(342739564245);
  cout<<result[1]<<"×"<<result[0]<<'='<<result[0]*result[1]<<endl;
}

bool shoulddo(string testname)
{
  int i;
  bool ret,listTests=false;
  if (testfail)
  {
    cout<<"failed before "<<testname<<endl;
    //sleep(2);
  }
  ret=args.size()==0;
  for (i=0;i<args.size();i++)
  {
    if (testname==args[i])
      ret=true;
    if (args[i]=="-l")
      listTests=true;
  }
  if (listTests)
    cout<<testname<<endl;
  return ret;
}

int main(int argc, char *argv[])
{
  int i;
  for (i=1;i<argc;i++)
    args.push_back(argv[i]);
  if (shoulddo("roundup"))
    testroundUp();
  cout<<"\nTest "<<(testfail?"failed":"passed")<<endl;
  return testfail;
}
