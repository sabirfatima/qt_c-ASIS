#include "todowidget.h"
#include "authmanager.h"
#include "sessionmanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>
#include <QDir>
#include <QMessageBox>
#include <QPushButton>
#include <QMouseEvent>
#include <cmath>

static const QColor C_EASY  (60,  220, 100);
static const QColor C_MEDIUM(255, 200,  40);
static const QColor C_HARD  (255,  70,  70);
static const QColor C_CYAN  (0,   200, 255);
static const QColor C_DIM   (60,  100, 160);

static QString inputStyle()
{
    return QString(
        "background:rgba(4,14,40,0.95);color:rgb(160,210,255);"
        "border:1px solid rgba(0,140,220,0.45);border-radius:8px;"
        "padding:6px 12px;font-family:'Courier New';font-size:13px;"
        "selection-background-color:rgba(0,140,220,0.5);");
}

static QString spinStyle()
{
    return QString(
        "QSpinBox{background:rgba(4,14,40,0.95);color:rgb(0,200,255);"
        "border:1px solid rgba(0,140,220,0.45);border-radius:8px;"
        "padding:6px 8px;font-family:'Courier New';font-size:13px;font-weight:bold;}"
        "QSpinBox::up-button,QSpinBox::down-button{"
        "background:rgba(0,80,150,0.4);border:none;width:18px;}");
}

// ════════════════════════════════════════════════════════
//  DatePickerWidget
// ════════════════════════════════════════════════════════
DatePickerWidget::DatePickerWidget(QWidget *parent) : QWidget(parent)
{
    m_selected = QDate(); // no date
    m_viewing  = QDate::currentDate();

    setFixedHeight(220);
    setStyleSheet("background:transparent;");

    // Prev / Next buttons
    m_prevBtn = new QPushButton("‹", this);
    m_nextBtn = new QPushButton("›", this);
    for (QPushButton *b : {m_prevBtn, m_nextBtn}) {
        b->setFixedSize(28, 26);
        b->setCursor(Qt::PointingHandCursor);
        b->setStyleSheet(
            "QPushButton{background:rgba(0,80,150,0.4);color:rgb(0,200,255);"
            "border:1px solid rgba(0,140,220,0.4);border-radius:6px;"
            "font-size:14px;font-weight:bold;}"
            "QPushButton:hover{background:rgba(0,120,200,0.6);}");
    }

    m_monthLabel = new QLabel(this);
    m_monthLabel->setStyleSheet(
        "color:rgb(0,200,255);font-family:'Courier New';font-size:11px;"
        "font-weight:bold;background:transparent;letter-spacing:2px;");
    m_monthLabel->setAlignment(Qt::AlignCenter);

    connect(m_prevBtn, &QPushButton::clicked, this, [this](){ prevMonth(); });
    connect(m_nextBtn, &QPushButton::clicked, this, [this](){ nextMonth(); });

    setSelectedDate(QDate()); // triggers label update
}

void DatePickerWidget::setSelectedDate(const QDate &d)
{
    m_selected = d;
    if (d.isValid()) m_viewing = QDate(d.year(), d.month(), 1);
    // Update month label
    if (m_monthLabel)
        m_monthLabel->setText(
            m_viewing.toString("MMMM yyyy").toUpper());
    update();
}

void DatePickerWidget::prevMonth()
{
    m_viewing = m_viewing.addMonths(-1);
    m_monthLabel->setText(m_viewing.toString("MMMM yyyy").toUpper());
    update();
}

void DatePickerWidget::nextMonth()
{
    m_viewing = m_viewing.addMonths(1);
    m_monthLabel->setText(m_viewing.toString("MMMM yyyy").toUpper());
    update();
}

QRect DatePickerWidget::cellRect(int row, int col) const
{
    // Grid starts at y=56 (after header + day labels)
    int cellW = (width() - 8) / 7;
    int cellH = 26;
    int x = 4 + col * cellW;
    int y = 56 + row * cellH;
    return QRect(x, y, cellW - 2, cellH - 2);
}

int DatePickerWidget::dayAt(int row, int col) const
{
    // First day of month (1=Mon..7=Sun in Qt)
    int firstDow = m_viewing.dayOfWeek(); // 1=Mon
    int offset   = firstDow - 1;          // 0-based
    int day      = row * 7 + col - offset + 1;
    if (day < 1 || day > m_viewing.daysInMonth()) return -1;
    return day;
}

void DatePickerWidget::mousePressEvent(QMouseEvent *e)
{
    for (int row = 0; row < 6; ++row) {
        for (int col = 0; col < 7; ++col) {
            QRect r = cellRect(row, col);
            int d   = dayAt(row, col);
            if (d > 0 && r.contains(e->pos())) {
                m_selected = QDate(m_viewing.year(), m_viewing.month(), d);
                emit dateSelected(m_selected);
                update();
                return;
            }
        }
    }
    QWidget::mousePressEvent(e);
}

void DatePickerWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // Background panel
    p.setBrush(QColor(4, 14, 42, 230));
    p.setPen(QPen(QColor(0, 120, 200, 80), 1));
    p.drawRoundedRect(rect().adjusted(1,1,-1,-1), 10, 10);

    // Position nav buttons and label
    m_prevBtn->move(6, 6);
    m_nextBtn->move(width() - 34, 6);
    if (m_monthLabel->width() == 0)
        m_monthLabel->setGeometry(38, 4, width()-76, 28);
    else
        m_monthLabel->setGeometry(38, 4, width()-76, 28);

    // Day headers (Mon-Sun)
    static const char *days[] = {"L","M","M","J","V","S","D"};
    int cellW = (width()-8)/7;
    p.setFont(QFont("Courier New", 8, QFont::Bold));
    for (int c = 0; c < 7; ++c) {
        bool isWeekend = (c >= 5);
        p.setPen(isWeekend ? QColor(255,100,100,160) : QColor(0,160,200,160));
        QRect hr(4 + c*cellW, 34, cellW-2, 18);
        p.drawText(hr, Qt::AlignCenter, days[c]);
    }

    QDate today = QDate::currentDate();

    // Calendar cells
    for (int row = 0; row < 6; ++row) {
        for (int col = 0; col < 7; ++col) {
            int d = dayAt(row, col);
            if (d < 0) continue;

            QDate cellDate(m_viewing.year(), m_viewing.month(), d);
            QRect r = cellRect(row, col);

            bool isToday    = (cellDate == today);
            bool isSelected = (cellDate == m_selected);
            bool isPast     = (cellDate < today);

            // Cell background
            if (isSelected) {
                p.setBrush(QColor(0, 140, 220, 200));
                p.setPen(QPen(QColor(0, 200, 255, 220), 1.2));
            } else if (isToday) {
                p.setBrush(QColor(0, 80, 140, 120));
                p.setPen(QPen(QColor(0, 180, 255, 150), 1));
            } else {
                p.setBrush(QColor(0, 0, 0, 0));
                p.setPen(Qt::NoPen);
            }
            p.drawRoundedRect(r, 5, 5);

            // Day number text
            QColor tc;
            if (isSelected)    tc = Qt::white;
            else if (isPast)   tc = QColor(60, 90, 140, 140);
            else if (col >= 5) tc = QColor(255, 120, 120, 180);
            else               tc = QColor(160, 210, 255, 200);

            p.setPen(tc);
            p.setFont(QFont("Courier New", 8, isSelected ? QFont::Bold : QFont::Normal));
            p.drawText(r, Qt::AlignCenter, QString::number(d));
        }
    }
}

// ════════════════════════════════════════════════════════
//  DiffButton
// ════════════════════════════════════════════════════════
DiffButton::DiffButton(const QString &text, QColor col, QWidget *parent)
    : QPushButton(text, parent), m_color(col)
{
    setFixedHeight(34); setMinimumWidth(80);
    setCursor(Qt::PointingHandCursor);
}

void DiffButton::paintEvent(QPaintEvent *)
{
    QPainter p(this); p.setRenderHint(QPainter::Antialiasing);
    QRectF r=QRectF(rect()).adjusted(1,1,-1,-1);
    p.setBrush(m_selected
                   ? QColor(m_color.red(),m_color.green(),m_color.blue(),50)
                   : QColor(4,12,32,180));
    p.setPen(Qt::NoPen); p.drawRoundedRect(r,8,8);
    p.setPen(QPen(m_selected?m_color:QColor(0,70,120,100),
                  m_selected?1.5f:1.0f));
    p.setBrush(Qt::NoBrush); p.drawRoundedRect(r,8,8);
    QFont f("Courier New",10,m_selected?QFont::Bold:QFont::Normal);
    f.setLetterSpacing(QFont::AbsoluteSpacing,2);
    p.setFont(f); p.setPen(m_selected?m_color:C_DIM);
    p.drawText(r,Qt::AlignCenter,text());
}

// ════════════════════════════════════════════════════════
//  TaskCard
// ════════════════════════════════════════════════════════
QColor TaskCard::diffColor() const
{
    switch(m_task.difficulty){
    case Difficulty::Easy:   return C_EASY;
    case Difficulty::Medium: return C_MEDIUM;
    case Difficulty::Hard:   return C_HARD;
    }
    return C_EASY;
}

