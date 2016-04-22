#ifndef DOWNLOADLISTMODEL_H
#define DOWNLOADLISTMODEL_H

#include <QUrl>
#include <QDebug>
#include <QFileInfo>
#include <QAbstractListModel>
#include "loadInfo.h"

class DListModel : public QAbstractListModel
{
    Q_OBJECT

public:

    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    explicit DListModel(QObject *parent = 0);
    int currentIndex() const;
    Q_INVOKABLE int rowCount(const QModelIndex & parent = QModelIndex()) const;		//总行数
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;	//用于在qml显示的内容

    void setCurrentIndex(const int & i);

    enum Role {
        titleRole = Qt::UserRole + 1,
        sizeRole,
        progressRole,
    };

public slots:
    void downloadResultHandle(DownloadInfoList filelist);

signals:
    void currentIndexChanged();

private:
    int mIndex;
    int mCurrentIndex;
    DownloadInfoList mDList;
//    UploadInfoList mUList;
    QHash<int, QByteArray>roleNames() const;
};




#endif // DOWNLOADLISTMODEL_H
