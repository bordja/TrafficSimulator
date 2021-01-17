#include "frame.h"
#include <QDebug>
Frame::Frame()
{
    this->timestamp = 0;
}

quint64 Frame::getTimestamp() const
{
    return timestamp;
}

void Frame::setTimestamp(const quint64 &value)
{
    timestamp = value;
}

void Frame::appendMapObject(MapObject *mapObject, type mapObjType)
{
    if(mapObjType == pedestrian)
    {
        this->pedestrians.append(mapObject);
    }
    else if(mapObjType == vehicle)
    {
        this->vehicles.append(mapObject);
    }
}

void Frame::printLists()
{
    for(int i = 0; i < pedestrians.size(); i++)
    {
       qDebug()<<"Pedestrian["<<i<<"]";
       pedestrians[i]->printInfo();
    }
    for(int i = 0; i < vehicles.size(); i++)
    {
       qDebug()<<"Vehicle["<<i<<"]";
       vehicles[i]->printInfo();
    }
}

QList<MapObject *> *Frame::getListPointer(type t)
{
    if(t == pedestrian)
    {
        return &pedestrians;
    }
    else if(t == vehicle)
    {
        return &vehicles;
    }
    else
    {
        return nullptr;
    }
}
