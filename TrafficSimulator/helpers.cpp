#include "helpers.h"
#include "PolygonBuilder.h"
Helpers::Helpers()
{

}

Geometry* Helpers::createGeometryFromPoints(QList<Point *>* points)
{
    qDebug()<<"SLOT";
    PolygonBuilder* polygonBuilder = new PolygonBuilder(SpatialReference::wgs84(), this);
    for(int i = 0; i < points->size(); i++)
    {
        polygonBuilder->addPoint(*(points->at(i)));
    }
    Geometry* geometry = new Geometry(polygonBuilder->toGeometry());
    delete polygonBuilder;
    return geometry;
}
