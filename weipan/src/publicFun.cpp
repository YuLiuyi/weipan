//              this is a public function cpp file, provide some simple function;


#include"publicFun.h"
#include <QDebug>


//去除url编码
QString removeUrlEncode(const QString &urlStr)
{
    QUrl url;
    url.setUrl(urlStr.toLatin1());
    return url.toString();
}


