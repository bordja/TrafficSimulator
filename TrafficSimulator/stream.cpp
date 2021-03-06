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
    this->state = NOT_ACTIVE;
}

Stream::Stream(QString fileName)
{
    this->openFile(fileName);
    this->currentFrame = 0;
    this->state = NOT_ACTIVE;
    this->numberOfFrames = (this->file->size() - headerSize) / frameSize;
    this->frame = new Frame();
    this->constants = new StreamConstants();
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

    fillFrameObjectList(collector,numPedestrian, PEDESTRIAN);

    filePosition = this->file->pos();
    filePosition += (maxPedestrianNum - numPedestrian) * 8;
    this->file->seek(filePosition);

    collector>>numVehicle;
    fillFrameObjectList(collector,numVehicle, VEHICLE);

    this->frame->setTimestamp(timestamp);

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

    constants->setCameraId(cameraId);
    constants->setOriginPoleId(originPoleId);
    constants->setUpperPoleId(upperPoleId);
    constants->setRightPoleId(rightPoleId);
    constants->setOriginX(originX);
    constants->setOriginY(imageHeight - originY);
    constants->setUpperPoleX(upperPoleX);
    constants->setUpperPoleY(imageHeight - upperPoleY);
    constants->setRightPoleX(rightPoleX);
    constants->setRightPoleY(imageHeight - rightPoleY);

    constants->calculateConstants();
}

void Stream::printStreamConstants()
{
    qDebug()<<"ORIGIN(X): "<<qSetRealNumberPrecision(realNumPrintPrecision)<<constants->getOriginLong();
    qDebug()<<"ORIGIN(Y): "<<qSetRealNumberPrecision(realNumPrintPrecision)<<constants->getOriginLat();
    qDebug()<<"UPPER(X): "<<qSetRealNumberPrecision(realNumPrintPrecision)<<constants->getUpperPoleLong();
    qDebug()<<"UPPER(Y): "<<qSetRealNumberPrecision(realNumPrintPrecision)<<constants->getUpperPoleLat();
    qDebug()<<"RIGHT(Y): "<<qSetRealNumberPrecision(realNumPrintPrecision)<<constants->getRightPoleLong();
    qDebug()<<"RIGHT(Y): "<<qSetRealNumberPrecision(realNumPrintPrecision)<<constants->getRightPoleLat();

    qDebug()<<"HPDX: "<<qSetRealNumberPrecision(realNumPrintPrecision)<<constants->getHorisontalPixelDisplacementX();
    qDebug()<<"HPDY: "<<qSetRealNumberPrecision(realNumPrintPrecision)<<constants->getHorisontalPixelDisplacementY();
    qDebug()<<"VPDX: "<<qSetRealNumberPrecision(realNumPrintPrecision)<<constants->getVerticalPixelDisplacementX();
    qDebug()<<"VPDY: "<<qSetRealNumberPrecision(realNumPrintPrecision)<<constants->getVerticalPixelDisplacementY();
}

void Stream::calculateCoordinates(mapObjectType type)
{

    QVector<MapObject*>* objectVector = this->getFrame()->getVectorPointer(type);

    for(int i = 0; i < objectVector->size(); i++)
    {
        if(direction == RIGHT)
        {
            Point* location = calculatePoint(objectVector->at(i), 0, 0);
            objectVector->at(i)->setBBoxTopLeft(location);

            Point* bBoxTopRight = calculatePoint(objectVector->at(i),objectVector->at(i)->getBBoxWidth(), 0);
            objectVector->at(i)->setBBoxTopRight(bBoxTopRight);

            Point* bBoxBottomLeft = calculatePoint(objectVector->at(i), 0, objectVector->at(i)->getBBoxHeight());
            objectVector->at(i)->setBBoxBottomLeft(bBoxBottomLeft);

            Point* bBoxBottomRight = calculatePoint(objectVector->at(i),objectVector->at(i)->getBBoxWidth(), objectVector->at(i)->getBBoxHeight());
            objectVector->at(i)->setBBoxBottomRight(bBoxBottomRight);
        }
        else {
            Point* location = calculatePoint(objectVector->at(i), objectVector->at(i)->getBBoxWidth(), objectVector->at(i)->getBBoxHeight());
            objectVector->at(i)->setBBoxTopLeft(location);

            Point* bBoxTopRight = calculatePoint(objectVector->at(i), 0, objectVector->at(i)->getBBoxHeight());
            objectVector->at(i)->setBBoxTopRight(bBoxTopRight);

            Point* bBoxBottomLeft = calculatePoint(objectVector->at(i), objectVector->at(i)->getBBoxWidth(), 0);
            objectVector->at(i)->setBBoxBottomLeft(bBoxBottomLeft);

            Point* bBoxBottomRight = calculatePoint(objectVector->at(i),0, 0);
            objectVector->at(i)->setBBoxBottomRight(bBoxBottomRight);
        }
    }
}

