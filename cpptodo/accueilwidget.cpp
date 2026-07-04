#include "accueilwidget.h"
#include <QRandomGenerator>
#include <QPainterPath>
#include <QFontDatabase>
#include <QtMath>

// ─────────────────────────────────────────────
//  Constructor / Destructor
// ─────────────────────────────────────────────
AccueilWidget::AccueilWidget(QWidget *parent)
    : QWidget(parent)
    , m_time(0.0f)
    , m_loadProgress(0)
    , m_titleOpacity(0.0f)
    , m_subtitleOpacity(0.0f)
    , m_glowPulse(0.0f)
    , m_loadingDone(false)
    , m_frame(0)
    , m_msgIndex(0)
    , m_msgTimer(0)
{
    setMinimumSize(900, 600);
    setAttribute(Qt::WA_OpaquePaintEvent);

    m_loadingMessages = {
        "Initialisation du moteur...",
        "Chargement des modules...",
        "Synchronisation des données...",
        "Préparation de l'interface...",
        "Bienvenue dans ASIS"
    };

    // Init scene elements
    initStars();
    initParticles();
    initRings();

    // Main animation timer — 60 fps
    m_animTimer = new QTimer(this);
    connect(m_animTimer, &QTimer::timeout, this, &AccueilWidget::updateAnimation);
    m_animTimer->start(16);

    // Loading bar animation: 0 → 100 over 4 seconds
    m_loadAnim = new QPropertyAnimation(this, "loadProgress");
    m_loadAnim->setDuration(4000);
    m_loadAnim->setStartValue(0);
    m_loadAnim->setEndValue(100);
    m_loadAnim->setEasingCurve(QEasingCurve::InOutCubic);
    connect(m_loadAnim, &QPropertyAnimation::finished,
            this,       &AccueilWidget::onLoadingFinished);
    m_loadAnim->start();
}

AccueilWidget::~AccueilWidget() {}

// ─────────────────────────────────────────────
//  Init helpers
// ─────────────────────────────────────────────
void AccueilWidget::initStars()
{
    auto *rng = QRandomGenerator::global();
    m_stars.resize(200);
    for (auto &s : m_stars) {
        s.pos          = { (float)rng->bounded(1920), (float)rng->bounded(1080) };
        s.brightness   = rng->bounded(100) / 100.0f;
        s.twinkleSpeed = 0.5f + rng->bounded(100) / 50.0f;
        s.twinklePhase = rng->bounded(628) / 100.0f;
        s.size         = 0.5f + rng->bounded(100) / 50.0f;
    }
}

void AccueilWidget::initParticles()
{
    auto *rng = QRandomGenerator::global();
    m_particles.resize(120);
    for (auto &p : m_particles) {
        p.pos     = { (float)rng->bounded(1920), (float)rng->bounded(1080) };
        float angle = rng->bounded(628) / 100.0f;
        float speed = 0.2f + rng->bounded(100) / 200.0f;
        p.vel     = { std::cos(angle) * speed, std::sin(angle) * speed };
        p.size    = 1.0f + rng->bounded(100) / 33.0f;
        p.opacity = 0.3f + rng->bounded(100) / 143.0f;
        p.hue     = 180.0f + rng->bounded(80);  // cyan → blue spectrum
        p.maxLife = 180.0f + rng->bounded(180);
        p.life    = rng->bounded((int)p.maxLife);
    }
}

void AccueilWidget::initRings()
{
    m_rings.resize(5);
    QColor colors[] = {
        QColor(0, 220, 255, 80),
        QColor(100, 140, 255, 60),
        QColor(0, 255, 200, 50),
        QColor(160, 80, 255, 40),
        QColor(0, 200, 255, 30)
    };
    float radii[] = { 80, 140, 200, 260, 320 };
    float speeds[] = { 0.3f, -0.2f, 0.15f, -0.25f, 0.1f };

    for (int i = 0; i < 5; ++i) {
        m_rings[i].center       = { 0.5f, 0.5f };   // normalised
        m_rings[i].radius       = radii[i];
        m_rings[i].targetRadius = radii[i];
        m_rings[i].angle        = 0;
        m_rings[i].speed        = speeds[i];
        m_rings[i].color        = colors[i];
    }
}

