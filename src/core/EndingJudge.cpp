#include "core/EndingJudge.h"

#include "core/GameState.h"

EndingJudge::EndingType EndingJudge::judge()
{
    GameState *state = GameState::instance();

    // 优先级 1：系统没修好 -> BAD（提交失败，最优先）
    if (!state->systemRepaired())
        return BAD;

    // 优先级 2：单人好感达到专属线 -> SPECIAL
    const int doubao = state->affinity(GameState::DOUBAO);
    const int naiwa = state->affinity(GameState::NAIWA);
    const int pig = state->affinity(GameState::PIG);

    if (doubao >= SPECIAL_AFFINITY
        || naiwa >= SPECIAL_AFFINITY
        || pig >= SPECIAL_AFFINITY)
        return SPECIAL;

    // 优先级 3：三人好感都达标 + 集齐全部隐藏线索 -> TRUE
    const bool allAffinity =
        doubao >= TRUE_AFFINITY
        && naiwa >= TRUE_AFFINITY
        && pig >= TRUE_AFFINITY;

    if (allAffinity && state->hasAllClues())
        return TRUE;

    // 其余：修好系统但没集齐全部秘密 -> GOOD
    return GOOD;
}

QString EndingJudge::title(EndingType type)
{
    switch (type) {
    case TRUE:
        return QStringLiteral("TRUE · 黎明之前");
    case SPECIAL:
        return QStringLiteral("SPECIAL · 特别的搭档");
    case GOOD:
        return QStringLiteral("GOOD · 成功逃离");
    case BAD:
        return QStringLiteral("BAD · 未完成的项目");
    }

    return QStringLiteral("");
}

QString EndingJudge::description(EndingType type)
{
    switch (type) {
    case TRUE:
        return QStringLiteral("三位伙伴的信任都已建立，你集齐了全部隐藏线索，"
                             "和大家一起修复了那套陷入死循环的自动验收程序。"
                             "天亮之前，实验楼的封锁终于解除。");
    case SPECIAL:
        return QStringLiteral("你与其中一位伙伴建立了深厚的信任，解锁了专属剧情。"
                             "在 TA 的帮助下，你顺利离开了实验楼。");
    case GOOD:
        return QStringLiteral("你修复了系统，顺利离开了实验楼。"
                             "虽然没有发现全部秘密，但你成功完成了这次逃离。");
    case BAD:
        return QStringLiteral("系统没有被修复，自动验收程序判定项目未完成。"
                             "Build Failed，实验楼依旧锁着。");
    }

    return QStringLiteral("");
}
