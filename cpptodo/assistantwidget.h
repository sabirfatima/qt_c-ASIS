#ifndef ASSISTANTWIDGET_H
#define ASSISTANTWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QPropertyAnimation>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

// ─────────────────────────────────────────────────────────
//  États émotionnels du personnage
// ─────────────────────────────────────────────────────────
enum class AssistantState {
    Idle,       // 😊 Sourire calme — accueil
    Studying,   // 🧐 Concentré     — timer en cours
    Sleeping,   // 😴 Dort          — timer pausé/reset
    Happy,      // 😄 Joyeux        — tâche complétée
    Cheering,   // 🎉 Victoire      — timer terminé
    Thinking,   // 🤔 Réfléchit     — todo list
    Proud,      // 😎 Fier          — login réussi
    Typing,     // ⌨️  Tape          — saisie tâche
};

// ─────────────────────────────────────────────────────────
//  Le personnage dessiné en QPainter
// ─────────────────────────────────────────────────────────
class AssistantCharacter : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(float bounce READ bounce WRITE setBounce)
    Q_PROPERTY(float armAngle READ armAngle WRITE setArmAngle)

public:
    explicit AssistantCharacter(QWidget *parent = nullptr);

    void setState(AssistantState state);
    AssistantState state() const { return m_state; }

    float bounce() const    { return m_bounce; }
    void setBounce(float v) { m_bounce = v; update(); }

    float armAngle() const    { return m_armAngle; }
    void setArmAngle(float v) { m_armAngle = v; update(); }

    void tick(); // appelé par timer animation

protected:
    void paintEvent(QPaintEvent *) override;

private:
    void drawBody(QPainter &p, float cx, float cy);
    void drawHead(QPainter &p, float cx, float cy);
    void drawEyes(QPainter &p, float cx, float cy);
    void drawMouth(QPainter &p, float cx, float cy);
    void drawArms(QPainter &p, float cx, float cy);
    void drawAccessory(QPainter &p, float cx, float cy); // livre, étoiles, ZZZ...
    void drawParticles(QPainter &p, float cx, float cy);

    AssistantState m_state     = AssistantState::Idle;
    float          m_bounce    = 0.0f;
    float          m_armAngle  = 0.0f;
    float          m_blinkT    = 0.0f;
    bool           m_blinking  = false;
    float          m_animT     = 0.0f;  // temps global animation
    int            m_blinkTimer = 0;

    // Particules flottantes (étoiles, Z, etc.)
    struct Particle {
        float x, y, alpha, speed, size;
    };
    QVector<Particle> m_particles;
};

// ─────────────────────────────────────────────────────────
//  Bulle de dialogue
// ─────────────────────────────────────────────────────────
class SpeechBubble : public QWidget
{
    Q_OBJECT
public:
    explicit SpeechBubble(QWidget *parent = nullptr);
    void showMessage(const QString &msg, int durationMs = 3000);
    void hideMessage();

protected:
    void paintEvent(QPaintEvent *) override;

private:
    QString m_text;
    QTimer  *m_hideTimer;
};

// ─────────────────────────────────────────────────────────
//  AssistantWidget — conteneur principal (flottant)
// ─────────────────────────────────────────────────────────
class AssistantWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AssistantWidget(QWidget *parent = nullptr);

    // ── Appelé depuis main.cpp selon les événements ──────
    void setIdle();
    void setStudying(const QString &taskName = "");
    void setSleeping();
    void setHappy(const QString &taskName = "");
    void setCheering();
    void setThinking();
    void setProud(const QString &username = "");
    void setTyping();

    void setParentSize(int w, int h); // reposition when parent resizes

protected:
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *)  override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void paintEvent(QPaintEvent *) override;

private slots:
    void onAnimTick();
    void onToggleVisibility();

private:
    void updatePosition();
    void updateMessage();

    AssistantCharacter *m_character;
    SpeechBubble       *m_bubble;
    QPushButton        *m_toggleBtn;
    QTimer             *m_animTimer;

    AssistantState      m_state = AssistantState::Idle;
    bool                m_visible = true;
    bool                m_dragging = false;
    QPoint              m_dragOffset;

    int m_parentW = 1100;
    int m_parentH = 700;

    float m_floatT = 0.0f; // flottement global
};

#endif // ASSISTANTWIDGET_H