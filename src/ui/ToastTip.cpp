#include "ui/ToastTip.h"

#include <QLabel>
#include <QTimer>
#include <QHBoxLayout>
#include <QtGlobal>

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
    m_label->setWordWrap(true);
    layout->addWidget(m_label);

    m_timer = new QTimer(this);
    m_timer->setSingleShot(true);
    connect(m_timer, &QTimer::timeout, this, &ToastTip::hide);

    hide();
}

void ToastTip::showMessage(const QString &text, int durationMs)
{
    m_label->setText(text);

    // 先按不限宽量一次自然宽度：短提示按内容自适应；
    // 长文本超过 maxW 才限宽自动换行，避免单行飘条顶出界面。
    const int maxW = 520;
    m_label->setMaximumWidth(QWIDGETSIZE_MAX);
    m_label->adjustSize();
    const int naturalW = m_label->sizeHint().width();

    int w, h;
    if (naturalW > maxW) {
        m_label->setMaximumWidth(maxW);
        m_label->adjustSize();
        w = maxW + 40;
        h = m_label->sizeHint().height() + 20;
    } else {
        w = naturalW + 40;
        h = m_label->sizeHint().height() + 20;
    }
    setFixedSize(w, h);

    show();
    raise();

    m_timer->start(durationMs);
}
