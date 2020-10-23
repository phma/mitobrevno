/******************************************************/
/*                                                    */
/* fuzzbrevno.cpp - main program for fuzzing          */
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
#include "interval.h"

using namespace std;
using namespace mitobrevno;

int main(int argc, char *argv[])
{
  MbHeader mbHeader;
  int nEvents=0;
  //vector<MbEvent> events;
  MbEvent event;
  string filename;
  if (argc>1)
  {
    filename=argv[1];
    mbHeader=openLogFileRead(filename);
    cout<<"Started at "<<mbHeader.startTime*mbHeader.num/(double)mbHeader.den<<endl;
    while (true)
    {
      event=readEvent();
      if (event.time==0)
	break;
      intervalize(event,mbHeader.startTime);
      nEvents++;
    }
    cout<<nEvents<<" events read\n";
  }
  treeize();
  return 0;
}
