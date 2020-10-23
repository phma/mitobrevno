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

#include "itree.h"

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
