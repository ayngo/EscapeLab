#ifndef ENDINGJUDGE_H
#define ENDINGJUDGE_H

#include <QString>

/**
 * @brief 四结局判定器（成员 C，第四阶段）。
 *
 * 集中根据全局 GameState 的好感度 / 线索 / 系统修复结果，
 * 从 TRUE / SPECIAL / GOOD / BAD 中选出一个结局。
 *
 * 说明：本类只做"读状态 -> 选结局"的纯逻辑，不改动成员 B 的对话，
 * 也不改动成员 A 的地图框架。所有阈值集中放在本类顶部常量里，
 * 待与成员 B 核对实际好感可达区间后，只需改这里的数字。
 */
class EndingJudge
{
public:
    /** 四种结局枚举。 */
    enum EndingType {
        TRUE,      // 黎明之前：三人好感都达标 + 集齐全部隐藏线索
        SPECIAL,   // 特别的搭档：某一位 NPC 好感达到专属阈值
        GOOD,      // 成功逃离：修好系统、顺利离开，但没发现全部秘密
        BAD        // 未完成的项目：系统没修好，Build Failed
    };

    /** 根据当前 GameState 判定结局类型。 */
    static EndingType judge();

    /** 结局类型 -> 展示标题（中文）。 */
    static QString title(EndingType type);

    /** 结局类型 -> 展示文案（占位，后续可按大纲润色）。 */
    static QString description(EndingType type);

    // —— 阈值（占位，待与成员 B 对齐实际好感可达区间）——
    // 大纲：TRUE 需三人好感均达标；SPECIAL 需单人好感达专属线。
    // 目前按大纲先取 60 / 80，若 B 的对话一轮加不到则在此调整。
    static constexpr int TRUE_AFFINITY = 60;    // 三人共同达标线
    static constexpr int SPECIAL_AFFINITY = 80; // 单人专属剧情线
};

#endif // ENDINGJUDGE_H
