/******************************************************/
/*                                                    */
/* mainwindow.h - main window                         */
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
#include <QMainWindow>
#include <QTimer>
#include <QtWidgets>
#include <QPixmap>
#include <string>
#include <array>
#include "mitobrevno.h"

class MainWindow: public QMainWindow
{
  Q_OBJECT
public:
  MainWindow(QWidget *parent=0);
  ~MainWindow();
  void makeActions();
  void makeStatusBar();
  void readSettings();
  void writeSettings();
  int getNumberThreads()
  {
    return numberThreads;
  }
  bool conversionBusy();
signals:
  void tinSizeChanged();
  void lengthUnitChanged(double unit);
  void colorSchemeChanged(int scheme);
  void noCloudArea();
public slots:
  //void tick();
  void openFile();
  void aboutProgram();
  void aboutQt();
protected:
  void closeEvent(QCloseEvent *event) override;
private:
  int numberThreads;
  MbHeader mbHeader;
  QTimer *timer;
  QFileDialog *fileDialog;
  QMessageBox *msgBox;
  QToolBar *toolbar;
  QMenu *fileMenu,*viewMenu,*settingsMenu,*helpMenu;
  QAction *openAction,*closeAction,*exitAction;
  QAction *configureAction;
  QAction *aboutProgramAction,*aboutQtAction;
};
