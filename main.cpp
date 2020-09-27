/******************************************************/
/*                                                    */
/* main.cpp - main program                            */
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
#include <QtGui>
#include <QtWidgets>
#include <QTranslator>
#include <iostream>
#include "config.h"
#include "mainwindow.h"

using namespace std;

int main(int argc, char *argv[])
{
  int exitStatus;
  QApplication app(argc, argv);
  QTranslator translator,qtTranslator;
  int nthreads;
  string exeFileName=argv[0],exeDir;
  size_t slashPos;
  slashPos=exeFileName.find_last_of("/\\");
  if (slashPos<exeFileName.length())
    exeDir=exeFileName.substr(0,slashPos);
  else
    exeDir=".";
  if (qtTranslator.load(QLocale(),QLatin1String("qt"),QLatin1String("_"),
                        QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
    app.installTranslator(&qtTranslator);
  if (translator.load(QLocale(),QLatin1String("mitobrevno"),
                      QLatin1String("_"),QString::fromStdString(exeDir)))
  {
    //cout<<"Translations found in executable's directory"<<endl;
    app.installTranslator(&translator);
  }
  else if (translator.load(QLocale(),QLatin1String("mitobrevno"),
                      QLatin1String("_"),QLatin1String(SHARE_DIR)))
  {
    //cout<<"Translations found in share directory"<<endl;
    app.installTranslator(&translator);
  }
  MainWindow window;
  nthreads=window.getNumberThreads();
  if (nthreads<1)
    nthreads=thread::hardware_concurrency();
  if (nthreads<1)
    nthreads=2;
  //startThreads(nthreads);
  window.show();
  exitStatus=app.exec();
  //waitForThreads(TH_STOP);
  //joinThreads();
  return exitStatus;
}
