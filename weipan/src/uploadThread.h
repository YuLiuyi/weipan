#ifndef UPLOADTHREAD_H
#define UPLOADTHREAD_H

#include <QThread>
#include <QNetworkReply>
#include <QNetworkRequest>
#include "loadInfo.h"

class UploadThread: public QThread
{
    Q_OBJECT

public:

    UploadThread(QObject *parent = 0);

    bool stop;

    int openFile(QByteArray &buf, const QString &filePath);

    void getAccessToken(QString token);

    void getDePath(QString toPath);

    void reqUploadFile(QString toPath);

    QString buildUploadFileUrl(QString &toPath);

    QString getRandNonce();

    void getUploadFilePath(QString path, QString fileName);

    void buildMultiPart(QByteArray &data);

    void run();                                 //线程运行


signals:
    void uploadReplyFinished(QNetworkReply*);

public slots:

    void error(QNetworkReply::NetworkError error);

private:

    QString mAccessToken;

    QNetworkReply *mUploadFileReply;

    QNetworkRequest mUploadRequest;

    QString mFilePath;

    QString mULFileName;

    QString mDePath;

    UploadInfoList mUploadList;

    bool mEnd;
};

#endif // UPLOADTHREAD_H
