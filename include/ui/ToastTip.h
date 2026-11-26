#ifndef TOASTTIP_H
#define TOASTTIP_H

#include <QWidget>

class QLabel;
class QTimer;

/**
 * @brief 屏幕中央临时提示条（成员 C）。
 *
 * 浮在地图视口上方，用于开门失败、操作反馈等简短中文提示。
 * 调用 showMessage(text) 显示，几秒后自动隐藏；重复调用会重置计时。
 * 不依赖任何队友模块，可独立挂载到任意视口。
 */
class ToastTip : public QWidget
{
    Q_OBJECT

public:
    explicit ToastTip(QWidget *parent = nullptr);

    /** 显示一条提示，durationMs 毫秒后自动隐藏（默认 2500ms）。 */
    void showMessage(const QString &text, int durationMs = 2500);

private:
    QLabel *m_label;
    QTimer *m_timer;
};

#endif // TOASTTIP_H