TaskCard::TaskCard(const Task &task, int index, QWidget *parent)
    : QWidget(parent), m_task(task), m_index(index)
{
    setFixedHeight(task.dueDate.isValid() ? 70 : 62);
    setStyleSheet("background:transparent;");
    QHBoxLayout *lay=new QHBoxLayout(this);
    lay->setContentsMargins(16,8,10,8); lay->setSpacing(10);

    QColor dc=diffColor();

    m_diffDot=new QLabel(this);
    m_diffDot->setFixedSize(14,14);
    m_diffDot->setStyleSheet(QString(
                                 "background:rgb(%1,%2,%3);border-radius:7px;")
                                 .arg(dc.red()).arg(dc.green()).arg(dc.blue()));

    m_check=new QCheckBox(this);
    m_check->setChecked(task.completed);
    m_check->setStyleSheet(QString(
                               "QCheckBox::indicator{width:20px;height:20px;"
                               "border:2px solid rgba(0,140,220,0.6);border-radius:5px;"
                               "background:rgba(4,14,40,0.9);}"
                               "QCheckBox::indicator:checked{background:rgb(%1,%2,%3);"
                               "border-color:rgb(%1,%2,%3);}")
                               .arg(dc.red()).arg(dc.green()).arg(dc.blue()));
    connect(m_check,&QCheckBox::checkStateChanged,this,[this](int){
        emit toggleCompleted(m_index);});

    // Name + date stacked vertically
    QWidget *info = new QWidget(this);
    info->setStyleSheet("background:transparent;");
    QVBoxLayout *infoL = new QVBoxLayout(info);
    infoL->setContentsMargins(0,0,0,0); infoL->setSpacing(2);

    m_nameLabel=new QLabel(task.name, info);
    m_nameLabel->setStyleSheet(task.completed
                                   ?"color:rgba(80,120,160,0.7);font-family:'Courier New';font-size:13px;"
                                     "text-decoration:line-through;background:transparent;"
                                   :"color:rgb(160,210,255);font-family:'Courier New';font-size:13px;"
                                     "font-weight:bold;background:transparent;");
    infoL->addWidget(m_nameLabel);

    m_dateLabel = new QLabel(info);
    if (task.dueDate.isValid()) {
        QDate today = QDate::currentDate();
        int daysLeft = today.daysTo(task.dueDate);
        QString dateStr = task.dueDate.toString("dd MMM yyyy");
        QString suffix;
        QColor dateColor;
        if (daysLeft < 0) {
            suffix = " (en retard)";
            dateColor = QColor(255, 80, 80, 200);
        } else if (daysLeft == 0) {
            suffix = " (aujourd'hui !)";
            dateColor = QColor(255, 180, 0, 220);
        } else if (daysLeft <= 3) {
            suffix = QString(" (%1j)").arg(daysLeft);
            dateColor = QColor(255, 160, 0, 200);
        } else {
            suffix = QString(" (%1j)").arg(daysLeft);
            dateColor = QColor(100, 180, 255, 160);
        }
        m_dateLabel->setText("📅 " + dateStr + suffix);
        m_dateLabel->setStyleSheet(QString(
                                       "color:rgba(%1,%2,%3,%4);font-family:'Courier New';"
                                       "font-size:10px;background:transparent;")
                                       .arg(dateColor.red()).arg(dateColor.green())
                                       .arg(dateColor.blue()).arg(dateColor.alpha()));
        infoL->addWidget(m_dateLabel);
    } else {
        m_dateLabel->hide();
    }

    m_timeLabel=new QLabel(QString("%1 min").arg(task.durationMin),this);
    m_timeLabel->setStyleSheet(
        "color:rgba(0,200,255,0.7);font-family:'Courier New';"
        "font-size:11px;background:transparent;min-width:50px;");
    m_timeLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    m_startBtn=new QPushButton("▶",this);
    m_startBtn->setFixedSize(32,32);
    m_startBtn->setCursor(Qt::PointingHandCursor);
    m_startBtn->setToolTip("Démarrer dans le Timer");
    m_startBtn->setStyleSheet(QString(
                                  "QPushButton{background:rgba(%1,%2,%3,0.15);color:rgb(%1,%2,%3);"
                                  "border:1px solid rgba(%1,%2,%3,0.4);border-radius:8px;"
                                  "font-size:13px;font-weight:bold;}"
                                  "QPushButton:hover{background:rgba(%1,%2,%3,0.35);}")
                                  .arg(dc.red()).arg(dc.green()).arg(dc.blue()));
    m_startBtn->setEnabled(!task.completed);
    m_startBtn->setVisible(!task.completed);
    connect(m_startBtn,&QPushButton::clicked,this,[this](){
        emit startTask(m_index);});

    m_deleteBtn=new QPushButton("✕",this);
    m_deleteBtn->setFixedSize(28,28);
    m_deleteBtn->setCursor(Qt::PointingHandCursor);
    m_deleteBtn->setStyleSheet(
        "QPushButton{background:rgba(200,50,50,0.15);color:rgba(255,100,100,0.7);"
        "border:1px solid rgba(200,50,50,0.3);border-radius:6px;font-size:12px;}"
        "QPushButton:hover{background:rgba(200,50,50,0.35);color:rgb(255,120,120);}");
    connect(m_deleteBtn,&QPushButton::clicked,this,[this](){emit deleteTask(m_index);});

    lay->addWidget(m_diffDot);
    lay->addWidget(m_check);
    lay->addWidget(info, 1);
    lay->addWidget(m_timeLabel);
    lay->addWidget(m_startBtn);
    lay->addWidget(m_deleteBtn);
}

void TaskCard::paintEvent(QPaintEvent *)
{
    QPainter p(this); p.setRenderHint(QPainter::Antialiasing);
    QRectF r=QRectF(rect()).adjusted(2,3,-2,-3);
    QColor dc=diffColor();
    p.setBrush(m_task.completed?QColor(6,14,36,160):QColor(8,20,54,200));
    p.setPen(Qt::NoPen); p.drawRoundedRect(r,10,10);
    QLinearGradient accent(r.left(),r.top(),r.left(),r.bottom());
    QColor a1=dc; a1.setAlpha(m_task.completed?60:180);
    QColor a2=dc; a2.setAlpha(0);
    accent.setColorAt(0,a1); accent.setColorAt(1,a2);
    p.setBrush(accent); p.setPen(Qt::NoPen);
    p.drawRoundedRect(QRectF(r.left(),r.top(),4,r.height()),2,2);

    // Overdue highlight
    if (m_task.dueDate.isValid() && m_task.dueDate < QDate::currentDate()
        && !m_task.completed) {
        p.setBrush(QColor(255,60,60,8));
        p.setPen(Qt::NoPen);
        p.drawRoundedRect(r,10,10);
    }

    QColor border=m_task.completed?QColor(0,60,100,60)
                                     :QColor(dc.red(),dc.green(),dc.blue(),55);
    p.setBrush(Qt::NoBrush); p.setPen(QPen(border,1));
    p.drawRoundedRect(r,10,10);
}

