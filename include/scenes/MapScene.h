
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
class ToastTip;
class Terminal;
class PasswordPanel;
class EndingPanel;
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

    /**
     * @brief 检查某扇门能否通行（成员 C 的门禁判定）。
     * @return true 放行（可传送）；false 拦住（已弹提示，不传送）。
     */
    bool tryDoor(Portal *portal);

    /** 走近终端按 E：打开密码面板（成员 C）。 */
    void tryTerminal();
    /** 密码正确后：标记系统已修复并提示。 */
    void onSystemRepaired();

    /** 走近猪按 F：投喂饼干（成员 C，独立于对话，不改动成员 B 逻辑）。 */
    void tryFeedPig();

    /** 按 R：查看背包里的隐藏实验记录（成员 C）。 */
    void tryReadRecord();

    /** 出口场景踩终点 S：判定结局并弹出结局面板（成员 C，第四阶段）。 */
    void tryExitEnding();

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

    // 屏幕提示条（成员 C）
    ToastTip *m_toast = nullptr;
    int m_doorHintCooldown = 0;
    int m_feedCooldown = 0;

    // 系统修复终端 + 密码面板（成员 C）
    Terminal *m_terminal = nullptr;
    PasswordPanel *m_passwordPanel = nullptr;
    bool m_passwordOpen = false;

    // 喂猪后落地生成的隐藏记录（成员 C，第三阶段）
    PickupItem *m_recordItem = nullptr;

    // 结局面板（成员 C，第四阶段）
    EndingPanel *m_endingPanel = nullptr;
    bool m_endingShown = false;
};

#endif
