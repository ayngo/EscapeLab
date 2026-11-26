
#include "Dialogue/DialogueWidget.h"
#include "ui/InventoryPanel.h"
#include "ui/ToastTip.h"
#include "ui/PasswordPanel.h"
#include "ui/EndingPanel.h"
#include "core/EndingJudge.h"
#include "entities/Terminal.h"
#include <QResizeEvent>

#include "scenes/MapScene.h"

#include "core/GameState.h"
#include "entities/Player.h"
#include "entities/Portal.h"
#include "entities/Npc.h"
#include "entities/GameObject.h"
#include "entities/PickupItem.h"

#include <QDebug>

#include <QElapsedTimer>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QLineF>
#include <QPainter>
#include <QTimer>
#include <QtMath>

namespace {
QElapsedTimer g_clock;
}

MapScene::MapScene(const MapConfig &config, QWidget *parent)
    : QGraphicsView(parent),
    m_config(config),
    m_scene(new QGraphicsScene(this)),
    m_player(nullptr),
    m_timer(new QTimer(this)),
    m_lastFrameMs(0.0)
{
    setScene(m_scene);
    setRenderHint(QPainter::Antialiasing);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFrameShape(QFrame::NoFrame);
    setFocusPolicy(Qt::StrongFocus);
    setOptimizationFlag(QGraphicsView::DontAdjustForAntialiasing, true);

    buildWorld();

    const QRectF sr = m_scene->sceneRect();
    setMinimumSize(static_cast<int>(sr.width()), static_cast<int>(sr.height()));

    if (!g_clock.isValid())
        g_clock.start();

    connect(m_timer, &QTimer::timeout, this, &MapScene::loop);
    m_timer->start(16);

    // 创建背包：左上角一个小图标，按 Tab 弹出列表
    m_inventoryPanel = new InventoryPanel(viewport());
    m_inventoryPanel->setParent(viewport());
    m_inventoryPanel->show();
    m_inventoryPanel->raise();
    m_inventoryPanel->setGeometry(12, 12, 44, 44);

    // 屏幕中央临时提示条（开门失败等反馈）
    m_toast = new ToastTip(viewport());
    m_toast->setParent(viewport());

    // 系统修复密码面板（成员 C）
    m_passwordPanel = new PasswordPanel(viewport());
    m_passwordPanel->setParent(viewport());
    connect(m_passwordPanel, &PasswordPanel::repaired,
            this, &MapScene::onSystemRepaired);
    connect(m_passwordPanel, &PasswordPanel::closed,
            this, [this]() {
                m_passwordOpen = false;
                m_passwordPanel->hide();
            });

    // 结局面板（成员 C，第四阶段）：全屏遮罩，先隐藏
    m_endingPanel = new EndingPanel(viewport());
    m_endingPanel->setParent(viewport());
    m_endingPanel->hide();
}

QString MapScene::sceneId() const
{
    return m_config.id;
}

QString MapScene::displayName() const
{
    return m_config.displayName;
}

QString MapScene::npcIdFromChar(QChar ch)
{
    if (ch == 'n')
        return GameState::NAIWA;
    if (ch == 'u')
        return GameState::DOUBAO;
    if (ch == 'p')
        return GameState::PIG;

    return QString();
}

