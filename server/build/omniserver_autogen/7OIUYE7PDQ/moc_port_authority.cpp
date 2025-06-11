/****************************************************************************
** Meta object code from reading C++ file 'port_authority.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/net/port_authority.h"
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'port_authority.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN13PortAuthorityE_t {};
} // unnamed namespace

template <> constexpr inline auto PortAuthority::qt_create_metaobjectdata<qt_meta_tag_ZN13PortAuthorityE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "PortAuthority",
        "portTestSuccess",
        "",
        "portTestFailure",
        "forwardTestSuccess",
        "forwardTestFailure",
        "forwardTestUnavailable",
        "testPort",
        "port",
        "testForward"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'portTestSuccess'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'portTestFailure'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'forwardTestSuccess'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'forwardTestFailure'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'forwardTestUnavailable'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'testPort'
        QtMocHelpers::SlotData<void(const qint32)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 8 },
        }}),
        // Slot 'testForward'
        QtMocHelpers::SlotData<void(const qint32)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 8 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<PortAuthority, qt_meta_tag_ZN13PortAuthorityE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject PortAuthority::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13PortAuthorityE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13PortAuthorityE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13PortAuthorityE_t>.metaTypes,
    nullptr
} };

void PortAuthority::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<PortAuthority *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->portTestSuccess(); break;
        case 1: _t->portTestFailure(); break;
        case 2: _t->forwardTestSuccess(); break;
        case 3: _t->forwardTestFailure(); break;
        case 4: _t->forwardTestUnavailable(); break;
        case 5: _t->testPort((*reinterpret_cast< std::add_pointer_t<qint32>>(_a[1]))); break;
        case 6: _t->testForward((*reinterpret_cast< std::add_pointer_t<qint32>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (PortAuthority::*)()>(_a, &PortAuthority::portTestSuccess, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (PortAuthority::*)()>(_a, &PortAuthority::portTestFailure, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (PortAuthority::*)()>(_a, &PortAuthority::forwardTestSuccess, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (PortAuthority::*)()>(_a, &PortAuthority::forwardTestFailure, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (PortAuthority::*)()>(_a, &PortAuthority::forwardTestUnavailable, 4))
            return;
    }
}

const QMetaObject *PortAuthority::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PortAuthority::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13PortAuthorityE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int PortAuthority::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void PortAuthority::portTestSuccess()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void PortAuthority::portTestFailure()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void PortAuthority::forwardTestSuccess()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void PortAuthority::forwardTestFailure()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void PortAuthority::forwardTestUnavailable()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
