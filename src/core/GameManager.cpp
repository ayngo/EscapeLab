
#include "core/GameManager.h"

#include "core/GameState.h"
#include "core/MapData.h"

#include "scenes/StartScene.h"
#include "scenes/MapScene.h"

#include "ui/MainWindow.h"

#include "dialogue/DialogueManager.h"
#include "dialogue/DialogueWidget.h"

GameManager::GameManager(QObject *parent)
    : QObject(parent),
    m_window(new MainWindow()),
    m_startScene(new StartScene()),
    m_dialogueManager(new DialogueManager(this)),
    m_dialogueWidget(new DialogueWidget())
{
    m_pageIndex["start"] =
        m_window->addPage(m_startScene);

    connect(
        m_startScene,
        &StartScene::startGame,
        this,
        &GameManager::startGame
        );

    // 创建所有地图
    const QList<MapConfig> maps = MapData::allMaps();

    for (const MapConfig &cfg : maps) {

        auto *scene = new MapScene(cfg);

        const int index = m_window->addPage(scene);

        m_scenes[cfg.id] = scene;
        m_pageIndex[cfg.id] = index;

        connect(
            scene,
            &MapScene::portalRequested,
            this,
            &GameManager::onPortalRequested
            );

        connect(
            scene,
            &MapScene::npcInteractRequested,
            this,
            &GameManager::onNpcInteract
            );
    }

    // 对话文本更新
    connect(
        m_dialogueManager,
        &DialogueManager::dialogueUpdated,
        this,
        [this]() {

            if (!m_activeScene)
                return;

            const DialogueNode *node =
                m_dialogueManager->currentNode();

            if (!node)
                return;

            const int affinity =
                GameState::instance()->affinity(
                    m_activeScene == nullptr
                        ? QString()
                        : node->id.startsWith("frog")
                              ? GameState::NAIWA
                              : node->speaker == "豆包"
                                    ? GameState::DOUBAO
                                    : node->speaker == "猪"
                                          ? GameState::PIG
                                          : GameState::NAIWA
                    );

            m_dialogueWidget->setNode(
                *node,
                affinity
                );
        }
        );

    // 点击对话框推进
    connect(
        m_dialogueWidget,
        &DialogueWidget::advanceRequested,
        m_dialogueManager,
        &DialogueManager::advance
        );

    // 点击选项
    connect(
        m_dialogueWidget,
        &DialogueWidget::choiceSelected,
        m_dialogueManager,
        &DialogueManager::choose
        );

    // 对话结束
    connect(
        m_dialogueManager,
        &DialogueManager::dialogueFinished,
        this,
        [this]() {

            if (m_activeScene) {
                m_activeScene->setDialogueActive(false);
            }

            m_activeScene = nullptr;
        }
        );

    GameState::instance()->reset();
}

void GameManager::show()
{
    m_window->show();
}

void GameManager::startGame()
{
    GameState::instance()->reset();

    // 总共设置 6 条好感度隐藏线索
    GameState::instance()->setTotalClues(6);

    MapScene *first =
        m_scenes.value(SceneId::Classroom);

    if (first) {
        first->placePlayer('P');
        m_window->switchToPage(
            m_pageIndex[SceneId::Classroom]
            );
    }
}

void GameManager::onPortalRequested(
    const QString &targetScene,
    QChar spawnChar)
{
    MapScene *target = m_scenes.value(targetScene);

    if (!target)
        return;

    target->placePlayer(spawnChar);

    m_window->switchToPage(
        m_pageIndex[targetScene]
        );
}

void GameManager::onNpcInteract(const QString &npcId)
{
    // 确定当前发出交互请求的地图
    MapScene *scene =
        qobject_cast<MapScene *>(sender());

    if (!scene)
        return;

    // 不允许同时开启多个对话
    if (m_dialogueManager->isActive())
        return;

    m_activeScene = scene;

    // 将对话框挂载到当前地图
    scene->attachDialogueWidget(
        m_dialogueWidget
        );

    scene->setDialogueActive(true);

    // 开始对话
    if (!m_dialogueManager->start(npcId)) {
        scene->setDialogueActive(false);
        m_activeScene = nullptr;
    }
}
