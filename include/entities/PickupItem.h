#ifndef PICKUPITEM_H
#define PICKUPITEM_H

#include <QGraphicsRectItem>
#include <QString>

// 可拾取道具：继承 QGraphicsRectItem，风格照 GameObject。
// 自己不做碰撞（玩家可以走上去），只负责显示 + 提供 itemId。
class PickupItem : public QGraphicsRectItem
{
public:
    PickupItem(const QString &itemId, int gridCol, int gridRow,
               int tileSize, QGraphicsItem *parent = nullptr);

    QString itemId() const;

    // 所在格子的逻辑矩形（给 MapScene 算距离用）
    QRectF logicalRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

private:
    QString m_itemId;
    int m_gridCol;
    int m_gridRow;
    int m_tileSize;
};

#endif // PICKUPITEM_H
