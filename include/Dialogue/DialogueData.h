
#ifndef DIALOGUEDATA_H
#define DIALOGUEDATA_H

#include <QString>
#include <QList>
#include <QHash>

struct DialogueChoice
{
    QString text;
    int affinityDelta = 0;
    QString nextNode;
};

struct DialogueNode
{
    QString id;
    QString speaker;
    QString text;
    QList<DialogueChoice> choices;
    QString nextNode;
};

using DialogueScript = QHash<QString, DialogueNode>;

class DialogueData
{
public:
    static QHash<QString, DialogueScript> createScripts();
};

#endif

