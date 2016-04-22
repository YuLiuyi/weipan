#ifndef THREAD_H
#define THREAD_H
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "filesInfo.h"

class Thread:public QThread
{
    Q_OBJECT
public:
    Thread(QObject *parent = 0);
    bool stop;
    void run();                                 //线程运行
    void setUrl(QUrl url);
    void reqMetaData(QUrl url);
    void proMetaData(const QByteArray &buf);


signals:
    void result(InfoList);
    void emptyFile();
public slots:
    void metaDataReplyFinished();


private:

    QUrl mUrl;
    InfoList mInfolist;
    QNetworkAccessManager *mManager;
    QNetworkReply *mMetaDataReply;
};
#endif // THREAD_H
