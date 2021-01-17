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
        if(streams->at(i)->getIsActive())
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
        streams->at(i)->getConstants()->calculateConstants();
    }
    updateActiveTimestamp();
    updateActiveStreams();
}

void StreamManager::readStreams()
{
    int mode = 0;
    for(int i = 0; i < streams->size(); i++)
    {
        if(streams->at(i)->getIsActive())
        {
            streams->at(i)->readFileData();
            streams->at(i)->calculateCoordinates(pedestrian);
            streams->at(i)->calculateCoordinates(vehicle);
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
        double longitude;
        double latitude;
        finalFrame->getVectorPointer(pedestrian)->clear();
        finalFrame->getVectorPointer(vehicle)->clear();
        for(int i = 0; i < streams->size(); i++)
        {
            for(int k = 0; k < streams->at(i)->getFrame()->getVectorPointer(pedestrian)->size(); k++)
            {
                longitude = streams->at(i)->getFrame()->getVectorPointer(pedestrian)->at(k)->getLocation()->x();
                latitude = streams->at(i)->getFrame()->getVectorPointer(pedestrian)->at(k)->getLocation()->y();
                MapObject* p = new MapObject(pedestrian, longitude, latitude, 0);
                finalFrame->appendMapObject(p, pedestrian);
            }
            for(int k = 0; k < streams->at(i)->getFrame()->getVectorPointer(vehicle)->size(); k++)
            {
                longitude = streams->at(i)->getFrame()->getVectorPointer(vehicle)->at(k)->getLocation()->x();
                latitude = streams->at(i)->getFrame()->getVectorPointer(vehicle)->at(k)->getLocation()->y();
                MapObject* v = new MapObject(vehicle, longitude, latitude, 0);
                finalFrame->appendMapObject(v, vehicle);
            }
        }
        break;
    }
}

void StreamManager::updateActiveStreams()
{
    for(int i = 0; i < streams->size(); i++)
    {
        if((quint64)(streams->at(i)->getFrame()->getTimestamp() - activeTimestamp) < fps60Freq)
        {
            streams->at(i)->setIsActive(true);
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
