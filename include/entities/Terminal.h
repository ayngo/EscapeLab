#ifndef TERMINAL_H
#define TERMINAL_H

#include <QGraphicsRectItem>
#include <QPoint>

/**
 * @brief 系统修复终端（成员 C）。
 *
 * 非固体，玩家可走近；外观是一台带屏幕的终端。
 * 只负责显示，距离判定与密码校验由 MapScene 处理。
 */
class Terminal : public QGraphicsRectItem
{
public:
    Terminal(int gridCol, int gridRow, int tileSize,
             QGraphicsItem *parent = nullptr);

    QRectF logicalRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

private:
    int m_gridCol;
    int m_gridRow;
    int m_tileSize;
};

#endif // TERMINAL_H
