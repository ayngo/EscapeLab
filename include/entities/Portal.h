#ifndef PORTAL_H
#define PORTAL_H

#include <QGraphicsRectItem>
#include <QChar>

#include "core/MapData.h"

/**
 * @brief 场景传送门（门洞）。
 *
 * 玩家走入门洞后由 MapScene 检测并请求 GameManager 切换到目标场景，
 * 玩家被放置到目标场景的命名落点上。
 */
class Portal : public QGraphicsRectItem
{
public:
    Portal(int gridCol, int gridRow, int tileSize, QChar doorChar,
           const PortalTarget &target, QGraphicsItem *parent = nullptr);

    QChar doorChar() const;
    QString targetScene() const;
    QChar spawnChar() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

private:
    QChar m_doorChar;
    PortalTarget m_target;
};

#endif // PORTAL_H
