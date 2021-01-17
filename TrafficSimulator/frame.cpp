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

void Frame::appendMapObject(MapObject *mapObject, type mapObjType)
{
    if(mapObjType == pedestrian)
    {
        this->pedestrians->append(mapObject);
    }
    else if(mapObjType == vehicle)
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

QVector<MapObject*>* Frame::getVectorPointer(type t)
{
    if(t == pedestrian)
    {
        return this->pedestrians;
    }
    else if(t == vehicle)
    {
        return this->vehicles;
    }
    else
    {
        return nullptr;
    }
}
