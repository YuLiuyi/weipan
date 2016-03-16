#include "MyApp_Workspace.h"
#include <QDebug>
#include <QQmlContext>

MyApp_Workspace::MyApp_Workspace()
    : CWorkspace()
{
    qRegisterMetaType<InfoList>("InfoList");
    m_view = SYBEROS::SyberosGuiCache::qQuickView();
    QObject::connect(m_view->engine(), SIGNAL(quit()), qApp, SLOT(quit()));
    mMainListModel = new MainListModel;
    m_view->rootContext()->setContextProperty("mainListModel", mMainListModel);
//    qmlRegisterType<MainListModel>("ListModel", 1, 0, "MListModel");

    m_view->rootContext()->setContextProperty("contrl", &contrl);
    QObject::connect(&contrl,SIGNAL(result(InfoList)), mMainListModel, SLOT(handleList(InfoList)));
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


