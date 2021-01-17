#include <QDataStream>
#include "streammanager.h"
#include "common.h"
StreamManager::StreamManager()
{
    finalFrame = new Frame();
}

StreamManager::StreamManager(TrafficSimulator& simulator)
{
    finalFrame = new Frame();
    connect(this, &StreamManager::dataReady,&simulator,&TrafficSimulator::updateGraphic);
    connect(&simulator,&TrafficSimulator::graphicUpdated,this, &StreamManager::readStreams);
}

void StreamManager::addStream(Stream &stream)
{
    this->streams.append(&stream);
}

void StreamManager::run()
{
    readStreams();
}

void StreamManager::init()
{
    updateActiveTimestamp();
    updateActiveStreams();
}

void StreamManager::readStreams()
{
    int mode = 0;
    for(int i = 0; i < streams.size(); i++)
    {
        if(streams[i]->getIsActive())
        {
            streams[i]->readFileData();
            streams[i]->calculateCoordinates(pedestrian);
            streams[i]->calculateCoordinates(vehicle);
        }
        updateFinalFrame(mode);
        updateActiveTimestamp();
        updateActiveStreams();

        emit(dataReady(this->finalFrame));
    }
}

void StreamManager::updateFinalFrame(int mode)
{
    switch (mode) {
    case 0:
        finalFrame->getListPointer(pedestrian)->clear();
        finalFrame->getListPointer(vehicle)->clear();
        for(int i = 0; i < streams.size(); i++)
        {
            for(int k = 0; k < streams[i]->getFrame().getListPointer(pedestrian)->size(); k++)
            {
                finalFrame->appendMapObject(streams[i]->getFrame().getListPointer(pedestrian)->at(k), pedestrian);
            }
            for(int j = 0; j < streams[i]->getFrame().getListPointer(vehicle)->size(); j++)
            {
                finalFrame->appendMapObject(streams[i]->getFrame().getListPointer(vehicle)->at(j), vehicle);
            }
        }
        break;
    }
}

void StreamManager::updateActiveStreams()
{
    for(int i = 0; i < streams.size(); i++)
    {
        if((quint64)(streams[i]->getFrame().getTimestamp() - activeTimestamp) < fps60Freq)
        {
            streams[i]->setIsActive(true);
        }
    }
}

void StreamManager::updateActiveTimestamp()
{
    quint64 timestamps [maxStreamNum];
    quint64 minTimestamp;

    for(int i = 0; i < streams.size(); i++)
    {
        timestamps[i] = streams[i]->readNextTimestamp();
        streams[i]->getFrame().setTimestamp(timestamps[i]);
    }

    minTimestamp = timestamps[0];

    for(int i = 0; i < streams.size(); i++)
    {
        if(minTimestamp > timestamps[i]){
            minTimestamp = timestamps[i];
        }
    }

    this->activeTimestamp = minTimestamp;
}
