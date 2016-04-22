#ifndef LOADINFO_H
#define LOADINFO_H
#include <QString>
#include <QUrl>

class LoadInfo
{
public:
    LoadInfo();
    explicit LoadInfo(qint64 size, QString title, int progress);
    int getSize() const;
    QString getTitle() const;
    int getProgress() const;

private:
    QString mTitle;
    qint64 mSize;
    int mProgress;
};

typedef QList <LoadInfo> UploadInfoList;
typedef QList <LoadInfo> DownloadInfoList;
#endif // LOADINFO_H
