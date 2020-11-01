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
#include <iomanip>
#include <map>
#include <algorithm>
#include "mitobrevno.h"
#include "interval.h"
#include "itree.h"

using namespace std;
using namespace mitobrevno;

void outputAligned(vector<Interval *> intervals)
{
  int i,j;
  map<int,int> maxWidths;
  vector<string> repre;
  for (i=0;i<intervals.size();i++)
  {
    repre=intervals[i]->toStrings();
    for (j=0;j<repre.size();j++)
      if (repre[j].length()>maxWidths[j])
	maxWidths[j]=repre[j].length();
  }
  for (i=0;i<intervals.size();i++)
  {
    repre=intervals[i]->toStrings();
    for (j=0;j<repre.size();j++)
    {
      if (j)
	cout<<' ';
      cout<<setw(maxWidths[j])<<repre[j];
    }
    cout<<endl;
  }
}

bool midBefore(Interval *a,Interval *b)
{
  return a->start+a->end<b->start+b->end;
}

int main(int argc, char *argv[])
{
  MbHeader mbHeader;
  int nEvents=0;
  int64_t startView,endView,midView;
  IntervalRange range;
  vector<Interval *> inView;
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
  cout<<"Events range from "<<intervalTree.getStart()<<" to "<<intervalTree.getEnd()<<endl;
  inView=intervalTree.matchingIntervals(8194,range);
  cout<<inView.size()<<" intervals are block writes\n";
  sort(inView.begin(),inView.end(),midBefore);
  //outputAligned(inView);
  midView=(intervalTree.getStart()+intervalTree.getEnd())/2;
  startView=midView-500000000;
  endView=midView+500000000;
  range.firstEnd=startView;
  range.lastStart=endView;
  inView=intervalTree.matchingIntervals(INT_MIN,range);
  cout<<inView.size()<<" intervals overlap middle 1 s\n";
  sort(inView.begin(),inView.end(),midBefore);
  outputAligned(inView);
  return 0;
}
