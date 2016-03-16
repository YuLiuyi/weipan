#ifndef MAINLISTMODEL_H
#define MAINLISTMODEL_H

#include <QUrl>
#include <QDebug>
#include <QFileInfo>
#include <QAbstractListModel>
#include "filesInfo.h"
#include "controller.h"

class MainListModel : public QAbstractListModel
{
    Q_OBJECT

public:

    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    explicit MainListModel(QObject *parent = 0);
    ~MainListModel() { }
    int currentIndex() const;
    Q_INVOKABLE int rowCount(const QModelIndex & parent = QModelIndex()) const;		//总行数
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;	//用于在qml显示的内容
    Q_INVOKABLE QUrl getcurrentPath() const;					//得到当前路径
    Q_INVOKABLE QString getPath(int index) const;
    Q_INVOKABLE bool getType(int index) const;
    Q_INVOKABLE void remove(int first, int last);				//删除
    Q_INVOKABLE void rename(QString name);

    void setCurrentIndex(const int & i);

    enum Role {
        pathRole = Qt::UserRole + 1,
        titleRole,
        sizeRole,
        dateRole,
        typeRole,
    };

private slots:
    void handleList(InfoList filelist);

signals:
    void currentIndexChanged();
    void deleteFinished();

private:

    int mCurrentIndex;
    QString mPath;
    QHash <int, QByteArray> roleNames() const;
    int mIndex;
    InfoList mInfolist;
};


#endif // MAINLISTMODEL_H
