#include "downloadListmodel.h"
#include <QFileInfo>
#include <QDir>

DListModel::DListModel(QObject *parent) : QAbstractListModel(parent)
{
    qDebug() <<__FUNCTION__<<__LINE__<< "list";
    mIndex = 0;
    mCurrentIndex = -1;             //设初值

}

int DListModel::currentIndex() const
{
    return mCurrentIndex;
}

int DListModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    qDebug()<<"=======mDList.count()======"<<mDList.count();
    return mDList.count();
}

QVariant DListModel::data(const QModelIndex & index, int role) const
{
    qDebug()<<"=======data======";
    if (index.row() < 0 || index.row() >= mDList.count())//是否越界
        return QVariant();
    const LoadInfo f = mDList.at(index.row());
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

void DListModel::downloadResultHandle(DownloadInfoList filelist)
{
    qDebug() << Q_FUNC_INFO ;
    beginResetModel();
    mDList = filelist;
    endResetModel();
}

void DListModel::setCurrentIndex(const int & i)
{
    if (i >= mDList.count() && mCurrentIndex != 0) {
        mCurrentIndex = 0;
        emit currentIndexChanged();
    } else if ((i >= 0) && (i < mDList.count()) && (mCurrentIndex != i)) {
        mCurrentIndex = i;
        emit currentIndexChanged();
    }
}

QHash<int, QByteArray> DListModel::roleNames() const
{
    qDebug() << "roleNames =";
    QHash<int, QByteArray> role;
    role[sizeRole] = "size";
    role[titleRole] = "filename";
    role[progressRole] = "progress";
    return role;
}
