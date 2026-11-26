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

    // 按道具 id 选择颜色与显示文字（成员 C 道具外观）
    QColor fill, border;
    QString label;

    if (m_itemId == QStringLiteral("key_classroom")) {
        fill = QColor(230, 200, 90);     // 金黄，门禁卡
        border = QColor(170, 140, 40);
        label = QStringLiteral("门禁卡");
    } else if (m_itemId == QStringLiteral("lab_password_note")) {
        fill = QColor(245, 240, 220);    // 米白纸，密码纸条
        border = QColor(150, 140, 110);
        label = QStringLiteral("纸条");
    } else if (m_itemId == QStringLiteral("cookie")) {
        fill = QColor(190, 140, 90);     // 棕黄，饼干
        border = QColor(120, 80, 40);
        label = QStringLiteral("饼干");
    } else if (m_itemId == QStringLiteral("hidden_record")) {
        fill = QColor(150, 170, 200);    // 蓝灰，隐藏记录
        border = QColor(90, 110, 140);
        label = QStringLiteral("记录");
    } else {
        fill = QColor(210, 210, 210);
        border = QColor(120, 120, 120);
        label = QStringLiteral("道具");
    }

    painter->setBrush(fill);
    painter->setPen(QPen(border, 2));
    painter->drawRoundedRect(r, 4, 4);

    painter->setPen(QPen(QColor(60, 50, 30)));
    painter->setFont(QFont(QStringLiteral("Microsoft YaHei"), 8));
    painter->drawText(r, Qt::AlignCenter, label);
}
