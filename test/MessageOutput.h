#pragma once

#include <QtCore>

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    auto localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        std::cout << "Debug: " << localMsg.constData() << std::endl;
        break;
    case QtInfoMsg:
        std::cout << "Info: " << localMsg.constData() << std::endl;
        break;
    case QtWarningMsg:
        std::cout << "Warning: " << localMsg.constData() << std::endl;
        break;
    case QtCriticalMsg:
        std::cout << "Critical: " << localMsg.constData() << " " <<
            context.file << " " <<
            context.line << " " <<
            context.function << std::endl;
        break;
    case QtFatalMsg:
        std::cout << "Fatal: " << localMsg.constData() << " " <<
            context.file << " " <<
            context.line << " " <<
            context.function << std::endl;
        abort();
    }
}
