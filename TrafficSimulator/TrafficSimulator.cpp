
// Copyright 2019 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.
//

// C++ API headers
#include "Basemap.h"

// Other headers
#include "TrafficSimulator.h"

#include "Map.h"
#include "MapGraphicsView.h"
#include "common.h"
#include "PolygonBuilder.h"
#include "SimpleFillSymbol.h"
#include "referencepointlist.h"
#include <QThread>
using namespace Esri::ArcGISRuntime;

TrafficSimulator::TrafficSimulator(QWidget* parent /*=nullptr*/):
    QMainWindow(parent)
{

    // Create the Widget view
    m_mapView = new MapGraphicsView(this);

    // Create a map using the topographic Basemap
    m_map = new Map(Basemap::imagery(this), this);

    // Set map to map view
    m_mapView->setMap(m_map);

    // set the mapView as the central widget
    setCentralWidget(m_mapView);

    // set view center
    const Point center(centerViewLong, centerViewLat, SpatialReference::wgs84());
    m_mapView->setViewpointCenter(center, centerViewZoom);

    dynamicOverlay = new GraphicsOverlay(this);
    m_mapView->graphicsOverlays()->append(dynamicOverlay);

    staticOverlay = new GraphicsOverlay(this);
    m_mapView->graphicsOverlays()->append(staticOverlay);
}

void TrafficSimulator::testGraphics()
{
    ReferencePointList lightPoles;
    SimpleMarkerSymbol* s = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle, QColor(Qt::cyan), 7, this);
    for(int i = 0; i< lightPoles.getPoles()->size(); i++)
    {
        Point* p = lightPoles.getPoles()->at(i)->getBBoxTopLeft();
        Graphic* graphicPoint = new Graphic(*p,s, this);
        staticOverlay->graphics()->append(graphicPoint);
    }
}

// destructor
TrafficSimulator::~TrafficSimulator()
{
}

void TrafficSimulator::updateGraphic(Frame* frame)
{
    this->dynamicOverlay->graphics()->clear();

    QVector<MapObject*>* pedestrians = frame->getVectorPointer(PEDESTRIAN);
    QVector<MapObject*>* vehicles = frame->getVectorPointer(VEHICLE);

    for(int i = 0; i < pedestrians->size(); i++)
    {
        createObjectGraphic(pedestrians->at(i), displayType);
    }

    for(int i = 0; i < vehicles->size(); i++)
    {
        createObjectGraphic(vehicles->at(i), displayType);
    }
    emit graphicUpdated();
}

void TrafficSimulator::createObjectGraphic(MapObject* mapObject, graphicType type)
{
    if(type == POINT)
    {
        Point* p = mapObject->getBBoxTopLeft();
        SimpleMarkerSymbol* s = mapObject->getPointSymbol();
        Graphic* graphicPoint = new Graphic(*p, s, this);
        dynamicOverlay->graphics()->append(graphicPoint);
        return;
    }
    else if(type == BOX)
    {
        Point* p1 = mapObject->getBBoxTopLeft();
        Point* p2 = mapObject->getBBoxTopRight();
        Point* p3 = mapObject->getBBoxBottomLeft();
        Point* p4 = mapObject->getBBoxBottomRight();

        QList<Point> points = {*p1,*p2,*p4, *p3};
        PolygonBuilder* polygonBuilder = new PolygonBuilder(SpatialReference::wgs84(), this);
        polygonBuilder->addPoints(points);

        SimpleFillSymbol* fillSymbol = mapObject->getFillSymbol();
        Graphic* fillGraphic = new Graphic(polygonBuilder->toGeometry(), fillSymbol, this);

        dynamicOverlay->graphics()->append(fillGraphic);
        return;
    }
    else
    {
        return;
    }
}
