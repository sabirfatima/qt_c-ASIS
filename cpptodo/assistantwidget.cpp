#include "assistantwidget.h"
#include <QPainterPath>
#include <QMouseEvent>
#include <QRandomGenerator>
#include <QtMath>
#include <QFontMetrics>

// ════════════════════════════════════════════════════════
//  AssistantCharacter
// ════════════════════════════════════════════════════════
AssistantCharacter::AssistantCharacter(QWidget *parent) : QWidget(parent)
{
    setFixedSize(90, 110);
    setStyleSheet("background:transparent;");
    setAttribute(Qt::WA_TranslucentBackground);

    for (int i = 0; i < 6; ++i) {
        Particle p;
        p.x     = QRandomGenerator::global()->bounded(80);
        p.y     = QRandomGenerator::global()->bounded(40);
        p.alpha = QRandomGenerator::global()->generateDouble();
        p.speed = 0.3f + QRandomGenerator::global()->generateDouble() * 0.5f;
        p.size  = 4.0f + QRandomGenerator::global()->generateDouble() * 6.0f;
        m_particles.append(p);
    }
}

void AssistantCharacter::setState(AssistantState state)
{
    m_state = state;
    m_animT = 0.0f;
    update();
}

void AssistantCharacter::tick()
{
    m_animT += 0.05f;

    m_blinkTimer++;
    if (m_blinkTimer > 120) { m_blinking = true; m_blinkTimer = 0; }
    if (m_blinking) {
        m_blinkT += 0.3f;
        if (m_blinkT >= M_PI) { m_blinking = false; m_blinkT = 0.0f; }
    }

    for (Particle &p : m_particles) {
        p.y -= p.speed; p.alpha -= 0.015f;
        if (p.alpha <= 0.0f || p.y < -20) {
            p.x = QRandomGenerator::global()->bounded(80);
            p.y = 100.0f; p.alpha = 0.8f;
            p.speed = 0.3f + QRandomGenerator::global()->generateDouble() * 0.5f;
        }
    }
    update();
}

void AssistantCharacter::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    float cx = width()  / 2.0f;
    float cy = height() / 2.0f + m_bounce * 4.0f;
    drawBody(p, cx, cy);
    drawHead(p, cx, cy);
    drawArms(p, cx, cy);
    drawEyes(p, cx, cy);
    drawMouth(p, cx, cy);
    drawAccessory(p, cx, cy);
    if (m_state == AssistantState::Happy ||
        m_state == AssistantState::Cheering ||
        m_state == AssistantState::Proud)
        drawParticles(p, cx, cy);
}

void AssistantCharacter::drawBody(QPainter &p, float cx, float cy)
{
    QColor bodyColor;
    switch(m_state) {
    case AssistantState::Studying:  bodyColor = QColor(80,140,220);  break;
    case AssistantState::Sleeping:  bodyColor = QColor(100,100,160); break;
    case AssistantState::Happy:     bodyColor = QColor(80,200,120);  break;
    case AssistantState::Cheering:  bodyColor = QColor(220,140,60);  break;
    case AssistantState::Thinking:  bodyColor = QColor(160,80,200);  break;
    case AssistantState::Proud:     bodyColor = QColor(60,180,200);  break;
    case AssistantState::Typing:    bodyColor = QColor(80,160,220);  break;
    default:                        bodyColor = QColor(80,160,220);  break;
    }
    QRadialGradient shadow(cx, cy+38, 28);
    shadow.setColorAt(0.0, QColor(0,0,0,60));
    shadow.setColorAt(1.0, QColor(0,0,0,0));
    p.setBrush(shadow); p.setPen(Qt::NoPen);
    p.drawEllipse(QPointF(cx,cy+38),28,8);

    QRectF body(cx-22,cy+8,44,46);
    QRadialGradient bg(cx-5,cy+12,30);
    bg.setColorAt(0.0, bodyColor.lighter(130));
    bg.setColorAt(1.0, bodyColor);
    p.setBrush(bg); p.setPen(QPen(bodyColor.darker(120),1.5));
    p.drawRoundedRect(body,18,18);

    p.setBrush(QColor(255,255,255,50)); p.setPen(Qt::NoPen);
    p.drawEllipse(QPointF(cx,cy+26),12,10);
}

