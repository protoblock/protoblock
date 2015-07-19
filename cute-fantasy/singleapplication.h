/*
 *  Credits to  : https://github.com/itay-grudev/SingleApplication
* The MIT License (MIT)
*
* Copyright (c) 2015 Itay Grudev
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
*    in the Software without restriction, including without limitation the rights
*    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*    copies of the Software, and to permit persons to whom the Software is
*    furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/

#ifndef SINGLE_APPLICATION_H
#define SINGLE_APPLICATION_H

// Change this to inherit from QGuiApplication or QCoreApplication
#define QAPPLICATION_CLASS QApplication

#define  QUOTE(C) #C
#define  INCLUDE_FILE(C) QUOTE(C)
#include INCLUDE_FILE(QAPPLICATION_CLASS)

class SingleApplicationPrivate;

/**
 * @brief The SingleApplication class handles multipe instances of the same Application
 * @see QApplication
 */
class SingleApplication : public QAPPLICATION_CLASS
{
  Q_OBJECT
public:
  explicit SingleApplication(int&, char *[]);
  ~SingleApplication();

signals:
  void showUp();

private slots:
  void slotConnectionEstablished();

private:
  SingleApplicationPrivate *d_ptr;
};

#endif // SINGLE_APPLICATION_H
