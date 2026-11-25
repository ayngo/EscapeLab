#include "entities/PickupItem.h"

#include <QFont>
#include <QPainter>

PickupItem::PickupItem(const QString &itemId, int gridCol, int gridRow,
                       int tileSize, QGraphicsItem *parent)
    : QGraphicsRectItem(0, 0, tileSize, tileSize, parent),
      m_itemId(itemId),
      m_gridCol(gridCol),
      m_gridRow(gridRow),
      m_tileSize(tileSize)
{
    setPos(gridCol * tileSize, gridRow * tileSize);
}

QString PickupItem::itemId() const
{
    return m_itemId;
}

QRectF PickupItem::logicalRect() const
{
    return QRectF(m_gridCol * m_tileSize, m_gridRow * m_tileSize,
                  m_tileSize, m_tileSize);
}

void PickupItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                       QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    // 道具整体缩小一点，摆在格子中间，像地上的小物件
    const QRectF r = rect().adjusted(4, 12, -4, -12);

    painter->setBrush(QColor(230, 200, 90)); // 金黄色，像卡片
    painter->setPen(QPen(QColor(170, 140, 40), 2));
    painter->drawRoundedRect(r, 4, 4);

    painter->setPen(QPen(QColor(120, 90, 20)));
    painter->setFont(QFont(QStringLiteral("Microsoft YaHei"), 8));
    painter->drawText(r, Qt::AlignCenter, QStringLiteral("门禁卡"));
}
