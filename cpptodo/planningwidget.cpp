#include "planningwidget.h"
#include "authmanager.h"
#include <QPainterPath>
#include <QLinearGradient>
#include <QRadialGradient>
#include <QMouseEvent>
#include <QScrollArea>
#include <QFrame>
#include <algorithm>
#include <QMap>
#include <cmath>

// ════════════════════════════════════════════════════════
//  Palette
// ════════════════════════════════════════════════════════
static const QColor CAL_EASY  (60,  220, 100);
static const QColor CAL_MEDIUM(255, 190,  30);
static const QColor CAL_HARD  (255,  70,  70);
static const QColor CAL_BG    (4,   14,  42);
static const QColor CAL_GRID  (0,   80, 140);
static const QColor CAL_TODAY (0,  140, 220);
static const QColor CAL_HEAD  (0,  200, 255);

// ════════════════════════════════════════════════════════
//  PlanCard
// ════════════════════════════════════════════════════════
QColor PlanCard::diffColor() const
{
    switch (m_pt.task.difficulty) {
    case Difficulty::Easy:   return CAL_EASY;
    case Difficulty::Medium: return CAL_MEDIUM;
    case Difficulty::Hard:   return CAL_HARD;
    }
    return Qt::white;
}

PlanCard::PlanCard(const PlannedTask &pt, QWidget *parent)
    : QWidget(parent), m_pt(pt)
{
    setMinimumHeight(80);
    setMaximumHeight(90);

    QHBoxLayout *hl = new QHBoxLayout(this);
    hl->setContentsMargins(16, 10, 16, 10);
    hl->setSpacing(16);

    QLabel *timeLbl = new QLabel(
        QString("%1h00").arg(pt.hour, 2, 10, QChar('0')), this);
    timeLbl->setFixedWidth(52);
    timeLbl->setStyleSheet(QString(
                               "color:%1;font-family:'Courier New';font-size:15px;"
                               "font-weight:bold;background:transparent;")
                               .arg(diffColor().name()));

    QWidget *info = new QWidget(this);
    info->setStyleSheet("background:transparent;");
    QVBoxLayout *vl = new QVBoxLayout(info);
    vl->setContentsMargins(0,0,0,0); vl->setSpacing(2);

    QLabel *name = new QLabel(pt.task.name, info);
    name->setStyleSheet("color:rgb(200,230,255);font-family:'Courier New';"
                        "font-size:12px;font-weight:bold;background:transparent;");

    QLabel *reason = new QLabel(pt.reason, info);
    reason->setStyleSheet("color:rgba(100,160,220,0.8);font-family:'Courier New';"
                          "font-size:10px;background:transparent;");
    reason->setWordWrap(true);

    vl->addWidget(name);
    vl->addWidget(reason);

    // Due date badge if set
    if (pt.task.dueDate.isValid()) {
        QString ds = "📅 " + pt.task.dueDate.toString("dd/MM/yyyy");
        QLabel *dl = new QLabel(ds, info);
        dl->setStyleSheet("color:rgba(0,200,255,0.7);font-family:'Courier New';"
                          "font-size:10px;background:transparent;");
        vl->addWidget(dl);
    }

    QLabel *dur = new QLabel(QString("%1 min").arg(pt.task.durationMin), this);
    dur->setFixedWidth(55);
    dur->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    dur->setStyleSheet("color:rgba(130,190,240,0.7);font-family:'Courier New';"
                       "font-size:10px;background:transparent;");

    hl->addWidget(timeLbl);
    hl->addWidget(info, 1);
    hl->addWidget(dur);
}

void PlanCard::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    QRectF r = QRectF(rect()).adjusted(1,1,-1,-1);
    p.setBrush(QColor(4, 14, 42, 190));
    p.setPen(Qt::NoPen);
    p.drawRoundedRect(r, 10, 10);
    QColor col = diffColor();
    p.setBrush(col);
    p.drawRoundedRect(QRectF(r.left(), r.top()+6, 3, r.height()-12), 2, 2);
    p.setPen(QPen(QColor(col.red(), col.green(), col.blue(), 40), 1));
    p.setBrush(Qt::NoBrush);
    p.drawRoundedRect(r, 10, 10);
}

// ════════════════════════════════════════════════════════
//  CalendarView helpers
// ════════════════════════════════════════════════════════
QColor CalendarView::diffColor(Difficulty d) const
{
    switch (d) {
    case Difficulty::Easy:   return CAL_EASY;
    case Difficulty::Medium: return CAL_MEDIUM;
    case Difficulty::Hard:   return CAL_HARD;
    }
    return CAL_EASY;
}

CalendarView::CalendarView(QWidget *parent) : QWidget(parent)
{
    setMinimumHeight(400);
    // Start at current week (Monday)
    QDate today = QDate::currentDate();
    int dow = today.dayOfWeek(); // 1=Mon
    m_weekStart  = today.addDays(1 - dow);
    m_monthStart = QDate(today.year(), today.month(), 1);
}

