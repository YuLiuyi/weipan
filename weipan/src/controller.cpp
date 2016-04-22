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
#include <QTextStream>
#include <QDate>
#include <QStringList>
#include <QSqlQuery>
#include <QDir>

Controller::Controller()
{

    mAuthoriseOk = false;
    mAccessToken = "";
    mUid = "";
    mManager = new QNetworkAccessManager();
    //    mUploadThread = new UploadThread();
    mUploadFilePath = "";
    //    connect(&mUploadThread, SIGNAL(uploadReplyFinished(QNetworkReply*)), this ,SLOT(handleUploadReply(QNetworkReply*)));
}

void Controller::error(QNetworkReply::NetworkError error)
{
    emit showError();
    sendRequest();
    qDebug() << Q_FUNC_INFO << "error=" << error;
}

bool Controller::isRequesting()
{
    return mIsRequesting;
}

void Controller::setRequesting(bool b)
{
    mIsRequesting = b;
    emit isRequestingChanged();
}

//-------------------request处理------------------
void Controller::sendRequest()
{
    QString reqTokenUrl = buildReqUrl();
    mInputUrl.setUrl(reqTokenUrl.toLatin1());
    qDebug() << Q_FUNC_INFO << "reqTokenUrl = " << reqTokenUrl;
    emit showWeb();
}

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
    qDebug() << Q_FUNC_INFO<<" mInputUrl = " << mInputUrl;
    return mInputUrl;
}

//提取accessToken
void Controller::getAccessToken(QString token, QString uid)
{
    mAccessToken = token;
    qDebug() << "token = " << mAccessToken;
    if (mAccessToken != "") {
        mAuthoriseOk = true;
    }
    mUid = uid;
    qDebug() << "uid = " << mUid;
    writeCfg();
}

//写入配置文件
void Controller::writeCfg()
{
    QDir::setCurrent("/home/user");
    cfgFile.setFileName("weipan.ini");
    if(NULL != mAccessToken)
    {
        QSettings *cfgReader = new QSettings(cfgFile.fileName(), QSettings::IniFormat);
        cfgReader->setIniCodec("UTF-8");
        cfgReader->beginGroup("config");
        cfgReader->setValue("OAuthInfo/oauth_token", mAccessToken);
        qDebug() <<Q_FUNC_INFO<<__LINE__<< "accessToken = " << mAccessToken;
        cfgReader->endGroup();
    }
}
//read .ini
void Controller::readCfg()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__<<"read .ini";
    QDir::setCurrent("/home/user");
    cfgFile.setFileName("weipan.ini");
    qDebug()<<Q_FUNC_INFO<<__LINE__<<"read .ini";
    QSettings *cfgReader = new QSettings(cfgFile.fileName(), QSettings::IniFormat);
    mAccessToken = cfgReader->value("config/OAuthInfo/oauth_token").toString();
    qDebug()<<Q_FUNC_INFO<<__LINE__<<"mAccessToken = " << mAccessToken;
    if(NULL == mAccessToken) {
        sendRequest();
    }

}

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


QString Controller::readFile(QString path)
{
    QFile file("/home/user" + path);

    qDebug() << Q_FUNC_INFO << "path==" << "/home/user" << path;
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug()<<"Can't open the file!";
        return NULL;
    }
    while(!file.atEnd()) {
        QByteArray line = file.readLine();
        QString str(line);
        qDebug()<< Q_FUNC_INFO<<str;
        return str;
    }
    file.close();
}


//------------------userInfo----------------------
void Controller::getUserInfo()
{
    QString reqUrl = builtGetUsrInfoReqUrl();
    QUrl mInputUrl;
    mInputUrl.setUrl(reqUrl.toLatin1());

    QNetworkRequest request;
    request.setUrl(mInputUrl);

    mUsrInfoReply =  mManager->get(request);

    connect(mUsrInfoReply, SIGNAL(finished()), this, SLOT(getUserInfoFinished()));
    connect(mUsrInfoReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));

}

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
void Controller::createFolder(QString folderName, QString fpath)
{
    if(folderName == "") {
        return;
    } else {
        QString createFolder = buildCreateFolderUrl();
        QUrl mInputUrl;
        mInputUrl.setUrl(createFolder.toLatin1());
        qDebug() << "createFolderUrl = " << mInputUrl;
        qDebug() << "folderName = " << folderName;

        QByteArray append("root=sandbox&path="+fpath.toLocal8Bit()+"/"+folderName.toLocal8Bit());
        qDebug() << Q_FUNC_INFO << "append = " << append;

        QNetworkRequest request;
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        request.setHeader(QNetworkRequest::ContentLengthHeader, QVariant(append.size()).toString());
        request.setUrl(mInputUrl);

        mCreatFlderReply = mManager->post(request, append);
        setRequesting(true);
        qDebug() << Q_FUNC_INFO << "mCreatFlderReply = " << mCreatFlderReply;
        connect(mCreatFlderReply, SIGNAL(finished()), this, SLOT(creatflderReplyFinished()));
        connect(mCreatFlderReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
    }
}


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

void Controller::creatflderReplyFinished()
{
    int ret = mCreatFlderReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << "ret = " << ret;
    setRequesting(false);
    if(ret == 200) {
        QByteArray buf = mCreatFlderReply->readAll();
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
    QString date;
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
                QString s = obj.value("modified").toString();
                QStringList s1 = s.split("+");
                date = s1[0];
            }
            mInfolist.append(FileInfo(obj.value("path").toString(), date,
                                      "", title, type));
            emit(result(mInfolist));
            emit procFolderFinished();
        }
    }
}



