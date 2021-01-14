#ifndef STREAM_H
#define STREAM_H
#include <QFile>
#include "frame.h"
#include "streamconstants.h"
class Stream
{
public:
    Stream();
    Stream(QString);
    bool openFile(QString);
    void readFileData();

    Frame getFrame() const;
    void setFrame(const Frame &value);
    quint64 readInitialTimestamp();
    bool getIsActive() const;
    void setIsActive(bool value);

private:
    QFile* file;
    Frame frame;
    StreamConstants constants;
    quint16 currentFrame;
    bool isActive;

    void fillFrameObjectList(QDataStream& collector, int mapObjectNum, type mapObjectType);
    void updateCurrentFrame();
};

#endif // STREAM_H