void MapScene::buildWorld()
{
    const int ts = MapData::tileSize();
    const int rows = m_config.tiles.size();
    const int cols = m_config.tiles.first().size();

    m_scene->setSceneRect(0, 0, cols * ts, rows * ts);

    for (int row = 0; row < rows; ++row) {
        const QString line = m_config.tiles[row];

        for (int col = 0; col < cols; ++col) {
            const QChar ch = line[col];

            if (ch == '#' || ch == 'T' || ch == 'C') {
                auto *obj = new GameObject(
                    col, row, ts, GameObject::typeFromChar(ch)
                    );
                m_scene->addItem(obj);
            }
            else if (m_config.portalTargets.contains(ch)) {
                auto *portal = new Portal(
                    col, row, ts, ch, m_config.portalTargets[ch]
                    );

                m_portals.append(portal);
                m_scene->addItem(portal);
                m_spawns[ch] = QPoint(col, row);
            }
            else if (ch == 'P') {
                m_spawns['P'] = QPoint(col, row);
            }
            else if (ch == 'k') {
                auto *item = new PickupItem(
                    QStringLiteral("key_classroom"), col, row, ts
                    );
                m_items.append(item);
                m_scene->addItem(item);
            }
            else if (ch == 'o') {
                // 密码纸条（成员 C，第二阶段）
                auto *item = new PickupItem(
                    QStringLiteral("lab_password_note"), col, row, ts
                    );
                m_items.append(item);
                m_scene->addItem(item);
            }
            else if (ch == 'c') {
                // 饼干（成员 C，第二阶段）：喂给猪换线索
                auto *item = new PickupItem(
                    QStringLiteral("cookie"), col, row, ts
                    );
                m_items.append(item);
                m_scene->addItem(item);
            }
            else if (ch == 'm') {
                // 系统修复终端（成员 C）：走近按 E 弹出密码框
                m_terminal = new Terminal(col, row, ts);
                m_scene->addItem(m_terminal);
            }
            else if (ch == 'S') {
                // 出口场景终点空地标记（成员 C，第四阶段，方案 A）
                m_spawns['S'] = QPoint(col, row);
            }
            else if (ch.isLower()) {
                const QString npc = npcIdFromChar(ch);

                if (!npc.isEmpty()) {
                    QString sprite;

                    for (const NpcSpawn &sp : m_config.npcSpawns) {
                        if (sp.npcId == npc)
                            sprite = sp.spritePath;
                    }

                    auto *npcItem = new Npc(npc, col, row, ts, sprite);

                    connect(
                        npcItem,
                        &Npc::interactRequested,
                        this,
                        &MapScene::npcInteractRequested
                        );

                    m_npcs.append(npcItem);
                    m_scene->addItem(npcItem);
                }
                else {
                    m_spawns[ch] = QPoint(col, row);
                }
            }
        }
    }

    m_player = new Player(ts);
    m_scene->addItem(m_player);

    placePlayer('P');
}

QList<QRectF> MapScene::solidRects() const
{
    QList<QRectF> rects;
    const auto items = m_scene->items();

    for (QGraphicsItem *it : items) {
        GameObject *obj = dynamic_cast<GameObject *>(it);

        if (obj)
            rects.append(obj->logicalRect());
    }

    return rects;
}

void MapScene::placePlayer(QChar spawnChar)
{
    const int ts = MapData::tileSize();

    QPoint cell = m_spawns.value(spawnChar, m_spawns.value('P'));

    const qreal x = cell.x() * ts + (ts - 30) / 2.0;
    const qreal y = cell.y() * ts + (ts - 34);

    m_player->setPos(x, y);
    m_player->setMovement(0, 0);

    m_portalCooldown = 20;

    setFocus();
}

// 绑定对话框
void MapScene::attachDialogueWidget(DialogueWidget *widget)
{
    if (!widget)
        return;

    m_dialogueWidget = widget;

    // 将对话框放到地图视口上
    widget->setParent(viewport());

    const int margin = 12;
    const int height = 300;

    widget->setGeometry(
        margin,
        viewport()->height() - height - margin,
        viewport()->width() - margin * 2,
        height
        );

    widget->hide();
}

// 设置对话状态
void MapScene::setDialogueActive(bool active)
{
    m_dialogueActive = active;

    if (active) {
        m_pressedKeys.clear();

        if (m_player)
            m_player->setMovement(0, 0);

        if (m_dialogueWidget) {
            m_dialogueWidget->show();
            m_dialogueWidget->raise();
        }
    }
    else {
        if (m_dialogueWidget)
            m_dialogueWidget->hide();

        m_pressedKeys.clear();

        setFocus();
    }
}

