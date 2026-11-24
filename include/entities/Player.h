#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsObject>
#include <QList>
#include <QPixmap>
#include <QRectF>

/**
 * @brief 玩家角色。
 *
 * 由 MapScene 每帧调用 tick()，按当前移动方向（键盘输入）推进位置，
 * 并对固体矩形做分轴碰撞检测（先 X 后 Y），避免卡墙。
 * 成员 D 可通过 setSprite() 用贴图替换默认绘制。
 */
class Player : public QGraphicsObject
{
    Q_OBJECT

public:
    explicit Player(int tileSize, QGraphicsItem *parent = nullptr);

    /** 设置归一化移动方向（-1~1），由键盘状态计算。 */
    void setMovement(qreal vx, qreal vy);

    /** 推进一帧；solids 为场景坐标下的固体碰撞矩形。 */
    void tick(qreal deltaSeconds, const QList<QRectF> &solids);

    void setSpeed(qreal pixelsPerSecond);
    qreal speed() const;

    /** 用贴图替换默认矢量绘制；空 pixmap 恢复默认。 */
    void setSprite(const QPixmap &pixmap);

    /** 场景坐标下的碰撞盒（略小于绘制尺寸）。 */
    QRectF collisionRect() const;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

private:
    bool collides(const QList<QRectF> &solids) const;

    int m_tileSize;
    qreal m_speed = 220.0;
    qreal m_vx = 0.0;
    qreal m_vy = 0.0;
    int m_facing = 1;
    QPixmap m_sprite;
};

#endif // PLAYER_H
