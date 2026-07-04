#include "loginwidget.h"
#include "authmanager.h"
#include <QTimer>
#include <QPainterPath>

// ════════════════════════════════════════════════════════
//  Colours
// ════════════════════════════════════════════════════════
static const QColor C_CYAN   (0,   200, 255);
static const QColor C_GREEN  (60,  220, 100);
static const QColor C_AMBER  (255, 180,   0);
static const QColor C_RED    (255,  80,  80);

// ════════════════════════════════════════════════════════
//  AuthLineEdit
// ════════════════════════════════════════════════════════
AuthLineEdit::AuthLineEdit(const QString &placeholder,
                           bool isPassword,
                           QWidget *parent)
    : QLineEdit(parent)
{
    setPlaceholderText(placeholder);
    setFixedHeight(46);
    if (isPassword) setEchoMode(QLineEdit::Password);

    setStyleSheet(
        "QLineEdit {"
        "  background: transparent;"
        "  color: rgb(180,220,255);"
        "  border: none;"
        "  padding: 0 14px;"
        "  font-family: 'Courier New';"
        "  font-size: 13px;"
        "  selection-background-color: rgba(0,140,220,0.5);"
        "}"
        );
}

void AuthLineEdit::focusInEvent(QFocusEvent *e)
{
    m_focused = true;
    update();
    QLineEdit::focusInEvent(e);
}

void AuthLineEdit::focusOutEvent(QFocusEvent *e)
{
    m_focused = false;
    update();
    QLineEdit::focusOutEvent(e);
}

void AuthLineEdit::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QRectF r = QRectF(rect()).adjusted(1, 1, -1, -1);

    // Background
    p.setBrush(QColor(4, 14, 40, 230));
    p.setPen(Qt::NoPen);
    p.drawRoundedRect(r, 8, 8);

    // Border
    QColor border = m_focused
                        ? QColor(0, 200, 255, 200)
                        : QColor(0, 90, 150, 120);
    p.setPen(QPen(border, m_focused ? 1.5 : 1.0));
    p.setBrush(Qt::NoBrush);
    p.drawRoundedRect(r, 8, 8);

    // Bottom glow when focused
    if (m_focused) {
        QLinearGradient gl(r.left(), r.bottom() - 2, r.right(), r.bottom());
        gl.setColorAt(0, QColor(0, 0, 0, 0));
        gl.setColorAt(0.5, QColor(0, 200, 255, 80));
        gl.setColorAt(1, QColor(0, 0, 0, 0));
        p.setPen(QPen(QBrush(gl), 2));
        p.drawLine(QPointF(r.left() + 8, r.bottom()),
                   QPointF(r.right() - 8, r.bottom()));
    }

    // Let Qt draw the actual text / placeholder
    QLineEdit::paintEvent(e);
}

// ════════════════════════════════════════════════════════
//  AuthButton
// ════════════════════════════════════════════════════════
AuthButton::AuthButton(const QString &text, QColor accent, QWidget *parent)
    : QPushButton(text, parent), m_accent(accent)
{
    setFixedHeight(46);
    setCursor(Qt::PointingHandCursor);
}

void AuthButton::enterEvent(QEnterEvent *) { m_hovered = true;  update(); }
void AuthButton::leaveEvent(QEvent *)      { m_hovered = false; update(); }

void AuthButton::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QRectF r = QRectF(rect()).adjusted(1, 1, -1, -1);

    // Fill
    QColor fill = m_hovered
                      ? QColor(m_accent.red(), m_accent.green(), m_accent.blue(), 50)
                      : QColor(m_accent.red(), m_accent.green(), m_accent.blue(), 25);
    p.setBrush(fill);
    p.setPen(Qt::NoPen);
    p.drawRoundedRect(r, 10, 10);

    // Border
    p.setPen(QPen(m_accent, m_hovered ? 1.8 : 1.2));
    p.setBrush(Qt::NoBrush);
    p.drawRoundedRect(r, 10, 10);

    // Top shimmer
    QLinearGradient sh(r.topLeft(), QPointF(r.left(), r.top() + r.height() * 0.5));
    sh.setColorAt(0, QColor(255, 255, 255, 20));
    sh.setColorAt(1, QColor(255, 255, 255, 0));
    p.setBrush(sh);
    p.setPen(Qt::NoPen);
    p.drawRoundedRect(r, 10, 10);

    // Text
    QFont f("Courier New", 11, QFont::Bold);
    f.setLetterSpacing(QFont::AbsoluteSpacing, 3);
    p.setFont(f);
    p.setPen(m_hovered ? Qt::white : m_accent);
    p.drawText(r, Qt::AlignCenter, text());
}

