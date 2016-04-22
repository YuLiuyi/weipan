#include "listmodel.h"
#include <QFileInfo>
#include <QDir>

ListModel::ListModel(QObject *parent) : QAbstractListModel(parent)
{
    qDebug() <<__FUNCTION__<<__LINE__<< "list";
    mIndex = 0;
    mCurrentIndex = -1;             //设初值

}

int ListModel::currentIndex() const
{
    return mCurrentIndex;
}

int ListModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    qDebug()<<"=======mUList.count()======"<<mUList.count();
    return mUList.count();
}

QVariant ListModel::data(const QModelIndex & index, int role) const
{
    qDebug()<<"=======data======";
    if (index.row() < 0 || index.row() >= mUList.count())//是否越界
        return QVariant();
    const LoadInfo f = mUList.at(index.row());
    switch (role) {
    case sizeRole:
        return f.getSize();
        qDebug()<<"=======size======"<<f.getSize();
    case titleRole:
        return f.getTitle();
    case progressRole:
        return f.getProgress();
    default:
        return QVariant();
    }
}

void ListModel::UploadResultHandle(UploadInfoList filelist)
{
    qDebug() << Q_FUNC_INFO ;
    beginResetModel();
    mUList = filelist;
    endResetModel();
}

void ListModel::setCurrentIndex(const int & i)
{
    if (i >= mUList.count() && mCurrentIndex != 0) {
        mCurrentIndex = 0;
        emit currentIndexChanged();
    } else if ((i >= 0) && (i < mUList.count()) && (mCurrentIndex != i)) {
        mCurrentIndex = i;
        emit currentIndexChanged();
    }
}

QHash<int, QByteArray> ListModel::roleNames() const
{
    qDebug() << "roleNames =";
    QHash<int, QByteArray> role;
    role[sizeRole] = "size";
    role[titleRole] = "filename";
    role[progressRole] = "progress";
    return role;
}
