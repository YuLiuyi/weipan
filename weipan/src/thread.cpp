#include "thread.h"
#include <QDir>
#include <QDebug>
#include <QSettings>
#include <QCoreApplication>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QHttpPart>
#include <QFileInfo>
#include <QNetworkRequest>
#include "publicFun.h"


Thread::Thread(QObject *parent):
    QThread(parent)
{
    stop = false;
}

void Thread::setUrl(QUrl url)
{
    mUrl = url;
}

//----------------获取文件夹信息-----------------------
//获取文件夹信息
void Thread::reqMetaData(QUrl url)
{
    QNetworkRequest request;
    request.setUrl(QString(url.toEncoded()));
    mMetaDataReply = mManager->get(request);

    connect(mMetaDataReply, SIGNAL(finished()),this,SLOT(metaDataReplyFinished()));
//    connect(mMetaDataReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
}

//metaData读完
void Thread::metaDataReplyFinished()
{
    int ret = mMetaDataReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << Q_FUNC_INFO<<"ret===" << ret;
    if(ret == 200) {
        QByteArray getBuf = mMetaDataReply->readAll();
        qDebug() << Q_FUNC_INFO << "buf = " << getBuf;
//        m_thread->setInfo(getBuf);
//        m_thread->start();
        proMetaData(getBuf);
        mMetaDataReply->deleteLater();
    } else {
        qDebug() << Q_FUNC_INFO<<"error";
    }
}

void Thread::proMetaData(const QByteArray &buf)
{
    QJsonParseError jsonError;//Qt5新类
    QJsonDocument json = QJsonDocument::fromJson(buf, &jsonError);//Qt5新类
    QString title;
    bool type;
    if(jsonError.error == QJsonParseError::NoError)//Qt5新类
    {
        if(json.isObject())
        {
            QJsonObject obj = json.object();//Qt5新类
            if(obj.contains("contents")) {

                qDebug() << "======================contents:"<<obj.value("contents");

                QJsonArray jsonArray = obj["contents"].toArray();//

                qDebug() << "======================jsonArray:"<<jsonArray.size();

                if(jsonArray.size() == 0) {
                    qDebug() << "==============size==00000000000000======";
                    mInfolist.clear();
                    emit result(mInfolist);
                    emit emptyFile();
                } else {
                    foreach (const QJsonValue & value, jsonArray) {
                        QJsonObject obj = value.toObject();
                        QFileInfo fi(obj["path"].toString());
                        title = fi.fileName();
                        qDebug() << "======================path:"<<obj["path"].toString();
                        qDebug() << "======================title:"<<title;
                        qDebug() << "==============is_dir=======" << obj["is_dir"].toBool();
                        type = obj["is_dir"].toBool();
                        qDebug() << "======================type:"<<type;
                        mInfolist.append(FileInfo(obj["path"].toString(), obj["modified"].toString(),
                                    obj["size"].toString(), title, type));
                    }
                }
            }
        }
    }
}

void Thread::run()
{
    mInfolist.clear();
    emit result(mInfolist);
    reqMetaData(mUrl);
    emit result(mInfolist);
}

