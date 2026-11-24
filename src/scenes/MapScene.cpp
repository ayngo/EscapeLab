#include "scenes/MapScene.h"

#include "core/GameState.h"
#include "entities/Player.h"
#include "entities/Portal.h"
#include "entities/Npc.h"
#include "entities/GameObject.h"

#include <QElapsedTimer>
#include <QGraphicsScene>
#include <QKeyEvent>
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
                auto *obj = new GameObject(col, row, ts, GameObject::typeFromChar(ch));
                m_scene->addItem(obj);
            }
            else if (m_config.portalTargets.contains(ch)) {
                auto *portal = new Portal(col, row, ts, ch, m_config.portalTargets[ch]);
                m_portals.append(portal);
                m_scene->addItem(portal);
                m_spawns[ch] = QPoint(col, row);
            }
            else if (ch == 'P') {
                m_spawns['P'] = QPoint(col, row);
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
                    connect(npcItem, &Npc::interactRequested,
                            this, &MapScene::npcInteractRequested);
                    m_npcs.append(npcItem);
                    m_scene->addItem(npcItem);
                } else {
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

void MapScene::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) {
        QGraphicsView::keyPressEvent(event);
        return;
    }

    if (event->key() == Qt::Key_E) {
        tryInteract();
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
    const qreal now = g_clock.elapsed() / 1000.0;
    qreal dt = now - m_lastFrameMs;
    m_lastFrameMs = now;
    if (dt <= 0 || dt > 0.1)
        dt = 1.0 / 60.0;

    int hx = 0;
    int hy = 0;
    if (m_pressedKeys.contains(Qt::Key_Left) || m_pressedKeys.contains(Qt::Key_A))
        hx -= 1;
    if (m_pressedKeys.contains(Qt::Key_Right) || m_pressedKeys.contains(Qt::Key_D))
        hx += 1;
    if (m_pressedKeys.contains(Qt::Key_Up) || m_pressedKeys.contains(Qt::Key_W))
        hy -= 1;
    if (m_pressedKeys.contains(Qt::Key_Down) || m_pressedKeys.contains(Qt::Key_S))
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

    if (m_portalCooldown == 0) {
        const QRectF pr = m_player->collisionRect();
        for (Portal *portal : m_portals) {
            if (pr.intersects(portal->sceneBoundingRect())) {
                emit portalRequested(portal->targetScene(), portal->spawnChar());
                m_portalCooldown = 30;
                break;
            }
        }
    }

    updateNpcProximity();
}

void MapScene::updateNpcProximity()
{
    const QRectF pr = m_player->collisionRect();
    for (Npc *npc : m_npcs) {
        const qreal distance = QLineF(pr.center(), npc->sceneBoundingRect().center()).length();
        npc->setHintVisible(distance < MapData::tileSize() * 1.6);
    }
}

void MapScene::tryInteract()
{
    const QRectF pr = m_player->collisionRect();
    Npc *best = nullptr;
    qreal bestDist = 1e9;
    for (Npc *npc : m_npcs) {
        const qreal d = QLineF(pr.center(), npc->sceneBoundingRect().center()).length();
        if (d < MapData::tileSize() * 1.6 && d < bestDist) {
            best = npc;
            bestDist = d;
        }
    }
    if (best)
        best->requestInteract();
}

void MapScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    const int ts = MapData::tileSize();
    const QRectF sr = m_scene->sceneRect();

    painter->fillRect(rect, QColor(24, 28, 40));

    const int c0 = qMax(0, static_cast<int>(qFloor(rect.left() / ts)));
    const int c1 = qMin(m_config.tiles.first().size(),
                        static_cast<int>(qCeil(rect.right() / ts)));
    const int r0 = qMax(0, static_cast<int>(qFloor(rect.top() / ts)));
    const int r1 = qMin(m_config.tiles.size(),
                        static_cast<int>(qCeil(rect.bottom() / ts)));

    for (int row = r0; row < r1; ++row) {
        for (int col = c0; col < c1; ++col) {
            const QRectF cell(col * ts, row * ts, ts, ts);
            const bool even = (row + col) % 2 == 0;
            painter->fillRect(cell, even ? QColor(48, 56, 78) : QColor(44, 51, 72));
            painter->setPen(QPen(QColor(34, 40, 58), 1));
            painter->drawRect(cell);
        }
    }
    Q_UNUSED(sr)
}