void MapScene::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) {
        QGraphicsView::keyPressEvent(event);
        return;
    }

    // 密码面板打开时，键盘交给输入框，不处理移动/快捷键
    if (m_passwordOpen) {
        QGraphicsView::keyPressEvent(event);
        return;
    }

    if (event->key() == Qt::Key_E) {
        tryInteract();
        return;
    }

    // F：投喂饼干给猪（成员 C，独立按键，不占用对话 E）
    if (event->key() == Qt::Key_F) {
        tryFeedPig();
        return;
    }

    // R：查看隐藏实验记录（成员 C，第三阶段）
    if (event->key() == Qt::Key_R) {
        tryReadRecord();
        return;
    }

    // Tab：弹出/收起背包列表（不和对话键 E 冲突）
    if (event->key() == Qt::Key_Tab) {
        if (m_inventoryPanel)
            m_inventoryPanel->toggleList();
        return;
    }

    m_pressedKeys.insert(event->key());

    QGraphicsView::keyPressEvent(event);
}

void MapScene::keyReleaseEvent(QKeyEvent *event)
{
    if (!event->isAutoRepeat())
        m_pressedKeys.remove(event->key());

    QGraphicsView::keyReleaseEvent(event);
}

void MapScene::loop()
{
    if (m_dialogueActive || m_passwordOpen) {
        m_pressedKeys.clear();

        if (m_player)
            m_player->setMovement(0, 0);

        return;
    }

    const qreal now = g_clock.elapsed() / 1000.0;

    qreal dt = now - m_lastFrameMs;
    m_lastFrameMs = now;

    if (dt <= 0 || dt > 0.1)
        dt = 1.0 / 60.0;

    int hx = 0;
    int hy = 0;

    if (m_pressedKeys.contains(Qt::Key_Left)
        || m_pressedKeys.contains(Qt::Key_A))
        hx -= 1;

    if (m_pressedKeys.contains(Qt::Key_Right)
        || m_pressedKeys.contains(Qt::Key_D))
        hx += 1;

    if (m_pressedKeys.contains(Qt::Key_Up)
        || m_pressedKeys.contains(Qt::Key_W))
        hy -= 1;

    if (m_pressedKeys.contains(Qt::Key_Down)
        || m_pressedKeys.contains(Qt::Key_S))
        hy += 1;

    qreal vx = static_cast<qreal>(hx);
    qreal vy = static_cast<qreal>(hy);

    if (hx != 0 && hy != 0) {
        const qreal inv = 1.0 / qSqrt(2.0);
        vx *= inv;
        vy *= inv;
    }

    m_player->setMovement(vx, vy);
    m_player->tick(dt, solidRects());

    if (m_portalCooldown > 0)
        --m_portalCooldown;

    if (m_doorHintCooldown > 0)
        --m_doorHintCooldown;

    if (m_feedCooldown > 0)
        --m_feedCooldown;

    if (m_portalCooldown == 0) {
        const QRectF pr = m_player->collisionRect();

        for (Portal *portal : m_portals) {
            if (pr.intersects(portal->sceneBoundingRect())) {
                // 成员 C：门禁判定，不放行就不传送
                if (!tryDoor(portal))
                    break;

                emit portalRequested(
                    portal->targetScene(),
                    portal->spawnChar()
                    );

                m_portalCooldown = 30;
                break;
            }
        }
    }

    autoPickup();
    updateNpcProximity();

    // 第四阶段：出口场景踩到终点 S 才判定结局（方案 A）
    tryExitEnding();
}

bool MapScene::tryDoor(Portal *portal)
{
    // 成员 C 门禁：只有「教室 -> 走廊」的 A 门需要门禁卡，其余门直接放行
    const bool lockedDoor =
        m_config.id == SceneId::Classroom &&
        portal->targetScene() == SceneId::Hallway;

    if (!lockedDoor)
        return true;

    if (GameState::instance()->hasItem(QStringLiteral("key_classroom")))
        return true;

    // 没卡：拦住，弹一次提示，短冷却避免每帧重复弹
    if (m_doorHintCooldown == 0) {
        m_toast->showMessage(QStringLiteral("门锁着，需要门禁卡"));
        m_doorHintCooldown = 90; // 约 1.5 秒
    }

    return false;
}

void MapScene::updateNpcProximity()
{
    const QRectF pr = m_player->collisionRect();

    for (Npc *npc : m_npcs) {
        const qreal distance = QLineF(
                                   pr.center(),
                                   npc->sceneBoundingRect().center()
                                   ).length();

        npc->setHintVisible(
            distance < MapData::tileSize() * 1.6
            );
    }
}

