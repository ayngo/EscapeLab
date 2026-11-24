#ifndef MAPDATA_H
#define MAPDATA_H

#include <QChar>
#include <QList>
#include <QMap>
#include <QPoint>
#include <QString>
#include <QStringList>

/** 场景标识常量，全项目统一使用。 */
namespace SceneId
{
    inline const QString Classroom = QStringLiteral("classroom");
    inline const QString Hallway = QStringLiteral("hallway");
    inline const QString Lab = QStringLiteral("lab");
    inline const QString Exit = QStringLiteral("exit");
}

/** 一个 NPC 在地图上的生成信息。 */
struct NpcSpawn
{
    QString npcId;        // GameState 中使用的 NPC 标识
    int col = 0;          // 网格列
    int row = 0;          // 网格行
    QString spritePath;   // 精灵图（qrc 路径），可留空使用占位绘制
};

/** 一扇门的传送目标。 */
struct PortalTarget
{
    QString targetScene;  // 目标场景标识
    QChar spawnChar;      // 目标场景中的落点字符
};

/**
 * @brief 单个场景的完整配置。
 *
 * 地图由等长字符串组成，每个字符对应一个网格，约定：
 * '#' 墙 | '.' 地板 | 'P' 默认出生点 | 'T' 桌柜 | 'C' 电脑台
 * 大写字母 A/B/D... 为门（见 portalTargets）
 * 小写字母 a/b/d... 为命名落点（门传送的到达位置，按地板处理）
 * 'n' 'd' 'p' 为 NPC 位置（具体身份见 npcSpawns）
 * 'S' 为出口终点（结局判定点，成员 C 实现）
 */
struct MapConfig
{
    QString id;
    QString displayName;
    QStringList tiles;
    QMap<QChar, PortalTarget> portalTargets;
    QList<NpcSpawn> npcSpawns;
};

/** 提供全部场景的地图配置。 */
namespace MapData
{
    int tileSize();
    QList<MapConfig> allMaps();
    MapConfig map(const QString &sceneId);
}

#endif // MAPDATA_H
