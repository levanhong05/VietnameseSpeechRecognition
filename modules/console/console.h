#ifndef CONSOLE_H
#define CONSOLE_H

#include <QMenu>
#include <QObject>

#include "console_global.h"

#include "logger.h"
#include "consoledock.h"

namespace VietnameseSpeechRecognition {

class CONSOLESHARED_EXPORT Console : public QObject
{
    Q_OBJECT
public:
    explicit Console(QObject *parent = 0);

    Logger *logger() const;

    void log(QString msg, unsigned char priority = LogListModel::LOG_LOG);

    void logInfo(QString msg);
    void logError(QString msg);
    void logWarn(QString msg);
    void logDebug(QString msg);
    void logSuccess(QString msg);
    void logExtend(QString msg);

    ConsoleDock* dock();

public slots:
    //void registerConsoleCommand(QString cmd, QString handler);
    void clear();
    void clearCmd(QStringList);

private:
    Logger * m_logger;
    ConsoleDock * m_dock;
    QHash<QString, QString> commands;
    QMenu * toggleConsoleMenu;

private slots:
    void dispatchCommand(QString cmdline);
    //void collectConsoleCommands();
};

extern Console CONSOLESHARED_EXPORT console;

}

#endif // CONSOLE_H
