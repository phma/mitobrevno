/******************************************************/
/*                                                    */
/* interval.h - event intervals                       */
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

#include "mitobrevno.h"

struct Interval
{
  uint64_t start,end;
  int thread;
  int eventType;
  std::vector<int> intParams;
  std::vector<float> floatParams;
};

void intervalize (MbEvent &event,uint64_t startTime);
