/***************************************************************************
 *   Copyright (C) 2004 by Josep M. Mendoza                                *
 *   josep@mendoza-ginesta.net                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <kglobal.h>
#include <klocale.h>
#include <kconfig.h>
#include <kapplication.h>
#include <kmessagebox.h>
#include <kiconloader.h>
#include <qtimer.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qfile.h>

#include "kadm103x.h"

kadm103x::kadm103x(const QString& configFile, Type type, int actions, QWidget *parent, const char *name)
    : KPanelApplet(configFile, type, actions, parent, name)
{
    ksConfig = config();
    
    labelCPU = new QLabel(this);
    labelHD = new QLabel(this);
    iconCPU = new QLabel(this);
    iconHD = new QLabel(this);
    
    layout = new QGridLayout(this,2,2);
    
    layout->addWidget(iconCPU,0,0);
    layout->addWidget(labelCPU,0,1);
    layout->addWidget(iconHD,1,0);
    layout->addWidget(labelHD,1,1);
    
    QTimer *t = new QTimer(this);
    connect(t, SIGNAL(timeout()), SLOT(update()));
    t->start(5000, FALSE);
    
    iconCPU->setPixmap(SmallIcon("ksim_cpu"));
    iconHD->setPixmap(SmallIcon("hdd_mount"));
    
    labelCPU->setText("0ºC");
    labelHD->setText("0ºC");
    isWarned = 0;
    update();
}


kadm103x::~kadm103x()
{
}


void kadm103x::about()
{
    KMessageBox::information(0, i18n("<qt><b>kadm103x</b><br>"
                                     "Displays temperature values from iBook adm103x sensors<br />"
                                     "kadm103x is licensed to you under the terms of the GPL<br />"
                                     "<i>(C) 2004 Josep M. Mendoza</i></qt>"),
                                i18n("About kadm103x"));
}

int kadm103x::widthForHeight(int height) const
{
    return 60;
}

int kadm103x::heightForWidth(int width) const
{
    return height();
}

void kadm103x::resizeEvent(QResizeEvent *e)
{
}

void kadm103x::update()
{
    char buffer[25];
    FILE *cpuFile = fopen("/sys/devices/temperatures/info1", "r");
    double cpuTemp;
    int cpuFanStart;
    int cpuFanRamp;
    if (cpuFile)
    {
        while (fgets(buffer, sizeof(buffer), cpuFile))
        {
            sscanf(buffer, "T:%lf°C S:%d°C R:%d°C",&cpuTemp,&cpuFanStart,&cpuFanRamp);
        }
        labelCPU->setText(QString::number(cpuTemp,'f',1) + "ºC");
        fclose(cpuFile);
        isWarned = 0;
    }
    else
    {
        if (! isWarned )
        {
            isWarned = 1;
            KMessageBox::error(0,i18n("Can't open /sys/devices/temperatures/info1"));
            labelCPU->setText("--.-ºC");
            labelHD->setText("--.-ºC");
        }
        return;
    }
    FILE *hdFile = fopen("/sys/devices/temperatures/info0", "r");
    double hdTemp;
    int hdFanStart;
    int hdFanRamp;
    if (hdFile)
    {
        while (fgets(buffer, sizeof(buffer), hdFile))
        {
            sscanf(buffer, "T:%lf°C S:%d°C R:%d°C",&hdTemp,&hdFanStart,&hdFanRamp);
        }
        labelHD->setText(QString::number(hdTemp,'f',1) + "ºC");
        fclose(hdFile);
        isWarned = 0;
    }   
    else
    {
        if (! isWarned )
        {
            isWarned = 1;
            KMessageBox::error(0,i18n("Can't open /sys/devices/temperatures/info0"));
            labelCPU->setText("--.-ºC");
            labelHD->setText("--.-ºC");
        }
        return;;
    }
}

extern "C"
{
    KPanelApplet* init( QWidget *parent, const QString& configFile)
    {
        KGlobal::locale()->insertCatalogue("kadm103x");
        return new kadm103x(configFile, KPanelApplet::Stretch,
                             KPanelApplet::About, // | KPanelApplet::Help | KPanelApplet::Preferences,
                             parent, "kadm103x");
    }
}
