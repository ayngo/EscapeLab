

#include "dialogue/DialogueWidget.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QFont>
#include <QPixmap>
#include <QDebug>

DialogueWidget::DialogueWidget(QWidget *parent)
    : QWidget(parent)
{
    setObjectName("DialogueWidget");

    setAttribute(Qt::WA_StyledBackground, true);

    setStyleSheet(R"(
        #DialogueWidget {
            background-color: rgba(15, 24, 42, 245);
            border: 2px solid #91A9C8;
            border-radius: 12px;
        }

        QLabel {
            color: white;
        }

        QPushButton {
            color: white;
            background-color: #344B68;
            border: 1px solid #7189A6;
            border-radius: 7px;
            padding: 9px;
            text-align: left;
        }

        QPushButton:hover {
            background-color: #536F92;
        }

        QPushButton:disabled {
            color: white;
            background-color: transparent;
            border: none;
        }
    )");

    auto *mainLayout = new QVBoxLayout(this);

    mainLayout->setContentsMargins(14, 10, 14, 10);
    mainLayout->setSpacing(8);

    // 上半部分：头像 + 对话信息
    auto *topLayout = new QHBoxLayout();

    m_avatar = new QLabel(this);
    m_avatar->setFixedSize(110, 140);
    m_avatar->setAlignment(Qt::AlignCenter);
    m_avatar->setScaledContents(false);
    m_avatar->setStyleSheet(
        "background-color: transparent;"
        "border-radius: 10px;"
        );

    auto *rightLayout = new QVBoxLayout();

    m_name = new QLabel("奶蛙", this);
    m_name->setStyleSheet(
        "font-size: 18px;"
        "font-weight: bold;"
        "color: #F5D58A;"
        );

    m_affinity = new QLabel("好感度：30 / 100", this);

    m_textButton = new QPushButton(this);
    m_textButton->setMinimumHeight(65);
    m_textButton->setStyleSheet(
        "font-size: 15px;"
        "text-align: left;"
        );

    m_textButton->setCursor(Qt::PointingHandCursor);

    connect(m_textButton,
            &QPushButton::clicked,
            this,
            &DialogueWidget::advanceRequested);

    rightLayout->addWidget(m_name);
    rightLayout->addWidget(m_affinity);
    rightLayout->addWidget(m_textButton, 1);

    topLayout->addWidget(m_avatar);
    topLayout->addLayout(rightLayout, 1);

    mainLayout->addLayout(topLayout);

    // 下半部分：选项
    m_choiceLayout = new QVBoxLayout();
    m_choiceLayout->setSpacing(5);

    mainLayout->addLayout(m_choiceLayout);

    setMinimumHeight(280);

    // 默认显示奶蛙图片
    QPixmap pixmap(":/assets/characters/naiwa_think.png");

    if (!pixmap.isNull()) {
        m_avatar->setPixmap(
            pixmap.scaled(
                m_avatar->size(),
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation
                )
            );
    }
    else {
        qDebug() << "奶蛙头像加载失败！";
    }
}

void DialogueWidget::setNode(
    const DialogueNode &node,
    int affinity)
{
    m_name->setText(node.speaker);

    m_affinity->setText(
        QString("好感度：%1 / 100").arg(affinity)
        );

    m_textButton->setText(
        node.text + "\n\n（点击这里继续）"
        );

    // 根据说话角色切换对话框头像
    QString avatarPath;

    if (node.speaker == "奶蛙") {
        avatarPath = ":/assets/characters/naiwa_think.png";
    }
    else if (node.speaker == "豆包") {
        avatarPath = ":/assets/characters/doubao_avatar.jpg";
    }
    else if (node.speaker == "猪") {
        avatarPath = ":/assets/characters/pig_lie.jpg";
    }

    if (!avatarPath.isEmpty()) {
        QPixmap pixmap(avatarPath);

        if (!pixmap.isNull()) {
            m_avatar->setPixmap(
                pixmap.scaled(
                    m_avatar->size(),
                    Qt::KeepAspectRatio,
                    Qt::SmoothTransformation
                    )
                );
        }
        else {
            qDebug() << "NPC头像加载失败:" << avatarPath;
            m_avatar->clear();
        }
    }

    // 删除上一段对话的选项按钮
    while (QLayoutItem *item =
           m_choiceLayout->takeAt(0)) {

        if (QWidget *widget = item->widget())
            widget->deleteLater();

        delete item;
    }

    const bool hasChoices = !node.choices.isEmpty();

    m_textButton->setEnabled(!hasChoices);

    // 创建新选项
    for (int i = 0; i < node.choices.size(); ++i) {

        const DialogueChoice &choice =
            node.choices[i];

        auto *button = new QPushButton(
            QString("%1. %2")
                .arg(QChar('A' + i))
                .arg(choice.text),
            this
            );

        button->setMinimumHeight(38);

        connect(button,
                &QPushButton::clicked,
                this,
                [this, i]() {
                    emit choiceSelected(i);
                });

        m_choiceLayout->addWidget(button);
    }
}