void CalendarView::setMode(Mode m)
{
    m_mode = m;
    update();
}

void CalendarView::setTasks(const QVector<Task> &tasks)
{
    m_allTasks = tasks;
    m_tasksByDate.clear();
    for (int i = 0; i < tasks.size(); ++i) {
        const Task &t = tasks[i];
        if (t.dueDate.isValid())
            m_tasksByDate[t.dueDate].append(t);
    }
    update();
}

void CalendarView::setWeekStart(const QDate &monday)
{
    m_weekStart = monday;
    update();
}

void CalendarView::setMonthStart(const QDate &first)
{
    m_monthStart = first;
    update();
}

void CalendarView::goNext()
{
    if (m_mode == Weekly)
        m_weekStart = m_weekStart.addDays(7);
    else
        m_monthStart = m_monthStart.addMonths(1);
    update();
}

void CalendarView::goPrev()
{
    if (m_mode == Weekly)
        m_weekStart = m_weekStart.addDays(-7);
    else
        m_monthStart = m_monthStart.addMonths(-1);
    update();
}

QString CalendarView::headerText() const
{
    if (m_mode == Weekly) {
        QDate end = m_weekStart.addDays(6);
        return QString("Semaine du %1 au %2")
            .arg(m_weekStart.toString("dd MMM"))
            .arg(end.toString("dd MMM yyyy"));
    }
    return m_monthStart.toString("MMMM yyyy").toUpper();
}

void CalendarView::mousePressEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    QWidget::mousePressEvent(e);
}

// ────────────────────────────────────────────────────────
//  drawTaskChip
// ────────────────────────────────────────────────────────
void CalendarView::drawTaskChip(QPainter &p, const QRectF &r,
                                const Task &t, bool small)
{
    QColor dc = diffColor(t.difficulty);
    QColor bg = dc;
    bg.setAlpha(t.completed ? 40 : 70);
    p.setBrush(bg);
    p.setPen(QPen(dc, 0.7f));
    p.drawRoundedRect(r, 3, 3);

    // Task name
    QFont f("Courier New", small ? 7 : 8, QFont::Bold);
    p.setFont(f);
    QColor tc = t.completed ? QColor(120,160,200,160) : QColor(220,240,255,230);
    p.setPen(tc);
    QString txt = t.completed ? ("✓ " + t.name) : t.name;
    p.drawText(r.adjusted(3,1,-2,-1), Qt::AlignLeft | Qt::AlignVCenter,
               p.fontMetrics().elidedText(txt, Qt::ElideRight, int(r.width()-4)));
}

