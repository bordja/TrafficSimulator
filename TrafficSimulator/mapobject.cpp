#include "mapobject.h"
#include <QDebug>
MapObject::MapObject(QObject *parent) : QObject(parent)
{
    this->location = new Point(0, 0, SpatialReference::wgs84());
    this->mapObjectType = undefined;
    this->pointSymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle, QColor(Qt::white), 7, this);
}

MapObject::MapObject(type mapObjectType, quint16 xImgPix, quint16 yImgPix, quint16 bBoxWidth, quint16 bBoxHeight)
{
    this->mapObjectType = mapObjectType;
    this->imgPixPos = new Point(xImgPix,yImgPix,SpatialReference::wgs84());
    this->bBoxWidth = bBoxWidth;
    this->bBoxHeight = bBoxHeight;
    this->location = new Point(0, 0, SpatialReference::wgs84());
    if(mapObjectType == pedestrian)
    {
        this->pointSymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle, QColor(Qt::red), 7, this);
    }
    else if(mapObjectType == vehicle)
    {
        this->pointSymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle, QColor(Qt::green), 7, this);
    }
    else
    {
        this->pointSymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle, QColor(Qt::white), 7, this);
    }
}

MapObject::MapObject(type mapObjectType, double longitude, double latitude, int id)
{
    this->location = new Point(longitude, latitude, SpatialReference::wgs84());
    this->id = id;
    this->mapObjectType = mapObjectType;
    if(mapObjectType == pedestrian)
    {
        this->pointSymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle, QColor(Qt::red), 7, this);
    }
    else if(mapObjectType == vehicle)
    {
        this->pointSymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle, QColor(Qt::green), 7, this);
    }
    else
    {
        this->pointSymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle, QColor(Qt::white), 7, this);
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
