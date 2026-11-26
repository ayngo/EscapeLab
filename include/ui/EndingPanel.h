#ifndef ENDINGPANEL_H
#define ENDINGPANEL_H

#include <QWidget>

#include "core/EndingJudge.h"

class QLabel;
class QPushButton;

/**
 * @brief 结局展示面板（成员 C，第四阶段）。
 *
 * 全屏悬浮遮罩，显示结局标题、结局文案与「重新开始」按钮。
 * 由出口场景在判定出结局后调用 showEnding() 弹出。
 * 「重新开始」只发出 restartRequested() 信号，具体重开流程由外部
 * （GameManager）决定，本面板不直接改动游戏状态。
 */
class EndingPanel : public QWidget
{
    Q_OBJECT

public:
    explicit EndingPanel(QWidget *parent = nullptr);

    /** 按结局类型填充并弹出面板。 */
    void showEnding(EndingJudge::EndingType type);

signals:
    /** 玩家点击「重新开始」，请求外部重置并回到起点。 */
    void restartRequested();

private:
    QLabel *m_title = nullptr;
    QLabel *m_desc = nullptr;
    QPushButton *m_restart = nullptr;
};

#endif // ENDINGPANEL_H
