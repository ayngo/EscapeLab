#ifndef PASSWORDPANEL_H
#define PASSWORDPANEL_H

#include <QWidget>

class QLineEdit;
class QPushButton;
class QLabel;

/**
 * @brief 系统修复密码输入弹窗（成员 C）。
 *
 * 居中悬浮在视口上：输入管理员密码，确认后由外部校验。
 * 正确发出 repaired() 信号；错误显示重试提示。
 * 关闭时发 closed()，由 MapScene 恢复操作。
 */
class PasswordPanel : public QWidget
{
    Q_OBJECT

public:
    explicit PasswordPanel(QWidget *parent = nullptr);

    /** 打开面板并清空输入。 */
    void openPanel();

signals:
    /** 密码正确，系统已修复。 */
    void repaired();
    /** 面板被关闭（成功或取消），恢复玩家操作。 */
    void closed();

private:
    QLineEdit *m_edit;
    QPushButton *m_ok;
    QPushButton *m_cancel;
    QLabel *m_hint;

    void onSubmit();
};

#endif // PASSWORDPANEL_H
