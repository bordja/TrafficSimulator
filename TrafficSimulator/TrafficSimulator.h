
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

#ifndef TRAFFICSIMULATOR_H
#define TRAFFICSIMULATOR_H
#include "frame.h"
namespace Esri
{
namespace ArcGISRuntime
{
class Map;
class MapGraphicsView;
class GraphicsOverlay;
class Graphic;
}
}

#include <QMainWindow>

class TrafficSimulator : public QMainWindow
{
    Q_OBJECT
public:
    explicit TrafficSimulator(QWidget* parent = nullptr);
    void testGraphics();
    ~TrafficSimulator() override;

public slots:
    void updateGraphic(Frame*);
signals:
    void graphicUpdated();
private:
    Esri::ArcGISRuntime::Map*                   m_map = nullptr;
    Esri::ArcGISRuntime::MapGraphicsView*       m_mapView = nullptr;
    Esri::ArcGISRuntime::GraphicsOverlay*       dynamicOverlay = nullptr;
    Esri::ArcGISRuntime::GraphicsOverlay*       staticOverlay = nullptr;
};

#endif // TRAFFICSIMULATOR_H
