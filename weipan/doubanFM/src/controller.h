#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <QObject>

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller();
    ~Controller();
    void getChannelInfoReq();

signals:

private slots:
    void proChannelInfo();

};


#endif // CONTROLLER_H
