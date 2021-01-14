#ifndef MAPOBJECT_H
#define MAPOBJECT_H

#include <QObject>
#include "Point.h"
#include "SimpleMarkerSymbol.h"

using namespace Esri::ArcGISRuntime;
enum type {undefined = -1, pedestrian = 0, vehicle};

class MapObject : public QObject
{
    Q_OBJECT
public:
    explicit MapObject(QObject *parent = nullptr);
    explicit MapObject(type mapObjectType, quint16 xImgPix, quint16 yImgPix, quint16 bBoxWidth, quint16 bBoxHeight);
    void printInfo();

private:
    Point location;
    Point imgPixPos;
    quint16 bBoxWidth;
    quint16 bBoxHeight;
    SimpleMarkerSymbol* pointSymbol;
    type mapObjectType;
signals:

};

#endif // MAPOBJECT_H