// ════════════════════════════════════════════════════════
//  Helper: show message labels
// ════════════════════════════════════════════════════════
void LoginWidget::showError(QLabel *lbl, const QString &msg)
{
    lbl->setText("⚠  " + msg);
    lbl->setStyleSheet(
        "color: rgb(255,80,80); font-family:'Courier New';"
        "font-size:11px; background:transparent; padding:4px 0;"
        );
}

void LoginWidget::showSuccess(QLabel *lbl, const QString &msg)
{
    lbl->setText("✓  " + msg);
    lbl->setStyleSheet(
        "color: rgb(60,220,100); font-family:'Courier New';"
        "font-size:11px; background:transparent; padding:4px 0;"
        );
}

void LoginWidget::clearMessage(QLabel *lbl)
{
    lbl->clear();
}

// ════════════════════════════════════════════════════════
//  Tab link button helper (local lambda style)
// ════════════════════════════════════════════════════════
static QPushButton *linkBtn(const QString &text, QWidget *parent)
{
    QPushButton *b = new QPushButton(text, parent);
    b->setFlat(true);
    b->setCursor(Qt::PointingHandCursor);
    b->setStyleSheet(
        "QPushButton {"
        "  color: rgba(0,180,255,0.75);"
        "  font-family:'Courier New'; font-size:11px;"
        "  background:transparent; border:none;"
        "  text-decoration: underline;"
        "}"
        "QPushButton:hover { color: rgb(0,220,255); }"
        );
    return b;
}

static QLabel *makeLabel(const QString &t, int size = 11,
                         bool dim = false, QWidget *p = nullptr)
{
    QLabel *l = new QLabel(t, p);
    QString col = dim ? "rgba(80,130,190,0.8)" : "rgba(130,190,240,0.9)";
    l->setStyleSheet(QString(
                         "color:%1; font-family:'Courier New'; font-size:%2px;"
                         "letter-spacing:2px; background:transparent;"
                         ).arg(col).arg(size));
    return l;
}

// ════════════════════════════════════════════════════════
//  buildHeader
// ════════════════════════════════════════════════════════
QWidget *LoginWidget::buildHeader()
{
    QWidget *w = new QWidget(this);
    w->setFixedHeight(60);
    w->setStyleSheet(
        "background: rgba(6,14,38,0.95);"
        "border-bottom: 1px solid rgba(0,120,200,0.30);"
        );
    QHBoxLayout *lay = new QHBoxLayout(w);
    lay->setContentsMargins(24, 0, 24, 0);

    QLabel *logo = new QLabel("⚙  FLOWTASK — COMPTE", w);
    logo->setStyleSheet(
        "color:rgb(0,200,255);font-family:'Courier New';"
        "font-size:16px;font-weight:bold;letter-spacing:4px;"
        "background:transparent;"
        );
    lay->addWidget(logo);
    lay->addStretch();

    QPushButton *backBtn = new QPushButton("← RETOUR", w);
    backBtn->setFixedSize(110, 36);
    backBtn->setCursor(Qt::PointingHandCursor);
    backBtn->setStyleSheet(
        "QPushButton {"
        "  background:rgba(4,14,40,0.9); color:rgb(0,200,255);"
        "  border:1px solid rgba(0,160,220,0.5); border-radius:8px;"
        "  font-family:'Courier New'; font-size:11px;"
        "  font-weight:bold; letter-spacing:2px;"
        "}"
        "QPushButton:hover { background:rgba(0,50,100,0.9); }"
        );
    connect(backBtn, &QPushButton::clicked, this, &LoginWidget::goBack);
    lay->addWidget(backBtn);

    return w;
}

