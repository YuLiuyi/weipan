#include "controller.h"
#include <QSettings>
#include <QCoreApplication>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QHttpPart>
#include <QFileInfo>
#include <QIODevice>

Controller::Controller()
{

    mAuUrlChangeNum = 0;
    mAuthoriseOk = false;
    mAccessToken = "";
    mUid = "";
    mManager = new QNetworkAccessManager();
    mFilePath = "";
}

//-------------------request处理------------------
void Controller::sendRequest()
{
    QString reqTokenUrl = buildReqUrl();
    inputUrl.setUrl(reqTokenUrl.toLatin1());
    qDebug() << Q_FUNC_INFO << "reqTokenUrl = " << reqTokenUrl;
}

//request url
QString Controller::buildReqUrl()
{
    //"https://auth.sina.com.cn/oauth2/authorize?client_id=&redirect_uri=https://api.weibo.com/oauth2/default.html&response_type=token"
    QString para;
    QString _reqTokenUrl = AUTHORISE_SRC_URL;
    _reqTokenUrl.append("?");
    _reqTokenUrl.append(CLIENT_ID);
    _reqTokenUrl.append(CLIENT_KEY);//添加签名
    _reqTokenUrl.append("&");
    _reqTokenUrl.append(REDIRECT_URI);
    _reqTokenUrl.append("&");
    _reqTokenUrl.append(RESPONSE_TYPE);
    _reqTokenUrl.append("token");
    _reqTokenUrl.append("&");
    _reqTokenUrl.append("display=");
    _reqTokenUrl.append("mobile");
    _reqTokenUrl.append(removeUrlEncode(para));//添加去掉编码的参数
    return _reqTokenUrl;
}


//show web
QUrl Controller::loadWeb()
{
    qDebug() << Q_FUNC_INFO<<" inputUrl = " << inputUrl;
    return inputUrl;
}

//提取accessToken,usrID
void Controller::getAccessToken(QString token, QString uid)
{
    mAccessToken = token;
    qDebug() << "token = " << mAccessToken;
    if (mAccessToken != "") {
        mAuthoriseOk = true;
    }
    mUid = uid;
    qDebug() << "uid = " << mUid;
//    writeCfg();
}

//
bool Controller::authoriseOk()
{
    return mAuthoriseOk;
}

//打开文件
int Controller::openFile(QByteArray &buf, const QString &filePath)
{
    QFile file(filePath);
    if(file.open(QIODevice::ReadOnly))
    {
        buf = file.readAll();
    }
    file.close();
    return buf.size();
}

////写入配置文件
//void Controller::writeCfg()
//{
//    cfgFile.open(QIODevice::WriteOnly);
//    cfgFile.close();
//    QSettings *cfgReader = new QSettings(cfgFile.fileName(), QSettings::IniFormat);
//    cfgReader->setIniCodec("UTF-8");
//    cfgReader->setValue("OAuthInfo/oauth_token",mAccessTokenInfo.accessToken);
//    cfgReader->setValue("OAuthInfo/user_id",mAccessTokenInfo.usrID);
//    cfgReader->setValue("OAuthInfo/usrName","NULL");
//    cfgReader->setValue("OAuthInfo/uploadNode","NULL");
//}


//userInfo request
void Controller::getUserInfo()
{
    QString reqUrl = builtGetUsrInfoReqUrl();
    inputUrl.setUrl(reqUrl.toLatin1());
    mRequest.setUrl(inputUrl);
    mUsrInfoReply =  mManager->get(mRequest);
    connect(mUsrInfoReply, SIGNAL(finished()), this, SLOT(getUserInfoFinished()));

}

