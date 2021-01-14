
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

namespace Esri
{
namespace ArcGISRuntime
{
class Map;
class MapGraphicsView;
}
}

#include <QMainWindow>

class TrafficSimulator : public QMainWindow
{
    Q_OBJECT
public:
    explicit TrafficSimulator(QWidget* parent = nullptr);
    ~TrafficSimulator() override;

public slots:

private:
    Esri::ArcGISRuntime::Map*                   m_map = nullptr;
    Esri::ArcGISRuntime::MapGraphicsView*       m_mapView = nullptr;
};

#endif // TRAFFICSIMULATOR_H
