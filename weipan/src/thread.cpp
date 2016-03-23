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

Thread::Thread(QObject *parent):
    QThread(parent)
{
    stop = false;
}
void Thread::setInfo(QByteArray buf)
{
    mBuf = buf;
}
void Thread::procFiles()
{
    QJsonParseError jsonError;//Qt5新类
    QJsonDocument json = QJsonDocument::fromJson(mBuf, &jsonError);//Qt5新类
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
                    mList.clear();
                    emit procResult(mList);
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
                            mList.append(FileInfo(obj["path"].toString(), obj["modified"].toString(),
                                    obj["size"].toString(), title, type));
                    }
                }
            }
        }
    }
}


void Thread::run()
{
    qDebug()<<"=======run======";
    procFiles();
    emit procResult(mList);      //把m_list从线程发出去，在主线程进行处理
}

