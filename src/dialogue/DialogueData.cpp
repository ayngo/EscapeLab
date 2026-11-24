
#include "dialogue/DialogueData.h"

namespace
{
DialogueNode makeNode(
    const QString &id,
    const QString &speaker,
    const QString &text,
    const QList<DialogueChoice> &choices = {},
    const QString &next = QString())
{
    DialogueNode node;
    node.id = id;
    node.speaker = speaker;
    node.text = text;
    node.choices = choices;
    node.nextNode = next;
    return node;
}
}

QHash<QString, DialogueScript> DialogueData::createScripts()
{
    QHash<QString, DialogueScript> scripts;

    // =========================
    // 奶蛙
    // =========================

    DialogueScript frog;

    frog["start"] = makeNode(
        "start", "奶蛙",
        "哟！这不是还没下班的大学生吗？怎么还在实验楼里晃悠？",
        {
            {"大佬，求带！", 10, "frog_a"},
            {"你看起来不太聪明。", -10, "frog_b"},
            {"先告诉我这里发生了什么。", 5, "frog_c"}
        });

    frog["frog_a"] = makeNode(
        "frog_a", "奶蛙",
        "哈哈哈哈！有眼光！我就喜欢你这种识时务的人！",
        {}, "frog_key");

    frog["frog_b"] = makeNode(
        "frog_b", "奶蛙",
        "？？？你这是在质疑一只蛙的智慧？算了，讲台附近有张卡片，自己找去吧。",
        {}, "frog_key");

    frog["frog_c"] = makeNode(
        "frog_c", "奶蛙",
        "系统好像出问题了，门禁卡可能在讲台附近。",
        {}, "frog_key");

    frog["frog_key"] = makeNode(
        "frog_key", "奶蛙",
        "你找到门禁卡了吗？讲台附近可是个好地方。",
        {
            {"谢谢你提供的线索！", 10, "frog_end"},
            {"你是不是还隐瞒了什么？", 5, "frog_secret"},
            {"别卖关子了！", -5, "frog_end"}
        });

    frog["frog_secret"] = makeNode(
        "frog_secret", "奶蛙",
        "以前实验楼也出现过异常。听说这和一个自动实训验收程序有关。",
        {}, "frog_end");

    frog["frog_end"] = makeNode(
        "frog_end", "奶蛙",
        "嘿嘿，接下来就靠你自己了。记住，程序出错不一定是代码写错了。",
        {});

    frog["frog_bonus60"] = makeNode(
        "frog_bonus60", "奶蛙",
        "等等！我想起来了，管理员之前提过一个文件：TEST_FINAL_FINAL_REAL.cpp。它可能和自动验收程序有关！",
        {});

    frog["frog_bonus80"] = makeNode(
        "frog_bonus80", "奶蛙",
        "其实我一直在观察系统。每天晚上它都会运行一次验收程序，但这次似乎陷入了死循环。我们一起修复它吧！",
        {});

    scripts["naiwa"] = frog;

    // =========================
    // 豆包
    // =========================

    DialogueScript doubao;

    doubao["start"] = makeNode(
        "start", "豆包",
        "你好！我是实验楼智能助手豆包，很高兴为你服务。请问有什么可以帮助你的？",
        {
            {"你真是太靠谱了！", 10, "doubao_a"},
            {"你这回答和没回答一样。", -10, "doubao_b"},
            {"帮我检查一下系统。", 5, "doubao_c"}
        });

    doubao["doubao_a"] = makeNode(
        "doubao_a", "豆包",
        "感谢认可！系统需要管理员密码才能继续修复，密码可能藏在附近的纸条中。",
        {}, "doubao_password");

    doubao["doubao_b"] = makeNode(
        "doubao_b", "豆包",
        "收到反馈，正在优化回答质量。建议您保持冷静，检查附近的纸条。",
        {}, "doubao_password");

    doubao["doubao_c"] = makeNode(
        "doubao_c", "豆包",
        "正在切换至系统故障诊断模式。检测完成，需要管理员密码才能继续修复。",
        {}, "doubao_password");

    doubao["doubao_password"] = makeNode(
        "doubao_password", "豆包",
        "如果你找到了密码纸条，可以告诉我。我会帮你分析系统错误。",
        {
            {"我们一起分析程序！", 10, "doubao_a2"},
            {"我自己也能修。", -5, "doubao_a2"},
            {"先告诉我错误在哪里。", 5, "doubao_a2"}
        });

    doubao["doubao_a2"] = makeNode(
        "doubao_a2", "豆包",
        "检测到循环条件异常，可能导致程序无法正常退出。建议检查循环终止条件。",
        {}, "doubao_end");

    doubao["doubao_end"] = makeNode(
        "doubao_end", "豆包",
        "分析完成！祝你顺利修复系统。",
        {});

    doubao["doubao_bonus60"] = makeNode(
        "doubao_bonus60", "豆包",
        "我发现了一段隐藏日志。系统昨天进行过一次自动更新，更新后验收程序出现异常，关键词是 AUTO_CHECK。",
        {});

    doubao["doubao_bonus80"] = makeNode(
        "doubao_bonus80", "豆包",
        "我已经恢复部分系统日志。发现验收程序即使任务全部完成，仍然会再次执行，导致系统无法退出。我们一起修改判断逻辑吧！",
        {});

    scripts["doubao"] = doubao;

    // =========================
    // 猪
    // =========================

    DialogueScript pig;

    pig["start"] = makeNode(
        "start", "猪",
        "哼哼……出去干什么？食堂都关门了。",
        {
            {"你说得有道理，先休息一下。", 10, "pig_a"},
            {"我现在没空陪你聊天。", -10, "pig_b"},
            {"你是不是知道什么线索？", 5, "pig_c"}
        });

    pig["pig_a"] = makeNode(
        "pig_a", "猪",
        "哼哼！这就对了！不过我的零食好像不见了……",
        {}, "pig_food");

    pig["pig_b"] = makeNode(
        "pig_b", "猪",
        "哼……那你自己去找出口吧。",
        {}, "pig_food");

    pig["pig_c"] = makeNode(
        "pig_c", "猪",
        "线索？这个我倒是知道一点，不过嘛……我有点饿。",
        {}, "pig_food");

    pig["pig_food"] = makeNode(
        "pig_food", "猪",
        "你找到食物了吗？如果有饼干的话就更好了！",
        {
            {"都给你吃吧！", 15, "pig_end"},
            {"只给你一半。", 10, "pig_end"},
            {"吃完赶紧告诉我线索。", 5, "pig_end"}
        });

    pig["pig_end"] = makeNode(
        "pig_end", "猪",
        "哼哼！大厅柜子里有一份奇怪的实验记录，你可以去看看！",
        {});

    pig["pig_bonus60"] = makeNode(
        "pig_bonus60", "猪",
        "哼哼！我告诉你一个秘密，大厅柜子里有一份实验记录，上面写着自动验收程序存在 Bug。",
        {});

    pig["pig_bonus80"] = makeNode(
        "pig_bonus80", "猪",
        "这是管理员留下的测试记录，里面记录了自动验收程序的测试结果。现在我把它交给你！",
        {});

    scripts["pig"] = pig;

    return scripts;
}