// ════════════════════════════════════════════════════════
//  DB helpers
// ════════════════════════════════════════════════════════
bool TodoWidget::initTasksTable()
{
    QString dataPath = QStandardPaths::writableLocation(
        QStandardPaths::AppDataLocation);
    QDir().mkpath(dataPath);
    QString dbPath = dataPath + "/flowtask_users.db";

    if (QSqlDatabase::contains("tasks_conn")) {
        m_db = QSqlDatabase::database("tasks_conn");
    } else {
        m_db = QSqlDatabase::addDatabase("QSQLITE", "tasks_conn");
        m_db.setDatabaseName(dbPath);
    }

    if (!m_db.isOpen() && !m_db.open()) return false;

    QSqlQuery q(m_db);
    q.exec(
        "CREATE TABLE IF NOT EXISTS tasks ("
        "  id           INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  username     TEXT    NOT NULL,"
        "  name         TEXT    NOT NULL,"
        "  duration_min INTEGER NOT NULL DEFAULT 30,"
        "  difficulty   INTEGER NOT NULL DEFAULT 0,"
        "  completed    INTEGER NOT NULL DEFAULT 0,"
        "  due_date     TEXT    NOT NULL DEFAULT ''"
        ")"
        );

    // Migrate existing tables that don't have due_date
    addDueDateColumn();

    return !q.lastError().isValid();
}

void TodoWidget::addDueDateColumn()
{
    // Safe migration: add column if it doesn't exist
    QSqlQuery check(m_db);
    check.exec("SELECT due_date FROM tasks LIMIT 1");
    if (check.lastError().isValid()) {
        QSqlQuery alter(m_db);
        alter.exec("ALTER TABLE tasks ADD COLUMN due_date TEXT NOT NULL DEFAULT ''");
    }
}

void TodoWidget::loadTasksFromDb()
{
    if (!AuthManager::instance().isLoggedIn()) return;
    m_tasks.clear();

    QSqlQuery q(m_db);
    q.prepare(
        "SELECT id, name, duration_min, difficulty, completed, due_date"
        " FROM tasks WHERE username = :u ORDER BY id ASC"
        );
    q.bindValue(":u", AuthManager::instance().currentUsername());
    q.exec();

    while (q.next()) {
        Task t;
        t.id          = q.value(0).toInt();
        t.name        = q.value(1).toString();
        t.durationMin = q.value(2).toInt();
        t.difficulty  = static_cast<Difficulty>(q.value(3).toInt());
        t.completed   = q.value(4).toBool();
        QString ds    = q.value(5).toString();
        t.dueDate     = ds.isEmpty() ? QDate() : QDate::fromString(ds, "yyyy-MM-dd");
        m_tasks.append(t);
    }
}

void TodoWidget::saveTaskToDb(Task &task)
{
    if (!AuthManager::instance().isLoggedIn()) return;

    QSqlQuery q(m_db);
    q.prepare(
        "INSERT INTO tasks (username, name, duration_min, difficulty, completed, due_date)"
        " VALUES (:u, :n, :d, :diff, :c, :dd)"
        );
    q.bindValue(":u",    AuthManager::instance().currentUsername());
    q.bindValue(":n",    task.name);
    q.bindValue(":d",    task.durationMin);
    q.bindValue(":diff", static_cast<int>(task.difficulty));
    q.bindValue(":c",    task.completed ? 1 : 0);
    q.bindValue(":dd",   task.dueDate.isValid()
                           ? task.dueDate.toString("yyyy-MM-dd") : "");
    q.exec();
    task.id = q.lastInsertId().toInt();
}

void TodoWidget::updateTaskInDb(const Task &task)
{
    if (!AuthManager::instance().isLoggedIn() || task.id < 0) return;

    QSqlQuery q(m_db);
    q.prepare(
        "UPDATE tasks SET name=:n, duration_min=:d, difficulty=:diff,"
        " completed=:c, due_date=:dd WHERE id=:id"
        );
    q.bindValue(":n",    task.name);
    q.bindValue(":d",    task.durationMin);
    q.bindValue(":diff", static_cast<int>(task.difficulty));
    q.bindValue(":c",    task.completed ? 1 : 0);
    q.bindValue(":dd",   task.dueDate.isValid()
                           ? task.dueDate.toString("yyyy-MM-dd") : "");
    q.bindValue(":id",   task.id);
    q.exec();
}

void TodoWidget::deleteTaskFromDb(int taskId)
{
    if (!AuthManager::instance().isLoggedIn() || taskId < 0) return;
    QSqlQuery q(m_db);
    q.prepare("DELETE FROM tasks WHERE id = :id");
    q.bindValue(":id", taskId);
    q.exec();
}

void TodoWidget::migrateLocalTasksToDb(const QVector<Task> &localTasks)
{
    for (int i = 0; i < localTasks.size(); ++i) {
        Task t = localTasks[i];
        t.id = -1;
        saveTaskToDb(t);
        m_tasks.append(t);
    }
}