// ─────────────────────────────────────────────
//  Update slot
// ─────────────────────────────────────────────
void AccueilWidget::updateAnimation()
{
    m_time  += 0.016f;
    m_frame += 1;
    m_glowPulse = 0.5f + 0.5f * std::sin(m_time * 2.0f);

    // Fade-in title after 0.8 s
    if (m_time > 0.8f)
        m_titleOpacity = qMin(1.0f, m_titleOpacity + 0.018f);
    if (m_time > 1.6f)
        m_subtitleOpacity = qMin(1.0f, m_subtitleOpacity + 0.012f);

    // Cycle loading messages
    m_msgTimer++;
    if (m_msgTimer > 55) {
        m_msgTimer = 0;
        m_msgIndex = qMin(m_msgIndex + 1, m_loadingMessages.size() - 1);
    }

    updateStars();
    updateParticles();
    updateRings();
    update();
}

void AccueilWidget::updateStars()
{
    for (auto &s : m_stars)
        s.twinklePhase += s.twinkleSpeed * 0.016f;
}

void AccueilWidget::updateParticles()
{
    auto *rng = QRandomGenerator::global();
    for (auto &p : m_particles) {
        p.pos += p.vel;
        p.life++;

        // Wrap around
        if (p.pos.x() < 0)   p.pos.setX(width());
        if (p.pos.x() > width()) p.pos.setX(0);
        if (p.pos.y() < 0)   p.pos.setY(height());
        if (p.pos.y() > height()) p.pos.setY(0);

        // Respawn dead particles
        if (p.life > p.maxLife) {
            p.pos     = { (float)rng->bounded(width()), (float)rng->bounded(height()) };
            float a   = rng->bounded(628) / 100.0f;
            float sp  = 0.2f + rng->bounded(100) / 200.0f;
            p.vel     = { std::cos(a) * sp, std::sin(a) * sp };
            p.life    = 0;
            p.maxLife = 180.0f + rng->bounded(180);
            p.hue     = 180.0f + rng->bounded(80);
        }
    }
}

void AccueilWidget::updateRings()
{
    for (auto &r : m_rings)
        r.angle += r.speed * 0.016f;
}

// ─────────────────────────────────────────────
//  Property setter → triggers repaint
// ─────────────────────────────────────────────
void AccueilWidget::setLoadProgress(int value)
{
    m_loadProgress = value;
    update();
}

void AccueilWidget::onLoadingFinished()
{
    m_loadingDone = true;
    QTimer::singleShot(800, this, &AccueilWidget::loadingComplete);
}

void AccueilWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    m_center = { width() / 2.0f, height() / 2.0f };

    // Update loading bar geometry
    float bw = width() * 0.55f;
    float bh = 6.0f;
    m_barRect = QRectF(m_center.x() - bw / 2, height() * 0.72f, bw, bh);
}

// ─────────────────────────────────────────────
//  Paint
// ─────────────────────────────────────────────
void AccueilWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setRenderHint(QPainter::TextAntialiasing, true);
    p.setRenderHint(QPainter::SmoothPixmapTransform, true);

    drawBackground(p);
    drawStars(p);
    drawNebulaEffect(p);
    drawGrid(p);
    drawRings(p);
    drawParticles(p);
    drawCentralGlow(p);
    drawTitle(p);
    drawSubtitle(p);
    drawLoadingBar(p);
    drawLoadingText(p);
    drawScanlines(p);
}

// ── Background ──────────────────────────────
void AccueilWidget::drawBackground(QPainter &p)
{
    QLinearGradient grad(0, 0, 0, height());
    grad.setColorAt(0.0, QColor(2, 4, 18));
    grad.setColorAt(0.5, QColor(5, 10, 35));
    grad.setColorAt(1.0, QColor(2, 6, 22));
    p.fillRect(rect(), grad);
}

// ── Stars ───────────────────────────────────
void AccueilWidget::drawStars(QPainter &p)
{
    p.save();
    for (const auto &s : m_stars) {
        float tw = 0.5f + 0.5f * std::sin(s.twinklePhase);
        float alpha = s.brightness * tw;
        QColor c(200, 220, 255, (int)(alpha * 200));
        p.setPen(Qt::NoPen);
        p.setBrush(c);

        // normalised → actual coords
        float x = s.pos.x() / 1920.0f * width();
        float y = s.pos.y() / 1080.0f * height();
        p.drawEllipse(QPointF(x, y), s.size * tw, s.size * tw);
    }
    p.restore();
}

