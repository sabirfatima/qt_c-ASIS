#include "timer.h"
#include "authmanager.h"
#include "sessionmanager.h"
#include <cmath>
#include <QRandomGenerator>

static const QColor COL_CYAN  (0,  200, 255);
static const QColor COL_AMBER (255,180,   0);
static const QColor COL_GREEN (60, 220, 130);
static const QColor COL_DIM   (60, 100, 160);

static QString spinBoxStyle()
{
    return QString(
        "QSpinBox{background:rgba(4,14,40,0.95);color:rgb(0,200,255);"
        "border:1px solid rgba(0,140,220,0.45);border-radius:6px;"
        "padding:3px 6px;font-family:'Courier New';"
        "font-size:13px;font-weight:bold;}"
        "QSpinBox::up-button,QSpinBox::down-button{"
        "background:rgba(0,80,150,0.4);border:none;width:16px;}");
}

// ════════════════════════════════════════════════════════
//  BackgroundWidget
// ════════════════════════════════════════════════════════
BackgroundWidget::BackgroundWidget(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_NoSystemBackground);
    setStyleSheet("background:transparent;");
}

void BackgroundWidget::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);
    initParticles();
}

void BackgroundWidget::initParticles()
{
    m_particles.clear();
    if (width() <= 0 || height() <= 0) return;
    static const QColor colors[] = {
        QColor(0,200,255), QColor(60,220,130),
        QColor(255,180,0), QColor(180,100,255), QColor(255,255,255)
    };
    for (int i = 0; i < 80; ++i) {
        Particle p;
        p.pos  = QPointF(QRandomGenerator::global()->bounded(width()),
                        QRandomGenerator::global()->bounded(height()));
        float angle = QRandomGenerator::global()->generateDouble() * 2 * M_PI;
        float speed = 0.1f + QRandomGenerator::global()->generateDouble() * 0.3f;
        p.vel  = QPointF(std::cos(angle)*speed, std::sin(angle)*speed);
        p.size = 0.8f + QRandomGenerator::global()->generateDouble() * 2.5f;
        p.alpha = QRandomGenerator::global()->generateDouble();
        p.alphaSpeed = 0.003f + QRandomGenerator::global()->generateDouble()*0.008f;
        p.color = colors[QRandomGenerator::global()->bounded(5)];
        m_particles.append(p);
    }
}

void BackgroundWidget::tick()
{
    for (Particle &p : m_particles) {
        p.pos += p.vel;
        if (p.pos.x() < 0)       p.pos.setX(width());
        if (p.pos.x() > width())  p.pos.setX(0);
        if (p.pos.y() < 0)        p.pos.setY(height());
        if (p.pos.y() > height()) p.pos.setY(0);
        if (p.alphaSpeed > 0) {
            p.alpha += p.alphaSpeed;
            if (p.alpha >= 1.0f) { p.alpha=1.0f; p.alphaSpeed=-p.alphaSpeed; }
        } else {
            p.alpha += p.alphaSpeed;
            if (p.alpha <= 0.05f){ p.alpha=0.05f; p.alphaSpeed=-p.alphaSpeed; }
        }
    }
    m_pulseAngle += 0.012f;
    m_waveOffset += 0.018f;
    update();
}

