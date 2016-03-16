#ifndef FILESINFO_H
#define FILESINFO_H

#include <QString>
#include <QUrl>

class FileInfo
{
public:
    FileInfo();
    explicit FileInfo(QUrl path, QString date, QString size, QString title, bool type);
    QUrl getPath() const;
    QString getDate() const;
    QString getSize() const;
    QString getTitle() const;
    bool getType() const;


private:
    QString mDate, mSize, mTitle;
    QUrl mPath;
    int mType;
};

typedef QList <FileInfo> InfoList;

#endif // FILESINFO_H
