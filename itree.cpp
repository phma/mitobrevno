/******************************************************/
/*                                                    */
/* itree.cpp - interval tree                          */
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

#include <cassert>
#include "itree.h"

using namespace std;
using namespace mitobrevno;

IntervalTree intervalTree;

IntervalRange::IntervalRange()
{
  conjunction=true;
  firstStart=firstEnd=INT64_MIN;
  lastStart=lastEnd=INT64_MAX;
}

IntervalTree::IntervalTree()
{
  int i;
  for (i=0;i<4;i++)
    sub[i]=nullptr;
  start=end=side=0;
}

IntervalTree::IntervalTree(int64_t first,int64_t last)
{
  int i;
  for (i=0;i<4;i++)
    sub[i]=nullptr;
  start=first;
  end=last;
  side=last-first;
}

IntervalTree::IntervalTree(int64_t first,int64_t last,int64_t size)
{
  int i;
  for (i=0;i<4;i++)
    sub[i]=nullptr;
  start=first;
  end=last;
  side=size;
}

IntervalTree::~IntervalTree()
{
  int i;
  for (i=0;i<4;i++)
    delete sub[i];
}

int IntervalTree::subtree(int64_t first,int64_t last)
{
  int64_t iside=side;
  int ret=0;
  first-=start;
  last-=end;
  if (side<0)
  {
    iside=-iside;
    first=-first;
    last=-last;
  }
  if (first-last<(iside+1)/2)
    ret=1;
  if (first>iside/2)
    ret=2;
  if (-last>iside/2)
    ret=3;
  return ret;
}

void IntervalTree::insert(Interval iv)
{
  if (side==0)
  {
    assert(iv.start==start && iv.end==end);
    leaf.push_back(iv);
  }
  else
  {
    int subt=subtree(iv.start,iv.end);
    int sign=(side>0)-(side<0);
    int64_t subside=sign*(((side*sign)-1)/2);
    int64_t offset=sign*((side*sign)/2+1);
    int64_t substart=start,subend=end;
    if (!subt)
      subside=-sign*(((side*sign)-2*(side&1))/2);
    if (sub[subt]==nullptr)
    {
      switch (subt)
      {
	case 0:
	  substart=start+offset-sign;
	  subend=end-offset+sign;
	  break;
	case 1:
	  break;
	case 2:
	  substart=start+offset;
	  break;
	case 3:
	  subend=end-offset;
	  break;
      }
      sub[subt]=new IntervalTree(substart,subend,subside);
    }
    sub[subt]->insert(iv);
  }
}

int leg(int64_t a,int64_t b)
{
  return (a<b)+(a<=b);
}

bool IntervalTree::overlap(const IntervalRange &ir)
/* Returns true if the range overlaps the triangle covered by the tree.
 * Also returns true, wrongly, if the interval is in the other half of the square.
 */
{
  int legStart=leg(start,ir.firstStart)*27+leg(start,ir.lastStart)*9+
      leg(start-side,ir.firstStart)*3+leg(start-side,ir.lastStart);
  int legEnd=leg(end,ir.firstEnd)*27+leg(end,ir.lastEnd)*9+
      leg(end+side,ir.firstEnd)*3+leg(end+side,ir.lastEnd);
  bool startOverlap=legStart!=0 && legStart!=80;
  bool endOverlap=legEnd!=0 && legEnd!=80;
  if (ir.conjunction)
    return startOverlap && endOverlap;
  else
    return startOverlap || endOverlap;
}

vector<Interval *> IntervalTree::matchingIntervals(int eventType,const IntervalRange &ir)
{
  vector<Interval *> ret,part;
  int i,j;
  if (overlap(ir))
    if (side)
      for (i=0;i<4;i++)
	if (sub[i])
	{
	  part=sub[i]->matchingIntervals(eventType,ir);
	  for (j=0;j<part.size();j++)
	    ret.push_back(part[j]);
	}
	else
	  ;
    else
      for (i=0;i<leaf.size();i++)
	if (-abs(eventType)<-32768 || eventBase(eventType)==eventBase(leaf[i].eventType))
	  ret.push_back(&leaf[i]);
  return ret;
}
