#ifndef STARTSCENE_H
#define STARTSCENE_H

#include <QWidget>

class QPushButton;

/**
 * @brief 游戏开始界面。
 *
 * 展示标题、实验楼背景与主菜单（开始游戏 / 操作说明 / 退出）。
 * 点击开始游戏后发出 startGame()，由 GameManager 切入初始地图。
 */
class StartScene : public QWidget
{
    Q_OBJECT

public:
    explicit StartScene(QWidget *parent = nullptr);

signals:
    void startGame();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void showHelp() const;

private:
    QPushButton *m_startBtn;
    QPushButton *m_helpBtn;
    QPushButton *m_exitBtn;
};

#endif // STARTSCENE_H
