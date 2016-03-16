/****************************************************************************
** Meta object code from reading C++ file 'mainListModel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../yuan_weipan/src/mainListModel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainListModel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainListModel_t {
    QByteArrayData data[19];
    char stringdata[175];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainListModel_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainListModel_t qt_meta_stringdata_MainListModel = {
    {
QT_MOC_LITERAL(0, 0, 13),
QT_MOC_LITERAL(1, 14, 19),
QT_MOC_LITERAL(2, 34, 0),
QT_MOC_LITERAL(3, 35, 14),
QT_MOC_LITERAL(4, 50, 10),
QT_MOC_LITERAL(5, 61, 8),
QT_MOC_LITERAL(6, 70, 8),
QT_MOC_LITERAL(7, 79, 8),
QT_MOC_LITERAL(8, 88, 6),
QT_MOC_LITERAL(9, 95, 14),
QT_MOC_LITERAL(10, 110, 7),
QT_MOC_LITERAL(11, 118, 5),
QT_MOC_LITERAL(12, 124, 7),
QT_MOC_LITERAL(13, 132, 6),
QT_MOC_LITERAL(14, 139, 5),
QT_MOC_LITERAL(15, 145, 4),
QT_MOC_LITERAL(16, 150, 6),
QT_MOC_LITERAL(17, 157, 4),
QT_MOC_LITERAL(18, 162, 12)
    },
    "MainListModel\0currentIndexChanged\0\0"
    "deleteFinished\0handleList\0InfoList\0"
    "filelist\0rowCount\0parent\0getcurrentPath\0"
    "getPath\0index\0getType\0remove\0first\0"
    "last\0rename\0name\0currentIndex"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainListModel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       1,   88, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x06 /* Public */,
       3,    0,   65,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   66,    2, 0x08 /* Private */,

 // methods: name, argc, parameters, tag, flags
       7,    1,   69,    2, 0x02 /* Public */,
       7,    0,   72,    2, 0x22 /* Public | MethodCloned */,
       9,    0,   73,    2, 0x02 /* Public */,
      10,    1,   74,    2, 0x02 /* Public */,
      12,    1,   77,    2, 0x02 /* Public */,
      13,    2,   80,    2, 0x02 /* Public */,
      16,    1,   85,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 5,    6,

 // methods: parameters
    QMetaType::Int, QMetaType::QModelIndex,    8,
    QMetaType::Int,
    QMetaType::QUrl,
    QMetaType::QString, QMetaType::Int,   11,
    QMetaType::Bool, QMetaType::Int,   11,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   14,   15,
    QMetaType::Void, QMetaType::QString,   17,

 // properties: name, type, flags
      18, QMetaType::Int, 0x00495103,

 // properties: notify_signal_id
       0,

       0        // eod
};

void MainListModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainListModel *_t = static_cast<MainListModel *>(_o);
        switch (_id) {
        case 0: _t->currentIndexChanged(); break;
        case 1: _t->deleteFinished(); break;
        case 2: _t->handleList((*reinterpret_cast< InfoList(*)>(_a[1]))); break;
        case 3: { int _r = _t->rowCount((*reinterpret_cast< const QModelIndex(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 4: { int _r = _t->rowCount();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 5: { QUrl _r = _t->getcurrentPath();
            if (_a[0]) *reinterpret_cast< QUrl*>(_a[0]) = _r; }  break;
        case 6: { QString _r = _t->getPath((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 7: { bool _r = _t->getType((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 8: _t->remove((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 9: _t->rename((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MainListModel::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainListModel::currentIndexChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (MainListModel::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainListModel::deleteFinished)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject MainListModel::staticMetaObject = {
    { &QAbstractListModel::staticMetaObject, qt_meta_stringdata_MainListModel.data,
      qt_meta_data_MainListModel,  qt_static_metacall, 0, 0}
};


const QMetaObject *MainListModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainListModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainListModel.stringdata))
        return static_cast<void*>(const_cast< MainListModel*>(this));
    return QAbstractListModel::qt_metacast(_clname);
}

int MainListModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = currentIndex(); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setCurrentIndex(*reinterpret_cast< int*>(_v)); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 1;
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void MainListModel::currentIndexChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void MainListModel::deleteFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