// ════════════════════════════════════════════════════════
//  buildSignInPage
// ════════════════════════════════════════════════════════
QWidget *LoginWidget::buildSignInPage()
{
    QWidget *w = new QWidget;
    w->setStyleSheet("background:transparent;");

    QVBoxLayout *vl = new QVBoxLayout(w);
    vl->setContentsMargins(0, 0, 0, 0);
    vl->setSpacing(14);

    QLabel *title = new QLabel("CONNEXION", w);
    title->setStyleSheet(
        "color:rgb(0,200,255);font-family:'Courier New';"
        "font-size:22px;font-weight:bold;letter-spacing:6px;"
        "background:transparent;"
        );
    title->setAlignment(Qt::AlignCenter);

    QLabel *sub = makeLabel("Entrez vos identifiants", 11, true, w);
    sub->setAlignment(Qt::AlignCenter);

    m_siEmailInput = new AuthLineEdit("Email ou nom d'utilisateur", false, w);
    m_siPassInput  = new AuthLineEdit("Mot de passe", true, w);

    m_siMsg = new QLabel(w);
    m_siMsg->setWordWrap(true);
    m_siMsg->setMinimumHeight(20);

    AuthButton *signInBtn = new AuthButton("SE CONNECTER", C_CYAN, w);
    connect(signInBtn, &QPushButton::clicked, this, &LoginWidget::onSignIn);
    connect(m_siPassInput, &QLineEdit::returnPressed, this, &LoginWidget::onSignIn);

    // Links row
    QHBoxLayout *links = new QHBoxLayout;
    QPushButton *toSignUp = linkBtn("Créer un compte", w);
    QPushButton *toReset  = linkBtn("Mot de passe oublié ?", w);
    connect(toSignUp, &QPushButton::clicked, this, &LoginWidget::showSignUp);
    connect(toReset,  &QPushButton::clicked, this, &LoginWidget::showReset);
    links->addStretch();
    links->addWidget(toSignUp);
    links->addStretch();
    links->addWidget(toReset);
    links->addStretch();

    vl->addStretch();
    vl->addWidget(title);
    vl->addSpacing(4);
    vl->addWidget(sub);
    vl->addSpacing(20);
    vl->addWidget(m_siEmailInput);
    vl->addWidget(m_siPassInput);
    vl->addWidget(m_siMsg);
    vl->addSpacing(6);
    vl->addWidget(signInBtn);
    vl->addSpacing(12);
    vl->addLayout(links);
    vl->addStretch();

    return w;
}

// ════════════════════════════════════════════════════════
//  buildSignUpPage
// ════════════════════════════════════════════════════════
QWidget *LoginWidget::buildSignUpPage()
{
    QWidget *w = new QWidget;
    w->setStyleSheet("background:transparent;");

    QVBoxLayout *vl = new QVBoxLayout(w);
    vl->setContentsMargins(0, 0, 0, 0);
    vl->setSpacing(12);

    QLabel *title = new QLabel("CRÉER UN COMPTE", w);
    title->setStyleSheet(
        "color:rgb(60,220,100);font-family:'Courier New';"
        "font-size:22px;font-weight:bold;letter-spacing:6px;"
        "background:transparent;"
        );
    title->setAlignment(Qt::AlignCenter);

    m_suUsernameInput = new AuthLineEdit("Nom d'utilisateur", false, w);
    m_suEmailInput    = new AuthLineEdit("Email", false, w);
    m_suPassInput     = new AuthLineEdit("Mot de passe (min. 6 caractères)", true, w);
    m_suConfirmInput  = new AuthLineEdit("Confirmer le mot de passe", true, w);

    m_suMsg = new QLabel(w);
    m_suMsg->setWordWrap(true);
    m_suMsg->setMinimumHeight(20);

    AuthButton *signUpBtn = new AuthButton("CRÉER LE COMPTE", C_GREEN, w);
    connect(signUpBtn, &QPushButton::clicked, this, &LoginWidget::onSignUp);
    connect(m_suConfirmInput, &QLineEdit::returnPressed, this, &LoginWidget::onSignUp);

    QHBoxLayout *links = new QHBoxLayout;
    QPushButton *toSignIn = linkBtn("Déjà un compte ? Se connecter", w);
    connect(toSignIn, &QPushButton::clicked, this, &LoginWidget::showSignIn);
    links->addStretch();
    links->addWidget(toSignIn);
    links->addStretch();

    vl->addStretch();
    vl->addWidget(title);
    vl->addSpacing(16);
    vl->addWidget(m_suUsernameInput);
    vl->addWidget(m_suEmailInput);
    vl->addWidget(m_suPassInput);
    vl->addWidget(m_suConfirmInput);
    vl->addWidget(m_suMsg);
    vl->addSpacing(6);
    vl->addWidget(signUpBtn);
    vl->addSpacing(12);
    vl->addLayout(links);
    vl->addStretch();

    return w;
}