void BackgroundWidget::paintEvent(QPaintEvent *)
{
    if (m_particles.isEmpty()) return;
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QLinearGradient bg(0,0,0,height());
    bg.setColorAt(0.0, QColor(3,7,22));
    bg.setColorAt(0.5, QColor(5,12,34));
    bg.setColorAt(1.0, QColor(3,8,24));
    p.fillRect(rect(), bg);

    float pulse = 0.5f + 0.5f*std::sin(m_pulseAngle);
    QRadialGradient rg(width()/2.0, height()/2.0, width()*(0.35f+pulse*0.10f));
    rg.setColorAt(0.0, QColor(0,40,100,int(25+pulse*20)));
    rg.setColorAt(1.0, QColor(0,0,0,0));
    p.fillRect(rect(), rg);

    float cx=width()/2.0f, cy=height()/2.0f;
    for (int i=0; i<4; ++i) {
        float phase  = m_pulseAngle + i*(M_PI/2.0f);
        float radius = 80.0f + i*60.0f + std::sin(phase)*20.0f;
        float alpha  = 0.06f + 0.04f*std::sin(phase);
        p.setPen(QPen(QColor(0,180,255,int(alpha*255)), 1.0));
        p.setBrush(Qt::NoBrush);
        p.drawEllipse(QPointF(cx,cy), radius, radius);
    }

    auto cornerGlow=[&](float x, float y, QColor col){
        QRadialGradient g(x,y,180);
        g.setColorAt(0.0, QColor(col.red(),col.green(),col.blue(),18));
        g.setColorAt(1.0, QColor(0,0,0,0));
        p.fillRect(QRectF(x-180,y-180,360,360), g);
    };
    cornerGlow(0,       0,        QColor(0,200,255));
    cornerGlow(width(), 0,        QColor(60,220,130));
    cornerGlow(0,       height(), QColor(255,180,0));
    cornerGlow(width(), height(), QColor(180,100,255));

    QPainterPath wave;
    for (int x=0; x<=width(); x+=4) {
        float y = height()-60
                  + std::sin((x*0.012f)+m_waveOffset)*12.0f
                  + std::sin((x*0.007f)+m_waveOffset*0.6f)*8.0f;
        if(x==0) wave.moveTo(x,y); else wave.lineTo(x,y);
    }
    p.setPen(QPen(QColor(0,160,220,30), 1.5));
    p.setBrush(Qt::NoBrush);
    p.drawPath(wave);

    p.setPen(Qt::NoPen);
    for (const Particle &pt : m_particles) {
        QColor col = pt.color;
        col.setAlphaF(pt.alpha*0.75f);
        QRadialGradient glow(pt.pos, pt.size*3);
        glow.setColorAt(0.0, col);
        QColor tr=col; tr.setAlpha(0);
        glow.setColorAt(1.0, tr);
        p.setBrush(glow);
        p.drawEllipse(pt.pos, pt.size*3, pt.size*3);
        col.setAlphaF(pt.alpha);
        p.setBrush(col);
        p.drawEllipse(pt.pos, pt.size, pt.size);
    }
}

// ════════════════════════════════════════════════════════
//  ModeButton
// ════════════════════════════════════════════════════════
ModeButton::ModeButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent)
{
    setFixedHeight(34); setMinimumWidth(100);
    setCursor(Qt::PointingHandCursor);
}

void ModeButton::paintEvent(QPaintEvent *)
{
    QPainter p(this); p.setRenderHint(QPainter::Antialiasing);
    QRectF r=QRectF(rect()).adjusted(1,1,-1,-1);
    p.setBrush(m_selected?QColor(0,60,110):QColor(6,14,36,180));
    p.setPen(Qt::NoPen); p.drawRoundedRect(r,8,8);
    p.setPen(QPen(m_selected?COL_CYAN:QColor(0,70,120,100), m_selected?1.5:1.0));
    p.setBrush(Qt::NoBrush); p.drawRoundedRect(r,8,8);
    QFont f("Courier New",10,m_selected?QFont::Bold:QFont::Normal);
    f.setLetterSpacing(QFont::AbsoluteSpacing,2);
    p.setFont(f); p.setPen(m_selected?Qt::white:COL_DIM);
    p.drawText(r,Qt::AlignCenter,text());
}

// ════════════════════════════════════════════════════════
//  ControlButton
// ════════════════════════════════════════════════════════
ControlButton::ControlButton(const QString &text, QColor accent, QWidget *parent)
    : QPushButton(text, parent), m_accent(accent)
{
    setFixedHeight(44); setMinimumWidth(110);
    setCursor(Qt::PointingHandCursor);
}

void ControlButton::paintEvent(QPaintEvent *)
{
    QPainter p(this); p.setRenderHint(QPainter::Antialiasing);
    QRectF r=QRectF(rect()).adjusted(1,1,-1,-1);
    p.setBrush(QColor(6,16,40,200)); p.setPen(Qt::NoPen); p.drawRoundedRect(r,10,10);
    p.setPen(QPen(m_accent,1.4)); p.setBrush(Qt::NoBrush); p.drawRoundedRect(r,10,10);
    QLinearGradient sh(r.topLeft(),QPointF(r.left(),r.top()+r.height()*0.45));
    sh.setColorAt(0,QColor(255,255,255,18)); sh.setColorAt(1,QColor(255,255,255,0));
    p.setBrush(sh); p.setPen(Qt::NoPen); p.drawRoundedRect(r,10,10);
    QFont f("Courier New",11,QFont::Bold);
    f.setLetterSpacing(QFont::AbsoluteSpacing,2);
    p.setFont(f); p.setPen(m_accent);
    p.drawText(r,Qt::AlignCenter,text());
}

