#ifndef NPC_H
#define NPC_H

#include <QGraphicsObject>
#include <QPixmap>
#include <QString>

/**
 * @brief 非玩家角色（NPC）的框架实现。
 *
 * 成员 A 负责在地图上正确放置并提供交互入口；成员 B 在此基础上
 * 接入 Galgame 对话框、选项与好感度逻辑。玩家靠近并按下交互键时，
 * MapScene 调用 requestInteract()，由 GameManager 决定如何打开对话。
 */
class Npc : public QGraphicsObject
{
    Q_OBJECT

public:
    Npc(const QString &npcId, int gridCol, int gridRow, int tileSize,
        const QString &spritePath, QGraphicsItem *parent = nullptr);

    QString npcId() const;

    /** 显示/隐藏“按 E 交互”的提示。 */
    void setHintVisible(bool visible);

    /** 触发交互请求（成员 B 可改为连接到对话系统）。 */
    void requestInteract();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

signals:
    void interactRequested(const QString &npcId);

private:
    QString m_npcId;
    QPixmap m_sprite;
    bool m_hintVisible = false;
};

#endif // NPC_H
