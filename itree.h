/******************************************************/
/*                                                    */
/* itree.h - interval tree                            */
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

#include "interval.h"

/* The interval tree is made by plotting start time against end time and
 * splitting the resulting isosceles right triangle, whose hypotenuse contains
 * the instant intervals, as follows:
 * If the side is 0, it contains one interval (which may have two instances because of errors).
 * If the side is 2n-1 (n>0), it is split into 3 triangles of side n-1 and 1 of side 2-n.
 * If the side is 2n (n>0), it is split into 3 triangles of side n-1 and 1 of side -n.
 */
 // * * * *|* * * *
 // * * */*|* * *
 // * */* *|* *
 // */*_*_*|*
 // * * * *
 // * * *
 // * *
 // *
/* The bottom corner is (0,0). The corner is (0,7); the side is 7. It is divided into:
 * (0,7) side 3
 * (0,3) side 3
 * (4,7) side 3
 * (3,4) side -2.
 */

class IntervalTree
{
public:
  IntervalTree(int64_t first,int64_t last); // inclusive
  ~IntervalTree();
  int subtree(int64_t first,int64_t last);
private:
  IntervalTree(int64_t first,int64_t last,int64_t size);
  int64_t start,end,side;
  std::vector<Interval> leaf;
  IntervalTree *sub[4];
};
