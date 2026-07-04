/****************************************************************************
** Meta object code from reading C++ file 'assistantwidget.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../assistantwidget.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'assistantwidget.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN18AssistantCharacterE_t {};
} // unnamed namespace

template <> constexpr inline auto AssistantCharacter::qt_create_metaobjectdata<qt_meta_tag_ZN18AssistantCharacterE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "AssistantCharacter",
        "bounce",
        "armAngle"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
        // property 'bounce'
        QtMocHelpers::PropertyData<float>(1, QMetaType::Float, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'armAngle'
        QtMocHelpers::PropertyData<float>(2, QMetaType::Float, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<AssistantCharacter, qt_meta_tag_ZN18AssistantCharacterE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject AssistantCharacter::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN18AssistantCharacterE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN18AssistantCharacterE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN18AssistantCharacterE_t>.metaTypes,
    nullptr
} };

void AssistantCharacter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<AssistantCharacter *>(_o);
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<float*>(_v) = _t->bounce(); break;
        case 1: *reinterpret_cast<float*>(_v) = _t->armAngle(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setBounce(*reinterpret_cast<float*>(_v)); break;
        case 1: _t->setArmAngle(*reinterpret_cast<float*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *AssistantCharacter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AssistantCharacter::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN18AssistantCharacterE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int AssistantCharacter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
namespace {
struct qt_meta_tag_ZN12SpeechBubbleE_t {};
} // unnamed namespace

template <> constexpr inline auto SpeechBubble::qt_create_metaobjectdata<qt_meta_tag_ZN12SpeechBubbleE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "SpeechBubble"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<SpeechBubble, qt_meta_tag_ZN12SpeechBubbleE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject SpeechBubble::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12SpeechBubbleE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12SpeechBubbleE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN12SpeechBubbleE_t>.metaTypes,
    nullptr
} };

void SpeechBubble::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<SpeechBubble *>(_o);
    (void)_t;
    (void)_c;
    (void)_id;
    (void)_a;
}

const QMetaObject *SpeechBubble::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SpeechBubble::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12SpeechBubbleE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int SpeechBubble::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    return _id;
}
namespace {
struct qt_meta_tag_ZN15AssistantWidgetE_t {};
} // unnamed namespace

template <> constexpr inline auto AssistantWidget::qt_create_metaobjectdata<qt_meta_tag_ZN15AssistantWidgetE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "AssistantWidget",
        "onAnimTick",
        "",
        "onToggleVisibility"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'onAnimTick'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onToggleVisibility'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<AssistantWidget, qt_meta_tag_ZN15AssistantWidgetE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject AssistantWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15AssistantWidgetE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15AssistantWidgetE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN15AssistantWidgetE_t>.metaTypes,
    nullptr
} };

void AssistantWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<AssistantWidget *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->onAnimTick(); break;
        case 1: _t->onToggleVisibility(); break;
        default: ;
        }
    }
    (void)_a;
}

const QMetaObject *AssistantWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AssistantWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15AssistantWidgetE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int AssistantWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }
    return _id;
}
QT_WARNING_POP
