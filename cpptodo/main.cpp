#include "accueilwidget.h"
#include "timer.h"
#include "todowidget.h"
#include "loginwidget.h"
#include "authmanager.h"
#include "sessionmanager.h"
#include "statswidget.h"
#include "planningwidget.h"
#include "assistantwidget.h"
#include <QApplication>
#include <QMessageBox>
#include <QPushButton>
#include <QResizeEvent>

// ── Fenêtre principale qui héberge tous les widgets ───────
// Nécessaire pour que l'assistant soit superposé sur tout
class MainContainer : public QWidget
{
public:
    explicit MainContainer(QWidget *parent = nullptr) : QWidget(parent) {}
    AssistantWidget *assistant = nullptr;

protected:
    void resizeEvent(QResizeEvent *e) override {
        QWidget::resizeEvent(e);
        if (assistant)
            assistant->setParentSize(e->size().width(), e->size().height());
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("FlowTask Pro");
    app.setOrganizationName("FlowTask");

    if (!AuthManager::instance().initDatabase()) {
        QMessageBox::critical(nullptr, "Erreur",
                              "Impossible d'initialiser la base de données.");
        return 1;
    }
    if (!SessionManager::instance().initDatabase()) {
        QMessageBox::critical(nullptr, "Erreur",
                              "Impossible d'initialiser la base de sessions.");
        return 1;
    }

    AccueilWidget *accueil = new AccueilWidget();
    accueil->setWindowTitle("FlowTask Pro");
    accueil->resize(1100, 700);
    accueil->show();

    QObject::connect(accueil, &AccueilWidget::loadingComplete, [accueil]() {

        // ── Conteneur principal ───────────────────────────
        MainContainer *container = new MainContainer();
        container->setWindowTitle("FlowTask Pro");
        container->resize(1100, 700);
        container->setStyleSheet("background:rgb(3,7,22);");

        TimerWidget    *timer    = new TimerWidget(container);
        TodoWidget     *todo     = new TodoWidget(container);
        LoginWidget    *login    = new LoginWidget(container);
        StatsWidget    *stats    = new StatsWidget(container);
        PlanningWidget *planning = new PlanningWidget(container);

        for(QWidget *w : {(QWidget*)timer,(QWidget*)todo,(QWidget*)login,
                           (QWidget*)stats,(QWidget*)planning}){
            w->setWindowTitle("FlowTask Pro");
            w->setGeometry(0, 0, 1100, 700);
            w->hide();
        }

        // ── Assistant (superposé sur tout) ────────────────
        AssistantWidget *assistant = new AssistantWidget(container);
        container->assistant = assistant;
        assistant->setParentSize(1100, 700);
        assistant->raise(); // toujours au-dessus
        assistant->show();

        container->show();

        auto refreshHeaders = [timer, todo]() {
            timer->updateAuthState();
            todo->updateAuthState();
        };

        auto showWidget = [=](QWidget *w) {
            for(QWidget *x : {(QWidget*)timer,(QWidget*)todo,(QWidget*)login,
                               (QWidget*)stats,(QWidget*)planning})
                x->hide();
            w->setGeometry(0, 0, container->width(), container->height());
            w->show();
            assistant->raise(); // garder l'assistant au premier plan
        };

        // ── Afficher timer au départ ──────────────────────
        showWidget(timer);
        assistant->setIdle();

        // ── Timer ↔ Todo ──────────────────────────────────
        QObject::connect(timer, &TimerWidget::goToTodoList, [=]() {
            showWidget(todo);
            assistant->setThinking();
        });
        QObject::connect(todo, &TodoWidget::goToTimer, [=]() {
            showWidget(timer);
            assistant->setIdle();
        });

        // ── Timer START/PAUSE/RESET → assistant ───────────
        // On connecte via les signaux existants du timer
        // START → Studying
        QObject::connect(timer, &TimerWidget::taskStarted,
                         [=](const QString &name, int duration) {
                             todo->addTaskFromTimer(name, duration);
                             assistant->setStudying(name);
                         });

        // ── Timer → TodoList : tâche démarrée ─────────────
        // (déjà géré dans taskStarted ci-dessus)

        // ── TodoList → Timer : bouton ▶ START ─────────────
        QObject::connect(todo, &TodoWidget::startTaskInTimer,
                         [=](const QString &name, int duration) {
                             timer->loadTask(name, duration);
                             assistant->setStudying(name);
                         });

        // ── Tâche complétée → Happy ───────────────────────
        // On crée un signal dans todowidget ou on le capte via rebuildList
        // Pour l'instant on connecte via toggleCompleted indirectement
        // en écoutant les changements — on utilise un timer de détection
        QTimer *taskWatcher = new QTimer(container);
        taskWatcher->setInterval(500);
        int prevDone = 0;
        QObject::connect(taskWatcher, &QTimer::timeout, [=]() mutable {
            int done = 0;
            for(const Task &t : todo->getTasks())
                if(t.completed) done++;
            if(done > prevDone) {
                assistant->setHappy();
                prevDone = done;
            } else if (done < prevDone) {
                prevDone = done;
            }
        });
        taskWatcher->start();

        // ── Login depuis Timer ────────────────────────────
        QObject::connect(timer, &TimerWidget::goToLogin, [=]() {
            if (AuthManager::instance().isLoggedIn()) {
                AuthManager::instance().logout();
                todo->onAuthChanged(false);
                timer->updateAuthState();
                assistant->setIdle();
            } else {
                showWidget(login);
                assistant->setIdle();
            }
        });

        // ── Login depuis Todo ─────────────────────────────
        QObject::connect(todo, &TodoWidget::goToLogin, [=]() {
            if (AuthManager::instance().isLoggedIn()) {
                AuthManager::instance().logout();
                todo->onAuthChanged(false);
                timer->updateAuthState();
                todo->updateAuthState();
                assistant->setIdle();
            } else {
                showWidget(login);
                assistant->setIdle();
            }
        });

        // ── Login success ─────────────────────────────────
        QObject::connect(login, &LoginWidget::loginSuccess, [=]() mutable {
            refreshHeaders();
            showWidget(timer);

            // Message assistant — Proud
            assistant->setProud(AuthManager::instance().currentUsername());

            // Message tâches locales
            bool hasLocalTasks = !todo->getTasks().isEmpty();
            if (hasLocalTasks) {
                QMessageBox msgBox;
                msgBox.setWindowTitle("Tâches locales");
                msgBox.setText("Vous aviez des tâches créées avant la connexion.");
                msgBox.setInformativeText("Voulez-vous les ajouter à votre compte ?");
                msgBox.setIcon(QMessageBox::Question);
                msgBox.setStyleSheet(
                    "QMessageBox{background:rgb(8,18,52);}"
                    "QLabel{color:rgb(160,210,255);font-family:'Courier New';font-size:12px;}"
                    "QPushButton{background:rgba(4,14,40,0.9);color:rgb(0,200,255);"
                    "border:1px solid rgba(0,160,220,0.5);border-radius:8px;"
                    "padding:6px 16px;font-family:'Courier New';font-size:11px;font-weight:bold;}"
                    "QPushButton:hover{background:rgba(0,60,110,0.9);}");
                QPushButton *keepBtn    = msgBox.addButton("✓  Oui, les garder", QMessageBox::YesRole);
                QPushButton *discardBtn = msgBox.addButton("✕  Non, ignorer",    QMessageBox::NoRole);
                Q_UNUSED(discardBtn)
                msgBox.exec();
                todo->onAuthChanged(msgBox.clickedButton() == keepBtn);
            } else {
                todo->onAuthChanged(false);
            }
        });

        // ── Login back ────────────────────────────────────
        QObject::connect(login, &LoginWidget::goBack, [=]() {
            showWidget(timer);
            assistant->setIdle();
        });

        // ── Stats ─────────────────────────────────────────
        QObject::connect(timer, &TimerWidget::goToStats, [=]() {
            stats->refresh();
            showWidget(stats);
            assistant->setProud();
        });
        QObject::connect(stats, &StatsWidget::goBack, [=]() {
            showWidget(timer);
            assistant->setIdle();
        });

        // ── Planning ──────────────────────────────────────
        QObject::connect(todo, &TodoWidget::goToPlanning, [=]() {
            planning->setTasks(todo->getTasks());
            showWidget(planning);
            assistant->setThinking();
        });
        QObject::connect(planning, &PlanningWidget::goBack, [=]() {
            showWidget(todo);
            assistant->setThinking();
        });

        accueil->close();
        accueil->deleteLater();
    });

    return app.exec();
}