void AssistantCharacter::drawHead(QPainter &p, float cx, float cy)
{
    QColor headColor = (m_state==AssistantState::Sleeping)
    ? QColor(230,210,190) : QColor(255,230,200);
    QRectF head(cx-26,cy-32,52,50);
    QRadialGradient hg(cx-6,cy-20,28);
    hg.setColorAt(0.0, headColor.lighter(110));
    hg.setColorAt(1.0, headColor);
    p.setBrush(hg); p.setPen(QPen(QColor(200,170,140),1.5));
    p.drawEllipse(head);

    QRadialGradient cheekL(cx-16,cy+2,8);
    cheekL.setColorAt(0.0,QColor(255,160,160,180));
    cheekL.setColorAt(1.0,QColor(255,160,160,0));
    p.setBrush(cheekL); p.setPen(Qt::NoPen);
    p.drawEllipse(QPointF(cx-16,cy+2),8,6);

    QRadialGradient cheekR(cx+16,cy+2,8);
    cheekR.setColorAt(0.0,QColor(255,160,160,180));
    cheekR.setColorAt(1.0,QColor(255,160,160,0));
    p.setBrush(cheekR); p.setPen(Qt::NoPen);
    p.drawEllipse(QPointF(cx+16,cy+2),8,6);

    p.setBrush(QColor(255,220,190));
    p.setPen(QPen(QColor(200,170,140),1.0));
    p.drawEllipse(QPointF(cx-26,cy-14),6,8);
    p.drawEllipse(QPointF(cx+26,cy-14),6,8);
}

void AssistantCharacter::drawEyes(QPainter &p, float cx, float cy)
{
    float blinkScale = m_blinking ? qAbs(std::cos(m_blinkT)) : 1.0f;

    switch(m_state) {
    case AssistantState::Sleeping: {
        p.setPen(QPen(QColor(80,50,30),2.0,Qt::SolidLine,Qt::RoundCap));
        p.setBrush(Qt::NoBrush);
        QPainterPath eL,eR;
        eL.moveTo(cx-16,cy-12); eL.quadTo(cx-10,cy-16,cx-4,cy-12);
        eR.moveTo(cx+4,cy-12);  eR.quadTo(cx+10,cy-16,cx+16,cy-12);
        p.drawPath(eL); p.drawPath(eR);
        break;
    }
    case AssistantState::Happy:
    case AssistantState::Cheering:
    case AssistantState::Proud: {
        p.setPen(QPen(QColor(60,40,20),2.5,Qt::SolidLine,Qt::RoundCap));
        p.setBrush(Qt::NoBrush);
        QPainterPath eL,eR;
        eL.moveTo(cx-16,cy-10); eL.quadTo(cx-10,cy-18,cx-4,cy-10);
        eR.moveTo(cx+4,cy-10);  eR.quadTo(cx+10,cy-18,cx+16,cy-10);
        p.drawPath(eL); p.drawPath(eR);
        break;
    }
    case AssistantState::Thinking: {
        p.setBrush(QColor(60,40,20)); p.setPen(Qt::NoPen);
        p.drawEllipse(QPointF(cx-10,cy-12),5,5*blinkScale);
        p.setPen(QPen(QColor(60,40,20),2.0,Qt::SolidLine,Qt::RoundCap));
        p.setBrush(Qt::NoBrush);
        QPainterPath eR;
        eR.moveTo(cx+4,cy-12); eR.quadTo(cx+10,cy-16,cx+16,cy-12);
        p.drawPath(eR);
        break;
    }
    default: {
        p.setBrush(Qt::white); p.setPen(Qt::NoPen);
        p.drawEllipse(QPointF(cx-10,cy-12),7,7*blinkScale);
        p.drawEllipse(QPointF(cx+10,cy-12),7,7*blinkScale);
        p.setBrush(QColor(40,25,15));
        p.drawEllipse(QPointF(cx-9,cy-12),4,4*blinkScale);
        p.drawEllipse(QPointF(cx+11,cy-12),4,4*blinkScale);
        p.setBrush(Qt::white);
        p.drawEllipse(QPointF(cx-7,cy-14),1.5,1.5);
        p.drawEllipse(QPointF(cx+13,cy-14),1.5,1.5);
        break;
    }
    }
}

