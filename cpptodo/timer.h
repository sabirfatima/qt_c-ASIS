#ifndef TIMER_H
#define TIMER_H

#include <QWidget>
#include <QTimer>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QStackedWidget>
#include <QPainter>
#include <QPainterPath>
#include <QLinearGradient>
#include <QRadialGradient>
#include <QtMath>
#include <QVector>
#include <QPointF>

enum class TimerMode { Croissant, Decroissant, Pomodoro };

// ─────────────────────────────────────────────────────────
struct Particle {
    QPointF pos;
    QPointF vel;
    float   size;
    float   alpha;
    float   alphaSpeed;
    QColor  color;
};

// ─────────────────────────────────────────────────────────
class BackgroundWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BackgroundWidget(QWidget *parent = nullptr);
    void tick();
protected:
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *) override;
private:
    void initParticles();
    QVector<Particle> m_particles;
    float m_pulseAngle = 0.0f;
    float m_waveOffset = 0.0f;
};

// ─────────────────────────────────────────────────────────
class ModeButton : public QPushButton
{
    Q_OBJECT
public:
    explicit ModeButton(const QString &text, QWidget *parent = nullptr);
    void setSelected(bool s) { m_selected = s; update(); }
protected:
    void paintEvent(QPaintEvent *) override;
private:
    bool m_selected = false;
};

// ─────────────────────────────────────────────────────────
class ControlButton : public QPushButton
{
    Q_OBJECT
public:
    explicit ControlButton(const QString &text, QColor accent, QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *) override;
private:
    QColor m_accent;
};

// ─────────────────────────────────────────────────────────
class ArcDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit ArcDisplay(QWidget *parent = nullptr);
    void setTimeText(const QString &t) { m_timeText = t; update(); }
    void setSubText(const QString &t)  { m_subText  = t; update(); }
    void setTaskText(const QString &t) { m_taskText = t; update(); }
    void setProgress(float p)          { m_progress = qBound(0.0f, p, 1.0f); update(); }
    void setMode(TimerMode m)          { m_mode = m; update(); }
    void setPulsing(bool on)           { m_pulsing = on; }
    void tick();
protected:
    void paintEvent(QPaintEvent *) override;
private:
    QColor arcColor() const;
    QString   m_timeText = "00:00";
    QString   m_subText  = "PRÊT";
    QString   m_taskText = "";
    float     m_progress = 0.0f;
    bool      m_pulsing  = false;
    float     m_pulse    = 1.0f;
    bool      m_pulseUp  = false;
    TimerMode m_mode     = TimerMode::Croissant;
};

// ─────────────────────────────────────────────────────────
class TimerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TimerWidget(QWidget *parent = nullptr);
    void updateAuthState();

    // ← Appelé depuis main.cpp quand TodoWidget envoie une tâche
    void loadTask(const QString &name, int durationMin);

signals:
    void goToLogin();
    void goToTodoList();
    void goToStats();

    // ← Envoyé au START : ajoute la tâche dans TodoList
    void taskStarted(const QString &name, int durationMin);

protected:
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *) override;

private slots:
    void onTick();
    void onStart();
    void onPause();
    void onReset();
    void selectMode(int id);

private:
    QWidget *buildHeader();
    QWidget *buildModeSelector();
    QWidget *buildConfigPanel();
    QWidget *buildControls();
    QWidget *buildTaskInput();

    QString  formatTime(int sec) const;
    void     refreshDisplay();
    void     applyConfig();
    void     nextPomodoroPhase();

    TimerMode  m_mode       = TimerMode::Croissant;
    bool       m_running    = false;
    bool       m_started    = false;   // true after first Start, false after Reset
    int        m_elapsed    = 0;
    int        m_remaining  = 0;
    int        m_total      = 0;
    int        m_pomFocus   = 25;
    int        m_pomBreak   = 5;
    int        m_pomCycles  = 4;
    int        m_pomCurrent = 1;
    bool       m_pomOnBreak = false;

    QTimer          *m_ticker;
    ArcDisplay      *m_arc;
    BackgroundWidget*m_bg;
    ControlButton   *m_btnStart;
    ControlButton   *m_btnPause;
    ControlButton   *m_btnReset;
    ModeButton      *m_btnCroissant;
    ModeButton      *m_btnDecroissant;
    ModeButton      *m_btnPomodoro;
    QButtonGroup    *m_modeGroup;
    QStackedWidget  *m_configStack;
    QSpinBox        *m_decMin;
    QSpinBox        *m_decSec;
    QSpinBox        *m_pomFocusSpin;
    QSpinBox        *m_pomBreakSpin;
    QSpinBox        *m_pomCyclesSpin;
    QLineEdit       *m_taskInput;

    QPushButton     *m_authBtn;
    QPushButton     *m_statsBtn;
};

#endif // TIMER_H