#include "ui/EndingPanel.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

EndingPanel::EndingPanel(QWidget *parent)
    : QWidget(parent)
{
    setObjectName("EndingPanel");
    setAttribute(Qt::WA_StyledBackground, true);

    setStyleSheet(
        "#EndingPanel {"
        "  background-color: rgba(8, 12, 22, 235);"
        "}"
        "QLabel#endingTitle {"
        "  color: #F5D58A; font-size: 30px; font-weight: bold;"
        "}"
        "QLabel#endingDesc {"
        "  color: #D8DCE8; font-size: 16px;"
        "}"
        "QPushButton {"
        "  background-color: #F5D58A; color: #2a2010; border: none;"
        "  border-radius: 8px; padding: 10px 28px; font-weight: bold;"
        "  font-size: 15px;"
        "}"
        );

    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(40, 40, 40, 40);
    root->setSpacing(22);
    root->setAlignment(Qt::AlignCenter);

    m_title = new QLabel(QStringLiteral(""), this);
    m_title->setObjectName("endingTitle");
    m_title->setAlignment(Qt::AlignCenter);
    root->addWidget(m_title);

    m_desc = new QLabel(QStringLiteral(""), this);
    m_desc->setObjectName("endingDesc");
    m_desc->setAlignment(Qt::AlignCenter);
    m_desc->setWordWrap(true);
    m_desc->setMaximumWidth(560);
    root->addWidget(m_desc);

    m_restart = new QPushButton(QStringLiteral("重新开始"), this);
    root->addWidget(m_restart, 0, Qt::AlignCenter);

    connect(m_restart, &QPushButton::clicked,
            this, &EndingPanel::restartRequested);
}

void EndingPanel::showEnding(EndingJudge::EndingType type)
{
    m_title->setText(EndingJudge::title(type));
    m_desc->setText(EndingJudge::description(type));

    show();
    raise();
}