void AssistantCharacter::drawMouth(QPainter &p, float cx, float cy)
{
    p.setPen(QPen(QColor(180,80,80),2.0,Qt::SolidLine,Qt::RoundCap));
    p.setBrush(Qt::NoBrush);
    QPainterPath mouth;
    switch(m_state) {
    case AssistantState::Sleeping:
        mouth.moveTo(cx-6,cy+6); mouth.quadTo(cx,cy+9,cx+6,cy+6); break;
    case AssistantState::Happy:
    case AssistantState::Cheering:
    case AssistantState::Proud:
        mouth.moveTo(cx-10,cy+4); mouth.quadTo(cx,cy+14,cx+10,cy+4);
        p.setBrush(QColor(220,100,100,180)); break;
    case AssistantState::Thinking:
        mouth.moveTo(cx-6,cy+6); mouth.quadTo(cx+2,cy+5,cx+8,cy+8); break;
    case AssistantState::Studying:
    case AssistantState::Typing:
        mouth.moveTo(cx-7,cy+6); mouth.quadTo(cx,cy+8,cx+7,cy+6); break;
    default:
        mouth.moveTo(cx-8,cy+5); mouth.quadTo(cx,cy+11,cx+8,cy+5); break;
    }
    p.drawPath(mouth);
    if(m_state==AssistantState::Happy||m_state==AssistantState::Cheering){
        p.setBrush(QColor(220,120,120)); p.setPen(Qt::NoPen);
        p.drawEllipse(QPointF(cx,cy+10),4,3);
    }
}

void AssistantCharacter::drawArms(QPainter &p, float cx, float cy)
{
    QColor armColor(255,220,185);
    p.setPen(QPen(QColor(200,165,130),1.5));

    switch(m_state) {
    case AssistantState::Cheering: {
        float wave=std::sin(m_animT*2.0f)*8.0f;
        p.setBrush(armColor);
        QPainterPath aL,aR;
        aL.moveTo(cx-22,cy+14); aL.quadTo(cx-36,cy-4+wave,cx-32,cy-20+wave);
        aR.moveTo(cx+22,cy+14); aR.quadTo(cx+36,cy-4-wave,cx+32,cy-20-wave);
        p.drawPath(aL); p.drawEllipse(QPointF(cx-32,cy-22+wave),6,6);
        p.drawPath(aR); p.drawEllipse(QPointF(cx+32,cy-22-wave),6,6);
        break;
    }
    case AssistantState::Studying: {
        p.setBrush(armColor);
        QPainterPath aL,aR;
        aL.moveTo(cx-22,cy+16); aL.quadTo(cx-34,cy+24,cx-30,cy+36);
        aR.moveTo(cx+22,cy+16); aR.quadTo(cx+34,cy+10,cx+28,cy);
        p.drawPath(aL); p.drawEllipse(QPointF(cx-30,cy+38),6,6);
        p.drawPath(aR); p.drawEllipse(QPointF(cx+28,cy-2),6,6);
        break;
    }
    case AssistantState::Thinking: {
        p.setBrush(armColor);
        QPainterPath aL,aR;
        aL.moveTo(cx-22,cy+16); aL.quadTo(cx-30,cy+28,cx-26,cy+38);
        aR.moveTo(cx+22,cy+16); aR.quadTo(cx+32,cy+8,cx+18,cy+2);
        p.drawPath(aL); p.drawEllipse(QPointF(cx-26,cy+40),6,6);
        p.drawPath(aR); p.drawEllipse(QPointF(cx+16,cy),6,6);
        break;
    }
    case AssistantState::Typing: {
        float tap=std::sin(m_animT*6.0f)*4.0f;
        p.setBrush(armColor);
        QPainterPath aL,aR;
        aL.moveTo(cx-22,cy+16); aL.quadTo(cx-28,cy+28+tap,cx-22,cy+40+tap);
        aR.moveTo(cx+22,cy+16); aR.quadTo(cx+28,cy+28-tap,cx+22,cy+40-tap);
        p.drawPath(aL); p.drawEllipse(QPointF(cx-22,cy+42+tap),5,5);
        p.drawPath(aR); p.drawEllipse(QPointF(cx+22,cy+42-tap),5,5);
        break;
    }
    case AssistantState::Sleeping: {
        p.setBrush(armColor);
        QPainterPath aL,aR;
        aL.moveTo(cx-20,cy+18); aL.quadTo(cx-32,cy+30,cx-28,cy+44);
        aR.moveTo(cx+20,cy+18); aR.quadTo(cx+32,cy+30,cx+28,cy+44);
        p.drawPath(aL); p.drawEllipse(QPointF(cx-28,cy+46),6,6);
        p.drawPath(aR); p.drawEllipse(QPointF(cx+28,cy+46),6,6);
        break;
    }
    default: {
        float wave=std::sin(m_animT)*3.0f;
        p.setBrush(armColor);
        QPainterPath aL,aR;
        aL.moveTo(cx-22,cy+14); aL.quadTo(cx-34,cy+20+wave,cx-30,cy+34+wave);
        aR.moveTo(cx+22,cy+14); aR.quadTo(cx+34,cy+20-wave,cx+30,cy+34-wave);
        p.drawPath(aL); p.drawEllipse(QPointF(cx-30,cy+36+wave),6,6);
        p.drawPath(aR); p.drawEllipse(QPointF(cx+30,cy+36-wave),6,6);
        break;
    }
    }
}

