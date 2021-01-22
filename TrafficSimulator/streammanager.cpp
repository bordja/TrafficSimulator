#include <QDataStream>
#include "streammanager.h"
#include "common.h"
#include "PolygonBuilder.h"
#include <GeometryEngine.h>
#include <QThread>
static int cnt = 0;
StreamManager::StreamManager()
{
    finalFrame = new Frame();
    this->streams = new QList<Stream*>;
    this->pairs = new QList<QPair<MapObject*,MapObject*>*>;
}

StreamManager::StreamManager(TrafficSimulator& simulator, Helpers& helpers)
{
    finalFrame = new Frame();
    this->streams = new QList<Stream*>;
    connect(this, &StreamManager::dataReady,&simulator,&TrafficSimulator::updateGraphic);
    connect(&simulator,&TrafficSimulator::graphicUpdated,this, &StreamManager::readStreams);
    connect(this, &StreamManager::getGeometryFromPoints, &helpers, &Helpers::createGeometryFromPoints, Qt::QueuedConnection);
//    connect(this,SIGNAL(Geometry* StreamManager::getGeometryFromPoints(QList<MapObject*>*)), &helpers, SLOT(createGeometryFromPoints(QList<MapObject*>*)));
}

void StreamManager::addStream(Stream* stream)
{
    this->streams->push_back(stream);
    connect(stream, &Stream::streamFinished, this, &StreamManager::streamsFinished);
}

void StreamManager::streamsFinished()
{
    for(int i = 0; i < streams->size(); i++)
    {
        if(streams->at(i)->getState() == ACTIVE)
        {
            return;
        }
    }
    emit(finished());
    return;
}

void StreamManager::run()
{

    init();
    readStreams();
}

void StreamManager::init()
{
    for(int i = 0; i < streams->size(); i++)
    {
        streams->at(i)->readHeader();
    }
    updateActiveTimestamp();
    updateActiveStreams();
}

void StreamManager::readStreams()
{
    int mode = 1;
    for(int i = 0; i < streams->size(); i++)
    {
        if(streams->at(i)->getState() == ACTIVE)
        {
            streams->at(i)->readFileData();
            streams->at(i)->calculateCoordinates(PEDESTRIAN);
            streams->at(i)->calculateCoordinates(VEHICLE);
        }    
    }
    updateFinalFrame(mode);
    updateActiveTimestamp();
    updateActiveStreams();

    emit(dataReady(this->finalFrame));
}

void StreamManager::updateFinalFrame(int mode)
{
    finalFrame->getVectorPointer(PEDESTRIAN)->clear();
    finalFrame->getVectorPointer(VEHICLE)->clear();
    switch (mode) {
    case 0:
        for(int i = 0; i < streams->size(); i++)
        {
            fillFinalFrameData(PEDESTRIAN, streams->at(i));
            fillFinalFrameData(VEHICLE, streams->at(i));
        }
        break;

    case 1:
        //mergeDoubles(PEDESTRIAN);
        mergeDoubles(VEHICLE);
        break;
    }
}

void StreamManager::fillFinalFrameData(mapObjectType type, Stream* stream)
{
    QVector<MapObject*>* vectorPtr = stream->getFrame()->getVectorPointer(type);
    for(int i = 0; i < vectorPtr->size(); i++)
    {
        MapObject* finalFrameObject = vectorPtr->at(i);
        finalFrame->appendMapObject(finalFrameObject, type);
    }
}

void StreamManager::updateActiveStreams()
{
    for(int i = 0; i < streams->size(); i++)
    {
        if((quint64)(streams->at(i)->getFrame()->getTimestamp() - activeTimestamp) < fps60Freq)
        {
            if(streams->at(i)->getState() == NOT_ACTIVE)
            {
                streams->at(i)->setState(ACTIVE);
            }
        }
    }
}

void StreamManager::updateActiveTimestamp()
{
    quint64 timestamps [maxStreamNum];
    quint64 minTimestamp;

    for(int i = 0; i < streams->size(); i++)
    {
        timestamps[i] = streams->at(i)->readNextTimestamp();
        streams->at(i)->getFrame()->setTimestamp(timestamps[i]);
    }

    minTimestamp = timestamps[0];

    for(int i = 0; i < streams->size(); i++)
    {
        if(minTimestamp > timestamps[i]){
            minTimestamp = timestamps[i];
        }
    }

    this->activeTimestamp = minTimestamp;
}

void StreamManager::mergeDoubles(mapObjectType type)
{
    static int cnt = 0;
    for(int i = 0; i < streams->size(); i++)
    {
        for(int j = 0; j < streams->size(); j++)
        {
            if(i != j)
            {
                checkOverlapBBox(streams->at(i),streams->at(j), type);
                cnt++;
            }
        }
    }
}

void StreamManager::checkOverlapBBox(Stream* stream1, Stream* stream2, mapObjectType type)
{
    QVector<MapObject*>* vector1 = stream1->getFrame()->getVectorPointer(type);
    QVector<MapObject*>* vector2 = stream2->getFrame()->getVectorPointer(type);
    Geometry gUnion;

    for(int i = 0; i < vector1->size(); i++)
    {
        Point* p1 = vector1->at(i)->getBBoxTopLeft();
        Point* p2 = vector1->at(i)->getBBoxTopRight();
        Point* p3 = vector1->at(i)->getBBoxBottomRight();
        Point* p4 = vector1->at(i)->getBBoxBottomLeft();
        QList<Point*>* pList = new QList<Point*>;
        pList->append(p1);
        pList->append(p2);
        pList->append(p3);
        pList->append(p4);

        PolygonBuilder* polygonBuilder1 = new PolygonBuilder(SpatialReference::wgs84(), this);
        for(int i = 0; i < pList->size(); i++)
        {
            polygonBuilder1->addPoint(*(pList->at(i)));
        }

        Geometry* geometry1 = new Geometry(polygonBuilder1->toGeometry());
        for(int j = 0; j < vector2->size(); j++)
        {
            Point* q1 = vector2->at(j)->getBBoxTopLeft();
            Point* q2 = vector2->at(j)->getBBoxTopRight();
            Point* q3 = vector2->at(j)->getBBoxBottomRight();
            Point* q4 = vector2->at(j)->getBBoxBottomLeft();
            QList<Point*>* qList = new QList<Point*>;
            qList->append(q1);
            qList->append(q2);
            qList->append(q3);
            qList->append(q4);

            PolygonBuilder* polygonBuilder2 = new PolygonBuilder(SpatialReference::wgs84(), this);
            for(int i = 0; i < qList->size(); i++)
            {
                polygonBuilder2->addPoint(*(qList->at(i)));
            }

            Geometry* geometry2 = new Geometry(polygonBuilder2->toGeometry());


//            GeometryEngine::intersects(*geometry1, *geometry2);
//            qDebug()<<GeometryEngine::area(gUnion) * 1000000000;

            if(GeometryEngine::intersects(*geometry1, *geometry2))
            {
                finalFrame->appendMapObject(vector1->at(i), type);
                finalFrame->appendMapObject(vector2->at(j), type);
            }
            /* TO DO: FIX DELETES*/
            delete qList;
            delete polygonBuilder2;


        }
        delete pList;
        delete polygonBuilder1;
    }

}

