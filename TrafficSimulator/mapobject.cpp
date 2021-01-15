#include "mapobject.h"
#include <QDebug>
MapObject::MapObject(QObject *parent) : QObject(parent)
{
    this->location = Point(0, 0, SpatialReference::wgs84());
    this->mapObjectType = undefined;
    this->pointSymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle, QColor(Qt::white), 7, this);
}

MapObject::MapObject(type mapObjectType, quint16 xImgPix, quint16 yImgPix, quint16 bBoxWidth, quint16 bBoxHeight)
{
    this->mapObjectType = mapObjectType;
    this->imgPixPos = Point(xImgPix,yImgPix,SpatialReference::wgs84());
    this->bBoxWidth = bBoxWidth;
    this->bBoxHeight = bBoxHeight;

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
    this->location = Point(longitude, latitude, SpatialReference::wgs84());
    this->id = id;
    this->mapObjectType = mapObjectType;
}

void MapObject::printInfo()
{
    qDebug()<<"X:"<<this->imgPixPos.x();
    qDebug()<<"Y:"<<this->imgPixPos.y();
    qDebug()<<"BBW:"<<this->bBoxWidth;
    qDebug()<<"BBH:"<<this->bBoxHeight;

}

int MapObject::getId() const
{
    return id;
}

Point MapObject::getLocation() const
{
    return this->location;
}

Point* MapObject::getLocationByAddress()
{
    return &this->location;
}
