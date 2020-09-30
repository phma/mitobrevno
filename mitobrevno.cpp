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
#include <mutex>
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
  void writelong(ostream &file,uint64_t i);
  void writeint(ostream &file,int i);
  void write(ostream &file,const MbEvent &event);
  mutex mitoMutex;
}

void mitobrevno::writeint(ostream &file,int i)
{
  char buf[4];
  *(int *)buf=i;
  file.write(buf,4);
}

void mitobrevno::writelong(ostream &file,uint64_t i)
{
  char buf[8];
  *(uint64_t *)buf=i;
 file.write(buf,8);
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
  mitoMutex.lock();
  buffer.push(event);
  mitoMutex.unlock();
}

void mitobrevno::openLogFile(string fileName)
{
  mbFile.open(fileName,ios::binary|ios::out|ios::trunc);
  writeint(mbFile,0x043103bc); // file signature: μб in UTF-16 (or бμ big-endian)
  writeint(mbFile,0); // format version number
}

void mitobrevno::write(ostream &file,const MbEvent &event)
{
  int i;
  writelong(file,event.time);
  writeint(file,event.thread);
  writeint(file,event.eventType);
  for (i=0;i<4;i++)
    writeint(file,event.param[i]);
}

void mitobrevno::writeBufferedLog()
{
  MbEvent event;
  mitoMutex.lock();
  while (buffer.size())
  {
    write(mbFile,buffer.front());
    buffer.pop();
    mitoMutex.unlock();
    mitoMutex.lock();
  }
  mitoMutex.unlock();
}