// ── Nebula soft glow blobs ───────────────────
void AccueilWidget::drawNebulaEffect(QPainter &p)
{
    p.save();
    p.setCompositionMode(QPainter::CompositionMode_Plus);

    auto drawBlob = [&](float nx, float ny, float r, QColor c) {
        QRadialGradient g(width() * nx, height() * ny, r);
        c.setAlpha(0);
        g.setColorAt(1.0, c);
        c.setAlpha(30);
        g.setColorAt(0.0, c);
        p.setBrush(g);
        p.setPen(Qt::NoPen);
        p.drawEllipse(QPointF(width() * nx, height() * ny), r, r);
    };

    float t = m_time;
    drawBlob(0.5f + 0.05f * std::sin(t * 0.3f), 0.42f, 320, QColor(0, 150, 255));
    drawBlob(0.3f + 0.04f * std::cos(t * 0.2f), 0.55f, 200, QColor(80, 0, 200));
    drawBlob(0.7f + 0.03f * std::sin(t * 0.25f),0.45f, 180, QColor(0, 200, 180));

    p.restore();
}

// ── Perspective grid ─────────────────────────
void AccueilWidget::drawGrid(QPainter &p)
{
    p.save();
    float horizon = height() * 0.55f;
    float vanishX = width() / 2.0f;

    QPen pen(QColor(0, 140, 255, 18), 1);
    p.setPen(pen);

    // Horizontal lines
    int hLines = 18;
    for (int i = 0; i <= hLines; ++i) {
        float t     = (float)i / hLines;
        float y     = horizon + (height() - horizon) * t * t;
        float alpha = (int)(t * 38);
        pen.setColor(QColor(0, 160, 255, alpha));
        p.setPen(pen);
        p.drawLine(QPointF(0, y), QPointF(width(), y));
    }

    // Vertical lines diverging from vanishing point
    int vLines = 22;
    for (int i = -vLines / 2; i <= vLines / 2; ++i) {
        float spread = width() * 0.9f;
        float bx     = vanishX + i * (spread / vLines);
        int alpha    = (int)(22 - std::abs(i) * 0.5f);
        if (alpha < 4) alpha = 4;
        pen.setColor(QColor(0, 140, 255, alpha));
        p.setPen(pen);
        p.drawLine(QPointF(vanishX, horizon), QPointF(bx, height()));
    }
    p.restore();
}

// ── Orbital rings ────────────────────────────
void AccueilWidget::drawRings(QPainter &p)
{
    p.save();
    p.setCompositionMode(QPainter::CompositionMode_Plus);

    float cx = m_center.x();
    float cy = m_center.y() * 0.82f;

    for (int ri = 0; ri < m_rings.size(); ++ri) {
        const Ring &r = m_rings[ri];
        float rx = r.radius * 1.8f;   // elliptical
        float ry = r.radius * 0.5f;

        // Draw dashed arc using small segments
        int segs = 80;
        bool dash = false;
        for (int i = 0; i < segs; ++i) {
            float a0 = r.angle + (float)i       / segs * M_PI * 2;
            float a1 = r.angle + (float)(i + 1) / segs * M_PI * 2;
            dash = (i % 5 != 4);  // dashed pattern
            if (!dash) continue;

            float x0 = cx + std::cos(a0) * rx;
            float y0 = cy + std::sin(a0) * ry;
            float x1 = cx + std::cos(a1) * rx;
            float y1 = cy + std::sin(a1) * ry;

            // Fade bottom half
            float fadeFactor = 0.4f + 0.6f * (0.5f + 0.5f * std::sin(a0));
            QColor c = r.color;
            c.setAlpha((int)(c.alpha() * fadeFactor));
            p.setPen(QPen(c, 1.2f));
            p.drawLine(QPointF(x0, y0), QPointF(x1, y1));
        }

        // Bright node travelling around ring
        float na = r.angle + m_time * r.speed * 2.5f;
        float nx = cx + std::cos(na) * rx;
        float ny = cy + std::sin(na) * ry;
        QRadialGradient ng(nx, ny, 8);
        QColor nc = r.color; nc.setAlpha(255);
        ng.setColorAt(0, nc);
        nc.setAlpha(0);
        ng.setColorAt(1, nc);
        p.setBrush(ng);
        p.setPen(Qt::NoPen);
        p.drawEllipse(QPointF(nx, ny), 8, 8);
    }
    p.restore();
}

