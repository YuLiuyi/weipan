#include "MyApp_Workspace.h"
#include <QDebug>
#include <QQmlContext>

MyApp_Workspace::MyApp_Workspace()
    : CWorkspace()
{
    qRegisterMetaType<InfoList>("InfoList");
    qRegisterMetaType<UploadInfoList>("UpLoadInfoList");
    qRegisterMetaType<DownloadInfoList>("DownLoadInfoList");
    m_view = SYBEROS::SyberosGuiCache::qQuickView();
    QObject::connect(m_view->engine(), SIGNAL(quit()), qApp, SLOT(quit()));

    mListModel = new ListModel;
    m_view->rootContext()->setContextProperty("listmodel", mListModel);

    mDListModel = new DListModel;
    m_view->rootContext()->setContextProperty("downListmodel", mDListModel);


    mMainListModel = new MainListModel;
    m_view->rootContext()->setContextProperty("mainListModel", mMainListModel);
//    qmlRegisterType<MainListModel>("ListModel", 1, 0, "MListModel");

    m_view->rootContext()->setContextProperty("contrl", &contrl);
    QObject::connect(&contrl, SIGNAL(result(InfoList)), mMainListModel, SLOT(handleList(InfoList)));
    QObject::connect(&contrl, SIGNAL(uploadResult(UploadInfoList)), mListModel, SLOT(UploadResultHandle(UploadInfoList)));
    QObject::connect(&contrl, SIGNAL(downloadResult(DownloadInfoList)), mDListModel, SLOT(downloadResultHandle(DownloadInfoList)));
//    QObject::connect(&contrl, SIGNAL(uploadResult(UploadInfoList)), mListModel, SLOT(handleList(UploadInfoList)));
//    m_view->rootContext()->setContextProperty("thread", thread);
    m_view->setSource(QUrl("qrc:/qml/main.qml"));
    m_view->showFullScreen();
}

void MyApp_Workspace::onLaunchComplete(Option option, const QStringList& params)
{
    Q_UNUSED(params)

    switch (option) {
    case CWorkspace::HOME:
        qDebug()<< "Start by Home";
        break;
    case CWorkspace::URL:
        break;
    case CWorkspace::EVENT:
        break;
    case CWorkspace::DOCUMENT:
        break;
    default:
        break;
    }
}


