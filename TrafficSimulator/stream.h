#ifndef STREAM_H
#define STREAM_H
#include <QFile>
#include "frame.h"
#include "streamconstants.h"
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
    void calculateCoordinates(type mapObjectType);

    Frame* getFrame();
    bool getIsActive() const;
    void setIsActive(bool value);
    quint64 readNextTimestamp();
    quint16 getNumberOfFrames() const;
    void setNumberOfFrames(const quint16 &value);

    StreamConstants *getConstants() const;
signals:
    void streamFinished();
private:
    QFile* file;
    Frame* frame;
    StreamConstants* constants;
    quint16 currentFrame;
    quint16 numberOfFrames;
    bool isActive;

    void fillFrameObjectList(QDataStream& collector, int mapObjectNum, type mapObjectType);
    void updateCurrentFrame();
};

#endif // STREAM_H
