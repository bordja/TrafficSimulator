#include <QDataStream>
#include "streammanager.h"
#include "common.h"
#include "PolygonBuilder.h"
#include <GeometryEngine.h>
#include <QThread>

StreamManager::StreamManager()
{
    finalFrame = new Frame();
    this->streams = new QList<Stream*>;
    this->pairs = new QList<QPair<MapObject*,MapObject*>*>;
}

StreamManager::StreamManager(TrafficSimulator& simulator)
{
    finalFrame = new Frame();
    this->streams = new QList<Stream*>;
    this->pairs = new QList<QPair<MapObject*,MapObject*>*>;
    connect(this, &StreamManager::dataReady,&simulator,&TrafficSimulator::updateGraphic);
    connect(&simulator,&TrafficSimulator::graphicUpdated,this, &StreamManager::readStreams);
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
    for(int i = 0; i < streams->size(); i++)
    {
        if(streams->at(i)->getState() == ACTIVE)
        {
            streams->at(i)->readFileData();
            streams->at(i)->calculateCoordinates(PEDESTRIAN);
            streams->at(i)->calculateCoordinates(VEHICLE);
        }    
    }
    updateFinalFrame(alg);
    updateActiveTimestamp();
    updateActiveStreams();

    emit(dataReady(this->finalFrame));
}

void StreamManager::updateFinalFrame(int mode)
{
    finalFrame->getVectorPointer(PEDESTRIAN)->clear();
    finalFrame->getVectorPointer(VEHICLE)->clear();
    switch (mode) {
    case NO_MERGING:
        for(int i = 0; i < streams->size(); i++)
        {
            fillFinalFrameData(PEDESTRIAN, streams->at(i));
            fillFinalFrameData(VEHICLE, streams->at(i));
        }
        break;

    case MERGE_DOUBLES:
        //mergeDoubles(PEDESTRIAN);
        mergeDoubles(VEHICLE);
        for(int i = 0; i < streams->size(); i++)
        {
            fillFinalFrameData(PEDESTRIAN, streams->at(i));
            fillFinalFrameData(VEHICLE, streams->at(i));
        }
        break;
    }
}

void StreamManager::fillFinalFrameData(mapObjectType type, Stream* stream)
{
    QVector<MapObject*>* vectorPtr = stream->getFrame()->getVectorPointer(type);
    for(int i = 0; i < vectorPtr->size(); i++)
    {
        MapObject* finalFrameObject = vectorPtr->at(i);
        if(finalFrameObject->getIsValid())
        {
            finalFrame->appendMapObject(finalFrameObject, type);
        }

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
    /* Detect overlaping objects from different cameras */

    for(int i = 0; i < streams->size(); i++)
    {
        for(int j = 0; j < streams->size(); j++)
        {
            if(i != j)
            {
                checkOverlapBBox(streams->at(i),streams->at(j), type);
            }
        }
    }

    /* Mergeing doubles and adding them to the final frame */
    merge();
    pairs->clear();
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

            if(GeometryEngine::intersects(*geometry1, *geometry2))
            {
                QPair<MapObject*, MapObject*>* pair = new QPair<MapObject*, MapObject*>;
                pair->first = vector1->at(i);
                pair->second = vector2->at(j);
                appendPair(pair);

            }
            /* TO DO: FIX DELETES*/
            delete qList;
            delete polygonBuilder2;


        }
        delete pList;
        delete polygonBuilder1;
    }

}

void StreamManager::appendPair(QPair<MapObject *, MapObject *> * pair)
{
    for(int i = 0; i < pairs->size(); i++)
    {
        /* Removing double pairs */
        if(pair->first == pairs->at(i)->second)
        {
            if(pair->second == pairs->at(i)->first)
            {
                delete pair;
                return;
            }
        }
    }
    pairs->append(pair);
}

void StreamManager::merge()
{
    double longitudeFirst;
    double latitudeFirst;
    double longitudeSecond;
    double latitudeSecond;

    for(int i = 0; i < pairs->size(); i++)
    {
        pairs->at(i)->first->setIsValid(false);
        pairs->at(i)->second->setIsValid(false);

        pairs->at(i)->first->calculateCenter();
        pairs->at(i)->second->calculateCenter();

        longitudeFirst = pairs->at(i)->first->getCenter()->x();
        latitudeFirst = pairs->at(i)->first->getCenter()->y();
        longitudeSecond = pairs->at(i)->second->getCenter()->x();
        latitudeSecond = pairs->at(i)->second->getCenter()->y();

        MapObject* merged = new MapObject(pairs->at(i)->first->getType() \
                                          , (longitudeFirst + longitudeSecond) / 2 \
                                          ,(latitudeFirst + latitudeSecond) / 2 \
                                          ,0);

        longitudeFirst = pairs->at(i)->first->getBBoxTopLeft()->x();
        latitudeFirst = pairs->at(i)->first->getBBoxTopLeft()->y();
        longitudeSecond = pairs->at(i)->second->getBBoxTopLeft()->x();
        latitudeSecond = pairs->at(i)->second->getBBoxTopLeft()->y();

        Point* topLeft = new Point((longitudeFirst+longitudeSecond) / 2 \
                                   , (latitudeFirst+latitudeSecond) /2 \
                                   , SpatialReference::wgs84());
        merged->setBBoxTopLeft(topLeft);

        longitudeFirst = pairs->at(i)->first->getBBoxTopRight()->x();
        latitudeFirst = pairs->at(i)->first->getBBoxTopRight()->y();
        longitudeSecond = pairs->at(i)->second->getBBoxTopRight()->x();
        latitudeSecond = pairs->at(i)->second->getBBoxTopRight()->y();
        Point* topRight = new Point((longitudeFirst+longitudeSecond) / 2 \
                                   , (latitudeFirst+latitudeSecond) / 2 \
                                   , SpatialReference::wgs84());
        merged->setBBoxTopRight(topRight);

        longitudeFirst = pairs->at(i)->first->getBBoxBottomLeft()->x();
        latitudeFirst = pairs->at(i)->first->getBBoxBottomLeft()->y();
        longitudeSecond = pairs->at(i)->second->getBBoxBottomLeft()->x();
        latitudeSecond = pairs->at(i)->second->getBBoxBottomLeft()->y();
        Point* bottomLeft = new Point((longitudeFirst+longitudeSecond) / 2 \
                                   , (latitudeFirst+latitudeSecond) / 2 \
                                   , SpatialReference::wgs84());
        merged->setBBoxBottomLeft(bottomLeft);

        longitudeFirst = pairs->at(i)->first->getBBoxBottomRight()->x();
        latitudeFirst = pairs->at(i)->first->getBBoxBottomRight()->y();
        longitudeSecond = pairs->at(i)->second->getBBoxBottomRight()->x();
        latitudeSecond = pairs->at(i)->second->getBBoxBottomRight()->y();
        Point* bottomRight = new Point((longitudeFirst+longitudeSecond) / 2 \
                                   , (latitudeFirst+latitudeSecond) / 2 \
                                   , SpatialReference::wgs84());
        merged->setBBoxBottomRight(bottomRight);

        finalFrame->appendMapObject(merged, merged->getType());
    }
}

