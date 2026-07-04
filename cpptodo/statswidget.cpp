#include "statswidget.h"
#include "authmanager.h"
#include <QPainterPath>
#include <QLinearGradient>
#include <QRadialGradient>
#include <QScrollArea>
#include <QDateTime>

static const QColor C_CYAN (0, 200, 255);
static const QColor C_GREEN(60, 220, 100);
static const QColor C_AMBER(255, 180, 0);
static const QColor C_RED  (255, 80, 80);

// ════════════════════════════════════════════════════════
//  MiniBarChart
// ════════════════════════════════════════════════════════
MiniBarChart::MiniBarChart(QWidget *parent) : QWidget(parent)
{
    setMinimumHeight(160);
}

void MiniBarChart::setData(const QVector<DailyStats> &data)
{
    m_daily = data;
    update();
}

void MiniBarChart::setHourlyData(const QVector<HourlyProductivity> &data)
{
    m_hourly = data;
    update();
}

void MiniBarChart::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QRectF r = rect();

    // Background
    p.setBrush(QColor(4, 12, 36, 200));
    p.setPen(QPen(QColor(0, 80, 140, 80), 1));
    p.drawRoundedRect(r.adjusted(1,1,-1,-1), 10, 10);

    const int padding = 20;
    const int barArea = r.width() - padding * 2;
    const int chartH  = r.height() - padding * 2 - 20; // 20 for labels

    if (m_hourlyMode) {
        // ── Hourly mode ──
        if (m_hourly.isEmpty()) {
            p.setPen(QColor(80,130,190));
            p.setFont(QFont("Courier New", 10));
            p.drawText(r, Qt::AlignCenter, "Pas encore de données");
            return;
        }

        int count = 24;
        double maxScore = 100.0;
        double barW = barArea / double(count);

        for (int i = 0; i < count; ++i) {
            double score = 0;
            for (auto &h : m_hourly)
                if (h.hour == i) { score = h.avgScore; break; }

            double barH = (score / maxScore) * chartH;
            QRectF bar(padding + i * barW + barW * 0.15,
                       padding + chartH - barH,
                       barW * 0.7, barH);

            // Color by score
            QColor col;
            if (score >= 66)      col = C_GREEN;
            else if (score >= 33) col = C_AMBER;
            else                  col = C_CYAN;

            QLinearGradient grad(bar.topLeft(), bar.bottomLeft());
            grad.setColorAt(0, col);
            grad.setColorAt(1, QColor(col.red(), col.green(), col.blue(), 60));
            p.setBrush(grad);
            p.setPen(Qt::NoPen);
            p.drawRoundedRect(bar, 2, 2);
        }

        // Hour labels (every 4h)
        p.setPen(QColor(80, 130, 190));
        p.setFont(QFont("Courier New", 8));
        for (int i = 0; i <= 24; i += 4) {
            double x = padding + i * (barArea / 24.0);
            p.drawText(QRectF(x - 10, r.height() - 18, 24, 14),
                       Qt::AlignCenter, QString::number(i) + "h");
        }

    } else {
        // ── Daily mode ──
        if (m_daily.isEmpty()) {
            p.setPen(QColor(80,130,190));
            p.setFont(QFont("Courier New", 10));
            p.drawText(r, Qt::AlignCenter, "Pas encore de données");
            return;
        }

        int count = m_daily.size();
        double maxScore = 100.0;
        double barW = barArea / double(count);

        for (int i = 0; i < count; ++i) {
            double score = m_daily[i].productivityScore;
            double barH  = (score / maxScore) * chartH;

            QRectF bar(padding + i * barW + barW * 0.1,
                       padding + chartH - barH,
                       barW * 0.8, barH);

            QColor col = (score >= 66) ? C_GREEN : (score >= 33) ? C_AMBER : C_CYAN;

            QLinearGradient grad(bar.topLeft(), bar.bottomLeft());
            grad.setColorAt(0, col);
            grad.setColorAt(1, QColor(col.red(), col.green(), col.blue(), 50));
            p.setBrush(grad);
            p.setPen(Qt::NoPen);
            p.drawRoundedRect(bar, 3, 3);

            // Day label
            p.setPen(QColor(80,130,190));
            p.setFont(QFont("Courier New", 8));
            QString day = m_daily[i].date.toString("ddd");
            p.drawText(QRectF(padding + i * barW, r.height() - 18, barW, 14),
                       Qt::AlignCenter, day);
        }
    }

    // Title
    p.setPen(QColor(0, 180, 255, 160));
    p.setFont(QFont("Courier New", 9));
    QString title = m_hourlyMode ? "PRODUCTIVITÉ PAR HEURE" : "PRODUCTIVITÉ 7 JOURS";
    p.drawText(QRectF(padding, 6, r.width() - padding*2, 14), Qt::AlignCenter, title);
}