// ════════════════════════════════════════════════════════
//  ArcDisplay
// ════════════════════════════════════════════════════════
ArcDisplay::ArcDisplay(QWidget *parent) : QWidget(parent)
{
    setMinimumSize(240,240);
    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet("background:transparent;");
}

QColor ArcDisplay::arcColor() const
{
    switch(m_mode){
    case TimerMode::Croissant:   return COL_CYAN;
    case TimerMode::Decroissant: return COL_AMBER;
    case TimerMode::Pomodoro:    return COL_GREEN;
    }
    return COL_CYAN;
}

void ArcDisplay::tick()
{
    if(!m_pulsing){m_pulse=1.0f;return;}
    if(m_pulseUp){m_pulse+=0.03f;if(m_pulse>=1.15f)m_pulseUp=false;}
    else{m_pulse-=0.03f;if(m_pulse<=0.85f)m_pulseUp=true;}
    update();
}

void ArcDisplay::paintEvent(QPaintEvent *)
{
    QPainter p(this); p.setRenderHint(QPainter::Antialiasing);
    int side=qMin(width(),height())-24;
    qreal cx=width()/2.0, cy=height()/2.0;
    QRectF arc(cx-side/2.0,cy-side/2.0,side,side);
    QColor ac=arcColor();

    p.setPen(QPen(QColor(20,45,90,130),9));
    p.setBrush(Qt::NoBrush); p.drawEllipse(arc);

    if(m_progress>0.001f){
        QColor ac2=ac; ac2.setAlphaF(m_pulse);
        p.setPen(QPen(ac2,9,Qt::SolidLine,Qt::RoundCap));
        int span=-(int)(m_progress*360.0f*16);
        p.drawArc(arc,90*16,span);
        for(int i=1;i<=2;i++){
            QColor gc=ac; gc.setAlpha(20/i);
            p.setPen(QPen(gc,9+i*6,Qt::SolidLine,Qt::RoundCap));
            p.drawArc(arc,90*16,span);
        }
        float angleDeg=90.0f-m_progress*360.0f;
        float angleRad=qDegreesToRadians(angleDeg);
        float dotX=(float)cx+(side/2.0f)*std::cos(angleRad);
        float dotY=(float)cy-(side/2.0f)*std::sin(angleRad);
        QRadialGradient dg(dotX,dotY,10);
        dg.setColorAt(0,Qt::white); dg.setColorAt(0.4,ac);
        dg.setColorAt(1,QColor(0,0,0,0));
        p.setBrush(dg); p.setPen(Qt::NoPen);
        p.drawEllipse(QPointF(dotX,dotY),10,10);
    }

    float inner=side*0.60f;
    QRectF ip(cx-inner/2.0,cy-inner/2.0,inner,inner);
    QRadialGradient ig(cx,cy,inner/2.0);
    ig.setColorAt(0,QColor(12,26,68,220)); ig.setColorAt(1,QColor(4,10,28,220));
    p.setBrush(ig); p.setPen(Qt::NoPen); p.drawEllipse(ip);

    QFont tf("Courier New",30,QFont::Bold);
    p.setFont(tf);
    QColor gc=ac; gc.setAlpha(55); p.setPen(gc);
    p.drawText(ip.adjusted(-2,-2,-2,-2),Qt::AlignCenter,m_timeText);
    p.drawText(ip.adjusted(2,2,2,2),Qt::AlignCenter,m_timeText);
    p.setPen(Qt::white);
    p.drawText(ip,Qt::AlignCenter,m_timeText);

    QFont sf("Courier New",9);
    sf.setLetterSpacing(QFont::AbsoluteSpacing,3);
    p.setFont(sf);
    p.setPen(QColor(ac.red(),ac.green(),ac.blue(),170));
    QRectF subR=ip;
    subR.setTop(ip.center().y()+ip.height()*0.18);
    p.drawText(subR,Qt::AlignHCenter|Qt::AlignTop,m_subText);

    if(!m_taskText.isEmpty()){
        QFont tf2("Courier New",10,QFont::Bold);
        tf2.setLetterSpacing(QFont::AbsoluteSpacing,1);
        p.setFont(tf2);
        QColor tc=ac; tc.setAlpha(200); p.setPen(tc);
        QRectF taskR(cx-side/2.0, arc.bottom()+8, (double)side, 24);
        QFontMetrics fm(tf2);
        QString display=m_taskText;
        if(fm.horizontalAdvance(display)>taskR.width())
            display=fm.elidedText(display,Qt::ElideRight,taskR.width());
        p.drawText(taskR,Qt::AlignCenter,"▶  "+display);
    }
}

