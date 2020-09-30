/******************************************************/
/*                                                    */
/* mitobrevno.h - event logging                       */
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
#ifndef MITOBREVNO_H
#define MITOBREVNO_H
#include <string>
#include <chrono>
#include <climits>

struct MbEvent
{
  uint64_t time;
  int thread;
  int eventType;
  int param[4];
};

namespace mitobrevno
{
  void logEvent(int eventType,int param0=INT_MIN,int param1=INT_MIN,int param2=INT_MIN,int param3=INT_MIN);
  void openLogFile(std::string fileName);
  void describeEvent(int eventType,std::string description);
  void describeParam(int param,std::string description);
  void writeBufferedLog();
}

#endif
