/******************************************************/
/*                                                    */
/* zoom.cpp - zoom ratios for viewing                 */
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
#include <map>
#include <cmath>
#include <cassert>
#include "zoom.h"
#define ZOOMSTEP 1.259423132456698482765476450823
/* Used in GUI windows for zooming. 3 zoom steps are just under a ratio of 2;
 * 10 zoom steps are just over a ratio of 10.
 */

using namespace std;

map<int,double> zooms;

double zoomratio(int n)
{
  double ret=zooms[n];
  if (ret==0)
    zooms[n]=ret=pow(ZOOMSTEP,n);
  return ret;
}

int largestFit(double ratio)
// Returns the largest n such that zoomratio(n)<=ratio.
{
  int ret=floor(log(ratio)/log(ZOOMSTEP));
  assert(ratio>0);
  if (zoomratio(ret)>ratio)
    ret--;
  return ret;
}