void MapScene::tryInteract()
{
    if (m_dialogueActive || m_passwordOpen)
        return;

    const QRectF pr = m_player->collisionRect();

    // 先看是否走近系统修复终端（成员 C）
    if (m_terminal) {
        const qreal td = QLineF(
                             pr.center(),
                             m_terminal->logicalRect().center()
                             ).length();

        if (td < MapData::tileSize() * 1.6) {
            tryTerminal();
            return;
        }
    }

    // —— E 只负责和 NPC 对话（道具改为走近自动拾取）——
    Npc *best = nullptr;
    qreal bestDist = 1e9;

    for (Npc *npc : m_npcs) {
        const qreal d = QLineF(
                            pr.center(),
                            npc->sceneBoundingRect().center()
                            ).length();

        if (d < MapData::tileSize() * 1.6 && d < bestDist) {
            best = npc;
            bestDist = d;
        }
    }

    if (best)
        best->requestInteract();
}

void MapScene::tryTerminal()
{
    // 已修复过就不必再输
    if (GameState::instance()->systemRepaired()) {
        m_toast->showMessage(QStringLiteral("系统已经修复好了"));
        return;
    }

    m_passwordOpen = true;
    m_passwordPanel->openPanel();
}

void MapScene::onSystemRepaired()
{
    GameState::instance()->setSystemRepaired(true);
    m_passwordPanel->hide();
    m_passwordOpen = false;
    m_toast->showMessage(QStringLiteral("系统修复成功！封锁已解除"));
}

void MapScene::tryFeedPig()
{
    if (m_dialogueActive || m_passwordOpen)
        return;

    // 找最近的猪
    Npc *pig = nullptr;
    qreal bestDist = 1e9;
    const QRectF pr = m_player->collisionRect();

    for (Npc *npc : m_npcs) {
        if (npc->npcId() != GameState::PIG)
            continue;

        const qreal d = QLineF(
                            pr.center(),
                            npc->sceneBoundingRect().center()
                            ).length();

        if (d < MapData::tileSize() * 1.8 && d < bestDist) {
            pig = npc;
            bestDist = d;
        }
    }

    if (!pig)
        return; // 没靠近猪，不打扰

    // 已喂过：避免重复消耗
    if (GameState::instance()->hasFlag(QStringLiteral("pig_fed"))) {
        if (m_feedCooldown == 0) {
            m_toast->showMessage(QStringLiteral("猪已经吃饱了"));
            m_feedCooldown = 90;
        }
        return;
    }

    // 没有饼干
    if (!GameState::instance()->hasItem(QStringLiteral("cookie"))) {
        if (m_feedCooldown == 0) {
            m_toast->showMessage(QStringLiteral("身上没有饼干，没法喂"));
            m_feedCooldown = 90;
        }
        return;
    }

    // 投喂成功：消耗饼干，打标记，记一条线索（具体线索内容后续与成员 B 对齐）
    GameState::instance()->removeItem(QStringLiteral("cookie"));
    GameState::instance()->setFlag(QStringLiteral("pig_fed"));
    GameState::instance()->addClue(QStringLiteral("pig_clue"));

    // 第三阶段：猪想起了什么，在它旁边掉落一份隐藏实验记录，走近可拾取
    if (!m_recordItem) {
        const int ts = MapData::tileSize();
        const QPointF pc = pig->sceneBoundingRect().center();
        const int col = qBound(1, static_cast<int>(pc.x() / ts) + 1,
                              m_config.tiles.first().size() - 2);
        const int row = qBound(1, static_cast<int>(pc.y() / ts),
                               m_config.tiles.size() - 2);

        m_recordItem = new PickupItem(
            QStringLiteral("hidden_record"), col, row, ts
            );
        m_items.append(m_recordItem);
        m_scene->addItem(m_recordItem);
    }

    m_toast->showMessage(QStringLiteral("你把饼干喂给了猪，它似乎想起了什么，掉出一份记录"));
    m_feedCooldown = 90;
}

