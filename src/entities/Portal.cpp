#include "entities/Portal.h"

#include <QPainter>

Portal::Portal(int gridCol, int gridRow, int tileSize, QChar doorChar,
               const PortalTarget &target, QGraphicsItem *parent)
    : QGraphicsRectItem(0, 0, tileSize, tileSize, parent),
      m_doorChar(doorChar),
      m_target(target)
{
    setPos(gridCol * tileSize, gridRow * tileSize);
}

QChar Portal::doorChar() const
{
    return m_doorChar;
}

QString Portal::targetScene() const
{
    return m_target.targetScene;
}

QChar Portal::spawnChar() const
{
    return m_target.spawnChar;
}

void Portal::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    const QRectF r = rect();
    painter->fillRect(r, QColor(40, 48, 66));

    const QRectF door = r.adjusted(5, 3, -5, -2);
    painter->fillRect(door, QColor(124, 207, 178));
    painter->setPen(QPen(QColor(230, 246, 240), 2));
    painter->drawRect(door);

    painter->setPen(QPen(QColor(30, 60, 50), 1));
    painter->drawLine(door.center().x(), door.top(), door.center().x(), door.bottom());
}
