#include "mainListModel.h"
#include <QFileInfo>
#include <QDir>

MainListModel::MainListModel(QObject *parent) : QAbstractListModel(parent)
{
    qDebug() <<__FUNCTION__<<__LINE__<< "list 1111111111111111";
    mIndex = 0;
    mCurrentIndex = -1;             //设初值
}

int MainListModel::currentIndex() const
{
    return mCurrentIndex;
}

QUrl MainListModel::getcurrentPath() const
{
    if(mInfolist.size() > mCurrentIndex && mCurrentIndex >= 0) {//是否越界
        return mInfolist.at(mCurrentIndex).getPath();}
    return QUrl();
}

int MainListModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    qDebug()<<"=======mInfolist.count()======"<<mInfolist.count();
    return mInfolist.count();
}

void MainListModel::handleList(InfoList filelist)
{
    beginResetModel();
    mInfolist = filelist;
    endResetModel();
}

QVariant MainListModel::data(const QModelIndex & index, int role) const
{
    qDebug()<<"=======data======";
    if (index.row() < 0 || index.row() >= mInfolist.count())//是否越界
        return QVariant();
    const FileInfo f = mInfolist.at(index.row());
    switch (role) {
    case pathRole:
        return f.getPath();
    case sizeRole:
        return f.getSize();
    case titleRole:
        return f.getTitle();
    case dateRole:
        qDebug()<<"=======date======"<<f.getDate();
        return f.getDate();
    case typeRole:
        return f.getType();
    default:
        return QVariant();
    }
}

QString MainListModel::getPath(int index) const
{
    qDebug() << Q_FUNC_INFO << "====getPath===row====" << index;
    QString str;
    if (index >= 0 && index < mInfolist.size()) {
        str = mInfolist.at(index).getPath().toString();
    }
    qDebug() << Q_FUNC_INFO << "====getPath===path====" << str;
    return str;

}

bool MainListModel::getType(int index) const
{
    qDebug() << Q_FUNC_INFO << "====getType===row====" << index;
    bool i;
    if (index >= 0 && index < mInfolist.size()) {
        i = mInfolist.at(index).getType();
    }
    qDebug() << Q_FUNC_INFO << "====getType===Type====" << i;
    return i;

}

void MainListModel::remove(int first, int last)
{
    if ((first < 0) && (first >= mInfolist.count()))//是否越界
        return;
    if ((last < 0) && (last >= mInfolist.count()))//是否越界
        return;
    if (first > last) {
        first ^= last;
        last ^= first;
        first ^= last;
    }
    beginRemoveRows(QModelIndex(), first, last);
    qDebug()<< "first=" << first;
    while (first <= last) {
        qDebug() << "first=" << first;
        mInfolist.removeAt(first);
//        mPath = mInfolist.at(first).getPath().toString();
//        QFile fi(mPath);
//        bool type = mInfolist.at(first).getType();
//        qDebug() << Q_FUNC_INFO << "mPath===" << mPath;
//        if (!type)
//        {
//            QDir dir(mPath);
//            dir.rmdir(fi.fileName());
//        }
//        QFile::remove(fi.fileName());
        qDebug() << "first=" << first;
        last --;
    }
    emit deleteFinished();
    endRemoveRows();
    if (mCurrentIndex >= mInfolist.count()) {//是否越界
        setCurrentIndex(mInfolist.count() - 1);
    }
}

void MainListModel::rename(QString name)
{
    beginResetModel();
    mInfolist.at(mCurrentIndex).getTitle().replace(mCurrentIndex, name);
    qDebug()<<"renamed listName ===="<<name;
    endResetModel();
}

void MainListModel::setCurrentIndex(const int & i)
{
    if (i >= mInfolist.count() && mCurrentIndex != 0) {
        mCurrentIndex = 0;
        emit currentIndexChanged();
    } else if ((i >= 0) && (i < mInfolist.count()) && (mCurrentIndex != i)) {
        mCurrentIndex = i;
        emit currentIndexChanged();
    }
}

QHash<int, QByteArray> MainListModel::roleNames() const
{
    qDebug() << "roleNames =";
    QHash<int, QByteArray> role;
    role[sizeRole] = "size";
    role[pathRole] = "path";
    role[titleRole] = "title";
    role[dateRole] = "date";
    role[typeRole] = "type";
    return role;
}


