
#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QMap>
#include <QObject>
#include <QString>

class MainWindow;
class StartScene;
class MapScene;

class DialogueManager;
class DialogueWidget;

class GameManager : public QObject
{
    Q_OBJECT

public:
    explicit GameManager(QObject *parent = nullptr);

    void show();

private slots:
    void startGame();

    void onPortalRequested(
        const QString &targetScene,
        QChar spawnChar
        );

    void onNpcInteract(const QString &npcId);

private:
    MainWindow *m_window;
    StartScene *m_startScene;

    QMap<QString, MapScene *> m_scenes;
    QMap<QString, int> m_pageIndex;

    // 对话系统
    DialogueManager *m_dialogueManager = nullptr;
    DialogueWidget *m_dialogueWidget = nullptr;

    MapScene *m_activeScene = nullptr;
};

#endif
