
#ifndef DIALOGUEMANAGER_H
#define DIALOGUEMANAGER_H

#include <QObject>
#include <QHash>
#include "dialogue/DialogueData.h"

class DialogueManager : public QObject
{
    Q_OBJECT

public:
    explicit DialogueManager(QObject *parent = nullptr);

    bool start(const QString &npcId);

    const DialogueNode *currentNode() const;

    bool isActive() const;

    void advance();

    void choose(int index);

signals:
    void dialogueUpdated();
    void dialogueFinished();

private:
    void enterNode(const QString &nodeId);
    void checkBonusContent();

    QString m_npcId;
    QString m_currentNodeId;

    QHash<QString, DialogueScript> m_scripts;

    bool m_active = false;
};

#endif