// ════════════════════════════════════════════════════════
//  Auth state
// ════════════════════════════════════════════════════════
void TodoWidget::updateAuthState()
{
    bool logged = AuthManager::instance().isLoggedIn();
    if (logged) {
        QString name = AuthManager::instance().currentUsername();
        m_authBtn->setText("◉  " + name.toUpper());
        m_authBtn->setStyleSheet(
            "QPushButton{background:rgba(0,60,30,0.9);color:rgb(60,220,100);"
            "border:1px solid rgba(60,200,100,0.45);border-radius:8px;"
            "font-family:'Courier New';font-size:11px;font-weight:bold;letter-spacing:2px;}"
            "QPushButton:hover{background:rgba(0,90,40,0.9);}");
        m_planningBtn->setVisible(true);
    } else {
        m_authBtn->setText("CONNEXION");
        m_authBtn->setStyleSheet(
            "QPushButton{background:rgba(4,14,40,0.9);color:rgb(255,180,0);"
            "border:1px solid rgba(255,160,0,0.5);border-radius:8px;"
            "font-family:'Courier New';font-size:11px;font-weight:bold;letter-spacing:2px;}"
            "QPushButton:hover{background:rgba(60,40,0,0.9);}");
        m_planningBtn->setVisible(false);
    }
}

QVector<Task> TodoWidget::getTasks() const { return m_tasks; }

void TodoWidget::addTaskFromTimer(const QString &name, int durationMin)
{
    for (int i = 0; i < m_tasks.size(); ++i)
        if (m_tasks[i].name.toLower() == name.toLower()) return;

    Task t;
    t.id          = -1;
    t.name        = name;
    t.durationMin = durationMin > 0 ? durationMin : 25;
    t.difficulty  = Difficulty::Medium;
    t.completed   = false;

    if (AuthManager::instance().isLoggedIn()) saveTaskToDb(t);
    m_tasks.append(t);
    rebuildList();
}

void TodoWidget::onAuthChanged(bool keepLocalTasks)
{
    QVector<Task> local = m_tasks;
    m_tasks.clear();

    if (AuthManager::instance().isLoggedIn()) {
        loadTasksFromDb();
        if (keepLocalTasks && !local.isEmpty())
            migrateLocalTasksToDb(local);
    }
    updateAuthState();
    rebuildList();
}

// ════════════════════════════════════════════════════════
//  Calendar toggle
// ════════════════════════════════════════════════════════
void TodoWidget::toggleCalendar()
{
    bool visible = !m_calendarPanel->isVisible();
    m_calendarPanel->setVisible(visible);
    if (visible && m_datePicker->selectedDate().isValid())
        m_dateToggleBtn->setText("📅 " +
                                 m_datePicker->selectedDate().toString("dd/MM/yyyy") + "  ✕");
    else if (visible)
        m_dateToggleBtn->setText("📅 Choisir une date  ▲");
    else
        m_dateToggleBtn->setText(
            m_datePicker->selectedDate().isValid()
                ? "📅 " + m_datePicker->selectedDate().toString("dd/MM/yyyy")
                : "📅 Ajouter une date");
}

// ════════════════════════════════════════════════════════
//  buildHeader
// ════════════════════════════════════════════════════════
QWidget *TodoWidget::buildHeader()
{
    QWidget *w=new QWidget(this);
    w->setFixedHeight(60);
    w->setStyleSheet("background:rgba(6,14,38,0.95);"
                     "border-bottom:1px solid rgba(0,120,200,0.30);");
    QHBoxLayout *lay=new QHBoxLayout(w);
    lay->setContentsMargins(24,0,24,0);

    QLabel *logo=new QLabel("✓  ASIS— TO-DO",w);
    logo->setStyleSheet("color:rgb(60,220,100);font-family:'Courier New';"
                        "font-size:16px;font-weight:bold;letter-spacing:4px;"
                        "background:transparent;");
    lay->addWidget(logo); lay->addStretch();

    QPushButton *timerBtn=new QPushButton("TIMER",w);
    timerBtn->setFixedSize(100,36); timerBtn->setCursor(Qt::PointingHandCursor);
    timerBtn->setStyleSheet(
        "QPushButton{background:rgba(4,14,40,0.9);color:rgb(0,200,255);"
        "border:1px solid rgba(0,160,220,0.55);border-radius:8px;"
        "font-family:'Courier New';font-size:11px;font-weight:bold;letter-spacing:2px;}"
        "QPushButton:hover{background:rgba(0,60,110,0.9);}");
    connect(timerBtn,&QPushButton::clicked,this,&TodoWidget::goToTimer);

    m_planningBtn=new QPushButton("🗓 PLANNING",w);
    m_planningBtn->setFixedSize(130,36);
    m_planningBtn->setCursor(Qt::PointingHandCursor);
    m_planningBtn->setVisible(false);
    m_planningBtn->setStyleSheet(
        "QPushButton{background:rgba(4,14,40,0.9);color:rgb(60,220,100);"
        "border:1px solid rgba(60,200,100,0.45);border-radius:8px;"
        "font-family:'Courier New';font-size:11px;font-weight:bold;letter-spacing:2px;}"
        "QPushButton:hover{background:rgba(0,80,40,0.9);}");
    connect(m_planningBtn,&QPushButton::clicked,this,&TodoWidget::goToPlanning);

    m_authBtn=new QPushButton("CONNEXION",w);
    m_authBtn->setFixedSize(140,36); m_authBtn->setCursor(Qt::PointingHandCursor);
    connect(m_authBtn,&QPushButton::clicked,this,&TodoWidget::goToLogin);
    updateAuthState();

    lay->addWidget(timerBtn); lay->addSpacing(8);
    lay->addWidget(m_planningBtn); lay->addSpacing(8);
    lay->addWidget(m_authBtn);
    return w;
}

