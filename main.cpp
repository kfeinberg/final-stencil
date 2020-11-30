#include <QApplication>
#include "mainwindow.h"
#include "LSystem.h"
#include <iostream>

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

    // example use of LSystem class; TODO: REMOVE
    std::map<char, std::string> rules;
    rules['a'] = "ab";
    rules['b'] = "a";

    LSystem l = LSystem(rules, "a");

    std::cout << l.applyRules(0) << std::endl;
    std::cout << l.applyRules(1) << std::endl;
    std::cout << l.applyRules(2) << std::endl;
    std::cout << l.applyRules(3) << std::endl;
    std::cout << l.applyRules(4) << std::endl;

    return app.exec();
}
