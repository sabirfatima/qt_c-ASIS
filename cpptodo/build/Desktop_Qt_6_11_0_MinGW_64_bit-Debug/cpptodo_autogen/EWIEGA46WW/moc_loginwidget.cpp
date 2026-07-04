/****************************************************************************
** Meta object code from reading C++ file 'loginwidget.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../loginwidget.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'loginwidget.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN12AuthLineEditE_t {};
} // unnamed namespace

template <> constexpr inline auto AuthLineEdit::qt_create_metaobjectdata<qt_meta_tag_ZN12AuthLineEditE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "AuthLineEdit"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<AuthLineEdit, qt_meta_tag_ZN12AuthLineEditE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject AuthLineEdit::staticMetaObject = { {
    QMetaObject::SuperData::link<QLineEdit::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12AuthLineEditE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12AuthLineEditE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN12AuthLineEditE_t>.metaTypes,
    nullptr
} };

void AuthLineEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<AuthLineEdit *>(_o);
    (void)_t;
    (void)_c;
    (void)_id;
    (void)_a;
}

const QMetaObject *AuthLineEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AuthLineEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12AuthLineEditE_t>.strings))
        return static_cast<void*>(this);
    return QLineEdit::qt_metacast(_clname);
}

int AuthLineEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLineEdit::qt_metacall(_c, _id, _a);
    return _id;
}
namespace {
struct qt_meta_tag_ZN10AuthButtonE_t {};
} // unnamed namespace

template <> constexpr inline auto AuthButton::qt_create_metaobjectdata<qt_meta_tag_ZN10AuthButtonE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "AuthButton"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<AuthButton, qt_meta_tag_ZN10AuthButtonE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject AuthButton::staticMetaObject = { {
    QMetaObject::SuperData::link<QPushButton::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10AuthButtonE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10AuthButtonE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10AuthButtonE_t>.metaTypes,
    nullptr
} };

void AuthButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<AuthButton *>(_o);
    (void)_t;
    (void)_c;
    (void)_id;
    (void)_a;
}

const QMetaObject *AuthButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AuthButton::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10AuthButtonE_t>.strings))
        return static_cast<void*>(this);
    return QPushButton::qt_metacast(_clname);
}

int AuthButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPushButton::qt_metacall(_c, _id, _a);
    return _id;
}
namespace {
struct qt_meta_tag_ZN11LoginWidgetE_t {};
} // unnamed namespace

template <> constexpr inline auto LoginWidget::qt_create_metaobjectdata<qt_meta_tag_ZN11LoginWidgetE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "LoginWidget",
        "loginSuccess",
        "",
        "goBack",
        "onSignIn",
        "onSignUp",
        "onResetPassword",
        "showSignIn",
        "showSignUp",
        "showReset"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'loginSuccess'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'goBack'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onSignIn'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onSignUp'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onResetPassword'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'showSignIn'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'showSignUp'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'showReset'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<LoginWidget, qt_meta_tag_ZN11LoginWidgetE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject LoginWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11LoginWidgetE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11LoginWidgetE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN11LoginWidgetE_t>.metaTypes,
    nullptr
} };

void LoginWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<LoginWidget *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->loginSuccess(); break;
        case 1: _t->goBack(); break;
        case 2: _t->onSignIn(); break;
        case 3: _t->onSignUp(); break;
        case 4: _t->onResetPassword(); break;
        case 5: _t->showSignIn(); break;
        case 6: _t->showSignUp(); break;
        case 7: _t->showReset(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (LoginWidget::*)()>(_a, &LoginWidget::loginSuccess, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (LoginWidget::*)()>(_a, &LoginWidget::goBack, 1))
            return;
    }
}

const QMetaObject *LoginWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LoginWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11LoginWidgetE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int LoginWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void LoginWidget::loginSuccess()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void LoginWidget::goBack()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