// ════════════════════════════════════════════════════════
//  StatCard
// ════════════════════════════════════════════════════════
StatCard::StatCard(const QString &icon, const QString &label,
                   const QString &value, QColor accent, QWidget *parent)
    : QWidget(parent), m_accent(accent)
{
    setMinimumSize(140, 100);
    setMaximumHeight(110);

    QVBoxLayout *vl = new QVBoxLayout(this);
    vl->setContentsMargins(14, 10, 14, 10);
    vl->setSpacing(4);

    QLabel *iconLbl = new QLabel(icon + "  " + label, this);
    iconLbl->setStyleSheet(QString(
                               "color: rgba(%1,%2,%3,0.8); font-family:'Courier New';"
                               "font-size:10px; letter-spacing:2px; background:transparent;"
                               ).arg(accent.red()).arg(accent.green()).arg(accent.blue()));

    m_valueLabel = new QLabel(value, this);
    m_valueLabel->setStyleSheet(
        "color: rgb(200,230,255); font-family:'Courier New';"
        "font-size:18px; font-weight:bold; background:transparent;"
        );

    vl->addWidget(iconLbl);
    vl->addWidget(m_valueLabel);
    vl->addStretch();
}

void StatCard::setValue(const QString &v)
{
    m_valueLabel->setText(v);
}

void StatCard::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QRectF r = QRectF(rect()).adjusted(1,1,-1,-1);
    p.setBrush(QColor(4, 14, 42, 200));
    p.setPen(QPen(QColor(m_accent.red(), m_accent.green(), m_accent.blue(), 60), 1));
    p.drawRoundedRect(r, 10, 10);

    // Top accent line
    p.setPen(QPen(m_accent, 2));
    p.drawLine(QPointF(r.left()+10, r.top()+1),
               QPointF(r.left()+50, r.top()+1));
}

// ════════════════════════════════════════════════════════
//  StatsWidget
// ════════════════════════════════════════════════════════
StatsWidget::StatsWidget(QWidget *parent) : QWidget(parent)
{
    setMinimumSize(760, 560);

    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    root->addWidget(buildHeader());

    QScrollArea *scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    scroll->setStyleSheet("QScrollArea { border:none; background:transparent; }"
                          "QScrollBar:vertical { width:6px; background:transparent; }"
                          "QScrollBar::handle:vertical { background:rgba(0,150,220,0.4); border-radius:3px; }");

    QWidget *content = new QWidget;
    content->setStyleSheet("background:transparent;");
    QVBoxLayout *cl = new QVBoxLayout(content);
    cl->setContentsMargins(28, 20, 28, 28);
    cl->setSpacing(18);

    cl->addWidget(buildKpiRow());
    cl->addWidget(buildChartSection());
    cl->addWidget(buildInsightSection());
    cl->addStretch();

    scroll->setWidget(content);
    root->addWidget(scroll, 1);
    // ── Background animation ──────────────────────────────
    m_animTime = 0.f;
    QTimer *bgAnim = new QTimer(this);
    bgAnim->setInterval(50);
    connect(bgAnim, &QTimer::timeout, this, [this](){ m_animTime += 0.05f; update(); });
    bgAnim->start();
}

