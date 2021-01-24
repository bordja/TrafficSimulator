#ifndef STREAM_H
#define STREAM_H
#include <QFile>
#include "frame.h"
#include "streamconstants.h"
enum streamState {NOT_ACTIVE = 0, ACTIVE, FINISHED};
enum viewDirection {RIGHT = 0, LEFT};
class Stream : public QObject
{
    Q_OBJECT
public:
    Stream();
    Stream(QString);
    bool openFile(QString);
    void readFileData();
    void readHeader();
    void printStreamConstants();
    void calculateCoordinates(mapObjectType type);

    Frame* getFrame();
    streamState getState() const;
    void setState(streamState value);
    quint64 readNextTimestamp();
    quint16 getNumberOfFrames() const;
    void setNumberOfFrames(const quint16 &value);

    StreamConstants *getConstants() const;

    viewDirection getDirection() const;
    void setDirection(const viewDirection &value);

signals:
    void streamFinished();
private:
    QFile* file;
    Frame* frame;
    StreamConstants* constants;
    quint16 currentFrame;
    quint16 numberOfFrames;
    streamState state;
    viewDirection direction;

    void fillFrameObjectList(QDataStream& collector, int mapObjectNum, mapObjectType type);
    void updateCurrentFrame();
    Point* calculatePoint(MapObject* mapObject, quint16 xOffset, quint16 yOffset);
    bool filterObject(quint16 bBoxWidth, quint16 bBoxHeight);
};

#endif // STREAM_H
