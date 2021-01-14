#ifndef STREAMMANAGER_H
#define STREAMMANAGER_H

#include <QObject>
#include <QList>
#include "stream.h"
#include "frame.h"
class StreamManager
{
    Q_OBJECT
public:
    StreamManager();
    void addStream(Stream& stream);
    void run();
    void init();
public slots:
    void readStreams();
private:
    QList <Stream*> streams;
    Frame finalFrame;
    quint64 activeTimestamp;

};

#endif // STREAMMANAGER_H
