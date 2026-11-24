#ifndef MAPSCENE_H
#define MAPSCENE_H

#include <QGraphicsView>
#include <QHash>
#include <QSet>

#include "core/MapData.h"

class Player;
class Portal;
class Npc;
class QGraphicsScene;
class QTimer;

/**
 * @brief 可探索地图场景：渲染地图、处理键盘移动、碰撞与门传送。
 *
 * 每个房间对应一个 MapScene 实例，由 MapConfig 数据驱动构建。
 * 场景本身不直接切换窗口，而是通过信号把“传送/交互”请求上报给
 * GameManager，由其统一调度，保证模块解耦。
 */
class MapScene : public QGraphicsView
{
    Q_OBJECT

public:
    explicit MapScene(const MapConfig &config, QWidget *parent = nullptr);

    QString sceneId() const;
    QString displayName() const;

    /** 把玩家放到命名落点；找不到则放到默认出生点 P。 */
    void placePlayer(QChar spawnChar = QChar('P'));

signals:
    void portalRequested(const QString &targetScene, QChar spawnChar);
    void npcInteractRequested(const QString &npcId);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void drawBackground(QPainter *painter, const QRectF &rect) override;

private slots:
    void loop();

private:
    void buildWorld();
    QList<QRectF> solidRects() const;
    void updateNpcProximity();
    void tryInteract();

    static QString npcIdFromChar(QChar ch);

    MapConfig m_config;
    QGraphicsScene *m_scene;
    Player *m_player;
    QList<Portal *> m_portals;
    QList<Npc *> m_npcs;
    QHash<QChar, QPoint> m_spawns;

    QTimer *m_timer;
    QSet<int> m_pressedKeys;
    qreal m_lastFrameMs;
    int m_portalCooldown = 0;
};

#endif // MAPSCENE_H
