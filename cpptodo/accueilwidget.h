#ifndef ACCUEILWIDGET_H
#define ACCUEILWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QPropertyAnimation>
#include <QPainter>
#include <QVector>
#include <QPointF>
#include <QColor>
#include <QFont>
#include <QLinearGradient>
#include <QRadialGradient>
#include <cmath>

// Forward declaration
class AccueilWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int loadProgress READ loadProgress WRITE setLoadProgress)

public:
    explicit AccueilWidget(QWidget *parent = nullptr);
    ~AccueilWidget();

    int loadProgress() const { return m_loadProgress; }
    void setLoadProgress(int value);

signals:
    void loadingComplete();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateAnimation();
    void onLoadingFinished();

private:
    // Structs
    struct Particle {
        QPointF pos;
        QPointF vel;
        float size;
        float opacity;
        float hue;
        float life;
        float maxLife;
    };

    struct Ring {
        QPointF center;
        float radius;
        float targetRadius;
        float angle;
        float speed;
        QColor color;
    };

    struct Star {
        QPointF pos;
        float brightness;
        float twinkleSpeed;
        float twinklePhase;
        float size;
    };

    // Methods
    void initParticles();
    void initRings();
    void initStars();
    void updateParticles();
    void updateRings();
    void updateStars();

    void drawBackground(QPainter &p);
    void drawStars(QPainter &p);
    void drawNebulaEffect(QPainter &p);
    void drawRings(QPainter &p);
    void drawParticles(QPainter &p);
    void drawGrid(QPainter &p);
    void drawCentralGlow(QPainter &p);
    void drawLoadingBar(QPainter &p);
    void drawTitle(QPainter &p);
    void drawSubtitle(QPainter &p);
    void drawLoadingText(QPainter &p);
    void drawScanlines(QPainter &p);

    // Animation state
    QTimer *m_animTimer;
    QPropertyAnimation *m_loadAnim;
    float m_time;
    int m_loadProgress;
    float m_titleOpacity;
    float m_subtitleOpacity;
    float m_glowPulse;
    bool m_loadingDone;
    int m_frame;

    // Visual elements
    QVector<Particle> m_particles;
    QVector<Ring>     m_rings;
    QVector<Star>     m_stars;

    // Loading text cycling
    QStringList m_loadingMessages;
    int m_msgIndex;
    int m_msgTimer;

    // Cached geometry
    QRectF m_barRect;
    QPointF m_center;
};

#endif // ACCUEILWIDGET_H
