#include "entities/Terminal.h"

#include <QFont>
#include <QPainter>

Terminal::Terminal(int gridCol, int gridRow, int tileSize,
                   QGraphicsItem *parent)
    : QGraphicsRectItem(0, 0, tileSize, tileSize, parent),
      m_gridCol(gridCol),
      m_gridRow(gridRow),
      m_tileSize(tileSize)
{
    setPos(gridCol * tileSize, gridRow * tileSize);
}

QRectF Terminal::logicalRect() const
{
    return QRectF(m_gridCol * m_tileSize, m_gridRow * m_tileSize,
                  m_tileSize, m_tileSize);
}

void Terminal::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    // 机身
    painter->setBrush(QColor(70, 90, 120));
    painter->setPen(QPen(QColor(40, 55, 80), 2));
    painter->drawRoundedRect(rect().adjusted(3, 6, -3, -3), 4, 4);

    // 屏幕
    const QRectF screen = rect().adjusted(8, 10, -8, -14);
    painter->setBrush(QColor(120, 220, 180));
    painter->setPen(QPen(QColor(60, 140, 110), 1));
    painter->drawRect(screen);

    painter->setPen(QPen(QColor(20, 70, 55)));
    painter->setFont(QFont(QStringLiteral("Microsoft YaHei"), 7));
    painter->drawText(screen, Qt::AlignCenter, QStringLiteral("修复终端"));
}
