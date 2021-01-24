#ifndef STREAMMANAGER_H
#define STREAMMANAGER_H

#include <QObject>
#include <QList>
#include "stream.h"
#include "frame.h"
#include "TrafficSimulator.h"
#include "QPair"

class StreamManager : public QObject
{
    Q_OBJECT
public:
    StreamManager();
    StreamManager(TrafficSimulator&);
    void addStream(Stream* stream);
    void streamsFinished();

    void init();

    Frame* finalFrame;
public slots:
    void readStreams();
    void run();
signals:
    void dataReady(Frame*);
    void finished();
    Geometry* getGeometryFromPoints(QList<Point*>*);
private:
    QList<Stream*>* streams;
    QList<QPair<MapObject*, MapObject*>*>* pairs;
    quint64 activeTimestamp;
    void updateFinalFrame(int);
    void fillFinalFrameData(mapObjectType, Stream*);
    void updateActiveStreams();
    void updateActiveTimestamp();
    void mergeDoubles(mapObjectType type);
    void checkOverlapBBox(Stream* stream1, Stream* stream2, mapObjectType type);
    void appendPair(QPair<MapObject*, MapObject*>*);
    void merge();
};

#endif // STREAMMANAGER_H
