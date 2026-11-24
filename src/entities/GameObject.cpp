#include "entities/GameObject.h"

#include <QPainter>

GameObject::GameObject(int gridCol, int gridRow, int tileSize, Type type,
                       QGraphicsItem *parent)
    : QGraphicsRectItem(0, 0, tileSize, tileSize, parent),
      m_type(type),
      m_gridCol(gridCol),
      m_gridRow(gridRow),
      m_tileSize(tileSize)
{
    setPos(gridCol * tileSize, gridRow * tileSize);
}

QRectF GameObject::logicalRect() const
{
    return QRectF(m_gridCol * m_tileSize, m_gridRow * m_tileSize,
                  m_tileSize, m_tileSize);
}

GameObject::Type GameObject::objectType() const
{
    return m_type;
}

bool GameObject::isSolid() const
{
    return true;
}

GameObject::Type GameObject::typeFromChar(QChar ch)
{
    if (ch == '#')
        return Wall;
    if (ch == 'T')
        return Table;
    if (ch == 'C')
        return Computer;
    return Decoration;
}

void GameObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                       QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    QColor fill;
    QColor edge;
    switch (m_type) {
    case Wall:
        fill = QColor(58, 70, 92);
        edge = QColor(36, 44, 60);
        break;
    case Table:
        fill = QColor(150, 112, 78);
        edge = QColor(110, 78, 50);
        break;
    case Computer:
        fill = QColor(86, 110, 138);
        edge = QColor(56, 76, 100);
        break;
    default:
        fill = QColor(120, 120, 120);
        edge = QColor(90, 90, 90);
        break;
    }

    painter->fillRect(rect(), fill);
    painter->setPen(QPen(edge, 2));
    painter->drawRect(rect().adjusted(1, 1, -1, -1));

    if (m_type == Wall) {
        painter->setPen(QPen(edge, 1));
        const int mid = static_cast<int>(rect().center().y());
        painter->drawLine(rect().left() + 2, mid, rect().right() - 2, mid);
    }
}