// ════════════════════════════════════════════════════════
//  TimerWidget helpers
// ════════════════════════════════════════════════════════
QString TimerWidget::formatTime(int sec) const
{
    int h=sec/3600,m=(sec%3600)/60,s=sec%60;
    if(h>0) return QString("%1:%2:%3").arg(h,2,10,QChar('0'))
            .arg(m,2,10,QChar('0')).arg(s,2,10,QChar('0'));
    return QString("%1:%2").arg(m,2,10,QChar('0')).arg(s,2,10,QChar('0'));
}

void TimerWidget::applyConfig()
{
    switch(m_mode){
    case TimerMode::Decroissant:
        m_total=m_decMin->value()*60+m_decSec->value();
        m_remaining=m_total; m_elapsed=0; break;
    case TimerMode::Pomodoro:
        m_pomFocus=m_pomFocusSpin->value();
        m_pomBreak=m_pomBreakSpin->value();
        m_pomCycles=m_pomCyclesSpin->value();
        m_pomCurrent=1; m_pomOnBreak=false;
        m_total=m_pomFocus*60; m_remaining=m_total; m_elapsed=0; break;
    case TimerMode::Croissant:
        m_elapsed=0; break;
    }
}

void TimerWidget::refreshDisplay()
{
    QString task = m_taskInput ? m_taskInput->text().trimmed() : "";
    m_arc->setTaskText(task);

    switch(m_mode){
    case TimerMode::Croissant:
        m_arc->setTimeText(formatTime(m_elapsed));
        m_arc->setProgress(qMin(1.0f,m_elapsed/3600.0f));
        m_arc->setSubText(m_running?"EN COURS":(m_elapsed>0?"PAUSE":"PRÊT"));
        break;
    case TimerMode::Decroissant:
        m_arc->setTimeText(formatTime(m_remaining));
        m_arc->setProgress(m_total>0?1.0f-(float)m_remaining/m_total:0.0f);
        m_arc->setSubText(m_running?"EN COURS":(m_remaining<m_total?"PAUSE":"PRÊT"));
        break;
    case TimerMode::Pomodoro:{
        m_arc->setTimeText(formatTime(m_remaining));
        m_arc->setProgress(m_total>0?1.0f-(float)m_remaining/m_total:0.0f);
        QString sub=m_pomOnBreak
                          ?QString("PAUSE %1/%2").arg(m_pomCurrent).arg(m_pomCycles)
                          :QString("FOCUS %1/%2").arg(m_pomCurrent).arg(m_pomCycles);
        m_arc->setSubText(sub);
        break;}
    }
}

void TimerWidget::nextPomodoroPhase()
{
    if(!m_pomOnBreak){
        m_pomOnBreak=true; m_total=m_pomBreak*60; m_remaining=m_total;
    } else {
        m_pomOnBreak=false; m_pomCurrent++;
        if(m_pomCurrent>m_pomCycles){
            m_running=false; m_arc->setPulsing(false);
            SessionManager::instance().endSession(0);
            m_arc->setSubText("TERMINÉ !");
            m_btnStart->setEnabled(false);
            m_taskInput->setEnabled(true);
            return;
        }
        m_total=m_pomFocus*60; m_remaining=m_total;
    }
}

