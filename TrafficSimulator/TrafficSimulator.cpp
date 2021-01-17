
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
    SimpleMarkerSymbol* s = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle, QColor(Qt::red), 7, this);
    Graphic* graphicPoint = new Graphic(Point(19,43,SpatialReference::wgs84()),s, this);
    staticOverlay->graphics()->append(graphicPoint);
}

// destructor
TrafficSimulator::~TrafficSimulator()
{
}

void TrafficSimulator::updateGraphic(Frame* frame)
{

    this->dynamicOverlay->graphics()->clear();

    QList<MapObject*>* pedestrians = frame->getListPointer(pedestrian);
    QList<MapObject*>* vehicles = frame->getListPointer(vehicle);

    for(int i = 0; i < pedestrians->size(); i++)
    {
        Point* p = pedestrians->at(i)->getLocation();
        Graphic* graphicPoint = new Graphic(*p, pedestrians->at(i)->getPointSymbol(), this);
        qDebug()<<"X"<<pedestrians->at(i)->getLocation()->x();
        qDebug()<<"Y"<<pedestrians->at(i)->getLocation()->y();
        dynamicOverlay->graphics()->append(graphicPoint);
    }
    emit graphicUpdated();
}
