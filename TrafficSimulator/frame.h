#ifndef FRAME_H
#define FRAME_H

#include "mapobject.h"
#include <QVector>
class Frame
{
public:
    Frame();
    quint64 getTimestamp() const;
    void setTimestamp(const quint64 &value);
    void appendMapObject(MapObject* mapObject, type t);
    void printLists();
    QVector<MapObject*>* getVectorPointer(type);
private:
    QVector <MapObject*>* pedestrians;
    QVector <MapObject*>* vehicles;

    quint64 timestamp;

};

#endif // FRAME_H
