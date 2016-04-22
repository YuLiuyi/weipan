#include "downloadThread.h"
#include "publicFun.h"
#include <QFileInfo>

DownloadThread::DownloadThread(QObject *parent):
    QThread(parent)
{
    stop = false;
    mDownloadPath = "";
    mAccessToken = "";
    mManager = new QNetworkAccessManager;
}

void DownloadThread::run()
{
    reqDownload();
}

void DownloadThread::getToken(QString token)
{
    mAccessToken = token;
}

void DownloadThread::getDownloadPath(QString path)
{
    mDownloadPath = path;
}

void DownloadThread::reqDownload()
{

    QFileInfo fi(mDownloadPath);

    qDebug() << Q_FUNC_INFO << "downfile==path=" << mDownloadPath;

    QString file_name = "/home/user/" + fi.fileName();
    qDebug() <<__LINE__<< "fileName === " << file_name;

    QFile file(file_name);

    if(file.exists()) {
        file_name = "/home/user/f_" + fi.fileName();
        qDebug() <<__LINE__<< "fileName === " << file_name;
    }

    dwnFile = new QFile(file_name);
    qDebug() << __LINE__<< "fileName === " << file_name;

    if(!dwnFile->open(QIODevice::WriteOnly))
    {   //如果打开文件失败，则删除file，并使file指针为0，然后返回

        qDebug() << Q_FUNC_INFO << "无法打开文件";
        delete dwnFile;
        dwnFile = NULL;
        return;
    }

    QString downLoadFileUrl = buildDownFileUrl(mDownloadPath);
    inputUrl.setUrl(downLoadFileUrl.toUtf8());

    qDebug() << Q_FUNC_INFO << "inputUrl===" << inputUrl;
    mDownloadRequest.setUrl(inputUrl);
    mDownloadFileReply = mManager->get(mDownloadRequest);
    connect(mDownloadFileReply, SIGNAL(finished()), this, SLOT(downLoadFileReplyFinished()));
}

//构造 下载文件的url
QString DownloadThread::buildDownFileUrl(QString &fromPath)
{
    QString para;
    QString _dwnFileUrl = DOWNLOAD_FILE_URL;
    _dwnFileUrl.append(fromPath);
    _dwnFileUrl.append("?");
    _dwnFileUrl.append(ACCESS_TOKEN);
    _dwnFileUrl.append(mAccessToken);
    _dwnFileUrl.append(para);//添加无编码的参数
    return _dwnFileUrl;
}

void DownloadThread::downLoadFileReplyFinished()
{
    QVariant returnCode = mDownloadFileReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    qDebug() << Q_FUNC_INFO << __LINE__<<"returnCode = " << returnCode;

    if(301 == returnCode.toInt() ||  302 == returnCode.toInt())
    {
        for(int i = 0; i < mDownloadFileReply->rawHeaderPairs().count(); i++)
        {
            if(QString("location").compare(mDownloadFileReply->rawHeaderPairs().at(i).first, Qt::CaseInsensitive) == 0)
            {
                QUrl realDwnFileUrl = QUrl(mDownloadFileReply->rawHeaderPairs().at(i).second);
                qDebug() << "=====realDwnFileUrl = " << realDwnFileUrl.toString();
                mDownloadRequest.setUrl(realDwnFileUrl);
                mDownloadFileReply->deleteLater();
                mDownloadFileReply = mManager->get(mDownloadRequest);//用重定向地址重新下载
                connect(mDownloadFileReply, SIGNAL(readyRead()), this, SLOT(dwnFileRealReadReady()));

                connect(mDownloadFileReply, SIGNAL(downloadProgress(qint64,qint64)),
                        this, SLOT(downloadProgress(qint64,qint64)));
                connect(mDownloadFileReply, SIGNAL(finished()), this, SLOT(downLoadFileReplyFinished()));
                return;
            }
        }

    }else if(200 == returnCode.toInt()){
        qDebug() << Q_FUNC_INFO << "下载完成！";
        emit downloadFinished();
        dwnFile->flush();
        dwnFile->close();
        delete dwnFile;
        dwnFile = NULL;
    }
    mDownloadFileReply->deleteLater();
}

//重定向后 获得下载文件反馈
void DownloadThread::dwnFileRealReadReady()
{
    int ret = mDownloadFileReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    qDebug() << Q_FUNC_INFO << "ret = " << ret;

    if (dwnFile!=NULL) {
        qDebug() << "write====";

        QByteArray buf = mDownloadFileReply->readAll();
        qDebug() << Q_FUNC_INFO << "buf = " << buf;

        dwnFile->write(mDownloadFileReply->readAll());
    }
}

void DownloadThread::error(QNetworkReply::NetworkError error)
{
    qDebug() << Q_FUNC_INFO << "error = " << error;
}

void DownloadThread::downloadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    qDebug() << Q_FUNC_INFO << "bytesSent = " << bytesSent << "; bytesTotal = " << bytesTotal;
}
