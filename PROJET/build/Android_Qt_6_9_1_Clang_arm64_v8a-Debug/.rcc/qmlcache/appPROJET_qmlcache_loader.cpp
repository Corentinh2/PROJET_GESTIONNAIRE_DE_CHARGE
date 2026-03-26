#include <QtQml/qqmlprivate.h>
#include <QtCore/qdir.h>
#include <QtCore/qurl.h>
#include <QtCore/qhash.h>
#include <QtCore/qstring.h>

namespace QmlCacheGeneratedCode {
namespace _qt_qml_PROJET_Main_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_qml_PROJET_logique_js { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_qml_PROJET_AppButton_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_qml_PROJET_AppTextField_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_qml_PROJET_AppCard_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_qml_PROJET_VehiculesPage_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_qml_PROJET_BootPage_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_qml_PROJET_StatusPage_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_qml_PROJET_DashboardPage_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_qml_PROJET_SettingsPage_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_qml_PROJET_SessionsPage_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_qml_PROJET_KmPage_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}

}
namespace {
struct Registry {
    Registry();
    ~Registry();
    QHash<QString, const QQmlPrivate::CachedQmlUnit*> resourcePathToCachedUnit;
    static const QQmlPrivate::CachedQmlUnit *lookupCachedUnit(const QUrl &url);
};

Q_GLOBAL_STATIC(Registry, unitRegistry)


Registry::Registry() {
    resourcePathToCachedUnit.insert(QStringLiteral("/qt/qml/PROJET/Main.qml"), &QmlCacheGeneratedCode::_qt_qml_PROJET_Main_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt/qml/PROJET/logique.js"), &QmlCacheGeneratedCode::_qt_qml_PROJET_logique_js::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt/qml/PROJET/AppButton.qml"), &QmlCacheGeneratedCode::_qt_qml_PROJET_AppButton_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt/qml/PROJET/AppTextField.qml"), &QmlCacheGeneratedCode::_qt_qml_PROJET_AppTextField_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt/qml/PROJET/AppCard.qml"), &QmlCacheGeneratedCode::_qt_qml_PROJET_AppCard_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt/qml/PROJET/VehiculesPage.qml"), &QmlCacheGeneratedCode::_qt_qml_PROJET_VehiculesPage_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt/qml/PROJET/BootPage.qml"), &QmlCacheGeneratedCode::_qt_qml_PROJET_BootPage_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt/qml/PROJET/StatusPage.qml"), &QmlCacheGeneratedCode::_qt_qml_PROJET_StatusPage_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt/qml/PROJET/DashboardPage.qml"), &QmlCacheGeneratedCode::_qt_qml_PROJET_DashboardPage_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt/qml/PROJET/SettingsPage.qml"), &QmlCacheGeneratedCode::_qt_qml_PROJET_SettingsPage_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt/qml/PROJET/SessionsPage.qml"), &QmlCacheGeneratedCode::_qt_qml_PROJET_SessionsPage_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt/qml/PROJET/KmPage.qml"), &QmlCacheGeneratedCode::_qt_qml_PROJET_KmPage_qml::unit);
    QQmlPrivate::RegisterQmlUnitCacheHook registration;
    registration.structVersion = 0;
    registration.lookupCachedQmlUnit = &lookupCachedUnit;
    QQmlPrivate::qmlregister(QQmlPrivate::QmlUnitCacheHookRegistration, &registration);
}

Registry::~Registry() {
    QQmlPrivate::qmlunregister(QQmlPrivate::QmlUnitCacheHookRegistration, quintptr(&lookupCachedUnit));
}

const QQmlPrivate::CachedQmlUnit *Registry::lookupCachedUnit(const QUrl &url) {
    if (url.scheme() != QLatin1String("qrc"))
        return nullptr;
    QString resourcePath = QDir::cleanPath(url.path());
    if (resourcePath.isEmpty())
        return nullptr;
    if (!resourcePath.startsWith(QLatin1Char('/')))
        resourcePath.prepend(QLatin1Char('/'));
    return unitRegistry()->resourcePathToCachedUnit.value(resourcePath, nullptr);
}
}
int QT_MANGLE_NAMESPACE(qInitResources_qmlcache_appPROJET)() {
    ::unitRegistry();
    return 1;
}
Q_CONSTRUCTOR_FUNCTION(QT_MANGLE_NAMESPACE(qInitResources_qmlcache_appPROJET))
int QT_MANGLE_NAMESPACE(qCleanupResources_qmlcache_appPROJET)() {
    return 1;
}