void AssistantCharacter::drawAccessory(QPainter &p, float cx, float cy)
{
    switch(m_state) {
    case AssistantState::Studying: {
        QRectF book(cx-44,cy+28,28,20);
        p.setBrush(QColor(40,100,180));
        p.setPen(QPen(QColor(20,60,120),1.5));
        p.drawRoundedRect(book,3,3);
        p.setPen(QPen(QColor(200,220,255,150),1.0));
        for(int i=0;i<3;++i)
            p.drawLine(QPointF(cx-40,cy+33+i*4),QPointF(cx-20,cy+33+i*4));
        p.setPen(QPen(QColor(80,60,40),1.5));
        p.setBrush(QColor(200,230,255,80));
        p.drawEllipse(QPointF(cx-10,cy-12),6,5);
        p.drawEllipse(QPointF(cx+10,cy-12),6,5);
        p.drawLine(QPointF(cx-4,cy-12),QPointF(cx+4,cy-12));
        p.drawLine(QPointF(cx-16,cy-13),QPointF(cx-20,cy-14));
        p.drawLine(QPointF(cx+16,cy-13),QPointF(cx+20,cy-14));
        break;
    }
    case AssistantState::Sleeping: {
        float za=0.5f+0.5f*std::sin(m_animT*1.5f);
        QFont zf("Courier New",10,QFont::Bold);
        p.setFont(zf);
        for(int i=0;i<3;++i){
            float xz=cx+20+i*10;
            float yz=cy-20-i*12-std::sin(m_animT+i)*5;
            p.setPen(QColor(180,180,255,int((za-i*0.15f)*255)));
            p.drawText(QPointF(xz,yz),"z");
        }
        QPainterPath hat;
        hat.moveTo(cx-18,cy-28);
        hat.quadTo(cx-10,cy-50,cx+5,cy-44);
        hat.quadTo(cx+20,cy-38,cx+18,cy-28);
        hat.closeSubpath();
        p.setBrush(QColor(100,100,200));
        p.setPen(QPen(QColor(60,60,160),1.0));
        p.drawPath(hat);
        p.setBrush(Qt::white); p.setPen(Qt::NoPen);
        p.drawEllipse(QPointF(cx+5,cy-46),5,5);
        break;
    }
    case AssistantState::Happy: {
        for(int i=0;i<4;++i){
            float angle=m_animT+i*(M_PI/2.0f);
            float sx=cx+std::cos(angle)*32;
            float sy=cy-10+std::sin(angle)*18;
            float sz=4.0f+std::sin(m_animT*2+i)*2.0f;
            p.setBrush(QColor(255,220,50,200)); p.setPen(Qt::NoPen);
            QPainterPath star;
            for(int j=0;j<5;++j){
                float a1=j*2*M_PI/5-M_PI/2;
                float a2=a1+M_PI/5;
                QPointF outer(sx+std::cos(a1)*sz,sy+std::sin(a1)*sz);
                QPointF inner(sx+std::cos(a2)*sz*0.4f,sy+std::sin(a2)*sz*0.4f);
                if(j==0) star.moveTo(outer); else star.lineTo(outer);
                star.lineTo(inner);
            }
            star.closeSubpath(); p.drawPath(star);
        }
        break;
    }
    case AssistantState::Cheering: {
        for(int i=0;i<8;++i){
            float angle=m_animT*1.5f+i*(2*M_PI/8);
            float r=30+std::sin(m_animT+i)*10;
            float px=cx+std::cos(angle)*r;
            float py=cy-15+std::sin(angle*0.7f)*20;
            QColor cols[]={QColor(255,80,80),QColor(80,255,120),
                             QColor(80,160,255),QColor(255,220,50),QColor(255,120,200)};
            p.setBrush(cols[i%5]); p.setPen(Qt::NoPen);
            p.drawEllipse(QPointF(px,py),3,3);
        }
        break;
    }
    case AssistantState::Proud: {
        p.setBrush(QColor(255,200,50));
        p.setPen(QPen(QColor(200,150,20),1.5));
        p.drawEllipse(QPointF(cx+14,cy+20),10,10);
        p.setPen(QPen(QColor(180,120,10),1.0));
        QFont mf("Courier New",7,QFont::Bold); p.setFont(mf);
        p.drawText(QRectF(cx+4,cy+12,20,16),Qt::AlignCenter,"★");
        p.setPen(QPen(QColor(200,50,50),2.0));
        p.drawLine(QPointF(cx+14,cy+10),QPointF(cx+14,cy+5));
        break;
    }
    case AssistantState::Thinking: {
        float fy=cy-35+std::sin(m_animT*1.2f)*4.0f;
        QFont qf("Courier New",14,QFont::Bold); p.setFont(qf);
        p.setPen(QColor(200,160,255,200));
        p.drawText(QPointF(cx+18,fy),"?");
        break;
    }
    case AssistantState::Typing: {
        QRectF kb(cx-24,cy+42,48,16);
        p.setBrush(QColor(40,60,100,180));
        p.setPen(QPen(QColor(80,120,200,150),1.0));
        p.drawRoundedRect(kb,4,4);
        p.setBrush(QColor(100,150,220,180)); p.setPen(Qt::NoPen);
        for(int i=0;i<5;++i)
            p.drawRoundedRect(QRectF(cx-22+i*10,cy+44,8,5),1,1);
        break;
    }
    default: break;
    }
}

