#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QStackedWidget;

/**
 * @brief 应用主窗口。
 *
 * 用 QStackedWidget 承载开始界面与各个地图场景，GameManager 通过
 * switchToPage() 在不同页面间切换。窗口尺寸固定为地图设计尺寸。
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    int addPage(QWidget *page);
    void switchToPage(int index);
    int pageCount() const;

private:
    QStackedWidget *m_stack;
};

#endif // MAINWINDOW_H