// ════════════════════════════════════════════════════════
//  buildResetPage
// ════════════════════════════════════════════════════════
QWidget *LoginWidget::buildResetPage()
{
    QWidget *w = new QWidget;
    w->setStyleSheet("background:transparent;");

    QVBoxLayout *vl = new QVBoxLayout(w);
    vl->setContentsMargins(0, 0, 0, 0);
    vl->setSpacing(12);

    QLabel *title = new QLabel("RÉINITIALISER", w);
    title->setStyleSheet(
        "color:rgb(255,180,0);font-family:'Courier New';"
        "font-size:22px;font-weight:bold;letter-spacing:6px;"
        "background:transparent;"
        );
    title->setAlignment(Qt::AlignCenter);

    QLabel *sub = makeLabel("Entrez votre email et un nouveau mot de passe", 11, true, w);
    sub->setAlignment(Qt::AlignCenter);
    sub->setWordWrap(true);

    m_rstEmailInput   = new AuthLineEdit("Email du compte", false, w);
    m_rstNewPassInput = new AuthLineEdit("Nouveau mot de passe", true, w);
    m_rstConfirmInput = new AuthLineEdit("Confirmer le nouveau mot de passe", true, w);

    m_rstMsg = new QLabel(w);
    m_rstMsg->setWordWrap(true);
    m_rstMsg->setMinimumHeight(20);

    AuthButton *resetBtn = new AuthButton("RÉINITIALISER", C_AMBER, w);
    connect(resetBtn, &QPushButton::clicked, this, &LoginWidget::onResetPassword);
    connect(m_rstConfirmInput, &QLineEdit::returnPressed,
            this, &LoginWidget::onResetPassword);

    QHBoxLayout *links = new QHBoxLayout;
    QPushButton *toSignIn = linkBtn("← Retour à la connexion", w);
    connect(toSignIn, &QPushButton::clicked, this, &LoginWidget::showSignIn);
    links->addStretch();
    links->addWidget(toSignIn);
    links->addStretch();

    vl->addStretch();
    vl->addWidget(title);
    vl->addSpacing(6);
    vl->addWidget(sub);
    vl->addSpacing(16);
    vl->addWidget(m_rstEmailInput);
    vl->addWidget(m_rstNewPassInput);
    vl->addWidget(m_rstConfirmInput);
    vl->addWidget(m_rstMsg);
    vl->addSpacing(6);
    vl->addWidget(resetBtn);
    vl->addSpacing(12);
    vl->addLayout(links);
    vl->addStretch();

    return w;
}

// ════════════════════════════════════════════════════════
//  LoginWidget constructor
// ════════════════════════════════════════════════════════
LoginWidget::LoginWidget(QWidget *parent) : QWidget(parent)
{
    setMinimumSize(760, 560);
    setStyleSheet("background:transparent;");

    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    root->addWidget(buildHeader());

    // Card wrapper
    QWidget *card = new QWidget(this);
    card->setStyleSheet(
        "background: rgba(8,18,52,0.88);"
        "border: 1px solid rgba(0,120,200,0.30);"
        "border-radius: 16px;"
        );
    card->setMaximumWidth(480);

    m_stack = new QStackedWidget(card);
    m_stack->addWidget(buildSignInPage());  // index 0
    m_stack->addWidget(buildSignUpPage());  // index 1
    m_stack->addWidget(buildResetPage());   // index 2
    m_stack->setCurrentIndex(0);

    QVBoxLayout *cl = new QVBoxLayout(card);
    cl->setContentsMargins(40, 30, 40, 36);
    cl->addWidget(m_stack);

    // Centre the card
    QHBoxLayout *centreH = new QHBoxLayout;
    centreH->addStretch();
    centreH->addWidget(card, 0, Qt::AlignHCenter);
    centreH->addStretch();

    QVBoxLayout *centreV = new QVBoxLayout;
    centreV->addStretch();
    centreV->addLayout(centreH);
    centreV->addStretch();

    QWidget *content = new QWidget(this);
    content->setStyleSheet("background:transparent;");
    QVBoxLayout *contentL = new QVBoxLayout(content);
    contentL->setContentsMargins(0, 0, 0, 0);
    contentL->addLayout(centreV);

    root->addWidget(content, 1);
}
// ── Background animation ──────────────────────────────


