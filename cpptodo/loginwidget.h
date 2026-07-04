#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QPainter>
#include <QLinearGradient>
#include <QRadialGradient>

// ─────────────────────────────────────────────────────────
//  AuthLineEdit  — styled input field
// ─────────────────────────────────────────────────────────
class AuthLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit AuthLineEdit(const QString &placeholder,
                          bool isPassword = false,
                          QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *) override;
    void focusInEvent(QFocusEvent *)  override;
    void focusOutEvent(QFocusEvent *) override;

private:
    bool m_focused = false;
};

// ─────────────────────────────────────────────────────────
//  AuthButton  — primary action button
// ─────────────────────────────────────────────────────────
class AuthButton : public QPushButton
{
    Q_OBJECT
public:
    explicit AuthButton(const QString &text,
                        QColor accent,
                        QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *) override;
    void enterEvent(QEnterEvent *) override;
    void leaveEvent(QEvent *)      override;

private:
    QColor m_accent;
    bool   m_hovered = false;
};

// ─────────────────────────────────────────────────────────
//  LoginWidget  — full auth page (Sign In / Sign Up / Reset)
// ─────────────────────────────────────────────────────────
class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = nullptr);

signals:
    void loginSuccess();   // emitted after successful login or register
    void goBack();         // back to timer

protected:
    void paintEvent(QPaintEvent *) override;

private slots:
    void onSignIn();
    void onSignUp();
    void onResetPassword();
    void showSignIn();
    void showSignUp();
    void showReset();

private:
    QWidget *buildSignInPage();
    QWidget *buildSignUpPage();
    QWidget *buildResetPage();
    QWidget *buildHeader();

    void showError(QLabel *lbl, const QString &msg);
    void showSuccess(QLabel *lbl, const QString &msg);
    void clearMessage(QLabel *lbl);

    QStackedWidget *m_stack;

    // Sign In
    AuthLineEdit *m_siEmailInput;
    AuthLineEdit *m_siPassInput;
    QLabel       *m_siMsg;

    // Sign Up
    AuthLineEdit *m_suUsernameInput;
    AuthLineEdit *m_suEmailInput;
    AuthLineEdit *m_suPassInput;
    AuthLineEdit *m_suConfirmInput;
    QLabel       *m_suMsg;

    // Reset
    AuthLineEdit *m_rstEmailInput;
    AuthLineEdit *m_rstNewPassInput;
    AuthLineEdit *m_rstConfirmInput;
    QLabel       *m_rstMsg;
    float         m_animTime = 0.f;
};

#endif // LOGINWIDGET_H