Point* Stream::calculatePoint(MapObject* mapObject, quint16 xOffset, quint16 yOffset)
{
    quint16 xImgPix;
    quint16 yImgPix;
    qint16 deltaX;
    qint16 deltaY;
    double longitude;
    double latitude;

    longitude = constants->getOriginLong();
    latitude = constants->getOriginLat();

    xImgPix = mapObject->getImgPixPos()->x() + xOffset;
    yImgPix = mapObject->getImgPixPos()->y() - yOffset /* * getDirection()*/;
    deltaX = xImgPix - constants->getOriginX();
    deltaY = yImgPix - constants->getOriginY();

    longitude += ((deltaX * constants->getHorisontalPixelDisplacementX()) + (deltaY * constants->getVerticalPixelDisplacementX()));
    latitude += ((deltaX * constants->getHorisontalPixelDisplacementY()) + (deltaY * constants->getVerticalPixelDisplacementY()));

    Point* p = new Point(longitude,latitude,SpatialReference::wgs84());
    return p;

}

Frame* Stream::getFrame()
{
    return frame;
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

streamState Stream::getState() const
{
    return state;
}

void Stream::setState(streamState value)
{
    state = value;
}

quint16 Stream::getNumberOfFrames() const
{
    return numberOfFrames;
}

void Stream::setNumberOfFrames(const quint16 &value)
{
    numberOfFrames = value;
}

StreamConstants *Stream::getConstants() const
{
    return constants;
}

viewDirection Stream::getDirection() const
{
    return direction;
}

void Stream::setDirection(const viewDirection &value)
{
    direction = value;
}

void Stream::fillFrameObjectList(QDataStream &collector, int mapObjectNum, mapObjectType type)
{
    quint16 xPixTmp;
    quint16 yPixTmp;
    quint16 bBoxWidthTmp;
    quint16 bBoxHeightTmp;

    if(!(this->getFrame()->getVectorPointer(type)->isEmpty()))
    {
        this->getFrame()->getVectorPointer(type)->clear();
    }

    for(int i = 0; i < mapObjectNum; i++)
    {
        collector>>xPixTmp;
        collector>>yPixTmp;
        collector>>bBoxWidthTmp;
        collector>>bBoxHeightTmp;

        /* Filtering detected objects which have bBox dimensions larger than threshold */
        if(!filterObject(bBoxWidthTmp,bBoxHeightTmp))
        {
            if(cameraSpecificDetections)
            {
                /* Creating MapObject with different fillColor, depending on camId */
                quint8 camId = constants->getCameraId();
                MapObject* mapObject = new MapObject(type, xPixTmp, imageHeight - yPixTmp, bBoxWidthTmp, bBoxHeightTmp, camId);
                this->frame->appendMapObject(mapObject, type);
            }
            else
            {
                MapObject* mapObject = new MapObject(type, xPixTmp, imageHeight - yPixTmp, bBoxWidthTmp, bBoxHeightTmp);
                this->frame->appendMapObject(mapObject, type);
            }

        }
    }
}

bool Stream::filterObject(quint16 bBoxWidth, quint16 bBoxHeight)
{
    if((bBoxWidth < bBoxWidthThresh) && (bBoxHeight < bBoxHeightThresh))
    {
        return false;
    }
    else
    {
        return true;
    }
}

void Stream::updateCurrentFrame()
{

    this->currentFrame++;
    if(currentFrame == numberOfFrames)
    {
        this->state = FINISHED;
        this->getFrame()->setTimestamp(-1);
        emit(streamFinished());
    }
}

