#ifndef __MYAPP_WORKSPACE__
#define __MYAPP_WORKSPACE__

#include <QQuickView>
#include <QQuickWindow>
#include <QQmlEngine>
#include <QQmlComponent>
#include <SyberosGuiCache>
#include <cgui_application.h>
#include <cworkspace.h>
#include "controller.h"
#include "mainListModel.h"
#include "listmodel.h"
#include "loadInfo.h"
//#include "thread.h"
#include "uploadThread.h"
#include "downloadListmodel.h"


using namespace SYBEROS;

class MyApp_Workspace : public CWorkspace
{
    Q_OBJECT

private:
    QQuickView *m_view;
    Controller contrl;
    MainListModel *mMainListModel;
    ListModel *mListModel;
    DListModel *mDListModel;
//    Thread *thread;

public:
    MyApp_Workspace();

    // 应用启动结束时，回调此函数。根据传入的option，应用可以区分启动的方式。
    void onLaunchComplete(Option option, const QStringList& params);

};


#endif //__MYAPP_WORKSPACE__

