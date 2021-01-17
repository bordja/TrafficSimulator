#ifndef FRAME_H
#define FRAME_H

#include "mapobject.h"

class Frame
{
public:
    Frame();
    quint64 getTimestamp() const;
    void setTimestamp(const quint64 &value);
    void appendMapObject(MapObject* mapObject, type t);
    void printLists();
    QList<MapObject*>* getListPointer(type);
private:
    QList <MapObject*> pedestrians;
    QList <MapObject*> vehicles;
    quint64 timestamp;

};

#endif // FRAME_H
