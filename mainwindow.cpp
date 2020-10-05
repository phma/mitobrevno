/******************************************************/
/*                                                    */
/* mainwindow.cpp - main window                       */
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
#include "config.h"
#include "mainwindow.h"
#include "mitobrevno.h"
using namespace std;

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent)
{
  setWindowTitle(QApplication::translate("main", "Mitobrevno"));
  msgBox=new QMessageBox(this);
  toolbar=new QToolBar(this);
  addToolBar(Qt::TopToolBarArea,toolbar);
  toolbar->setIconSize(QSize(32,32));
  makeActions();
  show();
  timer=new QTimer(this);
  timer->start(50);
}

MainWindow::~MainWindow()
{
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  event->accept();
}

void MainWindow::openFile()
{
  int dialogResult;
  QStringList files;
  string fileName;
  fileDialog=new QFileDialog(this);
  fileDialog->setWindowTitle(tr("Open Log File"));
  fileDialog->setFileMode(QFileDialog::ExistingFile);
  fileDialog->setAcceptMode(QFileDialog::AcceptOpen);
  fileDialog->selectFile("");
  fileDialog->setNameFilter(tr("(*.log);;(*)"));
  dialogResult=fileDialog->exec();
  if (dialogResult)
  {
    files=fileDialog->selectedFiles();
    fileName=files[0].toStdString();
  }
  delete fileDialog;
  fileDialog=nullptr;
}

void MainWindow::aboutProgram()
{
  QString progName=tr("Mitobrevno");
  QMessageBox::about(this,tr("Mitobrevno"),
		     tr("%1\nVersion %2\nCopyright %3 Pierre Abbat\nLicense Apache 2")
		     .arg(progName).arg(QString(VERSION)).arg(COPY_YEAR));
}

void MainWindow::aboutQt()
{
  QMessageBox::aboutQt(this,tr("Mitobrevno"));
}

void MainWindow::makeActions()
{
  int i;
  fileMenu=menuBar()->addMenu(tr("&File"));
  viewMenu=menuBar()->addMenu(tr("&View"));
  settingsMenu=menuBar()->addMenu(tr("&Settings"));
  helpMenu=menuBar()->addMenu(tr("&Help"));
  // File menu
  openAction=new QAction(this);
  openAction->setIcon(QIcon::fromTheme("document-open"));
  openAction->setText(tr("Open"));
  fileMenu->addAction(openAction);
  connect(openAction,SIGNAL(triggered(bool)),this,SLOT(openFile()));
  exitAction=new QAction(this);
  exitAction->setIcon(QIcon::fromTheme("application-exit"));
  exitAction->setText(tr("Exit"));
  fileMenu->addAction(exitAction);
  connect(exitAction,SIGNAL(triggered(bool)),this,SLOT(close()));
  // Help menu
  aboutProgramAction=new QAction(this);
  aboutProgramAction->setText(tr("About Mitobrevno"));
  helpMenu->addAction(aboutProgramAction);
  connect(aboutProgramAction,SIGNAL(triggered(bool)),this,SLOT(aboutProgram()));
  aboutQtAction=new QAction(this);
  aboutQtAction->setText(tr("About Qt"));
  helpMenu->addAction(aboutQtAction);
  connect(aboutQtAction,SIGNAL(triggered(bool)),this,SLOT(aboutQt()));
}
