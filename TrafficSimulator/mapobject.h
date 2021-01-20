#ifndef MAPOBJECT_H
#define MAPOBJECT_H

#include <QObject>
#include "Point.h"
#include "SimpleMarkerSymbol.h"
#include "SimpleFillSymbol.h"

using namespace Esri::ArcGISRuntime;
enum mapObjectType {UNDEFINED = -1, PEDESTRIAN = 0, VEHICLE, POLE};

class MapObject : public QObject
{
    Q_OBJECT
public:
    explicit MapObject(QObject *parent = nullptr);
    explicit MapObject(mapObjectType mapObjectType, quint16 xImgPix, quint16 yImgPix, quint16 bBoxWidth, quint16 bBoxHeight);
    explicit MapObject(mapObjectType mapObjectType, double longitude, double latitude, int id);

    void printInfo();

    int getId() const;

    Point* getLocation();

    Point* getImgPixPos() const;

    void setLocation(Point *value);

    SimpleMarkerSymbol *getPointSymbol() const;

    quint16 getBBoxWidth() const;

    quint16 getBBoxHeight() const;

    void setBBoxTopRight(Point *value);

    void setBBoxBottomLeft(Point *value);

    void setBBoxBottomRight(Point *value);

    Point *getBBoxTopRight() const;

    Point *getBBoxBottomLeft() const;

    Point *getBBoxBottomRight() const;

    SimpleFillSymbol *getFillSymbol() const;

private:
    Point* location;
    Point* bBoxTopRight;
    Point* bBoxBottomLeft;
    Point* bBoxBottomRight;

    Point* imgPixPos;
    quint16 bBoxWidth;
    quint16 bBoxHeight;
    SimpleMarkerSymbol* pointSymbol;
    SimpleFillSymbol* fillSymbol;
    mapObjectType type;
    int id;
signals:

};

#endif // MAPOBJECT_H
