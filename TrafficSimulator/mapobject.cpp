#include "mapobject.h"
#include <QDebug>
#include "common.h"
MapObject::MapObject(QObject *parent) : QObject(parent)
{
    this->bBoxTopLeft = new Point(0, 0, SpatialReference::wgs84());
    this->type = UNDEFINED;
    this->pointSymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle, QColor(Qt::white), mapObjectPointSize, this);
    this->fillSymbol = new SimpleFillSymbol(SimpleFillSymbolStyle::Solid, QColor(Qt::white), this);
}

MapObject::MapObject(mapObjectType type, quint16 xImgPix, quint16 yImgPix, quint16 bBoxWidth, quint16 bBoxHeight)
{
    this->type = type;
    this->imgPixPos = new Point(xImgPix,yImgPix,SpatialReference::wgs84());
    this->bBoxWidth = bBoxWidth;
    this->bBoxHeight = bBoxHeight;
    this->bBoxTopLeft = new Point(0, 0, SpatialReference::wgs84());
    this->bBoxTopRight = new Point(0, 0, SpatialReference::wgs84());
    this->bBoxBottomLeft = new Point(0, 0, SpatialReference::wgs84());
    this->bBoxBottomRight = new Point(0, 0, SpatialReference::wgs84());
    if(type == PEDESTRIAN)
    {
        this->pointSymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle, QColor(Qt::red), mapObjectPointSize, this);
        this->fillSymbol = new SimpleFillSymbol(SimpleFillSymbolStyle::Solid, QColor(Qt::red), this);
    }
    else if(type == VEHICLE)
    {
        this->pointSymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle, QColor(Qt::green), mapObjectPointSize, this);
        this->fillSymbol = new SimpleFillSymbol(SimpleFillSymbolStyle::Solid, QColor(Qt::green), this);
    }
    else
    {
        this->pointSymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle, QColor(Qt::white), mapObjectPointSize, this);
        this->fillSymbol = new SimpleFillSymbol(SimpleFillSymbolStyle::Solid, QColor(Qt::white), this);
    }
}

MapObject::MapObject(mapObjectType type, quint16 xImgPix, quint16 yImgPix, quint16 bBoxWidth, quint16 bBoxHeight, quint8 camId)
{
    this->type = type;
    this->imgPixPos = new Point(xImgPix,yImgPix,SpatialReference::wgs84());
    this->bBoxWidth = bBoxWidth;
    this->bBoxHeight = bBoxHeight;
    this->bBoxTopLeft = new Point(0, 0, SpatialReference::wgs84());
    this->bBoxTopRight = new Point(0, 0, SpatialReference::wgs84());
    this->bBoxBottomLeft = new Point(0, 0, SpatialReference::wgs84());
    this->bBoxBottomRight = new Point(0, 0, SpatialReference::wgs84());

    /* Setting different colors depending on the camera ID */
    if(type == PEDESTRIAN)
    {
        this->fillSymbol = new SimpleFillSymbol(SimpleFillSymbolStyle::Solid, QColor(Qt::red), this);
        this->pointSymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle, QColor(Qt::red), mapObjectPointSize, this);

    }
    else if(type == VEHICLE)
    {
        this->pointSymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle, QColor(Qt::green), mapObjectPointSize, this);

        switch (camId) {
        case 1:
            this->fillSymbol = new SimpleFillSymbol(SimpleFillSymbolStyle::Solid, QColor(0,255,0), this);
            break;
        case 2:
            this->fillSymbol = new SimpleFillSymbol(SimpleFillSymbolStyle::Solid, QColor(255,255,255), this);
            break;
        case 3:
            this->fillSymbol = new SimpleFillSymbol(SimpleFillSymbolStyle::Solid, QColor(255,255,155), this);
            break;
        case 4:
            this->fillSymbol = new SimpleFillSymbol(SimpleFillSymbolStyle::Solid, QColor(255,255,255), this);
            break;
        }
    }
    else
    {
        this->pointSymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle, QColor(Qt::white), mapObjectPointSize, this);
        this->fillSymbol = new SimpleFillSymbol(SimpleFillSymbolStyle::Solid, QColor(Qt::white), this);
    }
}

MapObject::MapObject(mapObjectType type, double longitude, double latitude, int id)
{
    this->bBoxTopLeft = new Point(longitude, latitude, SpatialReference::wgs84());
    this->id = id;
    this->type = type;
    if(type == PEDESTRIAN)
    {
        this->pointSymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle, QColor(Qt::red), mapObjectPointSize, this);
        this->fillSymbol = new SimpleFillSymbol(SimpleFillSymbolStyle::Solid, QColor(Qt::red), this);
    }
    else if(type == VEHICLE)
    {
        this->pointSymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle, QColor(Qt::green), mapObjectPointSize, this);
        this->fillSymbol = new SimpleFillSymbol(SimpleFillSymbolStyle::Solid, QColor(Qt::green), this);
    }
    else
    {
        this->pointSymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle, QColor(Qt::white), mapObjectPointSize, this);
        this->fillSymbol = new SimpleFillSymbol(SimpleFillSymbolStyle::Solid, QColor(Qt::white), this);
    }
}

void MapObject::printInfo()
{
    qDebug()<<"X:"<<this->imgPixPos->x();
    qDebug()<<"Y:"<<this->imgPixPos->y();
    qDebug()<<"BBW:"<<this->bBoxWidth;
    qDebug()<<"BBH:"<<this->bBoxHeight;

}

int MapObject::getId() const
{
    return id;
}

Point* MapObject::getBBoxTopLeft()
{
    return this->bBoxTopLeft;
}

Point* MapObject::getImgPixPos() const
{
    return imgPixPos;
}

void MapObject::setBBoxTopLeft(Point *value)
{
    Point* old = bBoxTopLeft;
    bBoxTopLeft = value;

    if(old != nullptr)
    {
        delete old;
    }

}

SimpleMarkerSymbol *MapObject::getPointSymbol() const
{
    return pointSymbol;
}

quint16 MapObject::getBBoxWidth() const
{
    return bBoxWidth;
}

quint16 MapObject::getBBoxHeight() const
{
    return bBoxHeight;
}

void MapObject::setBBoxTopRight(Point *value)
{
    Point* old = bBoxTopRight;
    bBoxTopRight = value;

    if(old != nullptr)
    {
        delete old;
    }
}

void MapObject::setBBoxBottomLeft(Point *value)
{
    Point* old = bBoxBottomLeft;
    bBoxBottomLeft = value;
    if(old != nullptr)
    {
        delete old;
    }
}

void MapObject::setBBoxBottomRight(Point *value)
{
    Point* old = bBoxBottomRight;
    bBoxBottomRight = value;
    if(old != nullptr)
    {
        delete old;
    }
}

Point *MapObject::getBBoxTopRight() const
{
    return bBoxTopRight;
}

Point *MapObject::getBBoxBottomLeft() const
{
    return bBoxBottomLeft;
}

Point *MapObject::getBBoxBottomRight() const
{
    return bBoxBottomRight;
}

SimpleFillSymbol *MapObject::getFillSymbol() const
{
    return fillSymbol;
}

void MapObject::setId(int value)
{
    id = value;
}

quint8 MapObject::getCamId() const
{
    return camId;
}

void MapObject::setCamId(const quint8 &value)
{
    camId = value;
}
