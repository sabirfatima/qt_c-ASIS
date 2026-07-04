/****************************************************************************
** Meta object code from reading C++ file 'todowidget.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../todowidget.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'todowidget.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN16DatePickerWidgetE_t {};
} // unnamed namespace

template <> constexpr inline auto DatePickerWidget::qt_create_metaobjectdata<qt_meta_tag_ZN16DatePickerWidgetE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "DatePickerWidget",
        "dateSelected",
        "",
        "date"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'dateSelected'
        QtMocHelpers::SignalData<void(QDate)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QDate, 3 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<DatePickerWidget, qt_meta_tag_ZN16DatePickerWidgetE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject DatePickerWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16DatePickerWidgetE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16DatePickerWidgetE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN16DatePickerWidgetE_t>.metaTypes,
    nullptr
} };

void DatePickerWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<DatePickerWidget *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->dateSelected((*reinterpret_cast<std::add_pointer_t<QDate>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (DatePickerWidget::*)(QDate )>(_a, &DatePickerWidget::dateSelected, 0))
            return;
    }
}

const QMetaObject *DatePickerWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DatePickerWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16DatePickerWidgetE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int DatePickerWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void DatePickerWidget::dateSelected(QDate _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}
namespace {
struct qt_meta_tag_ZN10DiffButtonE_t {};
} // unnamed namespace

template <> constexpr inline auto DiffButton::qt_create_metaobjectdata<qt_meta_tag_ZN10DiffButtonE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "DiffButton"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<DiffButton, qt_meta_tag_ZN10DiffButtonE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject DiffButton::staticMetaObject = { {
    QMetaObject::SuperData::link<QPushButton::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10DiffButtonE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10DiffButtonE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10DiffButtonE_t>.metaTypes,
    nullptr
} };

void DiffButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<DiffButton *>(_o);
    (void)_t;
    (void)_c;
    (void)_id;
    (void)_a;
}

const QMetaObject *DiffButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DiffButton::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10DiffButtonE_t>.strings))
        return static_cast<void*>(this);
    return QPushButton::qt_metacast(_clname);
}

int DiffButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPushButton::qt_metacall(_c, _id, _a);
    return _id;
}
namespace {
struct qt_meta_tag_ZN8TaskCardE_t {};
} // unnamed namespace

template <> constexpr inline auto TaskCard::qt_create_metaobjectdata<qt_meta_tag_ZN8TaskCardE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "TaskCard",
        "toggleCompleted",
        "",
        "index",
        "deleteTask",
        "startTask"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'toggleCompleted'
        QtMocHelpers::SignalData<void(int)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Signal 'deleteTask'
        QtMocHelpers::SignalData<void(int)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Signal 'startTask'
        QtMocHelpers::SignalData<void(int)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<TaskCard, qt_meta_tag_ZN8TaskCardE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject TaskCard::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8TaskCardE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8TaskCardE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN8TaskCardE_t>.metaTypes,
    nullptr
} };

void TaskCard::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<TaskCard *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->toggleCompleted((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->deleteTask((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->startTask((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (TaskCard::*)(int )>(_a, &TaskCard::toggleCompleted, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (TaskCard::*)(int )>(_a, &TaskCard::deleteTask, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (TaskCard::*)(int )>(_a, &TaskCard::startTask, 2))
            return;
    }
}

const QMetaObject *TaskCard::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TaskCard::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8TaskCardE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int TaskCard::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void TaskCard::toggleCompleted(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void TaskCard::deleteTask(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void TaskCard::startTask(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}
namespace {
struct qt_meta_tag_ZN10TodoWidgetE_t {};
} // unnamed namespace

template <> constexpr inline auto TodoWidget::qt_create_metaobjectdata<qt_meta_tag_ZN10TodoWidgetE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "TodoWidget",
        "goToTimer",
        "",
        "goToLogin",
        "goToPlanning",
        "startTaskInTimer",
        "name",
        "durationMin",
        "onAddTask",
        "onToggleCompleted",
        "index",
        "onDeleteTask",
        "onStartTask",
        "selectDifficulty",
        "d",
        "toggleCalendar"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'goToTimer'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'goToLogin'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'goToPlanning'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'startTaskInTimer'
        QtMocHelpers::SignalData<void(const QString &, int)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 6 }, { QMetaType::Int, 7 },
        }}),
        // Slot 'onAddTask'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onToggleCompleted'
        QtMocHelpers::SlotData<void(int)>(9, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 10 },
        }}),
        // Slot 'onDeleteTask'
        QtMocHelpers::SlotData<void(int)>(11, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 10 },
        }}),
        // Slot 'onStartTask'
        QtMocHelpers::SlotData<void(int)>(12, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 10 },
        }}),
        // Slot 'selectDifficulty'
        QtMocHelpers::SlotData<void(int)>(13, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 14 },
        }}),
        // Slot 'toggleCalendar'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<TodoWidget, qt_meta_tag_ZN10TodoWidgetE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject TodoWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10TodoWidgetE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10TodoWidgetE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10TodoWidgetE_t>.metaTypes,
    nullptr
} };

void TodoWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<TodoWidget *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->goToTimer(); break;
        case 1: _t->goToLogin(); break;
        case 2: _t->goToPlanning(); break;
        case 3: _t->startTaskInTimer((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 4: _t->onAddTask(); break;
        case 5: _t->onToggleCompleted((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->onDeleteTask((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 7: _t->onStartTask((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 8: _t->selectDifficulty((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 9: _t->toggleCalendar(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (TodoWidget::*)()>(_a, &TodoWidget::goToTimer, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (TodoWidget::*)()>(_a, &TodoWidget::goToLogin, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (TodoWidget::*)()>(_a, &TodoWidget::goToPlanning, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (TodoWidget::*)(const QString & , int )>(_a, &TodoWidget::startTaskInTimer, 3))
            return;
    }
}

const QMetaObject *TodoWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TodoWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10TodoWidgetE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int TodoWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void TodoWidget::goToTimer()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void TodoWidget::goToLogin()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void TodoWidget::goToPlanning()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void TodoWidget::startTaskInTimer(const QString & _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1, _t2);
}
QT_WARNING_POP
