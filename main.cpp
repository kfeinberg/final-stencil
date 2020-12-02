#include <QApplication>
#include "mainwindow.h"
#include <iostream>

#include "trees/Turtle.h"
#include "trees/LSystem.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    bool startFullscreen = false;

    w.show();

    if (startFullscreen) {
        // We cannot use w.showFullscreen() here because on Linux that creates the
        // window behind all other windows, so we have to set it to fullscreen after
        // it has been shown.
        w.setWindowState(w.windowState() | Qt::WindowFullScreen);
    }

    // example use of LSystem class
    // source: https://github.com/abiusx/L3D/blob/master/L%2B%2B/tree.l%2B%2B
    std::map<char, std::string> rules;
    rules['A'] = "^F>(30)B\\B\\\\\B";
    rules['B'] = "[^^FL\\\\\\AL]";
    rules['L'] = "[^(60)[*(.3)]+(50)*(.28)]";

    LSystem l = LSystem(rules, "FA");

    std::string res = l.applyRules(3);
    Turtle t = Turtle();
    t.parseInput(res);

    return app.exec();
}