// ════════════════════════════════════════════════════════
//  loadTask — appelé depuis TodoWidget via main.cpp
// ════════════════════════════════════════════════════════
void TimerWidget::loadTask(const QString &name, int durationMin)
{
    if(m_running) return; // ne pas interrompre une session en cours
    m_taskInput->setText(name);

    // Pré-configurer le timer décroissant avec la durée de la tâche
    selectMode(1); // passe en mode Décroissant
    m_btnDecroissant->setSelected(true);
    m_configStack->setCurrentIndex(1);
    m_decMin->setValue(durationMin);
    m_decSec->setValue(0);

    refreshDisplay();
}

void TimerWidget::updateAuthState()
{
    if(AuthManager::instance().isLoggedIn()){
        QString name=AuthManager::instance().currentUsername();
        m_authBtn->setText("◉  "+name.toUpper());
        m_authBtn->setStyleSheet(
            "QPushButton{background:rgba(0,60,30,0.9);color:rgb(60,220,100);"
            "border:1px solid rgba(60,200,100,0.5);border-radius:8px;"
            "font-family:'Courier New';font-size:11px;"
            "font-weight:bold;letter-spacing:2px;}"
            "QPushButton:hover{background:rgba(0,90,40,0.9);}");
        m_statsBtn->setVisible(true);
    } else {
        m_authBtn->setText("CONNEXION");
        m_authBtn->setStyleSheet(
            "QPushButton{background:rgba(4,14,40,0.9);color:rgb(255,180,0);"
            "border:1px solid rgba(255,160,0,0.5);border-radius:8px;"
            "font-family:'Courier New';font-size:11px;"
            "font-weight:bold;letter-spacing:2px;}"
            "QPushButton:hover{background:rgba(60,40,0,0.9);}");
        m_statsBtn->setVisible(false);
    }
}

// ════════════════════════════════════════════════════════
//  Layout builders
// ════════════════════════════════════════════════════════
QWidget *TimerWidget::buildHeader()
{
    QWidget *w=new QWidget(this);
    w->setFixedHeight(60);
    w->setStyleSheet("background:rgba(6,14,38,0.92);"
                     "border-bottom:1px solid rgba(0,120,200,0.30);");
    QHBoxLayout *lay=new QHBoxLayout(w);
    lay->setContentsMargins(24,0,24,0);

    QLabel *logo=new QLabel("⏱  ASIS",w);
    logo->setStyleSheet("color:rgb(0,200,255);font-family:'Courier New';"
                        "font-size:16px;font-weight:bold;"
                        "letter-spacing:4px;background:transparent;");
    lay->addWidget(logo); lay->addStretch();

    QPushButton *todoBtn=new QPushButton("TO-DO LIST",w);
    todoBtn->setFixedSize(130,36); todoBtn->setCursor(Qt::PointingHandCursor);
    todoBtn->setStyleSheet(
        "QPushButton{background:rgba(4,14,40,0.9);color:rgb(0,200,255);"
        "border:1px solid rgba(0,160,220,0.55);border-radius:8px;"
        "font-family:'Courier New';font-size:11px;font-weight:bold;letter-spacing:2px;}"
        "QPushButton:hover{background:rgba(0,60,110,0.9);}");
    connect(todoBtn,&QPushButton::clicked,this,&TimerWidget::goToTodoList);

    m_statsBtn=new QPushButton("📊 STATS",w);
    m_statsBtn->setFixedSize(110,36);
    m_statsBtn->setCursor(Qt::PointingHandCursor);
    m_statsBtn->setVisible(false);
    m_statsBtn->setStyleSheet(
        "QPushButton{background:rgba(4,14,40,0.9);color:rgb(255,180,0);"
        "border:1px solid rgba(255,160,0,0.45);border-radius:8px;"
        "font-family:'Courier New';font-size:11px;font-weight:bold;letter-spacing:2px;}"
        "QPushButton:hover{background:rgba(60,40,0,0.9);}");
    connect(m_statsBtn,&QPushButton::clicked,this,&TimerWidget::goToStats);

    m_authBtn=new QPushButton("CONNEXION",w);
    m_authBtn->setFixedSize(140,36); m_authBtn->setCursor(Qt::PointingHandCursor);
    connect(m_authBtn,&QPushButton::clicked,this,&TimerWidget::goToLogin);
    updateAuthState();

    lay->addWidget(todoBtn); lay->addSpacing(8);
    lay->addWidget(m_statsBtn); lay->addSpacing(8);
    lay->addWidget(m_authBtn);
    return w;
}

