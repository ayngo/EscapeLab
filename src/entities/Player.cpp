#include "entities/Player.h"

#include <QPainter>

Player::Player(int tileSize, QGraphicsItem *parent)
    : QGraphicsObject(parent),
      m_tileSize(tileSize)
{
}

void Player::setMovement(qreal vx, qreal vy)
{
    m_vx = vx;
    m_vy = vy;
    if (vx > 0)
        m_facing = 1;
    else if (vx < 0)
        m_facing = -1;
}

void Player::tick(qreal deltaSeconds, const QList<QRectF> &solids)
{
    const qreal stepX = m_vx * m_speed * deltaSeconds;
    const qreal stepY = m_vy * m_speed * deltaSeconds;

    if (stepX != 0.0) {
        const qreal oldX = x();
        setPos(oldX + stepX, y());
        if (collides(solids))
            setPos(oldX, y());
    }

    if (stepY != 0.0) {
        const qreal oldY = y();
        setPos(x(), oldY + stepY);
        if (collides(solids))
            setPos(x(), oldY);
    }
}

void Player::setSpeed(qreal pixelsPerSecond)
{
    m_speed = pixelsPerSecond;
}

qreal Player::speed() const
{
    return m_speed;
}

void Player::setSprite(const QPixmap &pixmap)
{
    prepareGeometryChange();
    m_sprite = pixmap;
}

QRectF Player::collisionRect() const
{
    return mapRectToScene(QRectF(3, 8, 24, 26));
}

QRectF Player::boundingRect() const
{
    return QRectF(0, 0, 30, 34);
}

bool Player::collides(const QList<QRectF> &solids) const
{
    const QRectF me = collisionRect();
    for (const QRectF &s : solids) {
        if (me.intersects(s))
            return true;
    }
    return false;
}

void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    if (!m_sprite.isNull()) {
        painter->setRenderHint(QPainter::SmoothPixmapTransform);
        painter->drawPixmap(boundingRect().toRect(), m_sprite);
        return;
    }

    const QRectF r = boundingRect();

    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(70, 110, 200));
    painter->drawRoundedRect(QRectF(r.left() + 3, r.top() + 12, 24, 20), 6, 6);

    painter->setBrush(QColor(245, 214, 184));
    painter->drawEllipse(QPointF(r.center().x(), r.top() + 8), 8, 8);

    painter->setBrush(QColor(40, 40, 50));
    const qreal eyeX = r.center().x() + m_facing * 2.5;
    painter->drawEllipse(QPointF(eyeX, r.top() + 7), 1.6, 1.6);
}
