#ifndef INVENTORYPANEL_H
#define INVENTORYPANEL_H

#include <QWidget>

class QLabel;

/**
 * @brief 背包（成员 C）。
 *
 * 平时只在视口左上角显示一个小小的"背包"图标（带物品数量角标），
 * 不遮挡游戏画面。按 Tab（或点图标）弹出/收起物品列表。
 * 监听 GameState::itemChanged()，背包一变就自动刷新。
 * 鼠标悬停到某条道具上时，弹出该道具的用途与使用方法；
 * 移开后只显示道具名。
 */
class InventoryPanel : public QWidget
{
    Q_OBJECT

public:
    explicit InventoryPanel(QWidget *parent = nullptr);

    /** 手动刷新一次背包内容（初始化时调用）。 */
    void refresh();

    /** 切换物品列表的显示/隐藏（按 Tab 时调用）。 */
    void toggleList();

    /** 把道具 id 翻译成中文名称，未知 id 原样显示。 */
    static QString itemName(const QString &itemId);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    QLabel *m_icon;      // 左上角小背包图标（可点击）
    QLabel *m_badge;     // 物品数量角标
    QWidget *m_popup;    // 弹出的物品列表
    QWidget *m_listBox;  // 列表项容器（每行一个可悬停条目）
    QLabel *m_tip;       // 悬停时显示用途/用法的提示气泡
    bool m_open = false;

    void updatePopupGeometry();

    /** 返回某道具的"用途 + 使用方法"说明文本。 */
    static QString itemTip(const QString &itemId);
};

#endif // INVENTORYPANEL_H
