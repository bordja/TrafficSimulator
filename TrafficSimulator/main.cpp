
// Copyright 2019 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.
//

// Qt headers
#include <QApplication>
#include <QMessageBox>
#include <QThread>

#include "ArcGISRuntimeEnvironment.h"

#include "TrafficSimulator.h"
#include "streammanager.h"
#include "stream.h"
#include "common.h"
using namespace Esri::ArcGISRuntime;

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication application(argc, argv);

    // Before initializing ArcGIS Runtime, first set the
    // ArcGIS Runtime license setting required for your application.

    // ArcGISRuntimeEnvironment::setLicense("Place license string in here");

      QObject::connect(ArcGISRuntimeEnvironment::instance(), &ArcGISRuntimeEnvironment::errorOccurred, [](const Error& error){
        QMessageBox msgBox;
        msgBox.setText(error.message());
        msgBox.exec();
      });

      if (ArcGISRuntimeEnvironment::initialize() == false)
      {
        application.quit();
        return 1;
      }

    TrafficSimulator applicationWindow;
    StreamManager manager(applicationWindow);
    Stream* cam1 = new Stream(stream1);
    Stream* cam2 = new Stream(stream2);

    manager.addStream(cam1);
    manager.addStream(cam2);

    QThread producerThread;

    manager.moveToThread(&producerThread);
    producerThread.start();
    QObject::connect(&manager, SIGNAL(finished()), &producerThread, SLOT(quit()));

    applicationWindow.setMinimumWidth(800);
    applicationWindow.setMinimumHeight(600);
    applicationWindow.show();
    applicationWindow.testGraphics();

    manager.init();
    manager.run();

    return application.exec();
}