// ════════════════════════════════════════════════════════
//  buildInputArea
// ════════════════════════════════════════════════════════
QWidget *TodoWidget::buildInputArea()
{
    QWidget *w=new QWidget(this);
    w->setStyleSheet("background:rgba(8,18,50,0.85);"
                     "border:1px solid rgba(0,120,200,0.30);border-radius:12px;");
    QVBoxLayout *vl=new QVBoxLayout(w);
    vl->setContentsMargins(20,16,20,16); vl->setSpacing(10);

    QLabel *title=new QLabel("NOUVELLE TÂCHE",w);
    title->setStyleSheet("color:rgba(0,200,255,0.8);font-family:'Courier New';"
                         "font-size:11px;letter-spacing:4px;background:transparent;");

    m_nameInput=new QLineEdit(w);
    m_nameInput->setPlaceholderText("Nom de la tâche...");
    m_nameInput->setFixedHeight(40);
    m_nameInput->setStyleSheet(inputStyle());

    // Row: duration + difficulty + add button
    QHBoxLayout *row=new QHBoxLayout; row->setSpacing(10);
    auto lbl=[&](const QString &t)->QLabel*{
        QLabel *l=new QLabel(t,w);
        l->setStyleSheet("color:rgba(100,160,220,0.8);font-family:'Courier New';"
                         "font-size:10px;letter-spacing:2px;background:transparent;");
        return l;};

    m_durationSpin=new QSpinBox(w);
    m_durationSpin->setRange(1,480); m_durationSpin->setValue(30);
    m_durationSpin->setFixedSize(80,36);
    m_durationSpin->setStyleSheet(spinStyle());

    m_btnEasy  =new DiffButton("FACILE",   C_EASY,  w);
    m_btnMedium=new DiffButton("MOYEN",    C_MEDIUM,w);
    m_btnHard  =new DiffButton("DIFFICILE",C_HARD,  w);
    m_btnEasy->setSelected(true);

    connect(m_btnEasy,  &QPushButton::clicked,this,[this]{selectDifficulty(0);});
    connect(m_btnMedium,&QPushButton::clicked,this,[this]{selectDifficulty(1);});
    connect(m_btnHard,  &QPushButton::clicked,this,[this]{selectDifficulty(2);});

    // Date toggle button
    m_dateToggleBtn = new QPushButton("📅 Ajouter une date", w);
    m_dateToggleBtn->setFixedHeight(34);
    m_dateToggleBtn->setCursor(Qt::PointingHandCursor);
    m_dateToggleBtn->setStyleSheet(
        "QPushButton{background:rgba(0,60,120,0.5);color:rgb(100,180,255);"
        "border:1px solid rgba(0,120,200,0.4);border-radius:8px;"
        "font-family:'Courier New';font-size:10px;padding:0 10px;}"
        "QPushButton:hover{background:rgba(0,90,160,0.6);}");
    connect(m_dateToggleBtn, &QPushButton::clicked, this, &TodoWidget::toggleCalendar);

    // Clear date button
    QPushButton *clearDate = new QPushButton("✕", w);
    clearDate->setFixedSize(26, 26);
    clearDate->setCursor(Qt::PointingHandCursor);
    clearDate->setToolTip("Effacer la date");
    clearDate->setStyleSheet(
        "QPushButton{background:rgba(100,30,30,0.4);color:rgba(255,100,100,0.8);"
        "border:1px solid rgba(200,50,50,0.3);border-radius:6px;font-size:11px;}"
        "QPushButton:hover{background:rgba(180,40,40,0.5);}");
    connect(clearDate, &QPushButton::clicked, this, [this](){
        m_datePicker->setSelectedDate(QDate());
        m_calendarPanel->setVisible(false);
        m_dateToggleBtn->setText("📅 Ajouter une date");
    });

    QPushButton *addBtn=new QPushButton("+ AJOUTER",w);
    addBtn->setFixedSize(120,36); addBtn->setCursor(Qt::PointingHandCursor);
    addBtn->setStyleSheet(
        "QPushButton{background:rgba(0,80,40,0.8);color:rgb(60,220,100);"
        "border:1px solid rgba(60,200,100,0.55);border-radius:8px;"
        "font-family:'Courier New';font-size:11px;font-weight:bold;letter-spacing:2px;}"
        "QPushButton:hover{background:rgba(0,120,60,0.9);}");
    connect(addBtn,&QPushButton::clicked,this,&TodoWidget::onAddTask);
    connect(m_nameInput,&QLineEdit::returnPressed,this,&TodoWidget::onAddTask);

    row->addWidget(lbl("DURÉE")); row->addWidget(m_durationSpin);
    row->addSpacing(6);
    row->addWidget(lbl("DIFFICULTÉ"));
    row->addWidget(m_btnEasy); row->addWidget(m_btnMedium); row->addWidget(m_btnHard);
    row->addSpacing(6);
    row->addWidget(m_dateToggleBtn, 1);
    row->addWidget(clearDate);
    row->addSpacing(6);
    row->addWidget(addBtn);

    // Calendar panel (hidden by default)
    m_calendarPanel = new QWidget(w);
    m_calendarPanel->setVisible(false);
    QVBoxLayout *calL = new QVBoxLayout(m_calendarPanel);
    calL->setContentsMargins(0, 4, 0, 0); calL->setSpacing(0);

    m_datePicker = new DatePickerWidget(m_calendarPanel);
    connect(m_datePicker, &DatePickerWidget::dateSelected,
            this, [this](QDate d) {
                m_calendarPanel->setVisible(false);
                m_dateToggleBtn->setText("📅 " + d.toString("dd/MM/yyyy"));
            });
    calL->addWidget(m_datePicker);

    vl->addWidget(title);
    vl->addWidget(m_nameInput);
    vl->addLayout(row);
    vl->addWidget(m_calendarPanel);
    return w;
}