//bulit userInfo request url
QString Controller::builtGetUsrInfoReqUrl()
{
    //https://api.weipan.cn/2/account/info?access_token=
    QString para;
    QString _reqUrl = GET_USR_INFO_URL;
    _reqUrl.append("?");
    _reqUrl.append(ACCESS_TOKEN);
    _reqUrl.append(mAccessToken);
    _reqUrl.append(removeUrlEncode(para));//添加去掉编码的参数
    qDebug() << Q_FUNC_INFO << "_reqUrl = " << _reqUrl;
    return _reqUrl;
}

//bulit userInfo request url finished
void Controller::getUserInfoFinished()
{
    int ret = mUsrInfoReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if(ret == 200) {
    QByteArray getBuf = mUsrInfoReply->readAll();

    qDebug() << Q_FUNC_INFO << "buf = " << getBuf;

    procUsrInfo(mUsrInfoStru, getBuf);

    mUsrInfoReply->deleteLater();
    } else {
        qDebug() << Q_FUNC_INFO<<"error";
        return;
    }
}

//process userInfo
void Controller::procUsrInfo(struUsrInfo &usrInfo, const QByteArray &buf)
{
    QJsonParseError jsonError;//Qt5新类
    QJsonDocument json = QJsonDocument::fromJson(buf, &jsonError);//Qt5新类
    if(jsonError.error == QJsonParseError::NoError)//Qt5新类
    {
        if(json.isObject())
        {
            QJsonObject obj = json.object();//Qt5新类
            usrInfo.usrID = obj.value("uid").toString();
            qDebug()<<Q_FUNC_INFO<<"usrID"<<usrInfo.usrID;
            usrInfo.usrName = obj.value("user_name").toString();
            usrInfo.gender = obj.value("gender").toString();

            if(obj["quota_info"].isObject())
            {
                QJsonObject quota = obj["quota_info"].toObject();

                qDebug() << "quota:" << quota.value("quota");
                usrInfo.quotaTotal = quota.value("quota").toString();
                qDebug() << "consumed:" << quota.value("consumed");
                usrInfo.quotaUsed = quota.value("consumed").toString();
                return;
            }
        }

    }
}

//show userInfo/qml
QString Controller::showUserInfo(int index)
{
    switch (index) {
    case 0:
        qDebug()<<Q_FUNC_INFO<<mUsrInfoStru.usrID;
        return mUsrInfoStru.usrID;
    case 1:
        return mUsrInfoStru.usrName;
    case 2:
        if(mUsrInfoStru.gender == "f")
            return "女";
        else if(mUsrInfoStru.gender == "m")
            return "男";
        else return "未知";
    case 3:
        return mUsrInfoStru.quotaTotal+" B";
    case 4:
        return mUsrInfoStru.quotaUsed+" B";
    default:
        return NULL;
    }
}