// ════════════════════════════════════════════════════════
//  drawWeekly — Google Calendar style
//  Rows = hours 7h-22h, Cols = Mon-Sun
// ════════════════════════════════════════════════════════
void CalendarView::drawWeekly(QPainter &p)
{
    const int HOUR_START = 7;
    const int HOUR_END   = 22;
    const int HOURS      = HOUR_END - HOUR_START + 1;
    const int DAYS       = 7;

    const int headerH = 38;
    const int hourW   = 46;
    int gridW  = width()  - hourW;
    int gridH  = height() - headerH;
    int colW   = gridW / DAYS;
    float rowH = (float)gridH / HOURS;

    QDate today = QDate::currentDate();

    // ── Column headers (day names + dates) ─────────────
    for (int d = 0; d < DAYS; ++d) {
        QDate date = m_weekStart.addDays(d);
        bool isToday = (date == today);
        QRectF hr(hourW + d*colW, 0, colW, headerH);

        if (isToday) {
            p.setBrush(QColor(0, 100, 200, 60));
            p.setPen(Qt::NoPen);
            p.drawRoundedRect(hr.adjusted(2,2,-2,2), 6, 6);
        }

        static const char *dayNames[] = {"LUN","MAR","MER","JEU","VEN","SAM","DIM"};
        QFont f1("Courier New", 8, QFont::Bold);
        f1.setLetterSpacing(QFont::AbsoluteSpacing, 1);
        p.setFont(f1);
        p.setPen(isToday ? CAL_TODAY : QColor(80,140,200,180));
        p.drawText(QRectF(hourW+d*colW, 2, colW, 16), Qt::AlignCenter, dayNames[d]);

        QFont f2("Courier New", 11, isToday ? QFont::Bold : QFont::Normal);
        p.setFont(f2);
        p.setPen(isToday ? Qt::white : QColor(160,210,255,200));
        p.drawText(QRectF(hourW+d*colW, 18, colW, 20),
                   Qt::AlignCenter, QString::number(date.day()));
    }

    // ── Hour rows ─────────────────────────────────────
    QFont hf("Courier New", 8);
    p.setFont(hf);
    for (int h = 0; h < HOURS; ++h) {
        float y = headerH + h * rowH;
        int hour = HOUR_START + h;

        // Hour label
        p.setPen(QColor(60, 100, 160, 140));
        p.drawText(QRectF(0, y+1, hourW-4, rowH-1),
                   Qt::AlignRight | Qt::AlignTop,
                   QString("%1h").arg(hour, 2, 10, QChar('0')));

        // Horizontal grid line
        p.setPen(QPen(QColor(0, 60, 120, 60), 0.5f));
        p.drawLine(QPointF(hourW, y), QPointF(width(), y));

        // Half-hour dashed line
        p.setPen(QPen(QColor(0, 50, 100, 30), 0.4f, Qt::DashLine));
        p.drawLine(QPointF(hourW, y + rowH*0.5f), QPointF(width(), y + rowH*0.5f));
    }

    // ── Vertical column lines ─────────────────────────
    p.setPen(QPen(QColor(0, 70, 130, 80), 0.5f));
    for (int d = 0; d <= DAYS; ++d)
        p.drawLine(QPointF(hourW + d*colW, headerH),
                   QPointF(hourW + d*colW, height()));

    // ── Current time indicator ─────────────────────────
    if (today >= m_weekStart && today < m_weekStart.addDays(7)) {
        int dayCol = m_weekStart.daysTo(today);
        QTime now  = QTime::currentTime();
        float frac = (now.hour() - HOUR_START + now.minute()/60.f) / HOURS;
        frac = qBound(0.f, frac, 1.f);
        float ty = headerH + frac * gridH;
        float tx = hourW + dayCol * colW;
        // Circle + line
        p.setBrush(QColor(255, 80, 80));
        p.setPen(Qt::NoPen);
        p.drawEllipse(QPointF(tx + 3, ty), 4, 4);
        p.setPen(QPen(QColor(255, 80, 80, 180), 1.2f));
        p.drawLine(QPointF(tx, ty), QPointF(tx + colW, ty));
    }

    // ── Task chips (tasks with dueDate in this week) ───
    for (int d = 0; d < DAYS; ++d) {
        QDate date = m_weekStart.addDays(d);
        if (!m_tasksByDate.contains(date)) continue;

        const QVector<Task> &dayTasks = m_tasksByDate[date];
        int chipCount = dayTasks.size();

        for (int ti = 0; ti < chipCount; ++ti) {
            const Task &t = dayTasks[ti];

            // Place task at estimated hour (from AI plan or 9h default)
            float startHour = 9.f;
            float durFrac   = qMin(t.durationMin / 60.f, 1.5f);

            float chipTop = headerH + (startHour + ti*1.2f - HOUR_START) * rowH;
            float chipH   = qMax(rowH * durFrac, rowH * 0.6f);
            float chipX   = hourW + d*colW + 2 + (chipCount > 1 ? ti*8.f : 0);
            float chipW   = colW - 4 - (chipCount > 1 ? ti*8.f : 0);

            chipTop = qBound((float)headerH, chipTop, (float)(height() - 20));
            if (chipTop + chipH > height()) chipH = height() - chipTop;
            if (chipW < 8) continue;

            QRectF chip(chipX, chipTop, chipW, chipH);
            drawTaskChip(p, chip, t, chipH < 20);
        }
    }

    // ── Tasks without dates — shown as floating chips ──
    // shown in a small strip at the bottom of each day that has undated tasks
    // (skip for clarity — only dated tasks appear in calendar)
}

