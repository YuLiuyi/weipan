#ifndef THREAD_H
#define THREAD_H
#include <QThread>
#include "filesInfo.h"

class Thread:public QThread
{
    Q_OBJECT
public:
    Thread(QObject *parent = 0);
    void run();                                 //线程运行
    void setInfo(QByteArray buf);                 //
    bool stop;                                  //该线程是否停止
    void procFiles();//
    void procFolder();
signals:
    void procResult(InfoList);
    void emptyFile();

private:
    QByteArray            mBuf;
    InfoList              mList;
};
#endif // THREAD_H
