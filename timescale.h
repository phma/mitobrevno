/******************************************************/
/*                                                    */
/* timescale.h - time scale                           */
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
#include <QtWidgets>
#include <QScrollBar>
#include <string>
#include "zoom.h"

/* This class can handle a 64-bit time range (typically in nanoseconds), whereas
 * QScrollBar has only a 32-bit range. This necessitates cleverness with
 * the scrollbar.
 */

class TimeCanvas: public QWidget
{
  Q_OBJECT
public:
  TimeCanvas(QWidget *parent=0);
};
