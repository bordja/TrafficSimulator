#ifndef REFERENCEPOINTLIST_H
#define REFERENCEPOINTLIST_H

#include <QList>
#include "mapobject.h"
class ReferencePointList
{
public:
    ReferencePointList();
    Point* getPoleLocationFromId(int id);
private:
    QList<MapObject*>poles;
};

#endif // REFERENCEPOINTLIST_H
