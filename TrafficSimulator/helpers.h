#ifndef HELPERS_H
#define HELPERS_H

#include <QObject>
#include "Point.h"
#include "Graphic.h"

using namespace Esri::ArcGISRuntime;

class Helpers : public QObject
{
    Q_OBJECT
public:
    Helpers();
public slots:
    Geometry* createGeometryFromPoints(QList<Point*>* points);
};

#endif // HELPERS_H
