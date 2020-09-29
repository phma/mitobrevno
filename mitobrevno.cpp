/******************************************************/
/*                                                    */
/* mitobrevno.cpp - event logging                     */
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
#include <fstream>
#include <chrono>
#include <thread>
#include <map>
#include <queue>
#include "mitobrevno.h"
using namespace std;
using namespace mitobrevno;
namespace cr=std::chrono;

namespace mitobrevno
{
  ofstream mbFile;
  queue<MbEvent> buffer;
  cr::steady_clock clk;
  map<thread::id,int> threadNums;
}

void mitobrevno::logEvent(int eventType,int param0,int param1,int param2,int param3)
{
  cr::time_point<cr::steady_clock> now=clk.now();
  MbEvent event;
  event.time=now.time_since_epoch().count();
  if (!threadNums.count(this_thread::get_id()))
    threadNums[this_thread::get_id()]=threadNums.size();
  event.thread=threadNums[this_thread::get_id()];
  event.param[0]=param0;
  event.param[1]=param1;
  event.param[2]=param2;
  event.param[3]=param3;
  buffer.push(event);
}
