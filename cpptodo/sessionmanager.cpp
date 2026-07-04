

#include "sessionmanager.h"
#include "authmanager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include <QtMath>
#include <QMap>

// ════════════════════════════════════════════════════════
//  Singleton
// ════════════════════════════════════════════════════════
SessionManager &SessionManager::instance()
{
    static SessionManager inst;
    return inst;
}

// ════════════════════════════════════════════════════════
//  initDatabase
// ════════════════════════════════════════════════════════
bool SessionManager::initDatabase()
{
    // Reuse same DB file as AuthManager
    QString dataPath = QStandardPaths::writableLocation(
        QStandardPaths::AppDataLocation);
    QDir().mkpath(dataPath);
    QString dbPath = dataPath + "/flowtask_users.db";

    // Use a different connection name to avoid conflict
    if (QSqlDatabase::contains("session_conn")) {
        m_db = QSqlDatabase::database("session_conn");
    } else {
        m_db = QSqlDatabase::addDatabase("QSQLITE", "session_conn");
        m_db.setDatabaseName(dbPath);
    }

    if (!m_db.isOpen() && !m_db.open()) {
        qDebug() << "SessionManager: DB open error:" << m_db.lastError().text();
        return false;
    }

    QSqlQuery q(m_db);

    // Sessions table
    q.exec(
        "CREATE TABLE IF NOT EXISTS sessions ("
        "  id             INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  username       TEXT    NOT NULL,"
        "  start_time     TEXT    NOT NULL,"
        "  duration_sec   INTEGER NOT NULL DEFAULT 0,"
        "  timer_mode     TEXT    NOT NULL DEFAULT 'Croissant',"
        "  tasks_done     INTEGER NOT NULL DEFAULT 0,"
        "  prod_score     REAL    NOT NULL DEFAULT 0.0"
        ")"
        );

    if (q.lastError().isValid())
        qDebug() << "SessionManager table error:" << q.lastError().text();

    return true;
}

// ════════════════════════════════════════════════════════
//  currentUsername helper
// ════════════════════════════════════════════════════════
QString SessionManager::currentUsername() const
{
    return AuthManager::instance().currentUsername();
}

// ════════════════════════════════════════════════════════
//  startSession / endSession
// ════════════════════════════════════════════════════════
void SessionManager::startSession(const QString &timerMode)
{
    if (!AuthManager::instance().isLoggedIn()) return;
    m_sessionActive = true;
    m_sessionStart  = QDateTime::currentDateTime();
    m_sessionMode   = timerMode;
}

void SessionManager::endSession(int tasksCompleted)
{
    if (!m_sessionActive || !AuthManager::instance().isLoggedIn()) return;
    m_sessionActive = false;

    int durationSec = static_cast<int>(m_sessionStart.secsTo(QDateTime::currentDateTime()));
    if (durationSec < 30) return; // ignore sessions under 30 seconds

    double score = computeProductivityScore(durationSec, tasksCompleted);

    QSqlQuery q(m_db);
    q.prepare(
        "INSERT INTO sessions (username, start_time, duration_sec, timer_mode, tasks_done, prod_score)"
        " VALUES (:u, :s, :d, :m, :t, :p)"
        );
    q.bindValue(":u", currentUsername());
    q.bindValue(":s", m_sessionStart.toString(Qt::ISODate));
    q.bindValue(":d", durationSec);
    q.bindValue(":m", m_sessionMode);
    q.bindValue(":t", tasksCompleted);
    q.bindValue(":p", score);

    if (!q.exec())
        qDebug() << "Session insert error:" << q.lastError().text();
}

// ════════════════════════════════════════════════════════
//  computeProductivityScore
//  Formula: weighted average of duration + tasks completed
//  Max score = 100
// ════════════════════════════════════════════════════════
double SessionManager::computeProductivityScore(int durationSec, int tasksCompleted) const
{
    // Duration score: 25 min (1500s) = 50pts max
    double durScore = qMin(50.0, (durationSec / 1500.0) * 50.0);

    // Task score: each task = 10pts, max 50pts
    double taskScore = qMin(50.0, tasksCompleted * 10.0);

    return durScore + taskScore;
}

