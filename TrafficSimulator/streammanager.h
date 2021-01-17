#ifndef STREAMMANAGER_H
#define STREAMMANAGER_H

#include <QObject>
#include <QList>
#include "stream.h"
#include "frame.h"
#include "TrafficSimulator.h"
class StreamManager : public QObject
{
    Q_OBJECT
public:
    StreamManager();
    StreamManager(TrafficSimulator&);
    void addStream(Stream& stream);
    void run();
    void init();

    Frame* finalFrame;
public slots:
    void readStreams();
signals:
    void dataReady(Frame*);
private:
    QList <Stream*> streams;
    quint64 activeTimestamp;
    void updateFinalFrame(int);
    void updateActiveStreams();
    void updateActiveTimestamp();

};

#endif // STREAMMANAGER_H
