/******************************************************/
/*                                                    */
/* interval.cpp - event intervals                     */
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

#include <vector>
#include "interval.h"
#include "itree.h"

using namespace std;
using namespace mitobrevno;

/* In addition to the event types listed in mitobrevno.h, this module
 * generates events of type 0xfffffnnn, which are errors.
 */

vector<MbEvent> pending;
vector<Interval> intervals;

void storeInterval(Interval &iv)
{ // stub
  intervals.push_back(iv);
}

bool sameButTime(const MbEvent &a,const MbEvent &b)
{
  bool ret=eventBase(a.eventType)==eventBase(b.eventType) && a.thread==b.thread;
  int i;
  for (i=0;ret && i<a.intParams.size();i++)
    if (a.intParams[i]!=b.intParams[i])
      ret=false;
  for (i=0;ret && i<a.floatParams.size();i++)
    if (a.intParams[i]!=b.floatParams[i])
      ret=false;
  return ret;
}

bool stateChanged(const MbEvent &a,const MbEvent &b)
{
  bool ret=a.eventType==b.eventType && a.thread==b.thread && (b.eventType&~0x1000)==0x1000;
  return ret;
}

void intervalize(MbEvent &event,uint64_t startTime)
{
  int i;
  Interval iv;
  iv.start=iv.end=event.time-startTime;
  iv.thread=event.thread;
  iv.eventType=event.eventType;
  iv.intParams=event.intParams;
  iv.floatParams=event.floatParams;
  switch (event.eventType/0x1000)
  {
    case 0: // unpaired event
      storeInterval(iv);
      break;
    case 1: // state change
      for (i=0;i<pending.size();i++)
	if (stateChanged(pending[i],event))
	  break;
      if (i==pending.size())
	pending.push_back(event);
      else
      {
	iv.start=pending[i].time;
	iv.end=event.time-startTime;
	pending[i].time=event.time-startTime;
	storeInterval(iv);
      }
      break;
    case 2: // lock, or write-lock a RW lock
    case 4: // read-lock TODO handle locks, RW locks, and recursive locks differently
    case 6: // recursive lock
      pending.push_back(event);
      break;
    case 3: // unlock
    case 5: // read-unlock
    case 7: // recursive unlock
      for (i=0;i<pending.size();i++)
	if (sameButTime(pending[i],event))
	  break;
      if (i==pending.size())
      {
	iv.eventType=(event.eventType&0xfff)-0x2000; // error: end without start
	storeInterval(iv);
      }
      else
      {
	iv.eventType=pending[i].eventType;
	iv.start=pending[i].time-startTime;
	storeInterval(iv);
	swap(pending[i],pending.back());
	pending.resize(pending.size()-1);
      }
      break;
    default: // unknown event type
      iv.eventType=(event.eventType&0xfff)-0x1000;
      storeInterval(iv);
  }
}

void treeize()
{
  int64_t earliest=INT64_MAX,latest=0;
  int i;
  for (i=0;i<intervals.size();i++)
  {
    if (intervals[i].start<earliest)
      earliest=intervals[i].start;
    if (intervals[i].end<earliest)
      earliest=intervals[i].end;
    if (intervals[i].start>latest)
      latest=intervals[i].start;
    if (intervals[i].end>latest)
      latest=intervals[i].end;
  }
  intervalTree=IntervalTree(earliest,latest);
  for (i=0;i<intervals.size();i++)
    intervalTree.insert(intervals[i]);
}
