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
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <mutex>
#include <map>
#include <cmath>
#include <array>
#include <queue>
#include "mitobrevno.h"
using namespace std;
using namespace mitobrevno;
namespace cr=std::chrono;

/* Header records:
 * 00
 *	end of header
 * 01 eeee string
 *	event description
 * 02 eeee ii ff
 *	event eeee takes ii integers and ff floats
 * 03 eeee pp string
 *	event parameter description
 */

namespace mitobrevno
{
  bool headerClosed,logEnabled;
  fstream mbFile;
  queue<MbEvent> buffer;
  cr::steady_clock clk;
  map<thread::id,int> threadNums;
  map<int,array<int,2> > eventSizes;
  int eventBase(int eventType);
  short readshort(istream &file);
  int readint(istream &file);
  uint64_t readlong(istream &file);
  void writelong(ostream &file,uint64_t i);
  void writeint(ostream &file,int i);
  void writefloat(ostream &file,float i);
  void writeshort(ostream &file,short i);
  void writeustring(ostream &file,string s);
  void write(ostream &file,const MbEvent &event);
  mutex mitoMutex;
}

int mitobrevno::eventBase(int eventType)
{
  eventType&=0xffff;
  if (eventType>=0x2000 && eventType<0x8000)
    eventType=(eventType&0xfff)+0x2000;
  return eventType;
}

short mitobrevno::readshort(istream &file)
{
  char buf[2];
  file.read(buf,2);
  return *(short *)buf;
}

int mitobrevno::readint(istream &file)
{
  char buf[4];
  file.read(buf,4);
  return *(int *)buf;
}

uint64_t mitobrevno::readlong(istream &file)
{
  char buf[8];
  file.read(buf,8);
  return *(uint64_t *)buf;
}

void mitobrevno::writeshort(ostream &file,short i)
{
  char buf[2];
  *(short *)buf=i;
  file.write(buf,2);
}

void mitobrevno::writefloat(ostream &file,float i)
{
  char buf[4];
  *(float *)buf=i;
  file.write(buf,4);
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

void mitobrevno::writeustring(ostream &file,string s)
// FIXME: if s contains a null character, it should be written as c0 a0
{
  file.write(s.data(),s.length());
  file.put(0);
}

void mitobrevno::logEvent(int eventType,const vector<int> &intParams,const vector<float> &floatParams)
{
  cr::time_point<cr::steady_clock> now=clk.now();
  MbEvent event;
  int baseEvent=eventBase(eventType);
  if (logEnabled)
  {
    event.time=now.time_since_epoch().count();
    if (!threadNums.count(this_thread::get_id()))
      threadNums[this_thread::get_id()]=threadNums.size();
    event.eventType=eventType;
    event.thread=threadNums[this_thread::get_id()];
    event.intParams=intParams;
    event.floatParams=floatParams;
    if (!eventSizes.count(baseEvent))
    {
      eventSizes[baseEvent][0]=0;
      eventSizes[baseEvent][1]=0;
    }
    while (event.intParams.size()<eventSizes[baseEvent][0])
      event.intParams.push_back(INT_MIN);
    while (event.floatParams.size()<eventSizes[baseEvent][1])
      event.floatParams.push_back(NAN);
    event.intParams.resize(eventSizes[baseEvent][0]);
    event.floatParams.resize(eventSizes[baseEvent][1]);
    mitoMutex.lock();
    buffer.push(event);
    mitoMutex.unlock();
  }
}

void mitobrevno::openLogFile(string fileName)
{
  cr::time_point<cr::steady_clock> now=clk.now();
  char *envVar=getenv("MITOBREVNO_ON");
  if (envVar)
  {
    mbFile.open(fileName,ios::binary|ios::out|ios::trunc);
    writeint(mbFile,0x043103bc); // file signature: μб in UTF-16 (or бμ big-endian)
    writeint(mbFile,0); // format version number
    writelong(mbFile,now.time_since_epoch().count());
    writelong(mbFile,cr::steady_clock::period().num);
    writelong(mbFile,cr::steady_clock::period().den);
    headerClosed=false;
    logEnabled=true;
    cerr<<"Logging to "<<fileName<<endl;
  }
  else
  {
    logEnabled=false;
    cerr<<"Not logging because MITOBREVNO_ON is not set"<<endl;
  }
}

void mitobrevno::describeEvent(int eventType,std::string description)
{
  if (logEnabled)
  {
    mbFile.put(1);
    writeshort(mbFile,eventType);
    writeustring(mbFile,description);
  }
}

void mitobrevno::describeParam(int eventType,int param,std::string description)
{
  if (logEnabled)
  {
    mbFile.put(3);
    writeshort(mbFile,eventType);
    mbFile.put(param);
    writeustring(mbFile,description);
  }
}

void mitobrevno::formatParam(int eventType,int nInts,int nFloats)
{
  if (logEnabled)
  {
    mbFile.put(2);
    eventType=eventBase(eventType);
    nInts&=255;
    nFloats&=255;
    writeshort(mbFile,eventType);
    mbFile.put(nInts);
    mbFile.put(nFloats);
    eventSizes[eventType][0]=nInts;
    eventSizes[eventType][1]=nFloats;
  }
}

void mitobrevno::write(ostream &file,const MbEvent &event)
{
  int i;
  writelong(file,event.time);
  writeint(file,event.thread);
  writeshort(file,event.eventType);
  for (i=0;i<event.intParams.size();i++)
    writeint(file,event.intParams[i]);
  for (i=0;i<event.floatParams.size();i++)
    writefloat(file,event.floatParams[i]);
}

void mitobrevno::writeBufferedLog()
{
  MbEvent event;
  if (!headerClosed)
  {
    mbFile.put(0);
    headerClosed=true;
  }
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

MbHeader mitobrevno::openLogFileRead(string fileName)
{
  MbHeader ret;
  int filesig;
  mbFile.open(fileName,ios::binary|ios::in);
  filesig=readint(mbFile);
  if (filesig==0x043103bc)
  {
    ret.formatVersion=readint(mbFile);
    ret.startTime=readlong(mbFile);
    ret.num=readlong(mbFile);
    ret.den=readlong(mbFile);
  }
  else
    ret.num=ret.den=0;
  return ret;
}