// ════════════════════════════════════════════════════════
//  buildStatsBar
// ════════════════════════════════════════════════════════
QWidget *TodoWidget::buildStatsBar()
{
    QWidget *w=new QWidget(this);
    w->setFixedHeight(44);
    w->setStyleSheet("background:rgba(6,14,38,0.70);"
                     "border:1px solid rgba(0,100,180,0.25);border-radius:8px;");
    QHBoxLayout *lay=new QHBoxLayout(w);
    lay->setContentsMargins(20,0,20,0);
    QString s="color:rgba(100,170,230,0.85);font-family:'Courier New';"
                "font-size:11px;background:transparent;";
    m_statsDone =new QLabel("✓ 0 terminées",w); m_statsDone->setStyleSheet(s);
    m_statsTotal=new QLabel("◈ 0 tâches",w);    m_statsTotal->setStyleSheet(s);
    m_statsTime =new QLabel("⏱ 0 min",w);       m_statsTime->setStyleSheet(s);
    lay->addWidget(m_statsDone); lay->addStretch();
    lay->addWidget(m_statsTotal); lay->addStretch();
    lay->addWidget(m_statsTime);
    return w;
}

// ════════════════════════════════════════════════════════
//  Constructor
// ════════════════════════════════════════════════════════
TodoWidget::TodoWidget(QWidget *parent) : QWidget(parent)
{
    setMinimumSize(760,560);
    setStyleSheet("background:transparent;");

    initTasksTable();

    QVBoxLayout *root=new QVBoxLayout(this);
    root->setContentsMargins(0,0,0,0); root->setSpacing(0);
    root->addWidget(buildHeader());

    QWidget *content=new QWidget(this);
    content->setStyleSheet("background:transparent;");
    QVBoxLayout *cl=new QVBoxLayout(content);
    cl->setContentsMargins(36,24,36,24); cl->setSpacing(16);
    cl->addWidget(buildInputArea());
    cl->addWidget(buildStatsBar());

    QLabel *listTitle=new QLabel("MES TÂCHES",content);
    listTitle->setStyleSheet("color:rgba(0,200,255,0.6);font-family:'Courier New';"
                             "font-size:10px;letter-spacing:5px;background:transparent;");
    cl->addWidget(listTitle);

    QScrollArea *scroll=new QScrollArea(content);
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setStyleSheet(
        "QScrollArea{background:transparent;border:none;}"
        "QScrollBar:vertical{background:rgba(4,12,32,0.8);width:6px;border-radius:3px;}"
        "QScrollBar::handle:vertical{background:rgba(0,140,220,0.5);"
        "border-radius:3px;min-height:20px;}"
        "QScrollBar::add-line:vertical,QScrollBar::sub-line:vertical{height:0;}");

    m_listContainer=new QWidget;
    m_listContainer->setStyleSheet("background:transparent;");
    m_listLayout=new QVBoxLayout(m_listContainer);
    m_listLayout->setContentsMargins(0,0,0,0); m_listLayout->setSpacing(8);
    m_listLayout->addStretch();
    scroll->setWidget(m_listContainer);
    cl->addWidget(scroll,1);
    root->addWidget(content,1);

    // Background animation
    m_animTime = 0.f;
    QTimer *bgAnim = new QTimer(this);
    bgAnim->setInterval(33);
    connect(bgAnim, &QTimer::timeout, this, [this](){ m_animTime += 0.033f; update(); });
    bgAnim->start();
}

// ════════════════════════════════════════════════════════
//  rebuildList
// ════════════════════════════════════════════════════════
void TodoWidget::rebuildList()
{
    QLayoutItem *item;
    while((item=m_listLayout->takeAt(0))!=nullptr){
        if(item->widget()) item->widget()->deleteLater();
        delete item;
    }

    // Sort: incomplete first, then by due date (nearest first), then no date
    QVector<int> indices;
    for (int i = 0; i < m_tasks.size(); ++i) indices.append(i);
    // Bubble sort by: incomplete first, then due date
    for (int i = 0; i < indices.size()-1; ++i) {
        for (int j = i+1; j < indices.size(); ++j) {
            const Task &a = m_tasks[indices[i]];
            const Task &b = m_tasks[indices[j]];
            bool swap = false;
            if (a.completed && !b.completed) swap = true;
            else if (!a.completed && !b.completed) {
                if (!a.dueDate.isValid() && b.dueDate.isValid()) swap = true;
                else if (a.dueDate.isValid() && b.dueDate.isValid()
                         && a.dueDate > b.dueDate) swap = true;
            }
            if (swap) qSwap(indices[i], indices[j]);
        }
    }

    for (int i = 0; i < indices.size(); ++i) {
        TaskCard *card = new TaskCard(m_tasks[indices[i]], indices[i], m_listContainer);
        connect(card,&TaskCard::toggleCompleted,this,&TodoWidget::onToggleCompleted);
        connect(card,&TaskCard::deleteTask,     this,&TodoWidget::onDeleteTask);
        connect(card,&TaskCard::startTask,      this,&TodoWidget::onStartTask);
        m_listLayout->addWidget(card);
    }

    if (m_tasks.isEmpty()) {
        QLabel *empty=new QLabel("Aucune tâche. Ajoutez-en une ci-dessus.",m_listContainer);
        empty->setAlignment(Qt::AlignCenter);
        empty->setStyleSheet("color:rgba(60,100,160,0.7);font-family:'Courier New';"
                             "font-size:12px;background:transparent;padding:40px;");
        m_listLayout->addWidget(empty);
    }
    m_listLayout->addStretch();

    int done=0,totalMin=0;
    for (int i = 0; i < m_tasks.size(); ++i) {
        if (m_tasks[i].completed) done++;
        totalMin += m_tasks[i].durationMin;
    }
    m_statsDone->setText(QString("✓ %1 terminées").arg(done));
    m_statsTotal->setText(QString("◈ %1 tâches").arg(m_tasks.size()));
    m_statsTime->setText(QString("⏱ %1 min").arg(totalMin));
}

