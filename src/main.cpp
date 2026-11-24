#include <QApplication>

#include "core/GameManager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName(QStringLiteral("EscapeLab"));

    GameManager game;
    game.show();

    return app.exec();
}
