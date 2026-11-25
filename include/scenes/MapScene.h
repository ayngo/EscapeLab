
#ifndef MAPSCENE_H
#define MAPSCENE_H

#include <QGraphicsView>
#include <QHash>
#include <QSet>

#include "core/MapData.h"

class Player;
class Portal;
class Npc;
class PickupItem;
class DialogueWidget;
class InventoryPanel;
class QGraphicsScene;
class QTimer;
class QResizeEvent;

class MapScene : public QGraphicsView
{
    Q_OBJECT

public:
    explicit MapScene(
        const MapConfig &config,
        QWidget *parent = nullptr
        );

    QString sceneId() const;
    QString displayName() const;

    void placePlayer(QChar spawnChar = QChar('P'));

    // 对话系统接口
    void attachDialogueWidget(DialogueWidget *widget);
    void setDialogueActive(bool active);

signals:
    void portalRequested(
        const QString &targetScene,
        QChar spawnChar
        );

    void npcInteractRequested(const QString &npcId);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    void drawBackground(
        QPainter *painter,
        const QRectF &rect
        ) override;

    void resizeEvent(QResizeEvent *event) override;

private slots:
    void loop();

private:
    void buildWorld();
    QList<QRectF> solidRects() const;
    void updateNpcProximity();
    void tryInteract();
    void autoPickup();

    static QString npcIdFromChar(QChar ch);

    MapConfig m_config;

    QGraphicsScene *m_scene;
    Player *m_player;

    QList<Portal *> m_portals;
    QList<Npc *> m_npcs;
    QList<PickupItem *> m_items;

    QHash<QChar, QPoint> m_spawns;

    QTimer *m_timer;
    QSet<int> m_pressedKeys;

    qreal m_lastFrameMs;
    int m_portalCooldown = 0;

    // 对话系统
    DialogueWidget *m_dialogueWidget = nullptr;
    bool m_dialogueActive = false;

    // 背包面板
    InventoryPanel *m_inventoryPanel = nullptr;
};

#endif
