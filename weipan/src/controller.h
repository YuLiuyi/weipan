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
#include "loadInfo.h"
#include "mainListModel.h"
#include "downloadThread.h"
#include "uploadThread.h"

typedef struct _usrInfoJson
{
    QString quotaTotal;
    QString quotaUsed;
    QString usrName;
    QString usrID;
    QString gender;
}struUsrInfo;

typedef struct _tokenInfoJson
{
    QString auToken;
}struTkInfo;

class Controller : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isRequesting READ isRequesting NOTIFY isRequestingChanged)

public:
    explicit Controller();


    void sendRequest();                                 //发送request请求
    QString buildReqUrl();                                          //构建 request url


    Q_INVOKABLE QUrl loadWeb();         //

    Q_INVOKABLE void getAccessToken(QString token, QString uid);    //获取accessToken

    void writeCfg();                                                //token写入配置文件
    Q_INVOKABLE void readCfg();

    Q_INVOKABLE bool authoriseOk();

    int openFile(QByteArray &buf,const QString &filePath);          //open file

    Q_INVOKABLE QString readFile(QString path);

    //userInfo
    Q_INVOKABLE void getUserInfo();                                 //构建 UserInfo request url
    QString builtGetUsrInfoReqUrl();
    void procUsrInfo(struUsrInfo &usrInfo, const QByteArray &buf);  // 存储用户信息到结构体中
    Q_INVOKABLE QString showUserInfo(int index);


    //creat folder
    Q_INVOKABLE void createFolder(QString folderName, QString path);
    QString buildCreateFolderUrl();
    void procFolder(const QByteArray &buf);


    //folderInfo
    Q_INVOKABLE void reqMetaData(QString dataPath);                 //获取文件夹信息
    QString buildMetaDataUrl(QString &dataPath);                    //构造获取文件夹信息url
    void proMetaData(const QByteArray &buf);

    //upload file
    Q_INVOKABLE void reqUploadFile(QString toPath);
    void buildMultiPart(QByteArray &data, QNetworkRequest request);
    QString getRandNonce();
    QString buildUploadFileUrl(QString &toPath);
    Q_INVOKABLE void getUploadFilePath(QString path);


    //download file
    Q_INVOKABLE void reqDownloadFile();
    Q_INVOKABLE void getDwnloadPath(QString path);
    QString buildDownFileUrl(QString &fromPath);


    //delete file/folder
    Q_INVOKABLE void reqDeleteFile(QString path);
    QString buildDeleteFolderUrl();


signals:

    void showError();
    void showWeb();
    void result(InfoList);

    void getDataFinished();

    void uploadFinished();
    void uploadFailed();
    void uploadResult(UploadInfoList);

    void downloadFinished();
    void downloadResult(DownloadInfoList);

    void emptyFile();

    void procFolderFinished();

    void isRequestingChanged();


public slots:
    void getUserInfoFinished();

    void metaDataReplyFinished();

    void upLoadFileReplyFinished();
    void uploadProgress(qint64 bytesSent, qint64 bytesTotal);
//    void handleUploadReply(QNetworkReply *reply);

    void error(QNetworkReply::NetworkError error);

    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void dwnFileRealReadReady();
    void downLoadFileReplyFinished();

    void creatflderReplyFinished();
    void deleteflderReplyFinished();

    bool isRequesting();

    void setRequesting(bool);

private:

    QString               mAccessToken;
    QString               mUid;
    bool                  mAuthoriseOk;


    QUrl                  mInputUrl;
    QNetworkAccessManager *mManager;

    struTkInfo            mAuTokenInfo;

    struUsrInfo           mUsrInfoStru;
    QNetworkReply         *mUsrInfoReply;

    QNetworkReply         *mMetaDataReply;

    QNetworkRequest       mRequest;

    QNetworkReply         *mUploadFileReply;
    QString               mULFileName;
    UploadInfoList        mUploadList;
    int                   mUploadProgress;
    QString               mUploadFilePath;
    UploadThread          mUploadThread;

    QNetworkReply         *mDownloadFileReply;
    QNetworkReply         *mDwnFileRealReply;
    QByteArray            mDwnFileBuf;
    QString               mDLFileName;
    DownloadInfoList      mDownloadList;
    int                   mDownloadProgress;
    QString               mDownloadpath;
    QFile                 *dwnFile;
    DownloadThread        mDownloadThread;

    QNetworkReply         *mCreatFlderReply;
    QNetworkReply         *mDeleteFlderReply;

    QFile                 cfgFile;

    InfoList              mInfolist;

    MainListModel         listModel;

    bool mIsRequesting;

};


#endif // CONTROLLER_H