// ── Floating particles ───────────────────────
void AccueilWidget::drawParticles(QPainter &p)
{
    p.save();
    p.setCompositionMode(QPainter::CompositionMode_Plus);
    p.setPen(Qt::NoPen);

    for (const auto &pt : m_particles) {
        float lifeRatio = pt.life / pt.maxLife;
        float fade = lifeRatio < 0.1f  ? lifeRatio * 10.0f
                     : lifeRatio > 0.85f ? (1.0f - lifeRatio) / 0.15f
                                         : 1.0f;
        QColor c = QColor::fromHsvF(pt.hue / 360.0f, 0.8f, 1.0f, pt.opacity * fade);
        p.setBrush(c);
        p.drawEllipse(pt.pos, pt.size, pt.size);
    }
    p.restore();
}

// ── Central glow orb ─────────────────────────
void AccueilWidget::drawCentralGlow(QPainter &p)
{
    p.save();
    p.setCompositionMode(QPainter::CompositionMode_Plus);

    float cx = m_center.x();
    float cy = m_center.y() * 0.82f;
    float pulse = 0.85f + 0.15f * m_glowPulse;
    float r = 90.0f * pulse;

    QRadialGradient g(cx, cy, r);
    g.setColorAt(0.0, QColor(80, 200, 255, 120));
    g.setColorAt(0.3, QColor(40, 120, 255, 60));
    g.setColorAt(0.7, QColor(20, 60, 200, 20));
    g.setColorAt(1.0, QColor(0, 0, 0, 0));
    p.setBrush(g);
    p.setPen(Qt::NoPen);
    p.drawEllipse(QPointF(cx, cy), r, r);

    // Inner bright core
    float cr = 12.0f * pulse;
    QRadialGradient cg(cx, cy, cr);
    cg.setColorAt(0.0, QColor(255, 255, 255, 200));
    cg.setColorAt(0.4, QColor(120, 220, 255, 140));
    cg.setColorAt(1.0, QColor(0, 0, 0, 0));
    p.setBrush(cg);
    p.drawEllipse(QPointF(cx, cy), cr, cr);

    // Cross flare
    QPen fp(QColor(160, 230, 255, (int)(60 * pulse)), 1);
    p.setPen(fp);
    float fl = 50.0f * pulse;
    p.drawLine(QPointF(cx - fl, cy), QPointF(cx + fl, cy));
    p.drawLine(QPointF(cx, cy - fl), QPointF(cx, cy + fl));

    p.restore();
}

// ── Title ─────────────────────────────────────
void AccueilWidget::drawTitle(QPainter &p)
{
    if (m_titleOpacity <= 0.01f) return;
    p.save();
    p.setOpacity(m_titleOpacity);

    // Main title
    QFont titleFont("Courier New", 48, QFont::Bold);
    titleFont.setLetterSpacing(QFont::AbsoluteSpacing, 8);
    p.setFont(titleFont);

    QString title = "ASIS";
    QFontMetrics fm(titleFont);
    int tw = fm.horizontalAdvance(title);
    float tx = (width() - tw) / 2.0f;
    float ty = height() * 0.52f;

    // Glow shadow
    for (int i = 3; i >= 1; --i) {
        p.setPen(QColor(0, 180, 255, 40 * (4 - i)));
        p.drawText(QPointF(tx - i * 2, ty + i * 2), title);
        p.drawText(QPointF(tx + i * 2, ty + i * 2), title);
    }

    // Main text — gradient simulation via two passes
    p.setPen(QColor(0, 200, 255, 255));
    p.drawText(QPointF(tx, ty), title);
    p.setPen(QColor(255, 255, 255, 80));
    p.drawText(QPointF(tx, ty), title);

    p.restore();
}

// ── Subtitle ─────────────────────────────────
void AccueilWidget::drawSubtitle(QPainter &p)
{
    if (m_subtitleOpacity <= 0.01f) return;
    p.save();
    p.setOpacity(m_subtitleOpacity);

    QFont subFont("Courier New", 13);
    subFont.setLetterSpacing(QFont::AbsoluteSpacing, 4);
    p.setFont(subFont);

    QString sub = "PRODUCTIVITÉ · FOCUS · PLANNING INTELLIGENT";
    QFontMetrics fm(subFont);
    int sw = fm.horizontalAdvance(sub);
    float sx = (width() - sw) / 2.0f;
    float sy = height() * 0.57f;

    p.setPen(QColor(100, 200, 255, 180));
    p.drawText(QPointF(sx, sy), sub);

    // Decorative line
    float lw = sw * 0.6f;
    float lx = (width() - lw) / 2.0f;
    float ly = sy + 12.0f;
    QLinearGradient lg(lx, ly, lx + lw, ly);
    lg.setColorAt(0.0, QColor(0, 0, 0, 0));
    lg.setColorAt(0.3, QColor(0, 180, 255, 120));
    lg.setColorAt(0.7, QColor(0, 180, 255, 120));
    lg.setColorAt(1.0, QColor(0, 0, 0, 0));
    p.setPen(QPen(QBrush(lg), 1));
    p.drawLine(QPointF(lx, ly), QPointF(lx + lw, ly));

    p.restore();
}

