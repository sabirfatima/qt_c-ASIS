#ifndef TODOWIDGET_H
#define TODOWIDGET_H
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QPainterPath>
#include <QLinearGradient>
#include <QRadialGradient>
#include <QVector>
#include <QString>
#include <QSqlDatabase>
#include <QDate>
#include <QTimer>
#include <QFrame>

enum class Difficulty { Easy, Medium, Hard };

struct Task {
    int        id = -1;
    QString    name;
    int        durationMin;
    Difficulty difficulty;
    bool       completed;
    QDate      dueDate;       // ← NEW: optional due date
};

// ─────────────────────────────────────────────────────────
//  DatePickerWidget — inline custom calendar
// ─────────────────────────────────────────────────────────
class DatePickerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DatePickerWidget(QWidget *parent = nullptr);
    QDate selectedDate() const { return m_selected; }
    void  setSelectedDate(const QDate &d);

signals:
    void dateSelected(QDate date);

protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *) override;

private:
    void prevMonth();
    void nextMonth();
    QRect cellRect(int row, int col) const;
    int   dayAt(int row, int col) const;

    QDate   m_selected;
    QDate   m_viewing;   // which month is displayed
    QPushButton *m_prevBtn;
    QPushButton *m_nextBtn;
    QLabel      *m_monthLabel;
};

// ─────────────────────────────────────────────────────────
class DiffButton : public QPushButton
{
    Q_OBJECT
public:
    explicit DiffButton(const QString &text, QColor col, QWidget *parent = nullptr);
    void setSelected(bool s) { m_selected = s; update(); }
protected:
    void paintEvent(QPaintEvent *) override;
private:
    QColor m_color;
    bool   m_selected = false;
};

// ─────────────────────────────────────────────────────────
class TaskCard : public QWidget
{
    Q_OBJECT
public:
    explicit TaskCard(const Task &task, int index, QWidget *parent = nullptr);
signals:
    void toggleCompleted(int index);
    void deleteTask(int index);
    void startTask(int index);
protected:
    void paintEvent(QPaintEvent *) override;
private:
    Task        m_task;
    int         m_index;
    QCheckBox  *m_check;
    QLabel     *m_nameLabel;
    QLabel     *m_timeLabel;
    QLabel     *m_dateLabel;   // ← NEW
    QLabel     *m_diffDot;
    QPushButton*m_startBtn;
    QPushButton*m_deleteBtn;
    QColor      diffColor() const;
};

// ─────────────────────────────────────────────────────────
class TodoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TodoWidget(QWidget *parent = nullptr);
    void updateAuthState();
    QVector<Task> getTasks() const;
    void addTaskFromTimer(const QString &name, int durationMin);
    void onAuthChanged(bool keepLocalTasks);

signals:
    void goToTimer();
    void goToLogin();
    void goToPlanning();
    void startTaskInTimer(const QString &name, int durationMin);

protected:
    void paintEvent(QPaintEvent *) override;

private slots:
    void onAddTask();
    void onToggleCompleted(int index);
    void onDeleteTask(int index);
    void onStartTask(int index);
    void selectDifficulty(int d);
    void toggleCalendar();

private:
    bool   initTasksTable();
    void   loadTasksFromDb();
    void   saveTaskToDb(Task &task);
    void   updateTaskInDb(const Task &task);
    void   deleteTaskFromDb(int taskId);
    void   migrateLocalTasksToDb(const QVector<Task> &localTasks);
    void   addDueDateColumn();   // ← migrate existing DB

    QWidget *buildHeader();
    QWidget *buildInputArea();
    QWidget *buildStatsBar();
    void     rebuildList();
    QColor   diffColor(Difficulty d) const;

    QLineEdit        *m_nameInput;
    QSpinBox         *m_durationSpin;
    DiffButton       *m_btnEasy;
    DiffButton       *m_btnMedium;
    DiffButton       *m_btnHard;
    Difficulty        m_selectedDiff = Difficulty::Easy;
    DatePickerWidget *m_datePicker;
    QWidget          *m_calendarPanel;
    QPushButton      *m_dateToggleBtn;

    QVBoxLayout *m_listLayout;
    QWidget     *m_listContainer;

    QLabel      *m_statsDone;
    QLabel      *m_statsTotal;
    QLabel      *m_statsTime;
    QPushButton *m_authBtn;
    QPushButton *m_planningBtn;
    QVector<Task> m_tasks;

    QSqlDatabase m_db;
    float        m_animTime = 0.f;
};

#endif // TODOWIDGET_H