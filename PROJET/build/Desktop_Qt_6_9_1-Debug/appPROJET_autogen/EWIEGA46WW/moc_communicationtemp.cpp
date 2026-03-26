/****************************************************************************
** Meta object code from reading C++ file 'communicationtemp.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../communicationtemp.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'communicationtemp.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN17CommunicationTempE_t {};
} // unnamed namespace

template <> constexpr inline auto CommunicationTemp::qt_create_metaobjectdata<qt_meta_tag_ZN17CommunicationTempE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "CommunicationTemp",
        "connectionStatusChanged",
        "",
        "status",
        "stationReceived",
        "name",
        "kwh",
        "vehicleReceived",
        "km",
        "sessionReceived",
        "borne",
        "vehicule",
        "jours",
        "start",
        "end",
        "onConnected",
        "onBinaryMessageReceived",
        "message",
        "requestStationsList",
        "requestVehiclesList",
        "addVehicleToDb",
        "requestSessionsHistory",
        "programmerCharge"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'connectionStatusChanged'
        QtMocHelpers::SignalData<void(QString)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Signal 'stationReceived'
        QtMocHelpers::SignalData<void(QString, QString, QString)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 5 }, { QMetaType::QString, 6 }, { QMetaType::QString, 3 },
        }}),
        // Signal 'vehicleReceived'
        QtMocHelpers::SignalData<void(QString, QString)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 5 }, { QMetaType::QString, 8 },
        }}),
        // Signal 'sessionReceived'
        QtMocHelpers::SignalData<void(QString, QString, QString, QString, QString)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 10 }, { QMetaType::QString, 11 }, { QMetaType::QString, 12 }, { QMetaType::QString, 13 },
            { QMetaType::QString, 14 },
        }}),
        // Slot 'onConnected'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onBinaryMessageReceived'
        QtMocHelpers::SlotData<void(QByteArray)>(16, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QByteArray, 17 },
        }}),
        // Method 'requestStationsList'
        QtMocHelpers::MethodData<void()>(18, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'requestVehiclesList'
        QtMocHelpers::MethodData<void()>(19, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'addVehicleToDb'
        QtMocHelpers::MethodData<void(const QString &, const QString &)>(20, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 5 }, { QMetaType::QString, 8 },
        }}),
        // Method 'requestSessionsHistory'
        QtMocHelpers::MethodData<void()>(21, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'programmerCharge'
        QtMocHelpers::MethodData<void(const QString &, const QString &, const QString &, const QString &, const QString &)>(22, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 10 }, { QMetaType::QString, 11 }, { QMetaType::QString, 12 }, { QMetaType::QString, 13 },
            { QMetaType::QString, 14 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<CommunicationTemp, qt_meta_tag_ZN17CommunicationTempE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject CommunicationTemp::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17CommunicationTempE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17CommunicationTempE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN17CommunicationTempE_t>.metaTypes,
    nullptr
} };

void CommunicationTemp::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<CommunicationTemp *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->connectionStatusChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->stationReceived((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3]))); break;
        case 2: _t->vehicleReceived((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 3: _t->sessionReceived((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[5]))); break;
        case 4: _t->onConnected(); break;
        case 5: _t->onBinaryMessageReceived((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1]))); break;
        case 6: _t->requestStationsList(); break;
        case 7: _t->requestVehiclesList(); break;
        case 8: _t->addVehicleToDb((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 9: _t->requestSessionsHistory(); break;
        case 10: _t->programmerCharge((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[5]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (CommunicationTemp::*)(QString )>(_a, &CommunicationTemp::connectionStatusChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (CommunicationTemp::*)(QString , QString , QString )>(_a, &CommunicationTemp::stationReceived, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (CommunicationTemp::*)(QString , QString )>(_a, &CommunicationTemp::vehicleReceived, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (CommunicationTemp::*)(QString , QString , QString , QString , QString )>(_a, &CommunicationTemp::sessionReceived, 3))
            return;
    }
}

const QMetaObject *CommunicationTemp::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CommunicationTemp::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17CommunicationTempE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CommunicationTemp::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void CommunicationTemp::connectionStatusChanged(QString _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void CommunicationTemp::stationReceived(QString _t1, QString _t2, QString _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2, _t3);
}

// SIGNAL 2
void CommunicationTemp::vehicleReceived(QString _t1, QString _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2);
}

// SIGNAL 3
void CommunicationTemp::sessionReceived(QString _t1, QString _t2, QString _t3, QString _t4, QString _t5)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1, _t2, _t3, _t4, _t5);
}
QT_WARNING_POP