QWidget *StatsWidget::buildHeader()
{
    QWidget *w = new QWidget(this);
    w->setFixedHeight(60);
    w->setStyleSheet("background:rgba(6,14,38,0.95);"
                     "border-bottom:1px solid rgba(0,120,200,0.30);");
    QHBoxLayout *lay = new QHBoxLayout(w);
    lay->setContentsMargins(24, 0, 24, 0);

    QLabel *title = new QLabel("📊  STATISTIQUES & ANALYSE", w);
    title->setStyleSheet("color:rgb(0,200,255);font-family:'Courier New';"
                         "font-size:15px;font-weight:bold;letter-spacing:4px;"
                         "background:transparent;");
    lay->addWidget(title);
    lay->addStretch();

    QPushButton *back = new QPushButton("← RETOUR", w);
    back->setFixedSize(110, 36);
    back->setCursor(Qt::PointingHandCursor);
    back->setStyleSheet(
        "QPushButton{background:rgba(4,14,40,0.9);color:rgb(0,200,255);"
        "border:1px solid rgba(0,160,220,0.5);border-radius:8px;"
        "font-family:'Courier New';font-size:11px;font-weight:bold;letter-spacing:2px;}"
        "QPushButton:hover{background:rgba(0,50,100,0.9);}");
    connect(back, &QPushButton::clicked, this, &StatsWidget::goBack);
    lay->addWidget(back);

    return w;
}

QWidget *StatsWidget::buildKpiRow()
{
    QWidget *w = new QWidget(this);
    w->setStyleSheet("background:transparent;");
    QHBoxLayout *hl = new QHBoxLayout(w);
    hl->setContentsMargins(0,0,0,0);
    hl->setSpacing(12);

    m_cardScore    = new StatCard("⚡", "SCORE AUJOURD'HUI", "—",    C_CYAN,  w);
    m_cardSessions = new StatCard("🔁", "SESSIONS / SEMAINE", "—",   C_GREEN, w);
    m_cardTime     = new StatCard("⏱", "TEMPS / SEMAINE",    "—",   C_AMBER, w);
    m_cardBestHour = new StatCard("🏆", "MEILLEURE HEURE",    "—",   C_RED,   w);

    hl->addWidget(m_cardScore);
    hl->addWidget(m_cardSessions);
    hl->addWidget(m_cardTime);
    hl->addWidget(m_cardBestHour);

    return w;
}

QWidget *StatsWidget::buildChartSection()
{
    QWidget *w = new QWidget(this);
    w->setStyleSheet("background:transparent;");
    QVBoxLayout *vl = new QVBoxLayout(w);
    vl->setContentsMargins(0,0,0,0);
    vl->setSpacing(10);

    // Toggle button
    m_toggleChartBtn = new QPushButton("VOIR PAR HEURE ▶", w);
    m_toggleChartBtn->setFixedHeight(34);
    m_toggleChartBtn->setCursor(Qt::PointingHandCursor);
    m_toggleChartBtn->setStyleSheet(
        "QPushButton{background:rgba(0,60,120,0.5);color:rgb(0,180,255);"
        "border:1px solid rgba(0,140,220,0.4);border-radius:8px;"
        "font-family:'Courier New';font-size:10px;letter-spacing:2px;}"
        "QPushButton:hover{background:rgba(0,80,160,0.6);}");
    connect(m_toggleChartBtn, &QPushButton::clicked, this, [this]() {
        m_showHourly = !m_showHourly;
        m_barDaily->setMode(!m_showHourly);
        m_barHourly->setMode(m_showHourly);
        m_barDaily->setVisible(!m_showHourly);
        m_barHourly->setVisible(m_showHourly);
        m_toggleChartBtn->setText(m_showHourly ? "◀ VOIR PAR JOUR" : "VOIR PAR HEURE ▶");
    });

    m_barDaily  = new MiniBarChart(w);
    m_barHourly = new MiniBarChart(w);
    m_barHourly->setMode(true);
    m_barHourly->setVisible(false);

    vl->addWidget(m_toggleChartBtn, 0, Qt::AlignRight);
    vl->addWidget(m_barDaily);
    vl->addWidget(m_barHourly);

    return w;
}