//----------------创建文件夹------------------
void Controller::createFolder(QString folderName)
{
    if(folderName == "") {
        return;
    } else {
        QString createFolder = buildCreateFolderUrl();

        inputUrl.setUrl(createFolder.toLatin1());
        qDebug() << "createFolderUrl = " << inputUrl;
        qDebug() << "folderName = " << folderName;

        QByteArray append("root=sandbox&path=/123456/"+folderName.toLocal8Bit());
        qDebug() << Q_FUNC_INFO << "append = " << append;

        mRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        mRequest.setHeader(QNetworkRequest::ContentLengthHeader, QVariant(append.size()).toString());
        mRequest.setUrl(inputUrl);

        connect(mManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(creatflderReplyFinished(QNetworkReply*)));


        mCreatFlderReply = mManager->post(mRequest, append);
        qDebug() << Q_FUNC_INFO << "mCreatFlderReply = " << mCreatFlderReply;
        connect(mCreatFlderReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
    }
}


//build CreateFolder Url
QString Controller::buildCreateFolderUrl()
{
    QString para;
    QString newGetCreateFolderUrl(CREATE_FOLDER);
    QString _createFolderUrl = newGetCreateFolderUrl;
    _createFolderUrl.append("?");
    _createFolderUrl.append(ACCESS_TOKEN);
    _createFolderUrl.append(mAccessToken);
    _createFolderUrl.append(removeUrlEncode(para));//添加去掉编码的参数
    return _createFolderUrl;
}

//build CreateFolder Url Finished
void Controller::creatflderReplyFinished(QNetworkReply* reply)
{
    int ret = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << "ret = " << ret;
    if(ret == 200) {
    QByteArray buf = reply->readAll();
    qDebug() << Q_FUNC_INFO << "buf = " << buf;
    procFolder(buf);
    mCreatFlderReply->deleteLater();
    } else {
        qDebug() << Q_FUNC_INFO<<"error";
        return;
    }
}

void Controller::procFolder(const QByteArray &buf)
{
    QJsonParseError jsonError;//Qt5新类
    QJsonDocument json = QJsonDocument::fromJson(buf, &jsonError);//Qt5新类

    bool type;
    QString title;
    if(jsonError.error == QJsonParseError::NoError)//Qt5新类
    {
        if(json.isObject())
        {
            QJsonObject obj = json.object();//Qt5新类
            if(obj.contains("path"))
            {
                qDebug() << "======================path:"<<obj.value("path");
                QFileInfo fi(obj.value("path").toString());
                title = fi.fileName();
                qDebug() << "======================title:"<<title;
            }
            if(obj.contains("is_dir"))
            {
                qDebug() << "======================is_dir:"<<obj.value("is_dir");
                type = obj.value("is_dir").toBool();
                qDebug() << "======================type:"<<type;
            }
            if(obj.contains("size"))
            {
                qDebug() << "======================size:"<<obj.value("size");
            }

            if(obj.contains("modified"))
            {
                qDebug() << "======================modified:"<<obj.value("modified");
            }
            mInfolist.append(FileInfo(obj.value("path").toString(), obj.value("modified").toString(),
                                      "0", title, type));
            emit(result(mInfolist));
        }
    }
}



//----------------获取文件夹信息-----------------------
//获取文件夹信息
void Controller::reqMetaData(QString dataPath)
{
    mInfolist.clear();
    qDebug()<<Q_FUNC_INFO<<"datapath ========== "<<dataPath;
    QString metaDataUrl = buildMetaDataUrl(dataPath);

    inputUrl.setUrl(QString(metaDataUrl.toUtf8()));
    qDebug() << "metaData = " << metaDataUrl.toUtf8();
    qDebug() << "inpurtUrl = " << inputUrl << " after encoded: " << inputUrl.toEncoded();
    mRequest.setUrl(QString(inputUrl.toEncoded()));
    mMetaDataReply = mManager->get(mRequest);
    connect(mMetaDataReply, SIGNAL(finished()),this,SLOT(metaDataReplyFinished()));
}

//构造获取文件夹信息url
QString Controller::buildMetaDataUrl(QString &dataPath)
//https://api.weipan.cn/2/metadata/sandbox/<path>
{
    QString para;
    QString newGetMetaDataUrl(GET_META_DATA_URL);

    if(dataPath.length()>0)
    {
        newGetMetaDataUrl.append(dataPath.toUtf8());
        qDebug() << Q_FUNC_INFO << newGetMetaDataUrl.toUtf8().data();
    }
    QString _metaDataUrl = newGetMetaDataUrl;
    _metaDataUrl.append("?");
    _metaDataUrl.append(ACCESS_TOKEN);
    _metaDataUrl.append(mAccessToken);
    _metaDataUrl.append(removeUrlEncode(para));//添加去掉编码的参数
    return _metaDataUrl;
}

//metaData读完
void Controller::metaDataReplyFinished()
{
    int ret = mMetaDataReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(ret == 200) {
        QByteArray getBuf = mMetaDataReply->readAll();
        qDebug() << Q_FUNC_INFO << "buf = " << getBuf;
        procMetaData(getBuf);
        mMetaDataReply->deleteLater();
    } else {
        qDebug() << Q_FUNC_INFO<<"error";
    }
}

//提取文件夹信息
void Controller::procMetaData(const QByteArray &buf)
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

                QJsonArray jsonArray = obj["contents"].toArray();

                qDebug() << "======================jsonArray:"<<jsonArray.size();

                if(jsonArray.size() == 0) {
                    qDebug() << "==============size==00000000000000======";
                    emit(emptyFile());
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
//                        if(type) {
//                            QString size =  QString::number(jsonArray.size(), 10);
//                        mInfolist.append(FileInfo(obj["path"].toString(), obj["modified"].toString(),
//                                size, title, type));
//                        } else {
                            mInfolist.append(FileInfo(obj["path"].toString(), obj["modified"].toString(),
                                    obj["size"].toString(), title, type));
//                        }
                        emit(result(mInfolist));
                    }
                }
            }
        }
    }
}