// ════════════════════════════════════════════════════════
//  getDailyStats
// ════════════════════════════════════════════════════════
QVector<DailyStats> SessionManager::getDailyStats(int lastNDays) const
{
    QVector<DailyStats> result;
    if (!AuthManager::instance().isLoggedIn()) return result;

    QSqlQuery q(m_db);
    q.prepare(
        "SELECT DATE(start_time) as day,"
        "       SUM(duration_sec),"
        "       SUM(tasks_done),"
        "       AVG(prod_score)"
        " FROM sessions"
        " WHERE username = :u"
        "   AND DATE(start_time) >= DATE('now', :offset)"
        " GROUP BY day"
        " ORDER BY day ASC"
        );
    q.bindValue(":u", currentUsername());
    q.bindValue(":offset", QString("-%1 days").arg(lastNDays - 1));
    q.exec();

    while (q.next()) {
        DailyStats s;
        s.date               = QDate::fromString(q.value(0).toString(), "yyyy-MM-dd");
        s.totalSec           = q.value(1).toInt();
        s.tasksCompleted     = q.value(2).toInt();
        s.productivityScore  = q.value(3).toDouble();
        result.append(s);
    }
    return result;
}

// ════════════════════════════════════════════════════════
//  getHourlyProductivity
// ════════════════════════════════════════════════════════
QVector<HourlyProductivity> SessionManager::getHourlyProductivity() const
{
    QVector<HourlyProductivity> result;
    if (!AuthManager::instance().isLoggedIn()) return result;

    QSqlQuery q(m_db);
    q.prepare(
        "SELECT CAST(strftime('%H', start_time) AS INTEGER) as hour,"
        "       AVG(prod_score),"
        "       COUNT(*)"
        " FROM sessions"
        " WHERE username = :u"
        " GROUP BY hour"
        " ORDER BY hour ASC"
        );
    q.bindValue(":u", currentUsername());
    q.exec();

    while (q.next()) {
        HourlyProductivity h;
        h.hour     = q.value(0).toInt();
        h.avgScore = q.value(1).toDouble();
        h.sessions = q.value(2).toInt();
        result.append(h);
    }
    return result;
}

// ════════════════════════════════════════════════════════
//  getTodayProductivityScore
// ════════════════════════════════════════════════════════
double SessionManager::getTodayProductivityScore() const
{
    if (!AuthManager::instance().isLoggedIn()) return 0.0;

    QSqlQuery q(m_db);
    q.prepare(
        "SELECT AVG(prod_score) FROM sessions"
        " WHERE username = :u AND DATE(start_time) = DATE('now')"
        );
    q.bindValue(":u", currentUsername());
    q.exec();
    return q.next() ? q.value(0).toDouble() : 0.0;
}

// ════════════════════════════════════════════════════════
//  getTotalSessionsThisWeek
// ════════════════════════════════════════════════════════
int SessionManager::getTotalSessionsThisWeek() const
{
    if (!AuthManager::instance().isLoggedIn()) return 0;

    QSqlQuery q(m_db);
    q.prepare(
        "SELECT COUNT(*) FROM sessions"
        " WHERE username = :u AND DATE(start_time) >= DATE('now', '-6 days')"
        );
    q.bindValue(":u", currentUsername());
    q.exec();
    return q.next() ? q.value(0).toInt() : 0;
}

// ════════════════════════════════════════════════════════
//  getTotalTimeThisWeek  (seconds)
// ════════════════════════════════════════════════════════
int SessionManager::getTotalTimeThisWeek() const
{
    if (!AuthManager::instance().isLoggedIn()) return 0;

    QSqlQuery q(m_db);
    q.prepare(
        "SELECT SUM(duration_sec) FROM sessions"
        " WHERE username = :u AND DATE(start_time) >= DATE('now', '-6 days')"
        );
    q.bindValue(":u", currentUsername());
    q.exec();
    return q.next() ? q.value(0).toInt() : 0;
}

// ════════════════════════════════════════════════════════
//  getMostProductiveHour
// ════════════════════════════════════════════════════════
QString SessionManager::getMostProductiveHour() const
{
    auto hourly = getHourlyProductivity();
    if (hourly.isEmpty()) return "—";

    auto best = std::max_element(hourly.begin(), hourly.end(),
                                 [](const HourlyProductivity &a, const HourlyProductivity &b) {
                                     return a.avgScore < b.avgScore;
                                 });

    int h = best->hour;
    return QString("%1h00 – %2h00").arg(h, 2, 10, QChar('0'))
        .arg(h + 1, 2, 10, QChar('0'));
}

// ════════════════════════════════════════════════════════
//  getBestHoursForHardTasks
//  Returns top 3 hours sorted by productivity score
// ════════════════════════════════════════════════════════
QVector<int> SessionManager::getBestHoursForHardTasks() const
{
    auto hourly = getHourlyProductivity();

    // Sort descending by score
    std::sort(hourly.begin(), hourly.end(),
              [](const HourlyProductivity &a, const HourlyProductivity &b) {
                  return a.avgScore > b.avgScore;
              });

    QVector<int> result;
    for (int i = 0; i < qMin(3, hourly.size()); ++i)
        result.append(hourly[i].hour);

    return result;
}