//----------------获取文件夹信息-----------------------

//获取文件夹信息
void Controller::reqMetaData(QString dataPath)
{
    //    listModel.clear(mInfolist);
    mInfolist.clear();
    emit(result(mInfolist));
    qDebug()<<Q_FUNC_INFO<<"datapath ========== "<<dataPath;
    QString metaDataUrl = buildMetaDataUrl(dataPath);
    QUrl mInputUrl;
    mInputUrl.setUrl(QString(metaDataUrl.toUtf8()));
    qDebug() << "metaData = " << metaDataUrl.toUtf8();
    qDebug() << "inpurtUrl = " << mInputUrl << " after encoded: " << mInputUrl.toEncoded();

    QNetworkRequest request;
    request.setUrl(QString(mInputUrl.toEncoded()));
    mMetaDataReply = mManager->get(request);
    setRequesting(true);
    connect(mMetaDataReply, SIGNAL(finished()),this,SLOT(metaDataReplyFinished()));
    connect(mMetaDataReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
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
    setRequesting(false);
    int ret = mMetaDataReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    qDebug() << Q_FUNC_INFO<<"ret===" << ret;

    if(ret == 200) {
        QByteArray getBuf = mMetaDataReply->readAll();
        qDebug() << Q_FUNC_INFO << "buf = " << getBuf;

        proMetaData(getBuf);

        mMetaDataReply->deleteLater();

    } else {
        qDebug() << Q_FUNC_INFO<<"error";
    }
}

void Controller::proMetaData(const QByteArray &buf)
{
    QJsonParseError jsonError;
    QJsonDocument json = QJsonDocument::fromJson(buf, &jsonError);

    QString title;
    bool type;
    QString date;

    if(jsonError.error == QJsonParseError::NoError)
    {
        if(json.isObject())
        {
            QJsonObject obj = json.object();
            if(obj.contains("contents")) {

                qDebug() << "contents:"<<obj.value("contents");

                QJsonArray jsonArray = obj["contents"].toArray();//

                if(jsonArray.size() == 0) {
                    qDebug() <<__LINE__<< "=size=0 ";
                    emit emptyFile();
                    qDebug() <<__LINE__<< "empty signal ";
                    mInfolist.clear();
                    emit result(mInfolist);
                    return;
                } else {
                    foreach (const QJsonValue & value, jsonArray) {
                        QJsonObject obj = value.toObject();
                        QFileInfo fi(obj["path"].toString());

                        title = fi.fileName();
                        qDebug() <<__LINE__<<"path = "<<obj["path"].toString();
                        qDebug() <<__LINE__<<"title = "<<title;
                        qDebug() <<__LINE__<<"is_dir = " << obj["is_dir"].toBool();
                        type = obj["is_dir"].toBool();
                        qDebug() <<__LINE__<<"type = "<<type;

                        QString s = obj["modified"].toString();
                        QStringList s1 = s.split("+");

                        date = s1[0];

                        if(type) {
                            QJsonArray array = obj[title].toArray();
                            int ArraySize = array.size();
                            qDebug() <<__LINE__<< "ArraySize = "<<ArraySize;

                            mInfolist.append(FileInfo(obj["path"].toString(), date,
                                             "", title, type));
                        } else {
                            mInfolist.append(FileInfo(obj["path"].toString(), date,
                                             obj["size"].toString(), title, type));
                        }
                        emit result(mInfolist);
                    }
                }
            }
        }
    }
}


//------------------------------------------上传文件----------------------------------------

////uploadThread
//void Controller::reqUploadFile(QString toPath)
//{
//    qDebug()<<Q_FUNC_INFO<<"toPath = "<< toPath;
//    mUploadThread.getUploadFilePath(mUploadFilePath, mULFileName);
//    mUploadThread.getAccessToken(mAccessToken);
//    qDebug()<<Q_FUNC_INFO;
//    mUploadThread.getDePath(toPath);
//    qDebug()<<Q_FUNC_INFO;
//    mUploadThread.start();
//}

//void Controller::handleUploadReply(QNetworkReply *reply)
//{
//    int ret = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
//    qDebug() << Q_FUNC_INFO << "ret = " << ret;
//    if(ret != 200) {
//        emit uploadFailed();
//        return;
//    } else {
//        QByteArray buf = reply->readAll();
//        qDebug() << Q_FUNC_INFO << "getbuf = " << buf;

//        QJsonParseError jsonError;
//        QJsonDocument json = QJsonDocument::fromJson(buf, &jsonError);
//        if(jsonError.error == QJsonParseError::NoError)
//        {
//            if(json.isObject())
//            {
//                QJsonObject obj = json.object();
//                if(obj.contains("is_dir"))
//                {
//                    qDebug() << "======================is_dir:"<<obj.value("is_dir");

//                    if(obj["is_dir"].isObject())
//                    {
//                        QJsonObject weatherObj = obj["is_dir"].toObject();

//                        qDebug() << "is_dir:"<<weatherObj.toVariantMap();
//                        return;
//                    }
//                }
//            }
//        }
//        mUploadList.append(LoadInfo(62, mULFileName, 50));
//        emit uploadResult(mUploadList);
//        emit(uploadFinished());
//        reply->deleteLater();
//    }
//}

////uploadThread end


////upload file in mainThread

//返回随机数 oauth_nonce
QString Controller::getRandNonce()
{
    QTime time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);
    return QString().number(qrand());
}

