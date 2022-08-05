/****************************************************************************
** Meta object code from reading C++ file 'view.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../inc/myQTInc/view.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'view.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_view_t {
    QByteArrayData data[15];
    char stringdata0[140];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_view_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_view_t qt_meta_stringdata_view = {
    {
QT_MOC_LITERAL(0, 0, 4), // "view"
QT_MOC_LITERAL(1, 5, 15), // "oneImageDispaly"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 15), // "oneImageProcess"
QT_MOC_LITERAL(4, 38, 8), // "cv::Mat&"
QT_MOC_LITERAL(5, 47, 3), // "img"
QT_MOC_LITERAL(6, 51, 3), // "num"
QT_MOC_LITERAL(7, 55, 12), // "readyToReply"
QT_MOC_LITERAL(8, 68, 18), // "pushButtonGetImage"
QT_MOC_LITERAL(9, 87, 12), // "imageDisplay"
QT_MOC_LITERAL(10, 100, 4), // "indx"
QT_MOC_LITERAL(11, 105, 4), // "grab"
QT_MOC_LITERAL(12, 110, 7), // "grabNum"
QT_MOC_LITERAL(13, 118, 14), // "prepareToReply"
QT_MOC_LITERAL(14, 133, 6) // "Change"

    },
    "view\0oneImageDispaly\0\0oneImageProcess\0"
    "cv::Mat&\0img\0num\0readyToReply\0"
    "pushButtonGetImage\0imageDisplay\0indx\0"
    "grab\0grabNum\0prepareToReply\0Change"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_view[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,
       3,    2,   57,    2, 0x06 /* Public */,
       7,    0,   62,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   63,    2, 0x0a /* Public */,
       9,    1,   64,    2, 0x0a /* Public */,
      11,    1,   67,    2, 0x0a /* Public */,
      13,    0,   70,    2, 0x0a /* Public */,
      14,    0,   71,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, 0x80000000 | 4, QMetaType::Int,    5,    6,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void view::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<view *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->oneImageDispaly((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->oneImageProcess((*reinterpret_cast< cv::Mat(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->readyToReply(); break;
        case 3: _t->pushButtonGetImage(); break;
        case 4: _t->imageDisplay((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->grab((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->prepareToReply(); break;
        case 7: _t->Change(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (view::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&view::oneImageDispaly)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (view::*)(cv::Mat & , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&view::oneImageProcess)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (view::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&view::readyToReply)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject view::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_view.data,
    qt_meta_data_view,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *view::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *view::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_view.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int view::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void view::oneImageDispaly(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void view::oneImageProcess(cv::Mat & _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void view::readyToReply()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