QWidget *TimerWidget::buildTaskInput()
{
    QWidget *w=new QWidget(this);
    w->setStyleSheet("background:transparent;");
    QHBoxLayout *lay=new QHBoxLayout(w);
    lay->setContentsMargins(0,0,0,0); lay->setSpacing(10);
    lay->addStretch();

    QLabel *lbl=new QLabel("TÂCHE EN COURS :",w);
    lbl->setStyleSheet("color:rgba(0,200,255,0.7);font-family:'Courier New';"
                       "font-size:10px;letter-spacing:3px;background:transparent;");

    m_taskInput=new QLineEdit(w);
    m_taskInput->setPlaceholderText("Décrivez votre tâche...");
    m_taskInput->setFixedHeight(36);
    m_taskInput->setFixedWidth(340);
    m_taskInput->setStyleSheet(
        "QLineEdit{background:rgba(4,14,40,0.85);color:rgb(160,210,255);"
        "border:1px solid rgba(0,140,220,0.45);border-radius:8px;"
        "padding:4px 12px;font-family:'Courier New';font-size:12px;"
        "selection-background-color:rgba(0,140,220,0.5);}"
        "QLineEdit:focus{border:1px solid rgba(0,200,255,0.7);}");
    connect(m_taskInput,&QLineEdit::textChanged,this,[this](){ refreshDisplay(); });

    lay->addWidget(lbl);
    lay->addWidget(m_taskInput);
    lay->addStretch();
    return w;
}

QWidget *TimerWidget::buildModeSelector()
{
    QWidget *w=new QWidget(this);
    w->setStyleSheet("background:transparent;");
    QHBoxLayout *lay=new QHBoxLayout(w);
    lay->setContentsMargins(0,0,0,0); lay->setSpacing(10); lay->addStretch();

    m_btnCroissant  =new ModeButton("CROISSANT",w);
    m_btnDecroissant=new ModeButton("DÉCROISSANT",w);
    m_btnPomodoro   =new ModeButton("POMODORO",w);
    m_btnCroissant->setSelected(true);

    m_modeGroup=new QButtonGroup(this);
    m_modeGroup->addButton(m_btnCroissant,0);
    m_modeGroup->addButton(m_btnDecroissant,1);
    m_modeGroup->addButton(m_btnPomodoro,2);
    m_modeGroup->setExclusive(false);

    connect(m_btnCroissant,  &QPushButton::clicked,this,[this]{selectMode(0);});
    connect(m_btnDecroissant,&QPushButton::clicked,this,[this]{selectMode(1);});
    connect(m_btnPomodoro,   &QPushButton::clicked,this,[this]{selectMode(2);});

    lay->addWidget(m_btnCroissant);
    lay->addWidget(m_btnDecroissant);
    lay->addWidget(m_btnPomodoro);
    lay->addStretch();
    return w;
}