// ── Loading bar ──────────────────────────────
void AccueilWidget::drawLoadingBar(QPainter &p)
{
    if (m_barRect.isEmpty()) return;
    p.save();

    float bw   = m_barRect.width();
    float bh   = m_barRect.height();
    float bx   = m_barRect.x();
    float by   = m_barRect.y();
    float fill = bw * m_loadProgress / 100.0f;

    // Background track
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(255, 255, 255, 18));
    p.drawRoundedRect(m_barRect, bh / 2, bh / 2);

    // Filled portion — gradient
    if (fill > 0) {
        QLinearGradient fg(bx, by, bx + fill, by);
        fg.setColorAt(0.0, QColor(0, 140, 255, 220));
        fg.setColorAt(0.5, QColor(0, 220, 255, 255));
        fg.setColorAt(1.0, QColor(100, 255, 240, 255));
        p.setBrush(fg);
        p.drawRoundedRect(QRectF(bx, by, fill, bh), bh / 2, bh / 2);

        // Shimmer moving left to right
        float shimPos = bx + fill * ((m_frame % 60) / 60.0f);
        QLinearGradient sh(shimPos - 20, by, shimPos + 20, by);
        sh.setColorAt(0.0, QColor(255, 255, 255, 0));
        sh.setColorAt(0.5, QColor(255, 255, 255, 80));
        sh.setColorAt(1.0, QColor(255, 255, 255, 0));
        p.setBrush(sh);
        p.setClipRect(QRectF(bx, by, fill, bh));
        p.drawRoundedRect(QRectF(bx, by, fill, bh), bh / 2, bh / 2);
        p.setClipping(false);

        // Bright leading edge
        QRadialGradient eg(bx + fill, by + bh / 2, 14);
        eg.setColorAt(0.0, QColor(200, 255, 255, 200));
        eg.setColorAt(1.0, QColor(0, 0, 0, 0));
        p.setBrush(eg);
        p.drawEllipse(QPointF(bx + fill, by + bh / 2), 14, 14);
    }

    // Border
    p.setBrush(Qt::NoBrush);
    p.setPen(QPen(QColor(0, 160, 255, 60), 1));
    p.drawRoundedRect(m_barRect, bh / 2, bh / 2);

    // Percentage text
    QFont pctFont("Courier New", 10);
    pctFont.setLetterSpacing(QFont::AbsoluteSpacing, 2);
    p.setFont(pctFont);
    QString pct = QString("%1%").arg(m_loadProgress);
    p.setPen(QColor(0, 200, 255, 180));
    p.drawText(QPointF(bx + bw / 2 - 14, by - 8), pct);

    p.restore();
}

// ── Loading message ───────────────────────────
void AccueilWidget::drawLoadingText(QPainter &p)
{
    p.save();
    QFont msgFont("Courier New", 10);
    msgFont.setLetterSpacing(QFont::AbsoluteSpacing, 2);
    p.setFont(msgFont);

    QString msg = m_loadingMessages[m_msgIndex];
    if (!m_loadingDone) {
        // blinking cursor
        if ((m_frame / 30) % 2 == 0) msg += "_";
    }

    QFontMetrics fm(msgFont);
    int mw = fm.horizontalAdvance(msg);
    float mx = (width() - mw) / 2.0f;
    float my = m_barRect.y() + 28.0f;

    p.setPen(QColor(80, 180, 255, 160));
    p.drawText(QPointF(mx, my), msg);
    p.restore();
}

// ── CRT scanlines overlay ─────────────────────
void AccueilWidget::drawScanlines(QPainter &p)
{
    p.save();
    p.setCompositionMode(QPainter::CompositionMode_Multiply);
    for (int y = 0; y < height(); y += 3) {
        p.setPen(QColor(0, 0, 0, 12));
        p.drawLine(0, y, width(), y);
    }
    p.restore();
}
