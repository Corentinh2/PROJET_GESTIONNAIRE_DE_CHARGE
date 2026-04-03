/****************************************************************************
** Meta object code from reading C++ file 'communicationesp.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../communicationesp.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'communicationesp.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN16CommunicationEspE_t {};
} // unnamed namespace

template <> constexpr inline auto CommunicationEsp::qt_create_metaobjectdata<qt_meta_tag_ZN16CommunicationEspE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "CommunicationEsp",
        "connectionStatusChanged",
        "",
        "status",
        "calendrierRecu",
        "id",
        "borne",
        "vehicule",
        "jours",
        "start",
        "end",
        "alerteRecue",
        "type",
        "message",
        "activeBorneChanged",
        "activeVehiculeChanged",
        "onConnected",
        "onDisconnected",
        "onTextMessageReceived",
        "obtenirCalendrier",
        "ajouterCalendrier",
        "supprimerCalendrier",
        "marcheForcee",
        "activer",
        "activeBorne",
        "activeVehicule"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'connectionStatusChanged'
        QtMocHelpers::SignalData<void(QString)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Signal 'calendrierRecu'
        QtMocHelpers::SignalData<void(int, QString, QString, QString, QString, QString)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 5 }, { QMetaType::QString, 6 }, { QMetaType::QString, 7 }, { QMetaType::QString, 8 },
            { QMetaType::QString, 9 }, { QMetaType::QString, 10 },
        }}),
        // Signal 'alerteRecue'
        QtMocHelpers::SignalData<void(QString, QString)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 12 }, { QMetaType::QString, 13 },
        }}),
        // Signal 'activeBorneChanged'
        QtMocHelpers::SignalData<void()>(14, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'activeVehiculeChanged'
        QtMocHelpers::SignalData<void()>(15, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onConnected'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onDisconnected'
        QtMocHelpers::SlotData<void()>(17, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onTextMessageReceived'
        QtMocHelpers::SlotData<void(const QString &)>(18, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 13 },
        }}),
        // Method 'obtenirCalendrier'
        QtMocHelpers::MethodData<void()>(19, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'ajouterCalendrier'
        QtMocHelpers::MethodData<void(const QString &, const QString &, const QString &)>(20, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 8 }, { QMetaType::QString, 9 }, { QMetaType::QString, 10 },
        }}),
        // Method 'supprimerCalendrier'
        QtMocHelpers::MethodData<void(int)>(21, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 5 },
        }}),
        // Method 'marcheForcee'
        QtMocHelpers::MethodData<void(bool)>(22, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 23 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'activeBorne'
        QtMocHelpers::PropertyData<QString>(24, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 3),
        // property 'activeVehicule'
        QtMocHelpers::PropertyData<QString>(25, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 4),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<CommunicationEsp, qt_meta_tag_ZN16CommunicationEspE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject CommunicationEsp::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16CommunicationEspE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16CommunicationEspE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN16CommunicationEspE_t>.metaTypes,
    nullptr
} };

void CommunicationEsp::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<CommunicationEsp *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->connectionStatusChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->calendrierRecu((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[5])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[6]))); break;
        case 2: _t->alerteRecue((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 3: _t->activeBorneChanged(); break;
        case 4: _t->activeVehiculeChanged(); break;
        case 5: _t->onConnected(); break;
        case 6: _t->onDisconnected(); break;
        case 7: _t->onTextMessageReceived((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 8: _t->obtenirCalendrier(); break;
        case 9: _t->ajouterCalendrier((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3]))); break;
        case 10: _t->supprimerCalendrier((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 11: _t->marcheForcee((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (CommunicationEsp::*)(QString )>(_a, &CommunicationEsp::connectionStatusChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (CommunicationEsp::*)(int , QString , QString , QString , QString , QString )>(_a, &CommunicationEsp::calendrierRecu, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (CommunicationEsp::*)(QString , QString )>(_a, &CommunicationEsp::alerteRecue, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (CommunicationEsp::*)()>(_a, &CommunicationEsp::activeBorneChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (CommunicationEsp::*)()>(_a, &CommunicationEsp::activeVehiculeChanged, 4))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QString*>(_v) = _t->activeBorne(); break;
        case 1: *reinterpret_cast<QString*>(_v) = _t->activeVehicule(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setActiveBorne(*reinterpret_cast<QString*>(_v)); break;
        case 1: _t->setActiveVehicule(*reinterpret_cast<QString*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *CommunicationEsp::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CommunicationEsp::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16CommunicationEspE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CommunicationEsp::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 12;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void CommunicationEsp::connectionStatusChanged(QString _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void CommunicationEsp::calendrierRecu(int _t1, QString _t2, QString _t3, QString _t4, QString _t5, QString _t6)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2, _t3, _t4, _t5, _t6);
}

// SIGNAL 2
void CommunicationEsp::alerteRecue(QString _t1, QString _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2);
}

// SIGNAL 3
void CommunicationEsp::activeBorneChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void CommunicationEsp::activeVehiculeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
