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
  std::vector<int> intParams;
  std::vector<float> floatParams;
};

/* Call openLogFile before starting worker threads, then call describeEvent
 * and describeParam to enter strings which will be displayed when you examine
 * the log file. Call formatParam to tell the format of each type of event.
 * Call logEvent from worker threads and call writeBufferedLog regularly
 * from the main thread.
 *
 * Event types are shorts of the form 0xtnnn, where t is as follows:
 * 0: unpaired events
 * 1: reserved
 * 2,3: start and end events. It is an error if the same thread does 2nnn
 * twice in a row or 3nnn twice in a row for the same nnn. It is an error
 * if two threads are at the same time between 2nnn and 3nnn with the same nnn
 * and the same parameters.
 * 4,5: start and end events of read-locking. Two threads can be in the same
 * 4nnn-5nnn pair at the same time, but one thread cannot be in 4nnn-5nnn while
 * another thread is in 2nnn-3nnn.
 * 6,7: start and end events of recursive locking. One thread can be in multiple
 * 6nnn-7nnn pairs, but two threads cannot be in the same one.
 * 8-f: reserved
 */
namespace mitobrevno
{
  void logEvent(int eventType,const std::vector<int> &intParams,const std::vector<float> &floatParams);
  void openLogFile(std::string fileName);
  void describeEvent(int eventType,std::string description);
  void describeParam(int eventType,int param,std::string description);
  void formatParam(int eventType,int nInts,int nFloats);
  void writeBufferedLog();
}

#endif
