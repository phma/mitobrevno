/******************************************************/
/*                                                    */
/* roundup.cpp - find nearby factorable number        */
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
#include "roundup.h"
using namespace std;

array<unsigned long,2> roundUp(uint64_t n)
/* Returns two numbers whose product is n or slightly larger. One is less than
 * 2147483648; the other is as small as necessary.
 */
{
  array<unsigned long,2> ret;
  unsigned long startDivisor,endDivisor,i;
  vector<unsigned long> remainders;
  unsigned long bestRemainder=1,bestDivisor;
  startDivisor=n/2147483647;
  if (startDivisor==0)
    startDivisor=1;
  if (n%startDivisor)
    startDivisor++;
  endDivisor=(startDivisor+startDivisor/10)+10;
  for (i=startDivisor;i<=endDivisor;i++)
    remainders.push_back(n%i);
  for (i=remainders.size()-1;i<remainders.size();i--)
    if ((remainders[i]>=bestRemainder && bestRemainder>0) || remainders[i]==0)
    {
      bestRemainder=remainders[i];
      bestDivisor=i+startDivisor;
    }
  ret[1]=bestDivisor;
  ret[0]=n/ret[1];
  if (n%ret[1])
    ret[0]++;
  return ret;
}
