#include <QDataStream>
#include "streammanager.h"
#include "common.h"
StreamManager::StreamManager()
{
    finalFrame = new Frame();
    this->streams = new QList<Stream*>;
}

StreamManager::StreamManager(TrafficSimulator& simulator)
{
    finalFrame = new Frame();
    this->streams = new QList<Stream*>;
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
    int mode = 0;
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
    switch (mode) {
    case 0:
//        double longitude;
//        double latitude;
        finalFrame->getVectorPointer(PEDESTRIAN)->clear();
        finalFrame->getVectorPointer(VEHICLE)->clear();
        for(int i = 0; i < streams->size(); i++)
        {
//            for(int k = 0; k < streams->at(i)->getFrame()->getVectorPointer(PEDESTRIAN)->size(); k++)
//            {
//                longitude = streams->at(i)->getFrame()->getVectorPointer(PEDESTRIAN)->at(k)->getLocation()->x();
//                latitude = streams->at(i)->getFrame()->getVectorPointer(PEDESTRIAN)->at(k)->getLocation()->y();
//                MapObject* p = new MapObject(PEDESTRIAN, longitude, latitude, 0);
//                finalFrame->appendMapObject(p, PEDESTRIAN);
//            }
//            for(int k = 0; k < streams->at(i)->getFrame()->getVectorPointer(VEHICLE)->size(); k++)
//            {
//                longitude = streams->at(i)->getFrame()->getVectorPointer(VEHICLE)->at(k)->getLocation()->x();
//                latitude = streams->at(i)->getFrame()->getVectorPointer(VEHICLE)->at(k)->getLocation()->y();
//                MapObject* v = new MapObject(VEHICLE, longitude, latitude, 0);
//                finalFrame->appendMapObject(v, VEHICLE);
//            }
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
