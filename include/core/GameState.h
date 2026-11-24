#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <QObject>
#include <QMap>
#include <QSet>
#include <QStringList>

/**
 * @brief 全局共享游戏状态（单例）。
 *
 * 集中保存跨场景持久化的数据：NPC 好感度、剧情标志、背包道具、
 * 隐藏线索与系统修复状态。成员 B（好感度/对话）与成员 C（道具/谜题/结局）
 * 统一通过本类读写，避免数据散落在各个场景中。
 */
class GameState : public QObject
{
    Q_OBJECT

public:
    static GameState *instance();

    /** 三个 NPC 的固定标识，建议全项目统一使用这些键。 */
    static const QString NAIWA;
    static const QString DOUBAO;
    static const QString PIG;

    /** 好感度：读取 / 设置 / 增减（自动夹紧到 0~100）。 */
    int affinity(const QString &npcId) const;
    void setAffinity(const QString &npcId, int value);
    void addAffinity(const QString &npcId, int delta);

    /** 任意布尔型剧情标志，例如 "got_classroom_key"。 */
    bool hasFlag(const QString &flag) const;
    void setFlag(const QString &flag, bool on = true);

    /** 背包道具。 */
    QStringList inventory() const;
    bool hasItem(const QString &itemId) const;
    void addItem(const QString &itemId);
    void removeItem(const QString &itemId);

    /** 隐藏实验记录线索。 */
    void addClue(const QString &clueId);
    bool hasClue(const QString &clueId) const;
    int clueCount() const;
    int totalClues() const;
    void setTotalClues(int count);
    bool hasAllClues() const;

    /** 自动实训验收系统是否已被修复。 */
    bool systemRepaired() const;
    void setSystemRepaired(bool repaired);

    /** 恢复到初始状态，供重新开始游戏使用。 */
    void reset();

signals:
    void affinityChanged(const QString &npcId, int value);
    void itemChanged();
    void clueChanged(int count);

private:
    explicit GameState(QObject *parent = nullptr);

    QMap<QString, int> m_affinity;
    QSet<QString> m_flags;
    QStringList m_inventory;
    QSet<QString> m_clues;
    int m_totalClues = 0;
    bool m_systemRepaired = false;
};

#endif // GAMESTATE_H
