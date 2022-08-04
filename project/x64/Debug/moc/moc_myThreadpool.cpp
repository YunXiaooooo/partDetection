/****************************************************************************
** Meta object code from reading C++ file 'myThreadpool.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../inc/myInc/myThreadpool.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'myThreadpool.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_myThreadpool_t {
    QByteArrayData data[8];
    char stringdata0[64];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_myThreadpool_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_myThreadpool_t qt_meta_stringdata_myThreadpool = {
    {
QT_MOC_LITERAL(0, 0, 12), // "myThreadpool"
QT_MOC_LITERAL(1, 13, 7), // "enqueue"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 13), // "imageProcess*"
QT_MOC_LITERAL(4, 36, 10), // "imgProcess"
QT_MOC_LITERAL(5, 47, 8), // "cv::Mat&"
QT_MOC_LITERAL(6, 56, 3), // "img"
QT_MOC_LITERAL(7, 60, 3) // "num"

    },
    "myThreadpool\0enqueue\0\0imageProcess*\0"
    "imgProcess\0cv::Mat&\0img\0num"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_myThreadpool[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    3,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5, QMetaType::Int,    4,    6,    7,

       0        // eod
};

void myThreadpool::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<myThreadpool *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->enqueue((*reinterpret_cast< imageProcess*(*)>(_a[1])),(*reinterpret_cast< cv::Mat(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject myThreadpool::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_myThreadpool.data,
    qt_meta_data_myThreadpool,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *myThreadpool::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *myThreadpool::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_myThreadpool.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int myThreadpool::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
