#include "authmanager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>
#include <QDir>
#include <QCryptographicHash>
#include <QVariant>

// ════════════════════════════════════════════════════════
//  Singleton
// ════════════════════════════════════════════════════════
AuthManager &AuthManager::instance()
{
    static AuthManager inst;
    return inst;
}

// ════════════════════════════════════════════════════════
//  initDatabase
// ════════════════════════════════════════════════════════
bool AuthManager::initDatabase()
{
    QString dataPath = QStandardPaths::writableLocation(
        QStandardPaths::AppDataLocation);
    QDir().mkpath(dataPath);
    QString dbPath = dataPath + "/flowtask_users.db";

    m_db = QSqlDatabase::addDatabase("QSQLITE", "flowtask_conn");
    m_db.setDatabaseName(dbPath);

    if (!m_db.open()) return false;

    QSqlQuery q(m_db);
    q.exec(
        "CREATE TABLE IF NOT EXISTS users ("
        "  id       INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  username TEXT    NOT NULL UNIQUE,"
        "  email    TEXT    NOT NULL UNIQUE,"
        "  password TEXT    NOT NULL"
        ")"
        );

    return true;
}

// ════════════════════════════════════════════════════════
//  hashPassword  — SHA-256
// ════════════════════════════════════════════════════════
QString AuthManager::hashPassword(const QString &password) const
{
    QByteArray hashed = QCryptographicHash::hash(
        password.toUtf8(), QCryptographicHash::Sha256);
    return QString(hashed.toHex());
}

// ════════════════════════════════════════════════════════
//  isEmailTaken / isUsernameTaken
// ════════════════════════════════════════════════════════
bool AuthManager::isEmailTaken(const QString &email) const
{
    QSqlQuery q(m_db);
    q.prepare("SELECT COUNT(*) FROM users WHERE email = :e");
    q.bindValue(":e", email.toLower().trimmed());
    q.exec();
    return q.next() && q.value(0).toInt() > 0;
}

bool AuthManager::isUsernameTaken(const QString &username) const
{
    QSqlQuery q(m_db);
    q.prepare("SELECT COUNT(*) FROM users WHERE username = :u");
    q.bindValue(":u", username.toLower().trimmed());
    q.exec();
    return q.next() && q.value(0).toInt() > 0;
}

// ════════════════════════════════════════════════════════
//  registerUser
// ════════════════════════════════════════════════════════
QString AuthManager::registerUser(const QString &username,
                                  const QString &email,
                                  const QString &password)
{
    QString u = username.trimmed();
    QString e = email.toLower().trimmed();

    if (u.isEmpty())  return "Le nom d'utilisateur est requis.";
    if (u.length() < 3) return "Le nom d'utilisateur doit avoir au moins 3 caractères.";
    if (e.isEmpty())  return "L'email est requis.";
    if (!e.contains('@') || !e.contains('.'))
        return "Format d'email invalide.";
    if (password.length() < 6)
        return "Le mot de passe doit avoir au moins 6 caractères.";

    if (isUsernameTaken(u)) return "Ce nom d'utilisateur est déjà utilisé.";
    if (isEmailTaken(e))    return "Cet email est déjà utilisé.";

    QSqlQuery q(m_db);
    q.prepare("INSERT INTO users (username, email, password) "
              "VALUES (:u, :e, :p)");
    q.bindValue(":u", u.toLower());
    q.bindValue(":e", e);
    q.bindValue(":p", hashPassword(password));

    if (!q.exec()) return "Erreur base de données: " + q.lastError().text();

    // Auto-login after register
    m_loggedIn        = true;
    m_currentUsername = u;
    m_currentEmail    = e;

    return "";
}

// ════════════════════════════════════════════════════════
//  loginUser
// ════════════════════════════════════════════════════════
QString AuthManager::loginUser(const QString &emailOrUsername,
                               const QString &password)
{
    qDebug() << "DB ouverte ?" << m_db.isOpen();
    qDebug() << "Login tentative:" << emailOrUsername;

    QString input = emailOrUsername.trimmed().toLower();
    if (input.isEmpty())  return "Entrez votre email ou nom d'utilisateur.";
    if (password.isEmpty()) return "Entrez votre mot de passe.";

    QSqlQuery q(m_db);
    q.prepare("SELECT username, email FROM users "
              "WHERE (email = :i OR username = :i) AND password = :p");
    q.bindValue(":i", input);
    q.bindValue(":p", hashPassword(password));
    q.exec();

    qDebug() << "Query error:" << q.lastError().text();

    if (!q.next()) {
        qDebug() << "Aucun utilisateur trouvé !";
        return "Email/identifiant ou mot de passe incorrect.";
    }

    qDebug() << "Utilisateur trouvé !" << q.value(0).toString();

    m_loggedIn        = true;
    m_currentUsername = q.value(0).toString();
    m_currentEmail    = q.value(1).toString();

    return "";  // ← succès
}
// ════════════════════════════════════════════════════════
//  resetPassword
// ════════════════════════════════════════════════════════
QString AuthManager::resetPassword(const QString &email,
                                   const QString &newPassword)
{
    QString e = email.toLower().trimmed();
    if (e.isEmpty()) return "Entrez votre email.";
    if (!isEmailTaken(e)) return "Aucun compte associé à cet email.";
    if (newPassword.length() < 6)
        return "Le nouveau mot de passe doit avoir au moins 6 caractères.";

    QSqlQuery q(m_db);
    q.prepare("UPDATE users SET password = :p WHERE email = :e");
    q.bindValue(":p", hashPassword(newPassword));
    q.bindValue(":e", e);

    if (!q.exec()) return "Erreur base de données: " + q.lastError().text();
    return "";
}

// ════════════════════════════════════════════════════════
//  logout
// ════════════════════════════════════════════════════════
void AuthManager::logout()
{
    m_loggedIn        = false;
    m_currentUsername = "";
    m_currentEmail    = "";
}
