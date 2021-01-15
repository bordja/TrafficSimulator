#include <QDebug>
#include "streamconstants.h"

StreamConstants::StreamConstants()
{

}

void StreamConstants::setOriginPoleId(const quint32 &value)
{
    originPoleId = value;
}

void StreamConstants::setUpperPoleId(const quint32 &value)
{
    upperPoleId = value;
}

void StreamConstants::setRightPoleId(const quint32 &value)
{
    rightPoleId = value;
}

void StreamConstants::setOriginX(const quint16 &value)
{
    originX = value;
}

void StreamConstants::setOriginY(const quint16 &value)
{
    originY = value;
}

void StreamConstants::setUpperPoleX(const quint16 &value)
{
    upperPoleX = value;
}

void StreamConstants::setUpperPoleY(const quint16 &value)
{
    upperPoleY = value;
}

void StreamConstants::setRightPoleX(const quint16 &value)
{
    rightPoleX = value;
}

void StreamConstants::setRightPoleY(const quint16 &value)
{
    rightPoleY = value;
}

void StreamConstants::calculateConstants()
{
    quint16 deltaX;
    quint16 deltaY;

    this->originLong = this->getReferenceList().getPoleLocationFromId(originPoleId)->x();
    this->originLat = this->getReferenceList().getPoleLocationFromId(originPoleId)->y();

    this->upperPoleLong = this->getReferenceList().getPoleLocationFromId(upperPoleId)->x();
    this->upperPoleLat = this->getReferenceList().getPoleLocationFromId(upperPoleId)->y();

    this->rightPoleLong = this->getReferenceList().getPoleLocationFromId(rightPoleId)->x();
    this->rightPoleLat = this->getReferenceList().getPoleLocationFromId(rightPoleId)->y();

    deltaX = rightPoleX - originX;
    deltaY = upperPoleY - originY;

    horisontalPixelDisplacementX = (rightPoleLong - originLong) / deltaX;
    horisontalPixelDisplacementY = (rightPoleLat - originLat) / deltaX;
    verticalPixelDisplacementX = (upperPoleLong - originLong) / deltaY;
    verticalPixelDisplacementY = (upperPoleLat - originLat) / deltaY;
}

ReferencePointList StreamConstants::getReferenceList() const
{
    return referenceList;
}

quint32 StreamConstants::getOriginPoleId() const
{
    return originPoleId;
}

double StreamConstants::getOriginLong() const
{
    return originLong;
}

double StreamConstants::getOriginLat() const
{
    return originLat;
}

double StreamConstants::getUpperPoleLong() const
{
    return upperPoleLong;
}

double StreamConstants::getUpperPoleLat() const
{
    return upperPoleLat;
}

double StreamConstants::getRightPoleLong() const
{
    return rightPoleLong;
}

double StreamConstants::getRightPoleLat() const
{
    return rightPoleLat;
}

double StreamConstants::getHorisontalPixelDisplacementX() const
{
    return horisontalPixelDisplacementX;
}

double StreamConstants::getHorisontalPixelDisplacementY() const
{
    return horisontalPixelDisplacementY;
}

double StreamConstants::getVerticalPixelDisplacementX() const
{
    return verticalPixelDisplacementX;
}

double StreamConstants::getVerticalPixelDisplacementY() const
{
    return verticalPixelDisplacementY;
}