void Controller::reqUploadFile()
{
    QByteArray data;
    QString toPath = "/";
    QString uploadFileUrl = buildUploadFileUrl(toPath);

    inputUrl.setUrl(uploadFileUrl.toUtf8());
    qDebug()<<Q_FUNC_INFO<<"inputUrl = " << inputUrl;
    mUploadRequest.setUrl(inputUrl);
    buildMultiPart(data);//使用multipart方式上传 构造multipart包
    mUploadFileReply = mManager->put(mUploadRequest,data);//上传
    connect(mUploadFileReply, SIGNAL(finished()), this, SLOT(upLoadFileReplyFinished()));

}

void Controller::error(QNetworkReply::NetworkError error)
{
    qDebug() << Q_FUNC_INFO << "error=" << error;
}
void Controller::uploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    qDebug() << Q_FUNC_INFO << "bytesSent=" << bytesSent << "bytesTotal=" << bytesTotal;
}

//构建上传文件的url
QString Controller::buildUploadFileUrl(QString &toPath)
{
    QString para;
    QString _sendFileUrl = UPLOAD_URL;
    _sendFileUrl.append("/123456/"+mULFileName);  //file save path
    _sendFileUrl.append(toPath);
    _sendFileUrl.append("?");
    _sendFileUrl.append(ACCESS_TOKEN);
    _sendFileUrl.append(mAccessToken);
    _sendFileUrl.append(para);//添加无编码的参数
    return _sendFileUrl;
}

//返回随机数 oauth_nonce
QString Controller::getRandNonce()
{
    QTime time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);
    return QString().number(qrand());
}

void Controller::getUploadFilePath(QString path)
{
    mFilePath = path;
    QFileInfo fi(mFilePath);
    mULFileName = fi.fileName();
}

//构造multipart
void Controller::buildMultiPart(QByteArray &data)
{
    QByteArray needToUploadFile;
    QString localPath = mFilePath;
    qDebug() << Q_FUNC_INFO << "localPath = " << localPath;
    if(openFile(needToUploadFile,localPath)<=0)
    {
        qDebug()<<Q_FUNC_INFO<<"无法打开文件";
        return ;
    }
    qDebug() << Q_FUNC_INFO << "needToUploadFile = " << needToUploadFile << "==";

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
    qDebug() << Q_FUNC_INFO << "data : " << data << "data size :" << data.size() << ",length=" << data.length();
    mUploadRequest.setHeader(QNetworkRequest::ContentTypeHeader, contentType.toLatin1());
    mUploadRequest.setHeader(QNetworkRequest::ContentLengthHeader, QVariant(data.size()).toString());


}