// 按 R：查看背包里的隐藏实验记录（成员 C，第三阶段）
void MapScene::tryReadRecord()
{
    if (m_dialogueActive || m_passwordOpen)
        return;

    if (!GameState::instance()->hasItem(QStringLiteral("hidden_record"))) {
        m_toast->showMessage(QStringLiteral("背包里没有隐藏记录"));
        return;
    }

    m_toast->showMessage(
        QStringLiteral("隐藏实验记录：实验楼的封锁并非故障，而是一套存在 Bug 的自动实训验收程序。它误判项目未完成，才把整栋楼锁了起来。"),
        9000
        );
}

// 出口场景踩终点 S：判定结局并弹出结局面板（成员 C，第四阶段，方案 A）
void MapScene::tryExitEnding()
{
    if (m_endingShown)
        return;

    // 只在出口场景触发
    if (m_config.id != SceneId::Exit)
        return;

    // S 是地图里的终点空地标记，buildWorld 已把它收进 m_spawns
    if (!m_spawns.contains('S'))
        return;

    const int ts = MapData::tileSize();
    const QPoint cell = m_spawns.value('S');
    const QRectF targetRect(
        cell.x() * ts, cell.y() * ts, ts, ts
        );

    const QRectF pr = m_player->collisionRect();

    if (!pr.intersects(targetRect))
        return;

    m_endingShown = true;

    const EndingJudge::EndingType type = EndingJudge::judge();

    // 弹结局面板并停住玩家操作
    m_pressedKeys.clear();
    if (m_player)
        m_player->setMovement(0, 0);

    m_endingPanel->showEnding(type);
}

// 走近道具自动拾取，无需按键
void MapScene::autoPickup(){
    if (!m_player)
        return;

    const QRectF pr = m_player->collisionRect();

    for (int i = m_items.size() - 1; i >= 0; --i) {
        PickupItem *item = m_items[i];

        const qreal d = QLineF(
                            pr.center(),
                            item->logicalRect().center()
                            ).length();

        if (d < MapData::tileSize() * 1.6) {
            GameState::instance()->addItem(item->itemId());
            qDebug() << "自动捡到:" << item->itemId()
                     << "背包:" << GameState::instance()->inventory();
            m_items.removeAt(i);
            m_scene->removeItem(item);
            delete item;
        }
    }
}

void MapScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    const int ts = MapData::tileSize();
    const QRectF sr = m_scene->sceneRect();

    painter->fillRect(rect, QColor(24, 28, 40));

    const int c0 = qMax(
        0,
        static_cast<int>(qFloor(rect.left() / ts))
        );

    const int c1 = qMin(
        m_config.tiles.first().size(),
        static_cast<int>(qCeil(rect.right() / ts))
        );

    const int r0 = qMax(
        0,
        static_cast<int>(qFloor(rect.top() / ts))
        );

    const int r1 = qMin(
        m_config.tiles.size(),
        static_cast<int>(qCeil(rect.bottom() / ts))
        );

    for (int row = r0; row < r1; ++row) {
        for (int col = c0; col < c1; ++col) {
            const QRectF cell(col * ts, row * ts, ts, ts);

            const bool even = (row + col) % 2 == 0;

            painter->fillRect(
                cell,
                even ? QColor(48, 56, 78) : QColor(44, 51, 72)
                );

            painter->setPen(QPen(QColor(34, 40, 58), 1));
            painter->drawRect(cell);
        }
    }

    Q_UNUSED(sr)
}

// 窗口大小改变时，重新调整对话框位置
void MapScene::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);

    if (m_dialogueWidget) {
        const int margin = 12;
        const int height = 300;

        m_dialogueWidget->setGeometry(
            margin,
            viewport()->height() - height - margin,
            viewport()->width() - margin * 2,
            height
            );
    }

    if (m_inventoryPanel) {
        m_inventoryPanel->setGeometry(12, 12, 44, 44);
    }

    if (m_toast) {
        // 顶部水平居中，距顶 60px
        const int x = (viewport()->width() - m_toast->width()) / 2;
        m_toast->move(qMax(0, x), 60);
    }

    if (m_passwordPanel) {
        const int x = (viewport()->width() - m_passwordPanel->width()) / 2;
        const int y = (viewport()->height() - m_passwordPanel->height()) / 2;
        m_passwordPanel->move(qMax(0, x), qMax(0, y));
    }
}
