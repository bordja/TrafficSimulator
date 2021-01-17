#include <QThread>
#include <QDataStream>
#include <QDebug>
#include "stream.h"
#include "common.h"
#include "frame.h"
#include "mapobject.h"
#include "PointBuilder.h"
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
    numberOfFrames = (this->file->size() - headerSize) / frameSize;
    frame = Frame();
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

    /* Update current frame for the next read */

    updateCurrentFrame();

}

void Stream::readHeader()
{
    quint8 cameraId;

    quint32 originPoleId;
    quint16 originX;
    quint16 originY;
    quint32 upperPoleId;
    quint16 upperPoleX;
    quint16 upperPoleY;
    quint32 rightPoleId;
    quint16 rightPoleX;
    quint16 rightPoleY;
    quint32 notUsedPoleId;
    quint16 notUsedPoleX;
    quint16 notUsedPoleY;

    QDataStream collector(this->file);
    collector.setByteOrder(QDataStream::LittleEndian);
    this->file->seek(0);

    collector>>cameraId;
    collector>>upperPoleId;
    collector>>notUsedPoleId;
    collector>>originPoleId;
    collector>>rightPoleId;
    collector>>upperPoleX;
    collector>>upperPoleY;
    collector>>notUsedPoleX;
    collector>>notUsedPoleY;
    collector>>originX;
    collector>>originY;
    collector>>rightPoleX;
    collector>>rightPoleY;

    constants.setOriginPoleId(originPoleId);
    constants.setUpperPoleId(upperPoleId);
    constants.setRightPoleId(rightPoleId);
    constants.setOriginX(originX);
    constants.setOriginY(imageHeight - originY);
    constants.setUpperPoleX(upperPoleX);
    constants.setUpperPoleY(imageHeight - upperPoleY);
    constants.setRightPoleX(rightPoleX);
    constants.setRightPoleY(imageHeight - rightPoleY);

    constants.calculateConstants();
}

void Stream::printStreamConstants()
{
    qDebug()<<"ORIGIN(X): "<<qSetRealNumberPrecision(realNumPrintPrecision)<<constants.getOriginLong();
    qDebug()<<"ORIGIN(Y): "<<qSetRealNumberPrecision(realNumPrintPrecision)<<constants.getOriginLat();
    qDebug()<<"UPPER(X): "<<qSetRealNumberPrecision(realNumPrintPrecision)<<constants.getUpperPoleLong();
    qDebug()<<"UPPER(Y): "<<qSetRealNumberPrecision(realNumPrintPrecision)<<constants.getUpperPoleLat();
    qDebug()<<"RIGHT(Y): "<<qSetRealNumberPrecision(realNumPrintPrecision)<<constants.getRightPoleLong();
    qDebug()<<"RIGHT(Y): "<<qSetRealNumberPrecision(realNumPrintPrecision)<<constants.getRightPoleLat();

    qDebug()<<"HPDX: "<<qSetRealNumberPrecision(realNumPrintPrecision)<<constants.getHorisontalPixelDisplacementX();
    qDebug()<<"HPDY: "<<qSetRealNumberPrecision(realNumPrintPrecision)<<constants.getHorisontalPixelDisplacementY();
    qDebug()<<"VPDX: "<<qSetRealNumberPrecision(realNumPrintPrecision)<<constants.getVerticalPixelDisplacementX();
    qDebug()<<"VPDY: "<<qSetRealNumberPrecision(realNumPrintPrecision)<<constants.getVerticalPixelDisplacementY();
}

void Stream::calculateCoordinates(type mapObjectType)
{
    quint16 xImgPix;
    quint16 yImgPix;
    quint16 deltaX;
    quint16 deltaY;
    double longitude;
    double latitude;
    QList<MapObject*>* objectList = this->getFrame().getListPointer(mapObjectType);

    for(int i = 0; i < objectList->size(); i++)
    {
        longitude = constants.getOriginLong();
        latitude = constants.getOriginLat();

        xImgPix = objectList->at(i)->getImgPixPos()->x();
        yImgPix = objectList->at(i)->getImgPixPos()->y();
        deltaX = xImgPix - constants.getOriginX();
        deltaY = yImgPix - constants.getOriginY();

        longitude += ((deltaX * constants.getHorisontalPixelDisplacementX()) + (deltaY * constants.getVerticalPixelDisplacementX()));
        latitude += ((deltaX * constants.getHorisontalPixelDisplacementY()) + (deltaY * constants.getVerticalPixelDisplacementY()));

        PointBuilder pointBuilder(*(objectList->at(i)->getLocation()));
        pointBuilder.setX(longitude);
        pointBuilder.setY(latitude);
        Point location = pointBuilder.toPoint();
        objectList->at(i)->setLocation(&location);
    }
}

Frame& Stream::getFrame()
{
    return frame;
}

void Stream::setFrame(const Frame &value)
{
    frame = value;
}

quint64 Stream::readNextTimestamp()
{
    quint64 timestamp;
    this->file->seek(headerSize + frameSize * this->currentFrame);

    QDataStream collector(this->file);
    collector.setByteOrder(QDataStream::LittleEndian);

    collector>>timestamp;
    return timestamp;
}

bool Stream::getIsActive() const
{
    return isActive;
}

void Stream::setIsActive(bool value)
{
    isActive = value;
}

quint16 Stream::getNumberOfFrames() const
{
    return numberOfFrames;
}

void Stream::setNumberOfFrames(const quint16 &value)
{
    numberOfFrames = value;
}

void Stream::fillFrameObjectList(QDataStream &collector, int mapObjectNum, type mapObjectType)
{
    quint16 xPixTmp;
    quint16 yPixTmp;
    quint16 bBoxWidthTmp;
    quint16 bBoxHeightTmp;

    if(!(this->getFrame().getListPointer(mapObjectType)->isEmpty()))
    {
        this->getFrame().getListPointer(mapObjectType)->clear();
    }

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

    this->currentFrame++;
    if(currentFrame == numberOfFrames)
    {
        this->isActive = false;
        this->getFrame().setTimestamp(-1);
        qDebug()<<"finished, timestamp is set to: "<<this->getFrame().getTimestamp();
    }
}