// ════════════════════════════════════════════════════════
//  Page switch slots
// ════════════════════════════════════════════════════════
void LoginWidget::showSignIn() { m_stack->setCurrentIndex(0); }
void LoginWidget::showSignUp() { m_stack->setCurrentIndex(1); }
void LoginWidget::showReset()  { m_stack->setCurrentIndex(2); }

// ════════════════════════════════════════════════════════
//  Auth action slots
// ════════════════════════════════════════════════════════
void LoginWidget::onSignIn()
{
    clearMessage(m_siMsg);
    QString err = AuthManager::instance().loginUser(
        m_siEmailInput->text(), m_siPassInput->text());

    if (!err.isEmpty()) {
        showError(m_siMsg, err);
        return;
    }
    emit loginSuccess();
}

void LoginWidget::onSignUp()
{
    clearMessage(m_suMsg);

    if (m_suPassInput->text() != m_suConfirmInput->text()) {
        showError(m_suMsg, "Les mots de passe ne correspondent pas.");
        return;
    }

    QString err = AuthManager::instance().registerUser(
        m_suUsernameInput->text(),
        m_suEmailInput->text(),
        m_suPassInput->text());

    if (!err.isEmpty()) {
        showError(m_suMsg, err);
        return;
    }
    showSuccess(m_suMsg, "Compte créé ! Connexion en cours...");
    QTimer::singleShot(800, this, &LoginWidget::loginSuccess);
}

void LoginWidget::onResetPassword()
{
    clearMessage(m_rstMsg);

    if (m_rstNewPassInput->text() != m_rstConfirmInput->text()) {
        showError(m_rstMsg, "Les mots de passe ne correspondent pas.");
        return;
    }

    QString err = AuthManager::instance().resetPassword(
        m_rstEmailInput->text(),
        m_rstNewPassInput->text());

    if (!err.isEmpty()) {
        showError(m_rstMsg, err);
        return;
    }

    showSuccess(m_rstMsg, "Mot de passe réinitialisé. Vous pouvez vous connecter.");
    QTimer::singleShot(1200, this, &LoginWidget::showSignIn);
}

