#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QGraphicsRectItem>

/**
 * @brief 场景中的静态物件：墙、桌柜、电脑台等。
 *
 * 固体物件参与玩家碰撞；不同类型用不同颜色与边框绘制，
 * 成员 D 后续可替换为贴图而不影响碰撞逻辑。
 */
class GameObject : public QGraphicsRectItem
{
public:
    enum Type
    {
        Wall,
        Table,
        Computer,
        Decoration
    };

    GameObject(int gridCol, int gridRow, int tileSize, Type type,
               QGraphicsItem *parent = nullptr);

    Type objectType() const;
    bool isSolid() const;

    /** 场景坐标下的精确逻辑碰撞矩形（不含画笔宽度膨胀）。 */
    QRectF logicalRect() const;

    static Type typeFromChar(QChar ch);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

private:
    Type m_type;
    int m_gridCol;
    int m_gridRow;
    int m_tileSize;
};

#endif // GAMEOBJECT_H