QWidget *TimerWidget::buildConfigPanel()
{
    m_configStack=new QStackedWidget(this);
    m_configStack->setStyleSheet("background:transparent;");
    m_configStack->setFixedHeight(60);

    QWidget *pg0=new QWidget; pg0->setStyleSheet("background:transparent;");
    QLabel *noConf=new QLabel("Démarre et le chrono monte.",pg0);
    noConf->setStyleSheet("color:rgba(80,140,200,0.8);font-family:'Courier New';"
                          "font-size:11px;background:transparent;");
    QHBoxLayout *l0=new QHBoxLayout(pg0);
    l0->addStretch(); l0->addWidget(noConf); l0->addStretch();

    QWidget *pg1=new QWidget; pg1->setStyleSheet("background:transparent;");
    QHBoxLayout *l1=new QHBoxLayout(pg1);
    l1->setContentsMargins(0,0,0,0); l1->setSpacing(8); l1->addStretch();
    auto lbl=[](const QString &t,QWidget *p)->QLabel*{
        QLabel *l=new QLabel(t,p);
        l->setStyleSheet("color:rgba(120,180,255,0.7);font-family:'Courier New';"
                         "font-size:10px;background:transparent;letter-spacing:2px;");
        return l;};
    m_decMin=new QSpinBox(pg1); m_decMin->setRange(0,599); m_decMin->setValue(5);
    m_decMin->setFixedWidth(70); m_decMin->setStyleSheet(spinBoxStyle());
    m_decSec=new QSpinBox(pg1); m_decSec->setRange(0,59); m_decSec->setValue(0);
    m_decSec->setFixedWidth(70); m_decSec->setStyleSheet(spinBoxStyle());
    l1->addWidget(lbl("MIN",pg1)); l1->addWidget(m_decMin);
    l1->addSpacing(16);
    l1->addWidget(lbl("SEC",pg1)); l1->addWidget(m_decSec);
    l1->addStretch();

    QWidget *pg2=new QWidget; pg2->setStyleSheet("background:transparent;");
    QHBoxLayout *l2=new QHBoxLayout(pg2);
    l2->setContentsMargins(0,0,0,0); l2->setSpacing(8); l2->addStretch();
    m_pomFocusSpin =new QSpinBox(pg2); m_pomFocusSpin->setRange(1,120); m_pomFocusSpin->setValue(25);
    m_pomBreakSpin =new QSpinBox(pg2); m_pomBreakSpin->setRange(1,60);  m_pomBreakSpin->setValue(5);
    m_pomCyclesSpin=new QSpinBox(pg2); m_pomCyclesSpin->setRange(1,12); m_pomCyclesSpin->setValue(4);
    for(QSpinBox *sb:{m_pomFocusSpin,m_pomBreakSpin,m_pomCyclesSpin}){
        sb->setFixedWidth(65); sb->setStyleSheet(spinBoxStyle());}
    l2->addWidget(lbl("FOCUS",pg2));  l2->addWidget(m_pomFocusSpin);
    l2->addSpacing(10);
    l2->addWidget(lbl("PAUSE",pg2));  l2->addWidget(m_pomBreakSpin);
    l2->addSpacing(10);
    l2->addWidget(lbl("CYCLES",pg2)); l2->addWidget(m_pomCyclesSpin);
    l2->addStretch();

    m_configStack->addWidget(pg0);
    m_configStack->addWidget(pg1);
    m_configStack->addWidget(pg2);
    return m_configStack;
}

QWidget *TimerWidget::buildControls()
{
    QWidget *w=new QWidget(this);
    w->setStyleSheet("background:transparent;");
    QHBoxLayout *lay=new QHBoxLayout(w);
    lay->setContentsMargins(0,0,0,0); lay->setSpacing(16); lay->addStretch();

    m_btnStart=new ControlButton("START", COL_GREEN,w);
    m_btnPause=new ControlButton("PAUSE", COL_AMBER,w);
    m_btnReset=new ControlButton("RESET", QColor(220,80,80),w);
    m_btnPause->setEnabled(false);

    connect(m_btnStart,&QPushButton::clicked,this,&TimerWidget::onStart);
    connect(m_btnPause,&QPushButton::clicked,this,&TimerWidget::onPause);
    connect(m_btnReset,&QPushButton::clicked,this,&TimerWidget::onReset);

    lay->addWidget(m_btnStart);
    lay->addWidget(m_btnPause);
    lay->addWidget(m_btnReset);
    lay->addStretch();
    return w;
}

// ════════════════════════════════════════════════════════
//  Constructor
// ════════════════════════════════════════════════════════
TimerWidget::TimerWidget(QWidget *parent) : QWidget(parent)
{
    setMinimumSize(760,560);

    m_bg=new BackgroundWidget(this);
    m_bg->setGeometry(rect());

    m_ticker=new QTimer(this);
    m_ticker->setInterval(1000);
    connect(m_ticker,&QTimer::timeout,this,&TimerWidget::onTick);

    QVBoxLayout *root=new QVBoxLayout(this);
    root->setContentsMargins(0,0,0,0); root->setSpacing(0);
    root->addWidget(buildHeader());

    QWidget *content=new QWidget(this);
    content->setStyleSheet("background:transparent;");
    QVBoxLayout *cl=new QVBoxLayout(content);
    cl->setContentsMargins(40,20,40,20); cl->setSpacing(12);
    cl->addWidget(buildModeSelector());
    cl->addWidget(buildTaskInput());

    m_arc=new ArcDisplay(this);
    m_arc->setMinimumHeight(270);
    cl->addWidget(m_arc,1);
    cl->addWidget(buildConfigPanel());
    cl->addWidget(buildControls());

    root->addWidget(content,1);

    QTimer *anim=new QTimer(this);
    anim->setInterval(33);
    connect(anim,&QTimer::timeout,this,[this](){
        m_arc->tick(); m_bg->tick();
    });
    anim->start();

    refreshDisplay();
}

