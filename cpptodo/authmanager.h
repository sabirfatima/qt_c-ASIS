#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

#include <QString>
#include <QSqlDatabase>

// ─────────────────────────────────────────────────────────
//  AuthManager  — singleton, handles SQLite user storage
// ─────────────────────────────────────────────────────────
class AuthManager
{
public:
    static AuthManager &instance();

    bool        initDatabase();

    // returns empty string on success, error message on failure
    QString     registerUser(const QString &username,
                         const QString &email,
                         const QString &password);

    // returns empty string on success, error message on failure
    QString     loginUser(const QString &emailOrUsername,
                      const QString &password);

    // returns empty string on success, error message on failure
    QString     resetPassword(const QString &email,
                          const QString &newPassword);

    bool        isEmailTaken(const QString &email) const;
    bool        isUsernameTaken(const QString &username) const;

    // Session
    bool        isLoggedIn() const  { return m_loggedIn; }
    QString     currentUsername() const { return m_currentUsername; }
    QString     currentEmail()    const { return m_currentEmail; }
    void        logout();

private:
    AuthManager() = default;
    AuthManager(const AuthManager &) = delete;
    AuthManager &operator=(const AuthManager &) = delete;

    QString     hashPassword(const QString &password) const;

    QSqlDatabase m_db;
    bool         m_loggedIn        = false;
    QString      m_currentUsername;
    QString      m_currentEmail;
};

#endif // AUTHMANAGER_H
