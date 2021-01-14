#include <QDataStream>
#include "streammanager.h"
#include "common.h"
StreamManager::StreamManager()
{

}

void StreamManager::addStream(Stream &stream)
{
    this->streams.append(&stream);
}

void StreamManager::init()
{
    quint64 timestamps [maxStreamNum];
    quint64 minTimestamp;

    /* Read and set initial timestamp for each stream */
    for(int i = 0; i < streams.size(); i++)
    {
        timestamps[i] = streams[i]->readInitialTimestamp();
        streams[i]->getFrame().setTimestamp(timestamps[i]);
    }

    /* Find minimal timestamp */
    minTimestamp = timestamps[0];
    for(int i = 1; i < streams.size(); i++)
    {
        if(minTimestamp > timestamps[i]){
            minTimestamp = timestamps[i];
        }
    }

    this->activeTimestamp = minTimestamp;
}

void StreamManager::readStreams()
{

}

void StreamManager::updateActiveStreams()
{
    for(int i = 1; i < streams.size(); i++)
    {
        if((quint64)(streams[i]->getFrame().getTimestamp() - activeTimestamp) < fps60Freq)
        {
            streams[i]->setIsActive(true);
        }
    }
}
