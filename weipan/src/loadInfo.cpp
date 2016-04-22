#include "loadInfo.h"

LoadInfo::LoadInfo()
{

}
LoadInfo::LoadInfo(qint64 size,  QString title, int progress)
{
    mSize = size;
    mTitle = title;
    mProgress = progress;
}

int LoadInfo::getSize() const
{
    return mSize;
}

QString LoadInfo::getTitle() const
{
    return mTitle;
}

int LoadInfo::getProgress() const
{
    return mProgress;
}