void AssistantCharacter::drawParticles(QPainter &p, float cx, float cy)
{
    Q_UNUSED(cx) Q_UNUSED(cy)
    p.setPen(Qt::NoPen);
    for(const Particle &pt:m_particles){
        if(pt.alpha<=0.0f) continue;
        QColor col(255,220,80,int(pt.alpha*200));
        p.setBrush(col);
        p.drawEllipse(QPointF(pt.x,pt.y),pt.size*0.4f,pt.size*0.4f);
    }
}

// ════════════════════════════════════════════════════════
//  SpeechBubble — CORRIGÉE
// ════════════════════════════════════════════════════════
SpeechBubble::SpeechBubble(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet("background:transparent;");
    hide();

    m_hideTimer = new QTimer(this);
    m_hideTimer->setSingleShot(true);
    connect(m_hideTimer, &QTimer::timeout, this, &SpeechBubble::hideMessage);
}

void SpeechBubble::showMessage(const QString &msg, int durationMs)
{
    m_text = msg;

    // ── Calculer la taille exacte du texte ────────────────
    QFont font("Courier New", 9);
    QFontMetrics fm(font);

    int maxWidth  = 200;                          // largeur max de la bulle
    int padding   = 16;                           // marge intérieure
    int arrowH    = 12;                           // hauteur de la flèche

    // Calculer le rect nécessaire pour le texte avec retour à la ligne
    QRect textRect = fm.boundingRect(
        QRect(0, 0, maxWidth - padding * 2, 1000),
        Qt::AlignCenter | Qt::TextWordWrap,
        m_text
        );

    int bubbleW = qMax(textRect.width()  + padding * 2, 100);
    int bubbleH = textRect.height() + padding * 2;

    // Taille totale = bulle + flèche
    setFixedSize(bubbleW, bubbleH + arrowH);

    show();
    update();

    if (durationMs > 0) {
        m_hideTimer->stop();
        m_hideTimer->start(durationMs);
    }
}

