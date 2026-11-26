#include "ui/InventoryPanel.h"

#include <QEvent>
#include <QMouseEvent>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "core/GameState.h"

namespace {
// 左上角小图标尺寸
const int ICON_SIZE = 44;
}

InventoryPanel::InventoryPanel(QWidget *parent)
    : QWidget(parent)
{
    setObjectName("InventoryPanel");

    setAttribute(Qt::WA_StyledBackground, true);

    // 这个外层容器只负责摆图标，本身不画背景
    setStyleSheet("#InventoryPanel { background: transparent; }");

    // —— 小背包图标（可点击）——
    m_icon = new QLabel("背包", this);
    m_icon->setAlignment(Qt::AlignCenter);
    m_icon->setFixedSize(ICON_SIZE, ICON_SIZE);
    m_icon->move(0, 0);
    m_icon->setCursor(Qt::PointingHandCursor);
    m_icon->setStyleSheet(
        "QLabel {"
        "  background-color: rgba(15, 24, 42, 220);"
        "  border: 2px solid #91A9C8;"
        "  border-radius: 10px;"
        "  color: #F5D58A;"
        "  font-size: 13px;"
        "  font-weight: bold;"
        "}"
        );
    m_icon->installEventFilter(this);

    // —— 数量角标 ——
    m_badge = new QLabel("0", this);
    m_badge->setAlignment(Qt::AlignCenter);
    m_badge->setFixedSize(18, 18);
    m_badge->move(ICON_SIZE - 12, -4);
    m_badge->setStyleSheet(
        "QLabel {"
        "  background-color: #E06666;"
        "  color: white;"
        "  border-radius: 9px;"
        "  font-size: 11px;"
        "  font-weight: bold;"
        "}"
        );
    m_badge->hide();

    // —— 弹出的物品列表（默认隐藏）——
    // 父级直接用 viewport（this 的父控件），避免被 44x44 的图标容器裁剪
    m_popup = new QWidget(parentWidget());
    m_popup->setObjectName("InvPopup");
    m_popup->setStyleSheet(
        "#InvPopup {"
        "  background-color: rgba(15, 24, 42, 245);"
        "  border: 2px solid #91A9C8;"
        "  border-radius: 10px;"
        "}"
        );

    auto *popupLayout = new QVBoxLayout(m_popup);
    popupLayout->setContentsMargins(12, 10, 12, 10);
    popupLayout->setSpacing(4);

    auto *title = new QLabel("背包（悬停查看用途）", m_popup);
    title->setStyleSheet(
        "font-size: 15px; font-weight: bold; color: #F5D58A;"
        );
    popupLayout->addWidget(title);

    // 条目容器：每个道具一行，悬停该行显示用途/用法
    m_listBox = new QWidget(m_popup);
    auto *boxLayout = new QVBoxLayout(m_listBox);
    boxLayout->setContentsMargins(0, 0, 0, 0);
    boxLayout->setSpacing(2);
    popupLayout->addWidget(m_listBox);

    // —— 悬停提示气泡（默认隐藏）——
    m_tip = new QLabel(parentWidget());
    m_tip->setObjectName("InvTip");
    m_tip->setWordWrap(true);
    m_tip->setStyleSheet(
        "#InvTip {"
        "  background-color: rgba(20, 30, 52, 250);"
        "  border: 2px solid #F5D58A;"
        "  border-radius: 8px;"
        "  color: #E8D9A8;"
        "  font-size: 13px;"
        "  padding: 8px 10px;"
        "}"
        );
    m_tip->hide();

    m_popup->hide();

    // 背包一变就刷新
    connect(
        GameState::instance(),
        &GameState::itemChanged,
        this,
        &InventoryPanel::refresh
        );

    refresh();
}

