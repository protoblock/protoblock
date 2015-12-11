//  This file is part of Qt Bitcion Trader
//      https://github.com/JulyIGHOR/QtBitcoinTrader
//  Copyright (C) 2013-2015 July IGHOR <julyighor@gmail.com>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  In addition, as a special exception, the copyright holders give
//  permission to link the code of portions of this program with the
//  OpenSSL library under certain conditions as described in each
//  individual source file, and distribute linked combinations including
//  the two.
//
//  You must obey the GNU General Public License in all respects for all
//  of the code used other than OpenSSL. If you modify file(s) with this
//  exception, you may extend this exception to your version of the
//  file(s), but you are not obligated to do so. If you do not wish to do
//  so, delete this exception statement from your version. If you delete
//  this exception statement from all source files in the program, then
//  also delete it here.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "julylightchanges.h"
#include <QApplication>
#include "core.h"


JulyLightChanges::JulyLightChanges(QSpinBox *parent, bool always)
    : QObject() , mAlwayson(always)
{
    lastValue=0;
	parentSpinBox=parent;
	setParent(parentSpinBox);
    changeTimer=new QTimer;
    connect(changeTimer,SIGNAL(timeout()),this,SLOT(changeTimerSlot()));
    changeTimer->setSingleShot(true);
    valueChanged(parentSpinBox->value());
    connect(parent,SIGNAL(valueChanged(int)),this,SLOT(valueChanged(int)));
}

JulyLightChanges::~JulyLightChanges()
{
    if(changeTimer)delete changeTimer;
}

void JulyLightChanges::changeTimerSlot()
{
    if ( !mAlwayson || lastValue == 0)
        parentSpinBox->setStyleSheet("");
    else if ( lastValue > 0)
        parentSpinBox->setStyleSheet(UPCOLORa);
    else
        parentSpinBox->setStyleSheet(DOWNCOLORa);
}

void JulyLightChanges::valueChanged(int val)
{
    //qDebug() << "level2 JulyLightChanges::valueChanged " << val << lastValue;
    //QString newcolor("QSpinBox:disabled{color:%1; background: \"%2\";} QSpinBox {color:%1;background: \"%2\";}");
    changeTimer->stop();
    if(lastValue<=val)
        parentSpinBox->setStyleSheet(UPCOLOR);//newcolor.arg("Black").arg("Green"));
    else
        parentSpinBox->setStyleSheet(DOWNCOLOR);//newcolor.arg("Black").arg("#ffc8c8"));

    lastValue=val;
    changeTimer->start(2000);
}
/*
    else {
        if ( val < 0 && lastValue >= 0 ) {
            parentSpinBox->setStyleSheet(DOWNCOLORa);
        }
        if ( val > 0 && lastValue <= 0) {
            parentSpinBox->setStyleSheet(UPCOLORa);
        }
        else if( val == 0 && lastValue != 0) {
            parentSpinBox->setStyleSheet("");
        }
    }
}*/

const char* JulyLightChanges::LIGHTGREEN = "\"#c8ffc8\"";
const char* JulyLightChanges::LIGHTRED = "\"#ffc8c8\"";

QString JulyLightChanges::UPCOLOR =
        QString("QSpinBox:disabled{color:#000000; background: %1;} QSpinBox {color:#000000;background: %1;}")
        .arg(LIGHTGREEN);

QString JulyLightChanges::DOWNCOLOR =
        QString("QSpinBox:disabled{color:#000000; background: %1;} QSpinBox {color:#000000;background: %1;}")
        .arg(LIGHTRED);


QString JulyLightChanges::UPCOLORa =
        QString("QSpinBox:disabled{color:green;} QSpinBox {color:green;}")
        .arg(LIGHTGREEN);

QString JulyLightChanges::DOWNCOLORa =
        QString("QSpinBox:disabled{color:red;} QSpinBox {color:red;}")
        .arg(LIGHTRED);


