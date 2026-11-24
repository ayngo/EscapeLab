#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QMap>
#include <QObject>
#include <QString>

class MainWindow;
class StartScene;
class MapScene;

/**
 * @brief 游戏总控（状态机）与整合入口。
 *
 * 负责创建并持有主窗口、开始界面与全部地图场景，统一处理场景切换
 * （门传送）与 NPC 交互请求。成员 B 可在 onNpcInteract() 中接入
 * 对话系统；成员 C 可在出口场景接入结局判定。
 */
class GameManager : public QObject
{
    Q_OBJECT

public:
    explicit GameManager(QObject *parent = nullptr);

    void show();

private slots:
    void startGame();
    void onPortalRequested(const QString &targetScene, QChar spawnChar);
    void onNpcInteract(const QString &npcId);

private:
    MainWindow *m_window;
    StartScene *m_startScene;
    QMap<QString, MapScene *> m_scenes;
    QMap<QString, int> m_pageIndex;
};

#endif // GAMEMANAGER_H