// ════════════════════════════════════════════════════════
//  paintEvent
// ════════════════════════════════════════════════════════
void LoginWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // ── Dark aurora base ──────────────────────────────────
    QLinearGradient bg(0,0,0,height());
    bg.setColorAt(0.00, QColor(2,  4, 18));
    bg.setColorAt(0.45, QColor(5, 10, 30));
    bg.setColorAt(0.75, QColor(3,  7, 24));
    bg.setColorAt(1.00, QColor(1,  3, 14));
    p.fillRect(rect(), bg);

    float t = m_animTime;

    // ── Cute floating bubbles ─────────────────────────────
    p.save();
    p.setCompositionMode(QPainter::CompositionMode_Plus);
    struct Bubble { float x,y,r,speed,phase; QColor col; };
    Bubble bubbles[] = {
                        {0.15f,0,  55, 0.12f, 0.0f, QColor(0,140,255)},
                        {0.75f,0,  40, 0.09f, 1.2f, QColor(80,100,255)},
                        {0.40f,0,  65, 0.07f, 2.4f, QColor(0,180,220)},
                        {0.60f,0,  30, 0.14f, 0.8f, QColor(120,80,255)},
                        {0.85f,0,  48, 0.10f, 3.1f, QColor(0,160,200)},
                        {0.25f,0,  35, 0.11f, 1.8f, QColor(60,120,255)},
                        {0.90f,0,  22, 0.16f, 0.4f, QColor(0,200,255)},
                        };
    for (auto &b : bubbles) {
        float bx = b.x * width();
        float rise = std::fmod(t * b.speed + b.phase, 1.0f);
        float by = height() * (1.0f - rise) - b.r;
        float wobble = std::sin(t*1.2f + b.phase)*b.r*0.3f;
        bx += wobble;
        float fade = qMin(rise*4.f, 1.f) * qMin((1.f-rise)*3.f, 1.f);
        QRadialGradient bg2(bx,by,b.r);
        QColor c = b.col; c.setAlpha(int(fade*30));
        bg2.setColorAt(0.0f, c);
        c.setAlpha(int(fade*15));
        bg2.setColorAt(0.6f, c);
        c.setAlpha(0);
        bg2.setColorAt(1.0f, c);
        p.setBrush(bg2); p.setPen(Qt::NoPen);
        p.drawEllipse(QPointF(bx,by), b.r, b.r);
        // bubble outline (cute glossy look)
        QColor oc = b.col; oc.setAlpha(int(fade*50));
        p.setBrush(Qt::NoBrush);
        p.setPen(QPen(oc, 0.8f));
        p.drawEllipse(QPointF(bx,by), b.r, b.r);
        // top gloss
        QRadialGradient gl(bx-b.r*0.25f, by-b.r*0.3f, b.r*0.4f);
        gl.setColorAt(0.0f, QColor(255,255,255,int(fade*45)));
        gl.setColorAt(1.0f, QColor(255,255,255,0));
        p.setBrush(gl); p.setPen(Qt::NoPen);
        p.drawEllipse(QPointF(bx,by), b.r, b.r);
    }
    p.restore();

    // ── Aurora curtains left & right ─────────────────────
    float wave1 = std::sin(t*0.4f)*0.06f;
    float wave2 = std::sin(t*0.3f+1.f)*0.05f;
    QRadialGradient aL(width()*(0.05f+wave1), height()*0.5f, width()*0.5f);
    aL.setColorAt(0.0f, QColor(0,100,200,25));
    aL.setColorAt(1.0f, QColor(0,0,0,0));
    p.fillRect(rect(), aL);

    QRadialGradient aR(width()*(0.95f+wave2), height()*0.5f, width()*0.5f);
    aR.setColorAt(0.0f, QColor(80,60,200,20));
    aR.setColorAt(1.0f, QColor(0,0,0,0));
    p.fillRect(rect(), aR);

    // ── Soft shimmer lines ────────────────────────────────
    p.save();
    p.setCompositionMode(QPainter::CompositionMode_Plus);
    for (int i = 0; i < 4; ++i) {
        float ly = height()*(0.2f+i*0.2f) + std::sin(t*0.6f+i*0.9f)*20.f;
        QLinearGradient sl(0,ly,width(),ly);
        sl.setColorAt(0.0f, QColor(0,0,0,0));
        sl.setColorAt(0.4f, QColor(80,120,255,10));
        sl.setColorAt(0.6f, QColor(0,160,255,16));
        sl.setColorAt(1.0f, QColor(0,0,0,0));
        p.setPen(QPen(QBrush(sl), 1.0f));
        p.drawLine(QPointF(0,ly), QPointF(width(),ly));
    }
    p.restore();

    // ── Floating cute stars / sparkles ────────────────────
    p.save();
    p.setCompositionMode(QPainter::CompositionMode_Plus);
    p.setPen(Qt::NoPen);
    for (int i = 0; i < 28; ++i) {
        float sx = std::fmod(i*173.7f, 1000.f)/1000.f * width();
        float sy = std::fmod(i* 97.3f + i*i*0.04f, 1000.f)/1000.f * height();
        float blink = 0.2f + 0.8f*std::abs(std::sin(t*0.7f+i*1.13f));
        float sz = 0.6f + std::sin(i*2.1f)*0.4f;
        // 4-pointed sparkle for some
        if (i % 4 == 0) {
            QPainterPath sp;
            float s = sz*3.f*blink;
            sp.moveTo(sx, sy-s); sp.lineTo(sx+s*0.3f, sy-s*0.3f);
            sp.lineTo(sx+s, sy); sp.lineTo(sx+s*0.3f, sy+s*0.3f);
            sp.lineTo(sx, sy+s); sp.lineTo(sx-s*0.3f, sy+s*0.3f);
            sp.lineTo(sx-s, sy); sp.lineTo(sx-s*0.3f, sy-s*0.3f);
            sp.closeSubpath();
            p.setBrush(QColor(200,230,255,int(blink*100)));
            p.drawPath(sp);
        } else {
            p.setBrush(QColor(180,210,255,int(blink*80)));
            p.drawEllipse(QPointF(sx,sy), sz*blink, sz*blink);
        }
    }
    p.restore();

    // ── Vignette ─────────────────────────────────────────
    QRadialGradient vg(width()*0.5f, height()*0.5f, width()*0.65f);
    vg.setColorAt(0.4f, QColor(0,0,0,0));
    vg.setColorAt(1.0f, QColor(0,0,0,100));
    p.fillRect(rect(), vg);
}