#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QString>
#include <QVector>
#include <QDateTime>
#include <QSqlDatabase>

// ─────────────────────────────────────────────────────────
//  Data structures
// ─────────────────────────────────────────────────────────
struct SessionRecord {
    int       id;
    QString   username;
    QDateTime startTime;
    int       durationSec;   // total seconds worked
    QString   timerMode;     // "Croissant" | "Decroissant" | "Pomodoro"
    int       tasksCompleted;
};

struct HourlyProductivity {
    int hour;          // 0-23
    double avgScore;   // average productivity score for this hour
    int    sessions;   // number of sessions recorded at this hour
};

struct DailyStats {
    QDate  date;
    int    totalSec;
    int    tasksCompleted;
    double productivityScore; // 0.0 - 100.0
};

// ─────────────────────────────────────────────────────────
//  SessionManager — singleton
// ─────────────────────────────────────────────────────────
class SessionManager
{
public:
    static SessionManager &instance();

    bool initDatabase();  // call once at startup (after AuthManager)

    // Call when user starts/stops a timer session
    void startSession(const QString &timerMode);
    void endSession(int tasksCompleted);

    // Statistics queries (for logged-in user)
    QVector<DailyStats>        getDailyStats(int lastNDays = 7) const;
    QVector<HourlyProductivity> getHourlyProductivity() const;
    double                     getTodayProductivityScore() const;
    int                        getTotalSessionsThisWeek() const;
    int                        getTotalTimeThisWeek() const;      // seconds
    QString                    getMostProductiveHour() const;

    // Best hours for hard tasks (returns sorted list of hours)
    QVector<int>               getBestHoursForHardTasks() const;

private:
    SessionManager() = default;
    SessionManager(const SessionManager &) = delete;
    SessionManager &operator=(const SessionManager &) = delete;

    double computeProductivityScore(int durationSec, int tasksCompleted) const;
    QString currentUsername() const;

    QSqlDatabase m_db;
    bool         m_sessionActive = false;
    QDateTime    m_sessionStart;
    QString      m_sessionMode;
};

#endif // SESSIONMANAGER_H
