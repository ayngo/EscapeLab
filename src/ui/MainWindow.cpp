#include "ui/MainWindow.h"

#include <QStackedWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_stack(new QStackedWidget(this))
{
    setCentralWidget(m_stack);
    setWindowTitle(QStringLiteral("逃离实验楼 EscapeLab"));
    resize(960, 640);
    setMinimumSize(960, 640);
}

int MainWindow::addPage(QWidget *page)
{
    return m_stack->addWidget(page);
}

void MainWindow::switchToPage(int index)
{
    m_stack->setCurrentIndex(index);
}

int MainWindow::pageCount() const
{
    return m_stack->count();
}