QWidget *StatsWidget::buildInsightSection()
{
    QWidget *w = new QWidget(this);
    w->setStyleSheet(
        "background:rgba(4,14,42,0.85);"
        "border:1px solid rgba(0,120,200,0.25);"
        "border-radius:12px;");

    QVBoxLayout *vl = new QVBoxLayout(w);
    vl->setContentsMargins(20, 16, 20, 16);
    vl->setSpacing(8);

    QLabel *title = new QLabel("💡  ANALYSE IA", w);
    title->setStyleSheet("color:rgb(255,180,0);font-family:'Courier New';"
                         "font-size:12px;font-weight:bold;letter-spacing:3px;"
                         "background:transparent;");

    m_insightLabel = new QLabel("Connectez-vous pour accéder aux analyses.", w);
    m_insightLabel->setWordWrap(true);
    m_insightLabel->setStyleSheet("color:rgba(160,210,255,0.9);font-family:'Courier New';"
                                  "font-size:11px;line-height:1.6;background:transparent;");

    vl->addWidget(title);
    vl->addWidget(m_insightLabel);

    return w;
}

// ════════════════════════════════════════════════════════
//  refresh — call when widget becomes visible
// ════════════════════════════════════════════════════════
void StatsWidget::refresh()
{
    updateKpis();
    updateCharts();
    updateInsights();
}

void StatsWidget::updateKpis()
{
    if (!AuthManager::instance().isLoggedIn()) {
        m_cardScore->setValue("—");
        m_cardSessions->setValue("—");
        m_cardTime->setValue("—");
        m_cardBestHour->setValue("—");
        return;
    }

    double score = SessionManager::instance().getTodayProductivityScore();
    m_cardScore->setValue(QString::number(static_cast<int>(score)) + " / 100");

    int sessions = SessionManager::instance().getTotalSessionsThisWeek();
    m_cardSessions->setValue(QString::number(sessions));

    int secs = SessionManager::instance().getTotalTimeThisWeek();
    int h = secs / 3600, m = (secs % 3600) / 60;
    m_cardTime->setValue(QString("%1h%2").arg(h).arg(m, 2, 10, QChar('0')));

    m_cardBestHour->setValue(SessionManager::instance().getMostProductiveHour());
}

void StatsWidget::updateCharts()
{
    auto daily  = SessionManager::instance().getDailyStats(7);
    auto hourly = SessionManager::instance().getHourlyProductivity();
    m_barDaily->setData(daily);
    m_barHourly->setHourlyData(hourly);
}

void StatsWidget::updateInsights()
{
    if (!AuthManager::instance().isLoggedIn()) {
        m_insightLabel->setText("Connectez-vous pour accéder aux analyses.");
        return;
    }

    QVector<int> bestHours = SessionManager::instance().getBestHoursForHardTasks();
    double score = SessionManager::instance().getTodayProductivityScore();
    int sessions = SessionManager::instance().getTotalSessionsThisWeek();

    QString insight;

    if (sessions == 0) {
        insight = "Aucune session enregistrée cette semaine.\n"
                  "Commencez une session timer pour collecter vos données de productivité !";
    } else {
        insight = QString("📈  Score de productivité aujourd'hui : %1/100\n\n")
                      .arg(static_cast<int>(score));

        if (!bestHours.isEmpty()) {
            insight += "🏆  Vos meilleures heures de travail : ";
            for (int i = 0; i < bestHours.size(); ++i) {
                if (i > 0) insight += ", ";
                insight += QString("%1h00").arg(bestHours[i], 2, 10, QChar('0'));
            }
            insight += "\n\n";
            insight += "💡  Conseil : planifiez vos tâches difficiles (rouge) pendant ces créneaux "
                       "pour maximiser votre efficacité. Les tâches faciles peuvent être faites "
                       "à n'importe quel moment.";
        }

        if (score >= 70) {
            insight += "\n\n✅  Excellente productivité aujourd'hui ! Continuez ainsi.";
        } else if (score >= 40) {
            insight += "\n\n⚡  Productivité correcte. Essayez le mode Pomodoro pour améliorer "
                       "votre concentration.";
        } else {
            insight += "\n\n⚠️  Productivité faible. Faites une pause ou changez d'environnement.";
        }
    }

    m_insightLabel->setText(insight);
}

