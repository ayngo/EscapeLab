#include "entities/Npc.h"

#include <QPainter>

Npc::Npc(const QString &npcId, int gridCol, int gridRow, int tileSize,
         const QString &spritePath, QGraphicsItem *parent)
    : QGraphicsObject(parent),
      m_npcId(npcId)
{
    const int w = 36;
    const int h = 38;
    const int ox = (tileSize - w) / 2;
    const int oy = tileSize - h;
    setPos(gridCol * tileSize + ox, gridRow * tileSize + oy);

    if (!spritePath.isEmpty()) {
        QPixmap pm(spritePath);
        if (!pm.isNull())
            m_sprite = pm.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
}

QString Npc::npcId() const
{
    return m_npcId;
}

void Npc::setHintVisible(bool visible)
{
    if (m_hintVisible == visible)
        return;
    m_hintVisible = visible;
    update();
}

void Npc::requestInteract()
{
    emit interactRequested(m_npcId);
}

QRectF Npc::boundingRect() const
{
    return QRectF(0, 0, 36, 38);
}

void Npc::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    const QRectF r = boundingRect();

    if (!m_sprite.isNull()) {
        painter->setRenderHint(QPainter::SmoothPixmapTransform);
        painter->drawPixmap(r.toRect(), m_sprite);
    } else {
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(200, 120, 120));
        painter->drawRoundedRect(r, 8, 8);
        painter->setPen(QPen(Qt::white));
        painter->drawText(r, Qt::AlignCenter, m_npcId.left(1).toUpper());
    }

    if (m_hintVisible) {
        const QRectF bubble(r.center().x() - 11, -18, 22, 16);
        painter->setPen(QPen(QColor(60, 60, 70), 1));
        painter->setBrush(QColor(255, 248, 220));
        painter->drawRoundedRect(bubble, 6, 6);
        painter->setPen(QPen(QColor(60, 60, 70)));
        painter->drawText(bubble, Qt::AlignCenter, QStringLiteral("E"));
    }
}
