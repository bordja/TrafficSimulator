#include "mapobject.h"
#include <QDebug>
#include "common.h"
MapObject::MapObject(QObject *parent) : QObject(parent)
{
    this->location = new Point(0, 0, SpatialReference::wgs84());
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
    this->location = new Point(0, 0, SpatialReference::wgs84());
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

MapObject::MapObject(mapObjectType type, double longitude, double latitude, int id)
{
    this->location = new Point(longitude, latitude, SpatialReference::wgs84());
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

Point* MapObject::getLocation()
{
    return this->location;
}

Point* MapObject::getImgPixPos() const
{
    return imgPixPos;
}

void MapObject::setLocation(Point *value)
{
    Point* old = location;
    location = value;

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
