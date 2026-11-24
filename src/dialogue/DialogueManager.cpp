
#include "dialogue/DialogueManager.h"
#include "core/GameState.h"

DialogueManager::DialogueManager(QObject *parent)
    : QObject(parent),
    m_scripts(DialogueData::createScripts())
{
}

bool DialogueManager::start(const QString &npcId)
{
    if (!m_scripts.contains(npcId))
        return false;

    m_npcId = npcId;
    m_active = true;

    enterNode("start");
    return true;
}

const DialogueNode *DialogueManager::currentNode() const
{
    if (!m_active)
        return nullptr;

    const auto script = m_scripts.constFind(m_npcId);

    if (script == m_scripts.cend())
        return nullptr;

    const auto node = script->constFind(m_currentNodeId);

    if (node == script->cend())
        return nullptr;

    return &node.value();
}

bool DialogueManager::isActive() const
{
    return m_active;
}

void DialogueManager::enterNode(const QString &nodeId)
{
    if (!m_active)
        return;

    auto script = m_scripts.find(m_npcId);

    if (script == m_scripts.end()
        || !script->contains(nodeId)) {
        m_active = false;
        emit dialogueFinished();
        return;
    }

    m_currentNodeId = nodeId;

    emit dialogueUpdated();
}

void DialogueManager::choose(int index)
{
    const DialogueNode *node = currentNode();

    if (!node || index < 0
        || index >= node->choices.size())
        return;

    const DialogueChoice choice = node->choices[index];

    GameState *state = GameState::instance();

    // 修改当前 NPC 好感度
    state->addAffinity(m_npcId, choice.affinityDelta);

    // 根据选择跳转到下一个剧情节点
    enterNode(choice.nextNode);
}

void DialogueManager::advance()
{
    const DialogueNode *node = currentNode();

    if (!node)
        return;

    // 当前有选项时，不允许点击对话框跳过选择
    if (!node->choices.isEmpty())
        return;

    // 正常剧情跳转
    if (!node->nextNode.isEmpty()) {
        enterNode(node->nextNode);
        return;
    }

    // 当前剧情结束，检查是否有隐藏剧情
    checkBonusContent();
}

void DialogueManager::checkBonusContent()
{
    GameState *state = GameState::instance();

    const int value = state->affinity(m_npcId);

    const QString flag60 =
        "bonus60_" + m_npcId;

    const QString flag80 =
        "bonus80_" + m_npcId;

    // 60 好感度：隐藏线索
    if (value >= 60 && !state->hasFlag(flag60)) {

        state->setFlag(flag60);

        const QString clue =
            "clue_" + m_npcId + "_60";

        state->addClue(clue);

        enterNode(m_npcId + "_bonus60");
        return;
    }

    // 80 好感度：专属剧情
    if (value >= 80 && !state->hasFlag(flag80)) {

        state->setFlag(flag80);

        const QString clue =
            "clue_" + m_npcId + "_80";

        state->addClue(clue);

        enterNode(m_npcId + "_bonus80");
        return;
    }

    // 没有额外剧情，结束对话
    m_active = false;
    emit dialogueFinished();
}
