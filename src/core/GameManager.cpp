#include "core/GameManager.h"

#include "core/GameState.h"
#include "core/MapData.h"
#include "scenes/StartScene.h"
#include "scenes/MapScene.h"
#include "ui/MainWindow.h"

#include <QMessageBox>

GameManager::GameManager(QObject *parent)
    : QObject(parent),
      m_window(new MainWindow()),
      m_startScene(new StartScene())
{
    m_pageIndex[QStringLiteral("start")] = m_window->addPage(m_startScene);

    connect(m_startScene, &StartScene::startGame, this, &GameManager::startGame);

    const QList<MapConfig> maps = MapData::allMaps();
    for (const MapConfig &cfg : maps) {
        auto *scene = new MapScene(cfg);
        const int index = m_window->addPage(scene);
        m_scenes[cfg.id] = scene;
        m_pageIndex[cfg.id] = index;

        connect(scene, &MapScene::portalRequested,
                this, &GameManager::onPortalRequested);
        connect(scene, &MapScene::npcInteractRequested,
                this, &GameManager::onNpcInteract);
    }

    GameState::instance()->reset();
}

void GameManager::show()
{
    m_window->show();
}

void GameManager::startGame()
{
    GameState::instance()->reset();
    MapScene *first = m_scenes.value(SceneId::Classroom);
    if (first)
        first->placePlayer('P');
    m_window->switchToPage(m_pageIndex[SceneId::Classroom]);
}

void GameManager::onPortalRequested(const QString &targetScene, QChar spawnChar)
{
    MapScene *target = m_scenes.value(targetScene);
    if (!target)
        return;

    target->placePlayer(spawnChar);
    m_window->switchToPage(m_pageIndex[targetScene]);
}

void GameManager::onNpcInteract(const QString &npcId)
{
    // 成员 B 在此接入 Galgame 对话框与好感度逻辑；当前为框架占位反馈。
    const QString title = QStringLiteral("交互：%1").arg(npcId);
    QMessageBox::information(m_window, title,
        QStringLiteral("对话系统将由成员 B 接入（NPC：%1）。").arg(npcId));
}