void SpeechBubble::hideMessage()
{
    m_text.clear();
    hide();
}

void SpeechBubble::paintEvent(QPaintEvent *)
{
    if (m_text.isEmpty()) return;

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QFont font("Courier New", 9);
    p.setFont(font);

    int arrowH   = 12;
    int bubbleW  = width();
    int bubbleH  = height() - arrowH;
    int padding  = 8;

    QRectF bubble(0, 0, bubbleW, bubbleH);

    // ── Fond de la bulle ──────────────────────────────────
    p.setBrush(QColor(8, 18, 52, 235));
    p.setPen(QPen(QColor(0, 160, 255, 200), 1.5));
    p.drawRoundedRect(bubble, 12, 12);

    // ── Flèche pointant vers le bas (vers le personnage) ──
    QPainterPath arrow;
    float ax = bubbleW / 2.0f;
    arrow.moveTo(ax - 8, bubbleH);
    arrow.lineTo(ax,     bubbleH + arrowH);
    arrow.lineTo(ax + 8, bubbleH);
    arrow.closeSubpath();
    p.setBrush(QColor(8, 18, 52, 235));
    p.setPen(QPen(QColor(0, 160, 255, 200), 1.5));
    p.drawPath(arrow);

    // ── Texte ─────────────────────────────────────────────
    p.setPen(QColor(200, 230, 255));
    p.setFont(font);
    QRectF textRect(padding, padding, bubbleW - padding * 2, bubbleH - padding * 2);
    p.drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap, m_text);
}

// ════════════════════════════════════════════════════════
//  AssistantWidget
// ════════════════════════════════════════════════════════
AssistantWidget::AssistantWidget(QWidget *parent) : QWidget(parent)
{
    setFixedSize(220, 200);
    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet("background:transparent;");

    // Bulle de dialogue EN HAUT
    m_bubble = new SpeechBubble(this);
    m_bubble->move(10, 0);

    // Personnage EN BAS de la bulle
    m_character = new AssistantCharacter(this);
    m_character->move(60, 88);

    // Bouton masquer/afficher
    m_toggleBtn = new QPushButton("●", this);
    m_toggleBtn->setFixedSize(18, 18);
    m_toggleBtn->move(196, 90);
    m_toggleBtn->setCursor(Qt::PointingHandCursor);
    m_toggleBtn->setStyleSheet(
        "QPushButton{background:rgba(4,14,40,0.7);color:rgba(0,200,255,0.6);"
        "border:1px solid rgba(0,140,220,0.3);border-radius:9px;font-size:8px;}"
        "QPushButton:hover{color:rgb(0,200,255);}");
    connect(m_toggleBtn, &QPushButton::clicked, this, &AssistantWidget::onToggleVisibility);

    // Timer animation
    m_animTimer = new QTimer(this);
    m_animTimer->setInterval(33);
    connect(m_animTimer, &QTimer::timeout, this, &AssistantWidget::onAnimTick);
    m_animTimer->start();

    updatePosition();
    setIdle();
}

void AssistantWidget::setParentSize(int w, int h)
{
    m_parentW = w; m_parentH = h;
    updatePosition();
}