// ════════════════════════════════════════════════════════
//  Slots
// ════════════════════════════════════════════════════════
void TodoWidget::selectDifficulty(int d)
{
    m_selectedDiff=static_cast<Difficulty>(d);
    m_btnEasy->setSelected(d==0);
    m_btnMedium->setSelected(d==1);
    m_btnHard->setSelected(d==2);
}

void TodoWidget::onAddTask()
{
    QString name=m_nameInput->text().trimmed();
    if (name.isEmpty()) return;

    Task t;
    t.id          = -1;
    t.name        = name;
    t.durationMin = m_durationSpin->value();
    t.difficulty  = m_selectedDiff;
    t.completed   = false;
    t.dueDate     = m_datePicker->selectedDate();

    if (AuthManager::instance().isLoggedIn()) saveTaskToDb(t);
    m_tasks.append(t);
    m_nameInput->clear();

    // Reset date picker
    m_datePicker->setSelectedDate(QDate());
    m_calendarPanel->setVisible(false);
    m_dateToggleBtn->setText("📅 Ajouter une date");

    rebuildList();
}

void TodoWidget::onToggleCompleted(int index)
{
    if(index<0||index>=m_tasks.size()) return;
    m_tasks[index].completed = !m_tasks[index].completed;
    updateTaskInDb(m_tasks[index]);
    if(AuthManager::instance().isLoggedIn()){
        int done=0;
        for(int i=0;i<m_tasks.size();++i) if(m_tasks[i].completed) done++;
        SessionManager::instance().endSession(done);
    }
    rebuildList();
}

void TodoWidget::onDeleteTask(int index)
{
    if(index<0||index>=m_tasks.size()) return;
    deleteTaskFromDb(m_tasks[index].id);
    m_tasks.removeAt(index);
    rebuildList();
}

void TodoWidget::onStartTask(int index)
{
    if(index<0||index>=m_tasks.size()) return;
    const Task &t=m_tasks[index];
    emit startTaskInTimer(t.name, t.durationMin);
    emit goToTimer();
}

// ════════════════════════════════════════════════════════
//  paintEvent
// ════════════════════════════════════════════════════════
void TodoWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QLinearGradient bg(0,0,0,height());
    bg.setColorAt(0.00, QColor(2,  6, 20));
    bg.setColorAt(0.40, QColor(3, 12, 30));
    bg.setColorAt(0.75, QColor(2, 10, 25));
    bg.setColorAt(1.00, QColor(1,  5, 18));
    p.fillRect(rect(), bg);

    QRadialGradient aur1(width()*0.25f, height()*0.2f, width()*0.55f);
    aur1.setColorAt(0.0, QColor(0, 100, 60, 28));
    aur1.setColorAt(1.0, QColor(0,   0,  0,  0));
    p.fillRect(rect(), aur1);

    QRadialGradient aur2(width()*0.75f, height()*0.7f, width()*0.45f);
    aur2.setColorAt(0.0, QColor(0, 80, 140, 22));
    aur2.setColorAt(1.0, QColor(0,  0,   0,  0));
    p.fillRect(rect(), aur2);

    // Floating hexagons
    p.save();
    p.setCompositionMode(QPainter::CompositionMode_Plus);
    p.setBrush(Qt::NoBrush);
    float t = m_animTime;
    for (int row=-1; row<8; ++row) {
        for (int col=-1; col<12; ++col) {
            float hx = col*110.f + (row%2)*55.f;
            float hy = row*95.f + std::sin(t*0.4f+row*0.7f+col*0.5f)*6.f;
            float alpha = 6.f + 4.f*std::sin(t*0.3f+row+col);
            p.setPen(QPen(QColor(60,200,120,int(alpha)),0.6f));
            QPainterPath hex;
            for (int k=0;k<6;++k) {
                float a=k*M_PI/3.f;
                float px=hx+std::cos(a)*28.f, py=hy+std::sin(a)*28.f;
                if(k==0) hex.moveTo(px,py); else hex.lineTo(px,py);
            }
            hex.closeSubpath();
            p.drawPath(hex);
        }
    }
    p.restore();

    // Vignette
    QLinearGradient vig(0,0,0,height());
    vig.setColorAt(0.00, QColor(0,0,0,100));
    vig.setColorAt(0.15, QColor(0,0,0,0));
    vig.setColorAt(0.85, QColor(0,0,0,0));
    vig.setColorAt(1.00, QColor(0,0,0,80));
    p.fillRect(rect(), vig);
}