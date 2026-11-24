#include "scenes/StartScene.h"

#include <QApplication>
#include <QMessageBox>
#include <QPainter>
#include <QPushButton>
#include <QVBoxLayout>

StartScene::StartScene(QWidget *parent)
    : QWidget(parent),
      m_startBtn(new QPushButton(QStringLiteral("开始游戏"), this)),
      m_helpBtn(new QPushButton(QStringLiteral("操作说明"), this)),
      m_exitBtn(new QPushButton(QStringLiteral("退出游戏"), this))
{
    setMinimumSize(960, 640);

    const QString btnQss =
        "QPushButton{background:rgba(20,26,40,0.82);color:#e8eefc;"
        "border:1px solid #7f93b8;border-radius:8px;font-size:18px;padding:10px 60px;}"
        "QPushButton:hover{background:rgba(70,110,200,0.9);}";
    for (QPushButton *b : {m_startBtn, m_helpBtn, m_exitBtn})
        b->setStyleSheet(btnQss);

    auto *layout = new QVBoxLayout(this);
    layout->addStretch(5);
    layout->addWidget(m_startBtn, 0, Qt::AlignCenter);
    layout->addSpacing(14);
    layout->addWidget(m_helpBtn, 0, Qt::AlignCenter);
    layout->addSpacing(14);
    layout->addWidget(m_exitBtn, 0, Qt::AlignCenter);
    layout->addStretch(1);

    connect(m_startBtn, &QPushButton::clicked, this, &StartScene::startGame);
    connect(m_helpBtn, &QPushButton::clicked, this, &StartScene::showHelp);
    connect(m_exitBtn, &QPushButton::clicked, qApp, &QApplication::quit);
}

void StartScene::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter p(this);

    QPixmap bg(":/assets/backgrounds/lab_hall_b.jpg");
    if (!bg.isNull()) {
        p.setRenderHint(QPainter::SmoothPixmapTransform);
        p.drawPixmap(rect(), bg.scaled(size(), Qt::KeepAspectRatioByExpanding,
                                       Qt::SmoothTransformation));
    }

    p.fillRect(rect(), QColor(12, 16, 28, 150));

    QFont titleFont = p.font();
    titleFont.setPointSize(44);
    titleFont.setBold(true);
    p.setFont(titleFont);
    p.setPen(QColor(236, 242, 252));
    p.drawText(QRect(0, height() / 2 - 150, width(), 80), Qt::AlignCenter,
               QStringLiteral("逃离实验楼"));

    QFont subFont = p.font();
    subFont.setPointSize(15);
    p.setFont(subFont);
    p.setPen(QColor(178, 196, 224));
    p.drawText(QRect(0, height() / 2 - 70, width(), 34), Qt::AlignCenter,
               QStringLiteral("Escape the Lab · 高级语言编程实训小组作品"));
}

void StartScene::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    update();
}

void StartScene::showHelp() const
{
    QMessageBox box;
    box.setWindowTitle(QStringLiteral("操作说明"));
    box.setText(QStringLiteral(
        "移动：W A S D 或 方向键\n"
        "交互：靠近 NPC 后按 E 键\n"
        "目标：探索房间、收集道具、破解谜题，修复系统并逃离实验楼。\n"
        "绿色门洞可通往其他房间。"));
    box.setIcon(QMessageBox::Information);
    box.exec();
}
