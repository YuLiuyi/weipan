#include "filesInfo.h"

FileInfo::FileInfo()
{

}
FileInfo::FileInfo(QUrl path, QString date, QString size,  QString title, bool type)
{
    mPath = path;
    mSize = size;
    mDate = date;
    mTitle = title;
    mType = type;
}

QString FileInfo::getDate() const
{
    return mDate;
}

QString FileInfo::getSize() const
{
    return mSize;
}

QUrl FileInfo::getPath() const
{
    return mPath;
}

QString FileInfo::getTitle() const
{
    return mTitle;
}

bool FileInfo::getType() const
{
    return mType;
}
