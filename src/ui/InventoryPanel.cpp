#include "ui/InventoryPanel.h"

#include <QEvent>
#include <QMouseEvent>
#include <QLabel>
#include <QVBoxLayout>

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

    auto *title = new QLabel("背包", m_popup);
    title->setStyleSheet(
        "font-size: 15px; font-weight: bold; color: #F5D58A;"
        );

    m_list = new QLabel("（空）", m_popup);
    m_list->setStyleSheet("font-size: 13px; color: white;");
    m_list->setWordWrap(true);

    popupLayout->addWidget(title);
    popupLayout->addWidget(m_list);

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

    // 弹出列表内容
    if (items.isEmpty()) {
        m_list->setText("（空）");
        return;
    }

    QString text;

    for (int i = 0; i < items.size(); ++i) {
        if (i > 0)
            text += "\n";

        text += "· " + itemName(items[i]);
    }

    m_list->setText(text);
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
    }
}

void InventoryPanel::updatePopupGeometry()
{
    // 弹出框贴着图标右下方，宽度 180，高度按内容
    const int x = 12 + ICON_SIZE + 8;
    const int y = 12;
    m_popup->setGeometry(x, y, 180, 110);
}

bool InventoryPanel::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_icon && event->type() == QEvent::MouseButtonRelease) {
        toggleList();
        return true;
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
