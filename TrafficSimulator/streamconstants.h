#ifndef STREAMCONSTANTS_H
#define STREAMCONSTANTS_H


#include<QtGlobal>
#include "referencepointlist.h"
class StreamConstants
{
public:
    StreamConstants();
    void setOriginPoleId(const quint32 &value);

    void setUpperPoleId(const quint32 &value);

    void setRightPoleId(const quint32 &value);

    void setOriginX(const quint16 &value);

    void setOriginY(const quint16 &value);

    void setUpperPoleX(const quint16 &value);

    void setUpperPoleY(const quint16 &value);

    void setRightPoleX(const quint16 &value);

    void setRightPoleY(const quint16 &value);

    void calculateConstants();

    ReferencePointList getReferenceList() const;

    quint32 getOriginPoleId() const;

    double getOriginLong() const;

    double getOriginLat() const;

    double getUpperPoleLong() const;

    double getUpperPoleLat() const;

    double getRightPoleLong() const;

    double getRightPoleLat() const;

    double getHorisontalPixelDisplacementX() const;

    double getHorisontalPixelDisplacementY() const;

    double getVerticalPixelDisplacementX() const;

    double getVerticalPixelDisplacementY() const;

    quint16 getOriginX() const;

    quint16 getOriginY() const;

    quint8 getCameraId() const;
    void setCameraId(const quint8 &value);

private:
    quint8 cameraId;

    quint32 originPoleId;
    quint16 originX;
    quint16 originY;
    double originLong;
    double originLat;

    quint32 upperPoleId;
    quint16 upperPoleX;
    quint16 upperPoleY;
    double upperPoleLong;
    double upperPoleLat;

    quint32 rightPoleId;
    quint16 rightPoleX;
    quint16 rightPoleY;
    double rightPoleLong;
    double rightPoleLat;

    double horisontalPixelDisplacementX;
    double horisontalPixelDisplacementY;
    double verticalPixelDisplacementX;
    double verticalPixelDisplacementY;

    ReferencePointList referenceList;
};

#endif // STREAMCONSTANTS_H
