
#ifndef DIALOGUEWIDGET_H
#define DIALOGUEWIDGET_H

#include <QWidget>

class QLabel;
class QPushButton;
class QVBoxLayout;

#include "dialogue/DialogueData.h"

class DialogueWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DialogueWidget(QWidget *parent = nullptr);

    void setNode(const DialogueNode &node,
                 int affinity);

signals:
    void advanceRequested();
    void choiceSelected(int index);

private:
    QLabel *m_avatar;
    QLabel *m_name;
    QLabel *m_affinity;
    QPushButton *m_textButton;

    QVBoxLayout *m_choiceLayout;
};

#endif