void AssistantWidget::updatePosition()
{
    int x = m_parentW - width() - 20;
    int y = m_parentH - height() - 20;
    move(x, y);
}

void AssistantWidget::onAnimTick()
{
    m_floatT += 0.04f;
    float floatY = std::sin(m_floatT) * 4.0f;
    int baseY = m_parentH - height() - 20;
    if (!m_dragging)
        move(x(), baseY + (int)floatY);
    m_character->setBounce(std::sin(m_floatT * 1.5f) * 0.5f);
    m_character->tick();
}

void AssistantWidget::updateMessage()
{
    switch(m_state) {
    case AssistantState::Idle:
        m_bubble->showMessage("Bonjour !\nPrêt à travailler ?", 4000); break;
    case AssistantState::Studying:
        m_bubble->showMessage("Focus !\nOn y est !", 3000); break;
    case AssistantState::Sleeping:
        m_bubble->showMessage("Zzzz...\nRepose-toi !", 3000); break;
    case AssistantState::Happy:
        m_bubble->showMessage("Bravo !\nSuper boulot !", 3000); break;
    case AssistantState::Cheering:
        m_bubble->showMessage("TERMINÉ !\nExcellent !", 4000); break;
    case AssistantState::Thinking:
        m_bubble->showMessage("Quoi faire\naujourd'hui ?", 3000); break;
    case AssistantState::Proud:
        m_bubble->showMessage("Connecté !\nMode Premium !", 3500); break;
    case AssistantState::Typing:
        m_bubble->showMessage("Je t'écoute...", 2500); break;
    }
}

void AssistantWidget::setIdle()
{
    m_state=AssistantState::Idle;
    m_character->setState(AssistantState::Idle);
    updateMessage();
}
void AssistantWidget::setStudying(const QString &taskName)
{
    m_state=AssistantState::Studying;
    m_character->setState(AssistantState::Studying);
    if(!taskName.isEmpty())
        m_bubble->showMessage("En cours :\n" + taskName, 3000);
    else updateMessage();
}
void AssistantWidget::setSleeping()
{
    m_state=AssistantState::Sleeping;
    m_character->setState(AssistantState::Sleeping);
    updateMessage();
}
void AssistantWidget::setHappy(const QString &taskName)
{
    m_state=AssistantState::Happy;
    m_character->setState(AssistantState::Happy);
    if(!taskName.isEmpty())
        m_bubble->showMessage("Terminée !\n\""+taskName+"\"", 3000);
    else updateMessage();
}
void AssistantWidget::setCheering()
{
    m_state=AssistantState::Cheering;
    m_character->setState(AssistantState::Cheering);
    updateMessage();
}
void AssistantWidget::setThinking()
{
    m_state=AssistantState::Thinking;
    m_character->setState(AssistantState::Thinking);
    updateMessage();
}
void AssistantWidget::setProud(const QString &username)
{
    m_state=AssistantState::Proud;
    m_character->setState(AssistantState::Proud);
    if(!username.isEmpty())
        m_bubble->showMessage("Bienvenue\n" + username + " !", 4000);
    else updateMessage();
}
void AssistantWidget::setTyping()
{
    m_state=AssistantState::Typing;
    m_character->setState(AssistantState::Typing);
    updateMessage();
}

void AssistantWidget::mousePressEvent(QMouseEvent *e)
{
    if(e->button()==Qt::LeftButton){ m_dragging=true; m_dragOffset=e->pos(); }
}
void AssistantWidget::mouseMoveEvent(QMouseEvent *e)
{
    if(m_dragging){
        QPoint np=mapToParent(e->pos()-m_dragOffset);
        move(qBound(0,np.x(),m_parentW-width()),
             qBound(0,np.y(),m_parentH-height()));
    }
}
void AssistantWidget::mouseReleaseEvent(QMouseEvent *) { m_dragging=false; }

void AssistantWidget::onToggleVisibility()
{
    m_visible=!m_visible;
    m_character->setVisible(m_visible);
    if(!m_visible) m_bubble->hide();
    m_toggleBtn->setText(m_visible?"●":"○");
}

void AssistantWidget::paintEvent(QPaintEvent *) {}