// ════════════════════════════════════════════════════════
//  paintEvent
// ════════════════════════════════════════════════════════
void StatsWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // ── Deep blue base ────────────────────────────────────
    QLinearGradient bg(0,0,0,height());
    bg.setColorAt(0.00, QColor(2,  5, 20));
    bg.setColorAt(0.50, QColor(3, 10, 30));
    bg.setColorAt(1.00, QColor(1,  4, 18));
    p.fillRect(rect(), bg);

    // ── Constellation star field ──────────────────────────
    p.save();
    p.setCompositionMode(QPainter::CompositionMode_Plus);

    // Stars (deterministic positions, no QVector needed)
    struct StarDef { float x, y, size; };
    // 50 stars spread across the widget using golden ratio
    p.setPen(Qt::NoPen);
    for (int i = 0; i < 55; ++i) {
        float sx = std::fmod(i * 173.7f, 1000.f) / 1000.f * width();
        float sy = std::fmod(i *  97.3f + i*i*0.03f, 1000.f) / 1000.f * height();
        float tw = 0.4f + 0.6f*std::sin(m_animTime*0.9f + i*1.37f);
        float sz = 0.8f + std::sin(i*2.7f)*0.5f;
        p.setBrush(QColor(180,220,255, int(tw*140)));
        p.drawEllipse(QPointF(sx,sy), sz*tw, sz*tw);
    }

    // Constellation lines connecting nearby stars
    auto starPos = [&](int i) -> QPointF {
        return QPointF(
            std::fmod(i*173.7f,1000.f)/1000.f * width(),
            std::fmod(i*97.3f + i*i*0.03f, 1000.f)/1000.f * height()
            );
    };
    int pairs[][2] = {{0,3},{3,7},{7,12},{1,5},{5,9},{9,14},{2,6},{6,11},
                      {4,8},{8,13},{10,15},{15,20},{11,16},{16,21}};
    for (auto &pr : pairs) {
        QPointF a = starPos(pr[0]), b = starPos(pr[1]);
        float dist = QLineF(a,b).length();
        if (dist > width()*0.35f) continue;
        float alpha = qMax(0.f, 1.f - dist/(width()*0.35f));
        p.setPen(QPen(QColor(100,160,255,int(alpha*25)), 0.5f));
        p.drawLine(a,b);
    }
    p.restore();

    // ── Cyan glow top-centre (data/analytics feel) ────────
    QRadialGradient cg(width()*0.5f, height()*0.15f, width()*0.5f);
    cg.setColorAt(0.0, QColor(0,120,220,30));
    cg.setColorAt(1.0, QColor(0,  0,  0, 0));
    p.fillRect(rect(), cg);

    // ── Animated data-stream lines ────────────────────────
    p.save();
    p.setCompositionMode(QPainter::CompositionMode_Plus);
    for (int i = 0; i < 5; ++i) {
        float progress = std::fmod(m_animTime*0.3f + i*0.2f, 1.0f);
        float lx = width() * progress;
        float ly1 = height() * (0.1f + i*0.15f);
        float ly2 = ly1 + height()*0.08f;
        QLinearGradient dg(lx-40, ly1, lx+40, ly2);
        dg.setColorAt(0.0f, QColor(0,0,0,0));
        dg.setColorAt(0.4f, QColor(0,200,255,25));
        dg.setColorAt(0.6f, QColor(0,220,255,40));
        dg.setColorAt(1.0f, QColor(0,0,0,0));
        p.setPen(QPen(QBrush(dg), 1.2f));
        p.drawLine(QPointF(lx-40,ly1), QPointF(lx+40,ly2));
    }
    p.restore();

    // ── Subtle grid lines (graph paper feel) ──────────────
    p.save();
    p.setCompositionMode(QPainter::CompositionMode_Plus);
    int gridStep = 60;
    p.setPen(QPen(QColor(0,100,180,8), 0.5f));
    for (int x=0; x<width(); x+=gridStep)
        p.drawLine(x,0,x,height());
    for (int y=0; y<height(); y+=gridStep)
        p.drawLine(0,y,width(),y);
    p.restore();

    // ── Vignette ─────────────────────────────────────────
    QRadialGradient vg(width()/2.f, height()/2.f, width()*0.7f);
    vg.setColorAt(0.5f, QColor(0,0,0,0));
    vg.setColorAt(1.0f, QColor(0,0,0,80));
    p.fillRect(rect(), vg);
}