// ════════════════════════════════════════════════════════
//  drawMonthly
// ════════════════════════════════════════════════════════
void CalendarView::drawMonthly(QPainter &p)
{
    const int headerH = 32;
    int daysInMonth   = m_monthStart.daysInMonth();
    int firstDow      = m_monthStart.dayOfWeek(); // 1=Mon
    int totalCells    = ((firstDow - 1) + daysInMonth + 6) / 7 * 7;
    int rows          = totalCells / 7;

    float colW = (float)width() / 7.f;
    float rowH = (float)(height() - headerH) / rows;

    QDate today = QDate::currentDate();

    // ── Day headers ───────────────────────────────────
    static const char *dayNames[] = {"LUN","MAR","MER","JEU","VEN","SAM","DIM"};
    QFont hf("Courier New", 8, QFont::Bold);
    hf.setLetterSpacing(QFont::AbsoluteSpacing, 1);
    p.setFont(hf);
    for (int d = 0; d < 7; ++d) {
        bool isWeekend = (d >= 5);
        p.setPen(isWeekend ? QColor(255,100,100,160) : QColor(0,160,200,180));
        p.drawText(QRectF(d*colW, 0, colW, headerH),
                   Qt::AlignCenter, dayNames[d]);
    }

    // ── Grid lines ────────────────────────────────────
    p.setPen(QPen(QColor(0, 70, 130, 70), 0.5f));
    for (int c = 0; c <= 7; ++c)
        p.drawLine(QPointF(c*colW, headerH), QPointF(c*colW, height()));
    for (int r = 0; r <= rows; ++r)
        p.drawLine(QPointF(0, headerH+r*rowH), QPointF(width(), headerH+r*rowH));

    // ── Day cells ─────────────────────────────────────
    for (int cell = 0; cell < rows*7; ++cell) {
        int col = cell % 7;
        int row = cell / 7;
        int day = cell - (firstDow - 1) + 1;

        QRectF cellR(col*colW, headerH+row*rowH, colW, rowH);

        if (day < 1 || day > daysInMonth) {
            // Out of month — darken
            p.setBrush(QColor(0, 0, 0, 30));
            p.setPen(Qt::NoPen);
            p.drawRect(cellR);
            continue;
        }

        QDate cellDate(m_monthStart.year(), m_monthStart.month(), day);
        bool isToday   = (cellDate == today);
        bool isWeekend = (col >= 5);

        // Weekend tint
        if (isWeekend) {
            p.setBrush(QColor(100, 20, 20, 15));
            p.setPen(Qt::NoPen);
            p.drawRect(cellR);
        }

        // Today highlight
        if (isToday) {
            p.setBrush(QColor(0, 100, 200, 40));
            p.setPen(Qt::NoPen);
            p.drawRect(cellR);
        }

        // Day number
        QFont df("Courier New", 9, isToday ? QFont::Bold : QFont::Normal);
        p.setFont(df);

        if (isToday) {
            // Circle behind number
            QRectF circle(col*colW + colW/2 - 10, headerH+row*rowH + 3, 20, 20);
            p.setBrush(CAL_TODAY);
            p.setPen(Qt::NoPen);
            p.drawEllipse(circle);
            p.setPen(Qt::white);
        } else {
            p.setPen(isWeekend ? QColor(255,120,120,180) : QColor(160,210,255,180));
        }
        p.drawText(QRectF(col*colW+2, headerH+row*rowH+3, colW-4, 22),
                   Qt::AlignHCenter | Qt::AlignTop, QString::number(day));

        // Task chips for this day
        if (m_tasksByDate.contains(cellDate)) {
            const QVector<Task> &dayTasks = m_tasksByDate[cellDate];
            float chipH = qMin(16.f, (rowH - 28.f) / dayTasks.size());
            chipH = qMax(chipH, 12.f);
            int maxShow = int((rowH - 28) / chipH);

            for (int ti = 0; ti < qMin(dayTasks.size(), maxShow); ++ti) {
                QRectF chip(col*colW + 2,
                            headerH + row*rowH + 26 + ti*chipH,
                            colW - 4, chipH - 1);
                drawTaskChip(p, chip, dayTasks[ti], true);
            }
            // "+N more" label
            if (dayTasks.size() > maxShow) {
                QFont mf("Courier New", 7);
                p.setFont(mf);
                p.setPen(QColor(100,160,220,160));
                p.drawText(QRectF(col*colW+2,
                                  headerH+row*rowH+26+maxShow*chipH,
                                  colW-4, chipH),
                           Qt::AlignCenter,
                           QString("+%1").arg(dayTasks.size()-maxShow));
            }
        }
    }
}

void CalendarView::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // Background
    p.fillRect(rect(), QColor(3, 10, 30, 220));

    if (m_mode == Weekly) drawWeekly(p);
    else                  drawMonthly(p);
}

// ════════════════════════════════════════════════════════
//  PlanningWidget
// ════════════════════════════════════════════════════════

// ── View-selector button helper ──────────────────────────
static QPushButton *makeViewBtn(const QString &text, QWidget *parent)
{
    QPushButton *b = new QPushButton(text, parent);
    b->setFixedHeight(32);
    b->setMinimumWidth(90);
    b->setCursor(Qt::PointingHandCursor);
    b->setStyleSheet(
        "QPushButton{background:rgba(0,30,70,0.7);color:rgba(100,170,230,0.8);"
        "border:1px solid rgba(0,100,180,0.4);border-radius:8px;"
        "font-family:'Courier New';font-size:10px;font-weight:bold;letter-spacing:2px;}"
        "QPushButton:hover{background:rgba(0,60,120,0.85);color:rgb(0,200,255);"
        "border-color:rgba(0,160,220,0.6);}");
    return b;
}

static void setViewBtnActive(QPushButton *b, bool active)
{
    if (active)
        b->setStyleSheet(
            "QPushButton{background:rgba(0,80,160,0.85);color:rgb(0,200,255);"
            "border:1.5px solid rgba(0,180,255,0.7);border-radius:8px;"
            "font-family:'Courier New';font-size:10px;font-weight:bold;letter-spacing:2px;}");
    else
        b->setStyleSheet(
            "QPushButton{background:rgba(0,30,70,0.7);color:rgba(100,170,230,0.8);"
            "border:1px solid rgba(0,100,180,0.4);border-radius:8px;"
            "font-family:'Courier New';font-size:10px;font-weight:bold;letter-spacing:2px;}"
            "QPushButton:hover{background:rgba(0,60,120,0.85);color:rgb(0,200,255);}");
}