void Controller::getUploadFilePath(QString path)
{
    mUploadFilePath = path;
    qDebug() << "mFilePath===" << mUploadFilePath;
    QFileInfo fi(mUploadFilePath);
    mULFileName = fi.fileName();
}

void Controller::reqUploadFile(QString toPath)
{
    QByteArray data;
    QString uploadFileUrl = buildUploadFileUrl(toPath);
    QUrl mInputUrl;
    mInputUrl.setUrl(uploadFileUrl.toUtf8());
    qDebug()<<Q_FUNC_INFO<<"mInputUrl = " << mInputUrl;
    QNetworkRequest mUploadRequest;
    mUploadRequest.setUrl(mInputUrl);
    buildMultiPart(data, mUploadRequest);//使用multipart方式上传 构造multipart包
    mUploadFileReply = mManager->put(mUploadRequest,data);//上传
    connect(mUploadFileReply, SIGNAL(uploadProgress(qint64,qint64)),
            this, SLOT(uploadProgress(qint64,qint64)));
    connect(mUploadFileReply, SIGNAL(finished()), this, SLOT(upLoadFileReplyFinished()));
    connect(mUploadFileReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));

}

void Controller::uploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    qDebug() << Q_FUNC_INFO << "bytesSent=" << bytesSent << "bytesTotal=" << bytesTotal;

    if(bytesTotal != 0) {
        mUploadProgress = bytesSent*100/bytesTotal;

        qDebug() <<__LINE__<<Q_FUNC_INFO << mUploadProgress;

        mUploadList.clear();
        mUploadList.append(LoadInfo(bytesTotal,mULFileName , mUploadProgress));

    } else {
        qDebug() <<__LINE__<<Q_FUNC_INFO << mUploadProgress;
        mUploadProgress = 0;
    }

    emit uploadResult(mUploadList);
}

//构建上传文件的url
QString Controller::buildUploadFileUrl(QString &toPath)
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

//构造multipart
void Controller::buildMultiPart(QByteArray &data, QNetworkRequest request)
{
    QByteArray needToUploadFile;
    QString localPath = mUploadFilePath;
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
    request.setHeader(QNetworkRequest::ContentTypeHeader, contentType.toLatin1());
    request.setHeader(QNetworkRequest::ContentLengthHeader, QVariant(data.size()).toString());
}


