#include "core/MapData.h"
#include "core/GameState.h"

int MapData::tileSize()
{
    return 40;
}

QList<MapConfig> MapData::allMaps()
{
    return {
        map(SceneId::Classroom),
        map(SceneId::Hallway),
        map(SceneId::Lab),
        map(SceneId::Exit)
    };
}

MapConfig MapData::map(const QString &sceneId)
{
    MapConfig cfg;
    const int ts = tileSize();
    Q_UNUSED(ts)

    if (sceneId == SceneId::Classroom) {
        cfg.id = SceneId::Classroom;
        cfg.displayName = QStringLiteral("实训教室");
        cfg.tiles = {
            "########################",
            "#......................#",
            "#.n......k..............#",
            "#......................#",
            "#..TT....TT....TT......#",
            "#..TT....TT....TT......#",
            "#......................#",
            "#.....................bA",
            "#......CC....CC........#",
            "#......CC....CC........#",
            "#......................#",
            "#......................#",
            "#..TT....TT....TT......#",
            "#..TT....TT....TT......#",
            "#P.....................#",
            "########################"
        };
        cfg.portalTargets['A'] = {SceneId::Hallway, 'a'};
        cfg.npcSpawns = {{GameState::NAIWA, 0, 0, ":/assets/characters/naiwa_stand.jpg"}};
    }
    else if (sceneId == SceneId::Hallway) {
        cfg.id = SceneId::Hallway;
        cfg.displayName = QStringLiteral("走廊");
        cfg.tiles = {
            "########################",
            "#......................#",
            "#...........p..........#",
            "#......................#",
            "#..T..............T....#",
            "#..T..............T....#",
            "#......................#",
            "Ba....................fD",
            "#......................#",
            "#......................#",
            "#..T..............T....#",
            "#..T..............T....#",
            "#......................#",
            "#......................#",
            "#P..........g..........#",
            "############E###########"
        };
        cfg.portalTargets['B'] = {SceneId::Classroom, 'b'};
        cfg.portalTargets['D'] = {SceneId::Lab, 'd'};
        cfg.portalTargets['E'] = {SceneId::Exit, 'e'};
        cfg.npcSpawns = {{GameState::PIG, 0, 0, ":/assets/characters/pig_stand.jpg"}};
    }
    else if (sceneId == SceneId::Lab) {
        cfg.id = SceneId::Lab;
        cfg.displayName = QStringLiteral("计算机实验室");
        cfg.tiles = {
            "########################",
            "#......................#",
            "#......o...........u..#",
            "#......................#",
            "#..CC..CC..CC..CC......#",
            "#..CC..CC..CC..CC......#",
            "#..........m...........#",
            "Hd.....................#",
            "#......................#",
            "#..CC..CC..CC..CC......#",
            "#..CC..CC..CC..CC......#",
            "#......................#",
            "#..TT....TT....TT......#",
            "#..TT....TT....TT......#",
            "#P.....................#",
            "########################"
        };
        cfg.portalTargets['H'] = {SceneId::Hallway, 'f'};
        cfg.npcSpawns = {{GameState::DOUBAO, 0, 0, ":/assets/characters/doubao_full.jpg"}};
    }
    else if (sceneId == SceneId::Exit) {
        cfg.id = SceneId::Exit;
        cfg.displayName = QStringLiteral("实验楼出口");
        cfg.tiles = {
            "############K###########",
            "#...........e..........#",
            "#......................#",
            "#......................#",
            "#..T..............T....#",
            "#..T..............T....#",
            "#......................#",
            "#......................#",
            "#......................#",
            "#..T..............T....#",
            "#..T..............T....#",
            "#......................#",
            "#......................#",
            "#......................#",
            "#P..........S..........#",
            "########################"
        };
        cfg.portalTargets['K'] = {SceneId::Hallway, 'g'};
        cfg.npcSpawns = {};
    }

    return cfg;
}
