#ifndef STATSWIDGET_H
#define STATSWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QVector>
#include "sessionmanager.h"

// ─────────────────────────────────────────────────────────
//  MiniBarChart — draws a simple bar chart inline
// ─────────────────────────────────────────────────────────
class MiniBarChart : public QWidget
{
    Q_OBJECT
public:
    explicit MiniBarChart(QWidget *parent = nullptr);
    void setData(const QVector<DailyStats> &data);
    void setHourlyData(const QVector<HourlyProductivity> &data);
    void setMode(bool hourly) { m_hourlyMode = hourly; update(); }
protected:
    void paintEvent(QPaintEvent *) override;
private:
    QVector<DailyStats>         m_daily;
    QVector<HourlyProductivity> m_hourly;
    bool                        m_hourlyMode = false;
};

// ─────────────────────────────────────────────────────────
//  StatCard — single KPI card
// ─────────────────────────────────────────────────────────
class StatCard : public QWidget
{
    Q_OBJECT
public:
    explicit StatCard(const QString &icon, const QString &label,
                      const QString &value, QColor accent,
                      QWidget *parent = nullptr);
    void setValue(const QString &v);
protected:
    void paintEvent(QPaintEvent *) override;
private:
    QLabel *m_valueLabel;
    QColor  m_accent;
};

// ─────────────────────────────────────────────────────────
//  StatsWidget — full statistics page (Premium)
// ─────────────────────────────────────────────────────────
class StatsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StatsWidget(QWidget *parent = nullptr);
    void refresh();   // call when page becomes visible

signals:
    void goBack();

protected:
    void paintEvent(QPaintEvent *) override;

private:
    QWidget *buildHeader();
    QWidget *buildKpiRow();
    QWidget *buildChartSection();
    QWidget *buildInsightSection();

    void     updateKpis();
    void     updateCharts();
    void     updateInsights();

    StatCard     *m_cardScore;
    StatCard     *m_cardSessions;
    StatCard     *m_cardTime;
    StatCard     *m_cardBestHour;

    MiniBarChart *m_barDaily;
    MiniBarChart *m_barHourly;

    QLabel       *m_insightLabel;
    QPushButton  *m_toggleChartBtn;
    bool          m_showHourly = false;
    float         m_animTime  = 0.f;
};

#endif // STATSWIDGET_H