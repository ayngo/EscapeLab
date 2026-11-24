#include "core/GameState.h"

#include <algorithm>

const QString GameState::NAIWA = QStringLiteral("naiwa");
const QString GameState::DOUBAO = QStringLiteral("doubao");
const QString GameState::PIG = QStringLiteral("pig");

GameState *GameState::instance()
{
    static GameState s_instance;
    return &s_instance;
}

GameState::GameState(QObject *parent)
    : QObject(parent)
{
    m_affinity[NAIWA] = 0;
    m_affinity[DOUBAO] = 0;
    m_affinity[PIG] = 0;
}

int GameState::affinity(const QString &npcId) const
{
    return m_affinity.value(npcId, 0);
}

void GameState::setAffinity(const QString &npcId, int value)
{
    const int clamped = std::clamp(value, 0, 100);
    if (m_affinity.value(npcId, 0) == clamped)
        return;
    m_affinity[npcId] = clamped;
    emit affinityChanged(npcId, clamped);
}

void GameState::addAffinity(const QString &npcId, int delta)
{
    setAffinity(npcId, affinity(npcId) + delta);
}

bool GameState::hasFlag(const QString &flag) const
{
    return m_flags.contains(flag);
}

void GameState::setFlag(const QString &flag, bool on)
{
    if (on)
        m_flags.insert(flag);
    else
        m_flags.remove(flag);
}

QStringList GameState::inventory() const
{
    return m_inventory;
}

bool GameState::hasItem(const QString &itemId) const
{
    return m_inventory.contains(itemId);
}

void GameState::addItem(const QString &itemId)
{
    if (m_inventory.contains(itemId))
        return;
    m_inventory.append(itemId);
    emit itemChanged();
}

void GameState::removeItem(const QString &itemId)
{
    if (m_inventory.removeAll(itemId) > 0)
        emit itemChanged();
}

void GameState::addClue(const QString &clueId)
{
    if (m_clues.contains(clueId))
        return;
    m_clues.insert(clueId);
    emit clueChanged(m_clues.size());
}

bool GameState::hasClue(const QString &clueId) const
{
    return m_clues.contains(clueId);
}

int GameState::clueCount() const
{
    return m_clues.size();
}

int GameState::totalClues() const
{
    return m_totalClues;
}

void GameState::setTotalClues(int count)
{
    m_totalClues = count;
}

bool GameState::hasAllClues() const
{
    return m_totalClues > 0 && m_clues.size() >= m_totalClues;
}

bool GameState::systemRepaired() const
{
    return m_systemRepaired;
}

void GameState::setSystemRepaired(bool repaired)
{
    m_systemRepaired = repaired;
}

void GameState::reset()
{
    m_affinity[NAIWA] = 0;
    m_affinity[DOUBAO] = 0;
    m_affinity[PIG] = 0;
    m_flags.clear();
    m_inventory.clear();
    m_clues.clear();
    m_totalClues = 0;
    m_systemRepaired = false;

    emit affinityChanged(NAIWA, 0);
    emit affinityChanged(DOUBAO, 0);
    emit affinityChanged(PIG, 0);
    emit itemChanged();
    emit clueChanged(0);
}
