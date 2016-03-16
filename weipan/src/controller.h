#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QString>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QFile>
#include <QDebug>
#include "publicFun.h"
#include "filesInfo.h"

typedef struct _accessTokenJson
{
    QString accessToken;
    QString usrID;
    QString mUploadNode;
}struAccessToken;

typedef struct _usrInfoJson
{
    QString quotaTotal;
    QString quotaUsed;
    QString usrName;
    QString usrID;
    QString gender;
}struUsrInfo;

class Controller : public QObject
{
    Q_OBJECT

public:
    explicit Controller();

    //发送request请求
    Q_INVOKABLE void sendRequest();

    //构建 request url
    QString buildReqUrl();

    //qml需要的url
    Q_INVOKABLE QUrl loadWeb();

    //获取accessToken
    Q_INVOKABLE void getAccessToken(QString token, QString uid);

    Q_INVOKABLE bool authoriseOk();

    int openFile(QByteArray &buf,const QString &filePath);

    //写入配置文件
    void writeCfg();

    Q_INVOKABLE void getUserInfo();
    //构建 UserInfo request url
    QString builtGetUsrInfoReqUrl();

    // 存储用户信息到结构体中
    void procUsrInfo(struUsrInfo &usrInfo, const QByteArray &buf);

    Q_INVOKABLE QString showUserInfo(int index);

    //获取文件夹信息
    Q_INVOKABLE void reqMetaData(QString dataPath);

    //构造获取文件夹信息url
    QString buildMetaDataUrl(QString &dataPath);

    //提取文件夹信息
    void procMetaData(const QByteArray &buf);

    Q_INVOKABLE void reqUploadFile();

    void buildMultiPart(QByteArray &data);

    QString getRandNonce();

    QString buildUploadFileUrl(QString &toPath);

    Q_INVOKABLE void getUploadFilePath(QString path);

    Q_INVOKABLE void reqDownLoadFile();

    Q_INVOKABLE void getDwnloadPath(QString path);

    QString buildDownFileUrl(QString &fromPath);

    Q_INVOKABLE void createFolder(QString folderName);

    QString buildCreateFolderUrl();

    void procFolder(const QByteArray &buf);


signals:
    void result(InfoList);
    void emptyFile();
    void uploadFinished();
    void downloadFinished();

public slots:
    void getUserInfoFinished();

    void metaDataReplyFinished();

    void upLoadFileReplyFinished();
    void uploadProgress(qint64 bytesSent, qint64 bytesTotal);

    void error(QNetworkReply::NetworkError error);

    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void dwnFileRealReadReady();
    void downLoadFileReplyFinished();

    void creatflderReplyFinished(QNetworkReply* reply);

private:

    QUrl                  inputUrl;
    QNetworkRequest       mRequest;
    QNetworkRequest       mUploadRequest;
    QNetworkReply         *mCreatFlderReply;
    QNetworkReply         *mUsrInfoReply;
    QNetworkReply         *mMetaDataReply;
    QNetworkReply         *mUploadLocateReply;
    QNetworkReply         *mUploadFileReply;
    QNetworkReply         *mDownloadFileReply;
    QNetworkReply         *mDwnFileRealReply;
    QNetworkAccessManager *mManager;
    QFile                 cfgFile;
    QString               mFilePath;
    QString               mULFileName;
    int                   mAuUrlChangeNum;
    bool                  mAuthoriseOk;
    struAccessToken       mAccessTokenInfo;
    struUsrInfo           mUsrInfoStru;
    QByteArray            mDwnFileBuf;
    QFile                 *dwnFile;
    QString               mAccessToken;
    QString               mUid;
    QString               mDownloadpath;
    InfoList              mInfolist;
};


#endif // CONTROLLER_H
