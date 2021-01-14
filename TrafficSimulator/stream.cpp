#include <QThread>
#include <QDataStream>
#include <QDebug>
#include "stream.h"
#include "common.h"
#include "frame.h"
#include "mapobject.h"

Stream::Stream()
{
    this->currentFrame = 0;
    this->isActive=false;
}

Stream::Stream(QString fileName)
{
    this->openFile(fileName);
    this->currentFrame = 0;
    this->isActive=false;
}

bool Stream::openFile(QString filePath)
{
    this->file = new QFile(filePath);
    return this->file->open(QIODevice::ReadOnly);
}

void Stream::readFileData()
{
    QThread::msleep(sleepMs);
    quint64 timestamp;
    quint16 numPedestrian;
    quint16 numVehicle;
    quint64 filePosition;

    updateCurrentFrame();
    this->file->seek(headerSize + frameSize * this->currentFrame);

    QDataStream collector(this->file);
    collector.setByteOrder(QDataStream::LittleEndian);

    collector>>timestamp;
    collector>>numPedestrian;

    fillFrameObjectList(collector,numPedestrian,pedestrian);

    filePosition = this->file->pos();
    filePosition += (maxPedestrianNum - numPedestrian) * 8;
    this->file->seek(filePosition);

    collector>>numVehicle;
    fillFrameObjectList(collector,numVehicle,vehicle);

    this->frame.setTimestamp(timestamp);

}

Frame Stream::getFrame() const
{
    return frame;
}

void Stream::setFrame(const Frame &value)
{
    frame = value;
}

quint64 Stream::readInitialTimestamp()
{
    quint64 initTimestamp;
    this->file->seek(headerSize + frameSize * this->currentFrame);

    QDataStream collector(this->file);
    collector.setByteOrder(QDataStream::LittleEndian);

    collector>>initTimestamp;
    return initTimestamp;
}

bool Stream::getIsActive() const
{
    return isActive;
}

void Stream::setIsActive(bool value)
{
    isActive = value;
}

void Stream::fillFrameObjectList(QDataStream &collector, int mapObjectNum, type mapObjectType)
{
    quint16 xPixTmp;
    quint16 yPixTmp;
    quint16 bBoxWidthTmp;
    quint16 bBoxHeightTmp;

    for(int i = 0; i < mapObjectNum; i++)
    {
        collector>>xPixTmp;
        collector>>yPixTmp;
        collector>>bBoxWidthTmp;
        collector>>bBoxHeightTmp;

        MapObject* mapObject = new MapObject(mapObjectType, xPixTmp, yPixTmp, bBoxWidthTmp, bBoxHeightTmp);

        this->frame.appendMapObject(mapObject, mapObjectType);
    }
}

void Stream::updateCurrentFrame()
{
    static bool first = true;

    if(!first)
    {
        this->currentFrame++;
    }
    else
    {
        first = false;
    }
}
