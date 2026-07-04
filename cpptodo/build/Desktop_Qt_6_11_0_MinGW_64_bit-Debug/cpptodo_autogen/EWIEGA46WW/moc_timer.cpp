/****************************************************************************
** Meta object code from reading C++ file 'timer.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../timer.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'timer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.0. It"
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
struct qt_meta_tag_ZN16BackgroundWidgetE_t {};
} // unnamed namespace

template <> constexpr inline auto BackgroundWidget::qt_create_metaobjectdata<qt_meta_tag_ZN16BackgroundWidgetE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "BackgroundWidget"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<BackgroundWidget, qt_meta_tag_ZN16BackgroundWidgetE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject BackgroundWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16BackgroundWidgetE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16BackgroundWidgetE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN16BackgroundWidgetE_t>.metaTypes,
    nullptr
} };

void BackgroundWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<BackgroundWidget *>(_o);
    (void)_t;
    (void)_c;
    (void)_id;
    (void)_a;
}

const QMetaObject *BackgroundWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BackgroundWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16BackgroundWidgetE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int BackgroundWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    return _id;
}
namespace {
struct qt_meta_tag_ZN10ModeButtonE_t {};
} // unnamed namespace

template <> constexpr inline auto ModeButton::qt_create_metaobjectdata<qt_meta_tag_ZN10ModeButtonE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ModeButton"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ModeButton, qt_meta_tag_ZN10ModeButtonE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ModeButton::staticMetaObject = { {
    QMetaObject::SuperData::link<QPushButton::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10ModeButtonE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10ModeButtonE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10ModeButtonE_t>.metaTypes,
    nullptr
} };

void ModeButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ModeButton *>(_o);
    (void)_t;
    (void)_c;
    (void)_id;
    (void)_a;
}

const QMetaObject *ModeButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ModeButton::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10ModeButtonE_t>.strings))
        return static_cast<void*>(this);
    return QPushButton::qt_metacast(_clname);
}

int ModeButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPushButton::qt_metacall(_c, _id, _a);
    return _id;
}
namespace {
struct qt_meta_tag_ZN13ControlButtonE_t {};
} // unnamed namespace

template <> constexpr inline auto ControlButton::qt_create_metaobjectdata<qt_meta_tag_ZN13ControlButtonE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ControlButton"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ControlButton, qt_meta_tag_ZN13ControlButtonE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ControlButton::staticMetaObject = { {
    QMetaObject::SuperData::link<QPushButton::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13ControlButtonE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13ControlButtonE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13ControlButtonE_t>.metaTypes,
    nullptr
} };

void ControlButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ControlButton *>(_o);
    (void)_t;
    (void)_c;
    (void)_id;
    (void)_a;
}

const QMetaObject *ControlButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ControlButton::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13ControlButtonE_t>.strings))
        return static_cast<void*>(this);
    return QPushButton::qt_metacast(_clname);
}

int ControlButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPushButton::qt_metacall(_c, _id, _a);
    return _id;
}
namespace {
struct qt_meta_tag_ZN10ArcDisplayE_t {};
} // unnamed namespace

template <> constexpr inline auto ArcDisplay::qt_create_metaobjectdata<qt_meta_tag_ZN10ArcDisplayE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ArcDisplay"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ArcDisplay, qt_meta_tag_ZN10ArcDisplayE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ArcDisplay::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10ArcDisplayE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10ArcDisplayE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10ArcDisplayE_t>.metaTypes,
    nullptr
} };

void ArcDisplay::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ArcDisplay *>(_o);
    (void)_t;
    (void)_c;
    (void)_id;
    (void)_a;
}

const QMetaObject *ArcDisplay::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ArcDisplay::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10ArcDisplayE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ArcDisplay::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    return _id;
}
namespace {
struct qt_meta_tag_ZN11TimerWidgetE_t {};
} // unnamed namespace

template <> constexpr inline auto TimerWidget::qt_create_metaobjectdata<qt_meta_tag_ZN11TimerWidgetE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "TimerWidget",
        "goToLogin",
        "",
        "goToTodoList",
        "goToStats",
        "taskStarted",
        "name",
        "durationMin",
        "onTick",
        "onStart",
        "onPause",
        "onReset",
        "selectMode",
        "id"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'goToLogin'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'goToTodoList'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'goToStats'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'taskStarted'
        QtMocHelpers::SignalData<void(const QString &, int)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 6 }, { QMetaType::Int, 7 },
        }}),
        // Slot 'onTick'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onStart'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onPause'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onReset'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'selectMode'
        QtMocHelpers::SlotData<void(int)>(12, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 13 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<TimerWidget, qt_meta_tag_ZN11TimerWidgetE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject TimerWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11TimerWidgetE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11TimerWidgetE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN11TimerWidgetE_t>.metaTypes,
    nullptr
} };

void TimerWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<TimerWidget *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->goToLogin(); break;
        case 1: _t->goToTodoList(); break;
        case 2: _t->goToStats(); break;
        case 3: _t->taskStarted((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 4: _t->onTick(); break;
        case 5: _t->onStart(); break;
        case 6: _t->onPause(); break;
        case 7: _t->onReset(); break;
        case 8: _t->selectMode((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (TimerWidget::*)()>(_a, &TimerWidget::goToLogin, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (TimerWidget::*)()>(_a, &TimerWidget::goToTodoList, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (TimerWidget::*)()>(_a, &TimerWidget::goToStats, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (TimerWidget::*)(const QString & , int )>(_a, &TimerWidget::taskStarted, 3))
            return;
    }
}

const QMetaObject *TimerWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TimerWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11TimerWidgetE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int TimerWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void TimerWidget::goToLogin()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void TimerWidget::goToTodoList()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void TimerWidget::goToStats()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void TimerWidget::taskStarted(const QString & _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1, _t2);
}
QT_WARNING_POP