// ────────────────────────────────────────────────────────
QWidget *PlanningWidget::buildHeader()
{
    QWidget *w = new QWidget(this);
    w->setFixedHeight(60);
    w->setStyleSheet("background:rgba(6,14,38,0.95);"
                     "border-bottom:1px solid rgba(0,120,200,0.30);");
    QHBoxLayout *lay = new QHBoxLayout(w);
    lay->setContentsMargins(24,0,24,0);

    QLabel *title = new QLabel("🗓  PLANNING INTELLIGENT", w);
    title->setStyleSheet("color:rgb(60,220,100);font-family:'Courier New';"
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
    connect(back, &QPushButton::clicked, this, &PlanningWidget::goBack);
    lay->addWidget(back);

    return w;
}

QWidget *PlanningWidget::buildViewSelector()
{
    QWidget *w = new QWidget(this);
    w->setFixedHeight(48);
    w->setStyleSheet("background:transparent;");
    QHBoxLayout *lay = new QHBoxLayout(w);
    lay->setContentsMargins(28, 8, 28, 8);
    lay->setSpacing(8);

    m_btnList  = makeViewBtn("🤖 IA",      w);
    m_btnWeek  = makeViewBtn("📅 SEMAINE", w);
    m_btnMonth = makeViewBtn("📆 MOIS",    w);

    setViewBtnActive(m_btnList, true);

    connect(m_btnList,  &QPushButton::clicked, this, &PlanningWidget::switchToList);
    connect(m_btnWeek,  &QPushButton::clicked, this, &PlanningWidget::switchToWeek);
    connect(m_btnMonth, &QPushButton::clicked, this, &PlanningWidget::switchToMonth);

    lay->addWidget(m_btnList);
    lay->addWidget(m_btnWeek);
    lay->addWidget(m_btnMonth);
    lay->addStretch();

    // Legend
    auto dot = [&](QColor c, const QString &s) {
        QLabel *l = new QLabel("● " + s, w);
        l->setStyleSheet(QString("color:%1;font-family:'Courier New';"
                                 "font-size:10px;background:transparent;")
                             .arg(c.name()));
        lay->addWidget(l);
    };
    dot(CAL_EASY,   "Facile");
    dot(CAL_MEDIUM, "Moyen");
    dot(CAL_HARD,   "Difficile");

    return w;
}

QWidget *PlanningWidget::buildNavBar()
{
    m_navBar = new QWidget(this);
    m_navBar->setFixedHeight(40);
    m_navBar->setStyleSheet("background:transparent;");
    m_navBar->setVisible(false);

    QHBoxLayout *lay = new QHBoxLayout(m_navBar);
    lay->setContentsMargins(28, 4, 28, 4);
    lay->setSpacing(10);

    QPushButton *prev = new QPushButton("◀", m_navBar);
    QPushButton *next = new QPushButton("▶", m_navBar);
    for (QPushButton *b : {prev, next}) {
        b->setFixedSize(30, 30);
        b->setCursor(Qt::PointingHandCursor);
        b->setStyleSheet(
            "QPushButton{background:rgba(0,60,120,0.6);color:rgb(0,200,255);"
            "border:1px solid rgba(0,140,220,0.5);border-radius:6px;"
            "font-size:12px;font-weight:bold;}"
            "QPushButton:hover{background:rgba(0,100,180,0.8);}");
    }
    connect(prev, &QPushButton::clicked, this, &PlanningWidget::onPrev);
    connect(next, &QPushButton::clicked, this, &PlanningWidget::onNext);

    m_navLabel = new QLabel(m_navBar);
    m_navLabel->setStyleSheet(
        "color:rgb(160,210,255);font-family:'Courier New';"
        "font-size:11px;font-weight:bold;background:transparent;");
    m_navLabel->setAlignment(Qt::AlignCenter);

    lay->addWidget(prev);
    lay->addWidget(m_navLabel, 1);
    lay->addWidget(next);

    return m_navBar;
}

QWidget *PlanningWidget::buildListView()
{
    QScrollArea *scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    scroll->setStyleSheet(
        "QScrollArea{border:none;background:transparent;}"
        "QScrollBar:vertical{width:6px;background:transparent;}"
        "QScrollBar::handle:vertical{background:rgba(0,150,220,0.4);border-radius:3px;}");

    m_planContainer = new QWidget;
    m_planContainer->setStyleSheet("background:transparent;");
    m_planLayout = new QVBoxLayout(m_planContainer);
    m_planLayout->setContentsMargins(28, 12, 28, 28);
    m_planLayout->setSpacing(10);

    // Legend row
    QWidget *legend = new QWidget(m_planContainer);
    legend->setStyleSheet("background:transparent;");
    QHBoxLayout *ll = new QHBoxLayout(legend);
    ll->setContentsMargins(0,0,0,0); ll->setSpacing(20);
    auto makeDot = [](const QString &label, QColor col, QWidget *parent) {
        QLabel *l = new QLabel("● " + label, parent);
        l->setStyleSheet(QString("color:%1;font-family:'Courier New';"
                                 "font-size:10px;background:transparent;")
                             .arg(col.name()));
        return l;
    };
    ll->addWidget(makeDot("FACILE",    CAL_EASY,   legend));
    ll->addWidget(makeDot("MOYEN",     CAL_MEDIUM, legend));
    ll->addWidget(makeDot("DIFFICILE", CAL_HARD,   legend));
    ll->addStretch();

    QLabel *hint = new QLabel(
        "Planning basé sur vos heures de productivité réelles", m_planContainer);
    hint->setStyleSheet("color:rgba(80,130,190,0.8);font-family:'Courier New';"
                        "font-size:10px;background:transparent;letter-spacing:1px;");

    m_emptyLabel = new QLabel(
        "Aucune tâche à planifier.\nAjoutez des tâches dans votre Todo List.",
        m_planContainer);
    m_emptyLabel->setAlignment(Qt::AlignCenter);
    m_emptyLabel->setStyleSheet("color:rgba(80,130,190,0.7);font-family:'Courier New';"
                                "font-size:12px;background:transparent;");
    m_emptyLabel->setVisible(false);

    m_planLayout->addWidget(legend);
    m_planLayout->addWidget(hint);
    m_planLayout->addSpacing(8);
    m_planLayout->addWidget(m_emptyLabel);
    m_planLayout->addStretch();

    scroll->setWidget(m_planContainer);
    m_listView = scroll;
    return scroll;
}

// ════════════════════════════════════════════════════════
//  Constructor
// ════════════════════════════════════════════════════════
PlanningWidget::PlanningWidget(QWidget *parent) : QWidget(parent)
{
    setMinimumSize(760, 560);

    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(0,0,0,0); root->setSpacing(0);

    root->addWidget(buildHeader());
    root->addWidget(buildViewSelector());
    root->addWidget(buildNavBar());

    // Calendar view
    m_calView = new CalendarView(this);
    m_calView->setVisible(false);

    // List view (AI planning)
    QWidget *listW = buildListView();

    root->addWidget(listW,     1);
    root->addWidget(m_calView, 1);

    // Background animation
    m_animTime = 0.f;
    QTimer *bgAnim = new QTimer(this);
    bgAnim->setInterval(40);
    connect(bgAnim, &QTimer::timeout, this, [this](){ m_animTime += 0.04f; update(); });
    bgAnim->start();
}

// ════════════════════════════════════════════════════════
//  View switch slots
// ════════════════════════════════════════════════════════
void PlanningWidget::switchToList()
{
    m_listView->setVisible(true);
    m_calView->setVisible(false);
    m_navBar->setVisible(false);
    setViewBtnActive(m_btnList,  true);
    setViewBtnActive(m_btnWeek,  false);
    setViewBtnActive(m_btnMonth, false);
}

void PlanningWidget::switchToWeek()
{
    m_listView->setVisible(false);
    m_calView->setMode(CalendarView::Weekly);
    m_calView->setVisible(true);
    m_navBar->setVisible(true);
    m_navLabel->setText(m_calView->headerText());
    setViewBtnActive(m_btnList,  false);
    setViewBtnActive(m_btnWeek,  true);
    setViewBtnActive(m_btnMonth, false);
}

void PlanningWidget::switchToMonth()
{
    m_listView->setVisible(false);
    m_calView->setMode(CalendarView::Monthly);
    m_calView->setVisible(true);
    m_navBar->setVisible(true);
    m_navLabel->setText(m_calView->headerText());
    setViewBtnActive(m_btnList,  false);
    setViewBtnActive(m_btnWeek,  false);
    setViewBtnActive(m_btnMonth, true);
}

void PlanningWidget::onPrev()
{
    m_calView->goPrev();
    m_navLabel->setText(m_calView->headerText());
}

void PlanningWidget::onNext()
{
    m_calView->goNext();
    m_navLabel->setText(m_calView->headerText());
}

// ════════════════════════════════════════════════════════
//  setTasks / refresh / rebuildPlanList
// ════════════════════════════════════════════════════════
void PlanningWidget::setTasks(const QVector<Task> &tasks)
{
    m_tasks = tasks;
    m_calView->setTasks(tasks);
    rebuildPlanList();
}

void PlanningWidget::refresh()
{
    m_calView->setTasks(m_tasks);
    rebuildPlanList();
}

void PlanningWidget::rebuildPlanList()
{
    // Remove old plan cards (keep first 4: legend, hint, spacing, empty)
    while (m_planLayout->count() > 4) {
        QLayoutItem *item = m_planLayout->takeAt(4);
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }

    auto plan = generatePlan(m_tasks);

    if (plan.isEmpty()) {
        m_emptyLabel->setVisible(true);
        return;
    }
    m_emptyLabel->setVisible(false);

    int insertAt = 3;
    for (int i = 0; i < plan.size(); ++i) {
        PlanCard *card = new PlanCard(plan[i], m_planContainer);
        m_planLayout->insertWidget(insertAt++, card);
    }
}

// ════════════════════════════════════════════════════════
//  generatePlan — smart scheduling
// ════════════════════════════════════════════════════════
QVector<PlannedTask> PlanningWidget::generatePlan(const QVector<Task> &tasks) const
{
    QVector<PlannedTask> plan;
    if (tasks.isEmpty()) return plan;

    QVector<HourlyProductivity> hourlyData =
        SessionManager::instance().getHourlyProductivity();

    QMap<int,double> scoreMap;
    bool hasRealData = !hourlyData.isEmpty();

    if (hasRealData) {
        for (int i = 0; i < hourlyData.size(); ++i) {
            int h = hourlyData[i].hour;
            if (h >= 6 && h <= 22) scoreMap[h] = hourlyData[i].avgScore;
        }
        for (int h = 7; h <= 21; ++h)
            if (!scoreMap.contains(h)) scoreMap[h] = 0.0;
    } else {
        scoreMap[7]=20; scoreMap[8]=35; scoreMap[9]=75;
        scoreMap[10]=85; scoreMap[11]=80; scoreMap[12]=40;
        scoreMap[13]=30; scoreMap[14]=55; scoreMap[15]=70;
        scoreMap[16]=75; scoreMap[17]=65; scoreMap[18]=50;
        scoreMap[19]=35; scoreMap[20]=25; scoreMap[21]=15;
    }

    double maxScore = 1.0;
    QList<int> allHours = scoreMap.keys();
    for (int i = 0; i < allHours.size(); ++i)
        if (scoreMap[allHours[i]] > maxScore) maxScore = scoreMap[allHours[i]];

    double highT = maxScore * 0.60;
    double midT  = maxScore * 0.30;

    QVector<int> highH, midH, lowH;
    for (int i = 0; i < allHours.size(); ++i) {
        int h = allHours[i]; double s = scoreMap[h];
        if (s >= highT)     highH.append(h);
        else if (s >= midT) midH.append(h);
        else                lowH.append(h);
    }
    if (highH.isEmpty()) { highH.append(9); highH.append(10); }
    if (midH.isEmpty())  { midH.append(11); midH.append(14); }
    if (lowH.isEmpty())  { lowH.append(8);  lowH.append(13); }

    QVector<Task> hardT, medT, easyT;
    for (int i = 0; i < tasks.size(); ++i) {
        if (tasks[i].completed) continue;
        if      (tasks[i].difficulty == Difficulty::Hard)   hardT.append(tasks[i]);
        else if (tasks[i].difficulty == Difficulty::Medium) medT.append(tasks[i]);
        else                                                 easyT.append(tasks[i]);
    }
    for (int i = 0; i < hardT.size()-1; ++i)
        for (int j = i+1; j < hardT.size(); ++j)
            if (hardT[j].durationMin > hardT[i].durationMin) qSwap(hardT[i],hardT[j]);
    for (int i = 0; i < medT.size()-1; ++i)
        for (int j = i+1; j < medT.size(); ++j)
            if (medT[j].durationMin > medT[i].durationMin) qSwap(medT[i],medT[j]);
    for (int i = 0; i < easyT.size()-1; ++i)
        for (int j = i+1; j < easyT.size(); ++j)
            if (easyT[j].durationMin > easyT[i].durationMin) qSwap(easyT[i],easyT[j]);

    QMap<int,int> minutesUsed;
    const int MAX_MIN = 90;

    auto pickHour = [&](int dur, const QVector<int> &pref,
                        const QVector<int> &fall) -> int {
        for (int i = 0; i < pref.size(); ++i)
            if (minutesUsed.value(pref[i],0)+dur <= MAX_MIN) return pref[i];
        for (int i = 0; i < fall.size(); ++i)
            if (minutesUsed.value(fall[i],0)+dur <= MAX_MIN) return fall[i];
        for (int i = 0; i < allHours.size(); ++i)
            if (minutesUsed.value(allHours[i],0)+dur <= MAX_MIN+60) return allHours[i];
        return 20;
    };

    auto makeReason = [&](Difficulty diff, int hour) -> QString {
        QString diffStr = (diff==Difficulty::Hard)?"difficile"
                          :(diff==Difficulty::Medium)?"moyenne":"facile";
        QString hourStr = QString("%1h00").arg(hour,2,10,QChar('0'));
        if (diff == Difficulty::Hard)
            return hasRealData
                       ? QString("Tâche %1 → %2 : pic de productivité (%3/100)")
                             .arg(diffStr,hourStr).arg(int(scoreMap.value(hour,0)))
                       : QString("Tâche %1 → %2 : pic naturel de concentration")
                             .arg(diffStr,hourStr);
        if (diff == Difficulty::Medium)
            return hasRealData
                       ? QString("Tâche %1 → %2 : bonne productivité (%3/100)")
                             .arg(diffStr,hourStr).arg(int(scoreMap.value(hour,0)))
                       : QString("Tâche %1 → %2 : bonne fenêtre de travail")
                             .arg(diffStr,hourStr);
        return QString("Tâche %1 → %2 : créneau tranquille, effort minimal")
            .arg(diffStr,hourStr);
    };

    for (int i = 0; i < hardT.size(); ++i) {
        int h = pickHour(hardT[i].durationMin, highH, midH);
        minutesUsed[h] = minutesUsed.value(h,0) + hardT[i].durationMin + 10;
        PlannedTask pt; pt.task=hardT[i]; pt.hour=h;
        pt.reason=makeReason(Difficulty::Hard,h); plan.append(pt);
    }
    for (int i = 0; i < medT.size(); ++i) {
        int h = pickHour(medT[i].durationMin, midH, highH);
        minutesUsed[h] = minutesUsed.value(h,0) + medT[i].durationMin + 10;
        PlannedTask pt; pt.task=medT[i]; pt.hour=h;
        pt.reason=makeReason(Difficulty::Medium,h); plan.append(pt);
    }
    for (int i = 0; i < easyT.size(); ++i) {
        int h = pickHour(easyT[i].durationMin, lowH, midH);
        minutesUsed[h] = minutesUsed.value(h,0) + easyT[i].durationMin + 10;
        PlannedTask pt; pt.task=easyT[i]; pt.hour=h;
        pt.reason=makeReason(Difficulty::Easy,h); plan.append(pt);
    }

    for (int i = 0; i < plan.size()-1; ++i)
        for (int j = i+1; j < plan.size(); ++j)
            if (plan[j].hour < plan[i].hour) qSwap(plan[i],plan[j]);

    return plan;
}

// ════════════════════════════════════════════════════════
//  paintEvent
// ════════════════════════════════════════════════════════
void PlanningWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QLinearGradient bg(0,0,0,height());
    bg.setColorAt(0.00, QColor(2,  6, 18));
    bg.setColorAt(0.40, QColor(4, 10, 26));
    bg.setColorAt(0.75, QColor(3,  8, 22));
    bg.setColorAt(1.00, QColor(1,  4, 14));
    p.fillRect(rect(), bg);

    float t = m_animTime;

    // Timeline line
    float lx = width() * 0.13f;
    QLinearGradient tl(lx,0,lx,height());
    tl.setColorAt(0.0f, QColor(0,0,0,0));
    tl.setColorAt(0.1f, QColor(60,220,100,30));
    tl.setColorAt(0.5f, QColor(60,220,100,50));
    tl.setColorAt(0.9f, QColor(60,220,100,30));
    tl.setColorAt(1.0f, QColor(0,0,0,0));
    p.setPen(QPen(QBrush(tl),1.5f));
    p.drawLine(QPointF(lx,0),QPointF(lx,height()));

    // Travelling dot
    float tdot = std::fmod(t*0.25f,1.0f);
    float ty   = height()*tdot;
    QRadialGradient dg(lx,ty,10);
    dg.setColorAt(0.0f,QColor(60,220,100,200));
    dg.setColorAt(1.0f,QColor(0,0,0,0));
    p.setBrush(dg); p.setPen(Qt::NoPen);
    p.drawEllipse(QPointF(lx,ty),10,10);
    p.setBrush(QColor(200,255,200,220));
    p.drawEllipse(QPointF(lx,ty),3,3);

    // Aurora blobs
    QRadialGradient a1(width()*0.7f,height()*0.2f,width()*0.4f);
    a1.setColorAt(0.0f,QColor(60,160,80,18));
    a1.setColorAt(1.0f,QColor(0,0,0,0));
    p.fillRect(rect(),a1);

    QRadialGradient a2(width()*0.85f,height()*0.75f,width()*0.35f);
    a2.setColorAt(0.0f,QColor(255,160,30,12));
    a2.setColorAt(1.0f,QColor(0,0,0,0));
    p.fillRect(rect(),a2);

    // Vignette
    QLinearGradient vig(0,0,0,height());
    vig.setColorAt(0.00,QColor(0,0,0,90));
    vig.setColorAt(0.12,QColor(0,0,0,0));
    vig.setColorAt(0.88,QColor(0,0,0,0));
    vig.setColorAt(1.00,QColor(0,0,0,70));
    p.fillRect(rect(),vig);
}