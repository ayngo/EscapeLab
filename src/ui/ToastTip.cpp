#include "ui/ToastTip.h"

#include <QLabel>
#include <QTimer>
#include <QHBoxLayout>

ToastTip::ToastTip(QWidget *parent)
    : QWidget(parent)
{
    setObjectName("ToastTip");
    setAttribute(Qt::WA_StyledBackground, true);

    setStyleSheet(
        "#ToastTip {"
        "  background-color: rgba(15, 24, 42, 235);"
        "  border: 2px solid #F5D58A;"
        "  border-radius: 10px;"
        "}"
        "QLabel { color: #F5D58A; font-size: 15px; font-weight: bold; }"
        );

    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(18, 10, 18, 10);

    m_label = new QLabel("", this);
    m_label->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_label);

    m_timer = new QTimer(this);
    m_timer->setSingleShot(true);
    connect(m_timer, &QTimer::timeout, this, &ToastTip::hide);

    hide();
}

void ToastTip::showMessage(const QString &text, int durationMs)
{
    m_label->setText(text);
    m_label->adjustSize();

    // 按文字宽度自适应，整体居中由外部 setGeometry 控制
    const int w = m_label->sizeHint().width() + 40;
    const int h = m_label->sizeHint().height() + 20;
    setFixedSize(w, h);

    show();
    raise();

    m_timer->start(durationMs);
}