void InventoryPanel::refresh()
{
    const QStringList items = GameState::instance()->inventory();

    // 角标数量
    if (items.isEmpty()) {
        m_badge->hide();
    } else {
        m_badge->setText(QString::number(items.size()));
        m_badge->show();
        m_badge->raise();
    }

    // 清空旧条目
    QLayoutItem *child;
    while ((child = m_listBox->layout()->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    if (items.isEmpty()) {
        auto *empty = new QLabel("（空）", m_listBox);
        empty->setStyleSheet("font-size: 13px; color: white;");
        m_listBox->layout()->addWidget(empty);
        return;
    }

    // 每条道具一个可悬停行：悬停显示用途/用法，移开只留道具名
    for (const QString &itemId : items) {
        auto *row = new QLabel("· " + itemName(itemId), m_listBox);
        row->setStyleSheet(
            "QLabel {"
            "  font-size: 13px; color: white; padding: 3px 4px;"
            "  border-radius: 6px;"
            "}"
            "QLabel:hover { background-color: rgba(145,169,200,60); }"
            );

        const QString tipText = itemTip(itemId);

        // 悬停：显示提示气泡；离开：隐藏
        row->installEventFilter(this);
        row->setProperty("invTip", tipText);

        m_listBox->layout()->addWidget(row);
    }
}

void InventoryPanel::toggleList()
{
    m_open = !m_open;

    if (m_open) {
        updatePopupGeometry();
        m_popup->show();
        m_popup->raise();
    } else {
        m_popup->hide();
        m_tip->hide();
    }
}

void InventoryPanel::updatePopupGeometry()
{
    // 弹出框贴着图标右下方，宽度 200，高度按内容
    const int x = 12 + ICON_SIZE + 8;
    const int y = 12;
    m_popup->setGeometry(x, y, 200, 140);
}

bool InventoryPanel::eventFilter(QObject *watched, QEvent *event)
{
    // 背包图标点击：开合列表
    if (watched == m_icon && event->type() == QEvent::MouseButtonRelease) {
        toggleList();
        return true;
    }

    // 道具行悬停：显示/隐藏用途提示气泡
    if (watched->property("invTip").isValid()) {
        if (event->type() == QEvent::Enter) {
            const QString text = watched->property("invTip").toString();
            m_tip->setText(text);
            m_tip->adjustSize();

            // 气泡放在条目右侧，避免盖住列表
            QLabel *row = qobject_cast<QLabel *>(watched);
            if (row) {
                const QPoint gp = row->mapToGlobal(QPoint(0, 0));
                const QWidget *par = parentWidget();
                if (par) {
                    const QPoint local = par->mapFromGlobal(gp);
                    m_tip->move(local.x() + row->width() + 10, local.y());
                }
            }
            m_tip->show();
            m_tip->raise();
            return true;
        }

        if (event->type() == QEvent::Leave) {
            m_tip->hide();
            return true;
        }
    }

    return QWidget::eventFilter(watched, event);
}

QString InventoryPanel::itemName(const QString &itemId)
{
    if (itemId == QStringLiteral("key_classroom"))
        return QStringLiteral("门禁卡");

    if (itemId == QStringLiteral("cookie"))
        return QStringLiteral("饼干");

    if (itemId == QStringLiteral("lab_password_note"))
        return QStringLiteral("密码纸条");

    if (itemId == QStringLiteral("hidden_record"))
        return QStringLiteral("隐藏记录");

    return itemId;
}

QString InventoryPanel::itemTip(const QString &itemId)
{
    if (itemId == QStringLiteral("key_classroom"))
        return QStringLiteral("门禁卡\n用途：打开实训教室通往走廊的门。\n用法：走到教室门口自动开门。");

    if (itemId == QStringLiteral("cookie"))
        return QStringLiteral("饼干\n用途：喂给走廊的猪，换取隐藏线索。\n用法：靠近猪按 F 投喂。");

    if (itemId == QStringLiteral("lab_password_note"))
        return QStringLiteral("密码纸条\n用途：写着一段验收程序的 for 循环源码。\n用法：按 R 查看纸条；结合豆包的提示，从循环里找出管理员密码，再到出口终端按 E 输入。");

    if (itemId == QStringLiteral("hidden_record"))
        return QStringLiteral("隐藏记录\n用途：揭示实验楼封锁的真正原因。\n用法：按 R 查看内容。");

    return QString();
}
