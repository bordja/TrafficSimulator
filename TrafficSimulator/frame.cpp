#include "frame.h"
#include <QDebug>
Frame::Frame()
{
    this->timestamp = 0;
    this->pedestrians = new QVector<MapObject*>;
    this->vehicles = new QVector<MapObject*>;
}

quint64 Frame::getTimestamp() const
{
    return timestamp;
}

void Frame::setTimestamp(const quint64 &value)
{
    timestamp = value;
}

void Frame::appendMapObject(MapObject *mapObject, mapObjectType type)
{
    if(type == PEDESTRIAN)
    {
        this->pedestrians->append(mapObject);
    }
    else if(type == VEHICLE)
    {
        this->vehicles->append(mapObject);
    }
}

void Frame::printLists()
{
    for(int i = 0; i < pedestrians->size(); i++)
    {
       qDebug()<<"Pedestrian["<<i<<"]";
       pedestrians->at(i)->printInfo();
    }
    for(int i = 0; i < vehicles->size(); i++)
    {
       qDebug()<<"Vehicle["<<i<<"]";
       vehicles->at(i)->printInfo();
    }
}

QVector<MapObject*>* Frame::getVectorPointer(mapObjectType type)
{
    if(type == PEDESTRIAN)
    {
        return this->pedestrians;
    }
    else if(type == VEHICLE)
    {
        return this->vehicles;
    }
    else
    {
        return nullptr;
    }
}
