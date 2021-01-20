#ifndef COMMON_H
#define COMMON_H
#include <QDir>
#include <QString>
#include "TrafficSimulator.h"

static const QDir dataDir ("data/v9");
static const QString stream1 = dataDir.filePath("out_perspective_1");
static const QString stream2 = dataDir.filePath("out_perspective_3");
static const double centerViewLong = 19.8371305682558;
static const double centerViewLat = 45.2403304784098;
static const int centerViewZoom = 5000;
static const int sleepMs = 10;
static const int headerSize = 33;
static const int frameSize = 1212;
static const int maxPedestrianNum = 50;
static const int maxVehicleNum = 100;
static const int maxStreamNum = 2;
static const double fps60Freq = 16.6666666667;
static const int imageWidth = 1920;
static const int imageHeight = 1080;
static const int realNumPrintPrecision = 15;
static const int mapObjectPointSize = 4;
static const graphicType displayType = BOX;
static const quint16 bBoxWidthThresh = 500;
static const quint16 bBoxHeightThresh = 500;

#endif // COMMON_H