//上传文件 反馈完毕
void Controller::upLoadFileReplyFinished()
{
    int ret = mUploadFileReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << Q_FUNC_INFO << "ret = " << ret;

    QByteArray buf = mUploadFileReply->readAll();
    qDebug() << Q_FUNC_INFO << "getbuf = " << buf;

    QJsonParseError jsonError;
    QJsonDocument json = QJsonDocument::fromJson(buf, &jsonError);
    if(jsonError.error == QJsonParseError::NoError)
    {
        if(json.isObject())
        {
            QJsonObject obj = json.object();
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
    emit(uploadFinished());
    mUploadFileReply->deleteLater();
}



//----------------------------------------下载文件--------------------------------------------

////downloadThread

//void Controller::reqDownloadFile()
//{
//    mDownloadThread.getToken(mAccessToken);
//    mDownloadThread.getDownloadPath(mDownloadpath);
//    qDebug()<< Q_FUNC_INFO << "downloadPath = " << mDownloadpath;

//    mUploadThread.start();
//}

////downloadThread end

//请求下载文件
//GET https://api.weipan.cn/2/files/<root>/<path>
void Controller::reqDownloadFile()
{

    QFileInfo fi(mDownloadpath);

    qDebug() << Q_FUNC_INFO << "downfile==path=" << mDownloadpath;

    QString path = "/home/user/";

    QString dir_path = "/home/user/weipan/";

    QDir dir(path);

    if(!dir.mkdir("weipan")) {
        qDebug()<<__LINE__<<"weipan dir has existed";
    }

    QString file_name = dir_path + fi.fileName();
    qDebug() << "fileName === " << file_name;

    QFile file(file_name);

    if(file.exists()) {
        file_name = dir_path + "f_" + fi.fileName();
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
    QUrl mInputUrl;
    mInputUrl.setUrl(downLoadFileUrl.toUtf8());

    qDebug() << Q_FUNC_INFO << "mInputUrl===" << mInputUrl;
    mRequest.setUrl(mInputUrl);
    mDownloadFileReply = mManager->get(mRequest);
    //    connect(mDownloadFileReply, SIGNAL(readyRead()), this, SLOT(dwnFileRealReadReady()));
    //    connect(mDownloadFileReply, SIGNAL(downloadProgress(qint64,qint64)),
    //            this, SLOT(downloadProgress(qint64,qint64)));
    connect(mDownloadFileReply, SIGNAL(finished()), this, SLOT(downLoadFileReplyFinished()));
}

void Controller::getDwnloadPath(QString path)
{
    mDownloadpath = path;
    qDebug()<< Q_FUNC_INFO << "====path==="<< mDownloadpath;
    QFileInfo fi(mDownloadpath);
    mDLFileName = fi.fileName();
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
                connect(mDownloadFileReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
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
    if(bytesTotal != 0) {
        mDownloadProgress = bytesReceived*100/bytesTotal;

        qDebug() <<__LINE__<<Q_FUNC_INFO << mDownloadProgress;

        mDownloadList.clear();
        mDownloadList.append(LoadInfo(bytesTotal, mDLFileName , mDownloadProgress));

    } else {
        qDebug() <<__LINE__<<Q_FUNC_INFO << mDownloadProgress;
        mDownloadProgress = 0;
    }

    emit downloadResult(mDownloadList);
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

        dwnFile->write(buf);
    }
}



//--------------------------------------delete file---------------------------------------------------------------------
void Controller::reqDeleteFile(QString path)//
{

    QString deleteFolder = buildDeleteFolderUrl();

    QUrl mInputUrl;
    mInputUrl.setUrl(deleteFolder.toLatin1());
    qDebug() << "createFolderUrl = " << mInputUrl;
    qDebug() << "path = " << path;

    QByteArray append("root=sandbox&path="+ path.toLocal8Bit());
    qDebug() << Q_FUNC_INFO << "append = " << append;

    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::ContentLengthHeader, QVariant(append.size()).toString());
    request.setUrl(mInputUrl);

    mDeleteFlderReply = mManager->post(request, append);
    qDebug() << Q_FUNC_INFO << "mCreatFlderReply = " << mDeleteFlderReply;
    connect(mDeleteFlderReply, SIGNAL(finished()), this, SLOT(deleteflderReplyFinished()));
    connect(mDeleteFlderReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));

}



QString Controller::buildDeleteFolderUrl()
{
    QString para;
    QString deleteFolderUrl(DELETE_FILE_URL);
    QString _deleteFolderUrl = deleteFolderUrl;
    _deleteFolderUrl.append("?");
    _deleteFolderUrl.append(ACCESS_TOKEN);
    _deleteFolderUrl.append(mAccessToken);
    _deleteFolderUrl.append(removeUrlEncode(para));//添加去掉编码的参数
    return _deleteFolderUrl;
}

void Controller::deleteflderReplyFinished()
{
    int ret = mDeleteFlderReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << "ret = " << ret;
    if(ret == 200) {
        QByteArray buf = mDeleteFlderReply->readAll();
        qDebug() << Q_FUNC_INFO << "buf = " << buf;
        mDeleteFlderReply->deleteLater();
    } else {
        qDebug() << Q_FUNC_INFO<<"error";
        return;
    }
}

