#ifndef DOWNLOADTHREAD_H
#define DOWNLOADTHREAD_H
#include <QThread>
#include <QNetworkReply>
#include <QFile>
#include <QNetworkRequest>
#include "loadInfo.h"

class DownloadThread: public QThread
{
    Q_OBJECT

public:

    DownloadThread(QObject *parent = 0);

    bool stop;

    void getToken(QString token);

    void getDownloadPath(QString path);

    void reqDownload();

    QString buildDownFileUrl(QString &fromPath);

    void run();                                 //线程运行


signals:
//    void result(DownloadInfoList);

    void downloadFinished();

public slots:

    void error(QNetworkReply::NetworkError error);

    void downloadProgress(qint64 bytesSent, qint64 bytesTotal);

    void downLoadFileReplyFinished();

    void dwnFileRealReadReady();

private:

    QString mAccessToken;

    QString mDownloadPath;

    QFile   *dwnFile;

    QUrl inputUrl;

    QNetworkReply *mDownloadFileReply;

    QNetworkRequest mDownloadRequest;

    QNetworkAccessManager *mManager;

    QString mFilePath;

    DownloadInfoList mDownloadList;

    bool mEnd;
};
#endif // DOWNLOADTHREAD_H
