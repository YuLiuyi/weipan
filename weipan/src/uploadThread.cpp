#include "uploadThread.h"

#include <QNetworkAccessManager>
#include <QJsonParseError>
#include <QJsonObject>
#include <QFileInfo>
#include "publicFun.h"

UploadThread::UploadThread(QObject *parent):
    QThread(parent)
{
    stop = false;
    mEnd = false;
}
void UploadThread::run()
{
    qDebug()<<__LINE__<<Q_FUNC_INFO;
    reqUploadFile(mDePath);
    QByteArray buf = mUploadFileReply->readAll();
    qDebug() << Q_FUNC_INFO << "getbuf = " << buf;
    emit uploadReplyFinished(mUploadFileReply);
}

//打开文件
int UploadThread::openFile(QByteArray &buf, const QString &filePath)
{
    QFile file(filePath);
    if(file.open(QIODevice::ReadOnly))
    {
        buf = file.readAll();
    }
    file.close();
    return buf.size();
}

void UploadThread::getAccessToken(QString token)
{
    qDebug()<<Q_FUNC_INFO<<"token = "<<token;
    mAccessToken = token;
}

void UploadThread::getDePath(QString toPath)
{
    qDebug()<<Q_FUNC_INFO<<"toPath = "<<toPath;
    mDePath = toPath;
}

void UploadThread::reqUploadFile(QString toPath)
{
    QByteArray data;
    QUrl inputUrl;
    QNetworkAccessManager *manager;
    manager = new QNetworkAccessManager();

    QString uploadFileUrl = buildUploadFileUrl(toPath);

    inputUrl.setUrl(uploadFileUrl.toUtf8());

    qDebug()<<Q_FUNC_INFO<<"inputUrl = " << inputUrl;

    mUploadRequest.setUrl(inputUrl);

    buildMultiPart(data);//使用multipart方式上传 构造multipart包

    qDebug() << Q_FUNC_INFO << __LINE__ <<  "my data: " << data.size() <<  QString(data).toUtf8();
    mUploadFileReply = manager->put(mUploadRequest, data);//上传
    int ret = mUploadFileReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    connect(mUploadFileReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
    qDebug() << Q_FUNC_INFO << "ret = " << ret;
    qDebug() << Q_FUNC_INFO << "getbuf = " << mUploadFileReply->readAll();

}

//构建上传文件的url
QString UploadThread::buildUploadFileUrl(QString &toPath)
{
    QString para;
    QString _sendFileUrl = UPLOAD_URL;
    _sendFileUrl.append( toPath + "/" + mULFileName);  //file save path
    _sendFileUrl.append("?");
    _sendFileUrl.append(ACCESS_TOKEN);
    _sendFileUrl.append(mAccessToken);
    _sendFileUrl.append("&overwrite=false");
    _sendFileUrl.append(para);//添加无编码的参数
    return _sendFileUrl;
}

void UploadThread::error(QNetworkReply::NetworkError error)
{
    qDebug() << Q_FUNC_INFO << "error=" << error;
}

//返回随机数 oauth_nonce
QString UploadThread::getRandNonce()
{
    QTime time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);
    return QString().number(qrand());
}

void UploadThread::getUploadFilePath(QString path, QString fileName)
{
    mFilePath = path;
    qDebug() << Q_FUNC_INFO <<"mFilePath===" << mFilePath;

    mULFileName = fileName;
    qDebug() << Q_FUNC_INFO <<"mULFileName===" << mULFileName;
}

//构造multipart
void UploadThread::buildMultiPart(QByteArray &data)
{
    QByteArray needToUploadFile;
    QString localPath = mFilePath;
    qDebug() << Q_FUNC_INFO  << __LINE__ <<"mFilePath===" << mFilePath;
    qDebug() << Q_FUNC_INFO  << __LINE__ << "localPath = " << localPath;
    if(openFile(needToUploadFile,localPath)<=0)
    {
        qDebug()<<Q_FUNC_INFO<<"无法打开文件";
        return ;
    }
    qDebug() << Q_FUNC_INFO << __LINE__ << "needToUploadFile = " << needToUploadFile.size() << needToUploadFile  << "==";
    qDebug() << Q_FUNC_INFO << __LINE__  << "I am a new line.";

    QString crlf = "\r\n";
    QString boundary = "ugytydcjshfsd" + getRandNonce();
    QString contentType = "multipart/form-data; boundary=" + boundary; //开始的头部信息

    QString endBoundary = crlf + "--" + boundary + "--" + crlf; //结束处

    boundary = "--" + boundary + crlf; //每行数据开头
    QByteArray bond = boundary.toLatin1();

    data.append(bond);
    data.append(QString("Content-Disposition: form-data; name=\"file\"; filename=\"" +localPath+"\""+crlf).toLatin1());
    data.append(QString("Content-Type: application/octet-stream" + crlf).toLatin1());
    data.append(crlf.toLatin1());
    data.append(needToUploadFile);
    data.append(endBoundary.toLatin1());

    qDebug() << Q_FUNC_INFO << __LINE__  << "I am a new line,too.";

    qDebug() << Q_FUNC_INFO  << __LINE__ << "data : " << "data size :" << data.size() << ",length=" << data.length() << data ;
    mUploadRequest.setHeader(QNetworkRequest::ContentTypeHeader, contentType.toLatin1());
    mUploadRequest.setHeader(QNetworkRequest::ContentLengthHeader, QVariant(data.size()).toString());
}
