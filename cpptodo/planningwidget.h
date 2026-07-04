#ifndef PLANNINGWIDGET_H
#define PLANNINGWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QPainter>
#include <QPainterPath>
#include <QVector>
#include <QDate>
#include <QTimer>
#include <QMap>
#include "todowidget.h"
#include "sessionmanager.h"

struct PlannedTask {
    Task    task;
    int     hour;
    QString reason;
};

// ─────────────────────────────────────────────────────────
//  PlanCard  — one row in the AI list view
// ─────────────────────────────────────────────────────────
class PlanCard : public QWidget
{
    Q_OBJECT
public:
    explicit PlanCard(const PlannedTask &pt, QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *) override;
private:
    PlannedTask m_pt;
    QColor      diffColor() const;
};

// ─────────────────────────────────────────────────────────
//  CalendarView  — weekly OR monthly Google-Calendar-style grid
// ─────────────────────────────────────────────────────────
class CalendarView : public QWidget
{
    Q_OBJECT
public:
    enum Mode { Weekly, Monthly };

    explicit CalendarView(QWidget *parent = nullptr);
    void setMode(Mode m);
    void setTasks(const QVector<Task> &tasks);
    void setWeekStart(const QDate &monday);
    void setMonthStart(const QDate &firstOfMonth);

    void goNext();
    void goPrev();

    QDate currentWeekStart()  const { return m_weekStart; }
    QDate currentMonthStart() const { return m_monthStart; }
    QString headerText() const;

protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *) override;

private:
    void drawWeekly(QPainter &p);
    void drawMonthly(QPainter &p);
    void drawTaskChip(QPainter &p, const QRectF &r, const Task &t, bool small);
    QColor diffColor(Difficulty d) const;

    // Tasks that have a dueDate, keyed by date
    QMap<QDate, QVector<Task>> m_tasksByDate;
    // All tasks (for weekly hour-based display)
    QVector<Task> m_allTasks;

    Mode   m_mode      = Weekly;
    QDate  m_weekStart;    // always a Monday
    QDate  m_monthStart;   // always 1st of month
};

// ─────────────────────────────────────────────────────────
//  PlanningWidget
// ─────────────────────────────────────────────────────────
class PlanningWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlanningWidget(QWidget *parent = nullptr);
    void setTasks(const QVector<Task> &tasks);
    void refresh();

signals:
    void goBack();

protected:
    void paintEvent(QPaintEvent *) override;

private slots:
    void switchToList();
    void switchToWeek();
    void switchToMonth();
    void onPrev();
    void onNext();

private:
    QWidget *buildHeader();
    QWidget *buildViewSelector();
    QWidget *buildNavBar();
    QWidget *buildListView();

    QVector<PlannedTask> generatePlan(const QVector<Task> &tasks) const;
    void rebuildPlanList();

    // Subviews
    QWidget      *m_listView;
    CalendarView *m_calView;
    QWidget      *m_navBar;
    QLabel       *m_navLabel;

    // List view internals
    QVBoxLayout  *m_planLayout;
    QWidget      *m_planContainer;
    QLabel       *m_emptyLabel;

    // Mode buttons
    QPushButton  *m_btnList;
    QPushButton  *m_btnWeek;
    QPushButton  *m_btnMonth;

    QVector<Task> m_tasks;
    float         m_animTime = 0.f;
};

#endif // PLANNINGWIDGET_H