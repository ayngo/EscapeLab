#include "ui/PasswordPanel.h"

#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

PasswordPanel::PasswordPanel(QWidget *parent)
    : QWidget(parent)
{
    setObjectName("PasswordPanel");
    setAttribute(Qt::WA_StyledBackground, true);
    setFixedWidth(320);

    setStyleSheet(
        "#PasswordPanel {"
        "  background-color: rgba(15, 24, 42, 245);"
        "  border: 2px solid #F5D58A;"
        "  border-radius: 12px;"
        "}"
        "QLabel { color: #E8D9A8; font-size: 14px; }"
        "QLineEdit {"
        "  background-color: rgba(255,255,255,20);"
        "  color: #fff; border: 1px solid #F5D58A; border-radius: 6px;"
        "  padding: 6px; font-size: 15px;"
        "}"
        "QPushButton {"
        "  background-color: #F5D58A; color: #2a2010; border: none;"
        "  border-radius: 6px; padding: 7px 16px; font-weight: bold;"
        "}"
        "QPushButton#cancelBtn { background-color: rgba(255,255,255,40); color: #eee; }"
        );

    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(20, 18, 20, 18);
    root->setSpacing(12);

    auto *title = new QLabel(QStringLiteral("系统修复 — 输入管理员密码"), this);
    title->setAlignment(Qt::AlignCenter);
    root->addWidget(title);

    m_edit = new QLineEdit(this);
    m_edit->setPlaceholderText(QStringLiteral("请输入密码"));
    m_edit->setEchoMode(QLineEdit::Password);
    root->addWidget(m_edit);

    m_hint = new QLabel(QStringLiteral(""), this);
    m_hint->setAlignment(Qt::AlignCenter);
    m_hint->setStyleSheet("color: #FF9A9A;");
    root->addWidget(m_hint);

    auto *row = new QHBoxLayout();
    m_ok = new QPushButton(QStringLiteral("确认修复"), this);
    m_cancel = new QPushButton(QStringLiteral("取消"), this);
    m_cancel->setObjectName("cancelBtn");
    row->addStretch();
    row->addWidget(m_ok);
    row->addWidget(m_cancel);
    row->addStretch();
    root->addLayout(row);

    connect(m_ok, &QPushButton::clicked, this, &PasswordPanel::onSubmit);
    connect(m_cancel, &QPushButton::clicked, this, &PasswordPanel::closed);
    connect(m_edit, &QLineEdit::returnPressed, this, &PasswordPanel::onSubmit);
}

void PasswordPanel::openPanel()
{
    m_edit->clear();
    m_hint->setText(QStringLiteral(""));
    show();
    raise();
    m_edit->setFocus();
}

void PasswordPanel::onSubmit()
{
    // 管理员密码（成员 C 暂定，后续可与成员 B 对话线索对齐）
    const QString kAdminPassword = QStringLiteral("2024");

    if (m_edit->text().trimmed() == kAdminPassword) {
        emit repaired();
    } else {
        m_hint->setText(QStringLiteral("密码错误，请重试"));
        m_edit->clear();
        m_edit->setFocus();
    }
}