void TimerWidget::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);
    if(m_bg) m_bg->setGeometry(rect());
}

// ════════════════════════════════════════════════════════
//  Slots
// ════════════════════════════════════════════════════════
void TimerWidget::selectMode(int id)
{
    if(m_running) onReset();
    m_mode=static_cast<TimerMode>(id);
    m_arc->setMode(m_mode);
    m_btnCroissant->setSelected(id==0);
    m_btnDecroissant->setSelected(id==1);
    m_btnPomodoro->setSelected(id==2);
    m_configStack->setCurrentIndex(id);
    m_elapsed=m_remaining=m_total=0;
    m_started=false;   // reset on mode change
    m_pomCurrent=1; m_pomOnBreak=false;
    m_btnStart->setEnabled(true);
    refreshDisplay();
}

void TimerWidget::onStart()
{
    if(!m_running){
        // Only apply config and start session on FIRST start, not after pause
        if(!m_started){
            applyConfig();
            m_started = true;

            // ── Enregistrer session ───────────────────────────
            QString modeStr;
            switch(m_mode){
            case TimerMode::Croissant:   modeStr="Croissant";   break;
            case TimerMode::Decroissant: modeStr="Decroissant"; break;
            case TimerMode::Pomodoro:    modeStr="Pomodoro";    break;
            }
            SessionManager::instance().startSession(modeStr);

            // ── Envoyer la tâche à TodoList ───────────────────
            QString taskName = m_taskInput->text().trimmed();
            if(!taskName.isEmpty()){
                int duration = 0;
                if(m_mode == TimerMode::Decroissant)
                    duration = m_decMin->value();
                else if(m_mode == TimerMode::Pomodoro)
                    duration = m_pomFocus;
                emit taskStarted(taskName, duration);
            }

            m_taskInput->setEnabled(false);
        }

        // Resume: just restart the ticker and arc
        m_running = true;
        m_arc->setPulsing(true);
        m_ticker->start();
        m_btnStart->setEnabled(false);
        m_btnPause->setEnabled(true);
        refreshDisplay();
    }
}

void TimerWidget::onPause()
{
    if(m_running){
        m_running=false; m_arc->setPulsing(false);
        m_ticker->stop();
        m_btnStart->setEnabled(true); m_btnPause->setEnabled(false);
        refreshDisplay();
    }
}

void TimerWidget::onReset()
{
    if(m_running || m_elapsed>0 || m_remaining>0)
        SessionManager::instance().endSession(0);

    m_running  = false;
    m_started  = false;   // ← reset flag so next Start is fresh
    m_arc->setPulsing(false);
    m_ticker->stop();
    m_elapsed=m_remaining=m_total=0;
    m_pomCurrent=1; m_pomOnBreak=false;
    m_arc->setProgress(0.0f);
    m_btnStart->setEnabled(true); m_btnPause->setEnabled(false);
    m_taskInput->setEnabled(true);
    refreshDisplay();
}

void TimerWidget::onTick()
{
    switch(m_mode){
    case TimerMode::Croissant:
        m_elapsed++; break;
    case TimerMode::Decroissant:
        if(m_remaining>0) m_remaining--;
        if(m_remaining<=0){
            m_ticker->stop(); m_running=false; m_arc->setPulsing(false);
            m_arc->setSubText("TERMINÉ !");
            SessionManager::instance().endSession(0);
            m_taskInput->setEnabled(true);
            m_btnStart->setEnabled(false); m_btnPause->setEnabled(false);
        }
        break;
    case TimerMode::Pomodoro:
        if(m_remaining>0) m_remaining--;
        if(m_remaining<=0) nextPomodoroPhase();
        break;
    }
    refreshDisplay();
}

void TimerWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.fillRect(rect(), QColor(0,0,0,0));
}