//上传文件 反馈完毕
void Controller::upLoadFileReplyFinished()
{
    int ret = mUploadFileReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << Q_FUNC_INFO << "ret = " << ret;

    QByteArray buf = mUploadFileReply->readAll();
    qDebug() << Q_FUNC_INFO << "getbuf = " << buf;

    QJsonParseError jsonError;//Qt5新类
    QJsonDocument json = QJsonDocument::fromJson(buf, &jsonError);//Qt5新类
    if(jsonError.error == QJsonParseError::NoError)//Qt5新类
    {
        if(json.isObject())
        {
            QJsonObject obj = json.object();//Qt5新类
            if(obj.contains("is_dir"))
            {
                qDebug() << "======================is_dir:"<<obj.value("is_dir");

                if(obj["is_dir"].isObject())
                {
                    QJsonObject weatherObj = obj["is_dir"].toObject();

                    qDebug() << "is_dir:"<<weatherObj.toVariantMap();
                    return;
                }
            }
        }
    }
    qDebug()<<"111111111111111111111111111111";
    emit(uploadFinished());
    mUploadFileReply->deleteLater();
}

//----------------------------------------下载文件
//请求下载文件
//GET https://api.weipan.cn/2/files/<root>/<path>
void Controller::reqDownLoadFile()
{

    QFileInfo fi(mDownloadpath);

    qDebug() << Q_FUNC_INFO << "downfile==path=" << mDownloadpath;

    QString file_name = "/home/user/" + fi.fileName();
    qDebug() << "fileName === " << file_name;

    QFile file(file_name);

    if(file.exists()) {
        file_name = "/home/user/f_" + fi.fileName();
        qDebug() << "fileName === " << file_name;
    }

    dwnFile = new QFile(file_name);
    qDebug() << "fileName === " << file_name;

    if(!dwnFile->open(QIODevice::WriteOnly))
    {   //如果打开文件失败，则删除file，并使file指针为0，然后返回

        qDebug()<<Q_FUNC_INFO<<"无法打开文件";
        delete dwnFile;
        dwnFile = NULL;
        return;
    }

    QString downLoadFileUrl = buildDownFileUrl(mDownloadpath);
    inputUrl.setUrl(downLoadFileUrl.toUtf8());

    qDebug() << Q_FUNC_INFO << "inputUrl===" << inputUrl;
    mRequest.setUrl(inputUrl);
    mDownloadFileReply = mManager->get(mRequest);
    connect(mDownloadFileReply, SIGNAL(readyRead()), this, SLOT(dwnFileRealReadReady()));
    connect(mDownloadFileReply, SIGNAL(downloadProgress(qint64,qint64)),
            this, SLOT(downloadProgress(qint64,qint64)));
    connect(mDownloadFileReply, SIGNAL(finished()), this, SLOT(downLoadFileReplyFinished()));
}

void Controller::getDwnloadPath(QString path)
{
    mDownloadpath = path;
    qDebug()<< Q_FUNC_INFO << "====path==="<<mDownloadpath;
}

//构造 下载文件的url
QString Controller::buildDownFileUrl(QString &fromPath)
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


//反馈完毕 获取重定向地址
void Controller::downLoadFileReplyFinished()
{
    QVariant returnCode = mDownloadFileReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    qDebug() << "returnCode = " << returnCode;
    if(301 == returnCode.toInt() ||  302 == returnCode.toInt())
    {
        for(int i = 0; i < mDownloadFileReply->rawHeaderPairs().count(); i++)
        {
            if(QString("location").compare(mDownloadFileReply->rawHeaderPairs().at(i).first, Qt::CaseInsensitive) == 0)
            {
                QUrl realDwnFileUrl = QUrl(mDownloadFileReply->rawHeaderPairs().at(i).second);
                qDebug() << "=====realDwnFileUrl " << realDwnFileUrl.toString();
                mRequest.setUrl(realDwnFileUrl);
                mDownloadFileReply->deleteLater();
                mDownloadFileReply = mManager->get(mRequest);//用重定向地址重新下载
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

void Controller::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    qDebug() << Q_FUNC_INFO << "bytesSent=" << bytesReceived << "bytesTotal=" << bytesTotal;
}

//重定向后 获得下载文件反馈
void Controller::dwnFileRealReadReady()
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

