#include "console.h"

namespace VietnameseSpeechRecognition {

Console console;

Console::Console(QObject *parent) :
    QObject(parent)
{
    this->m_logger = new Logger();
    this->m_dock = NULL;
}

Logger *Console::logger() const
{
    return m_logger;
}

void Console::log(QString msg, unsigned char priority)
{
    switch (priority) {
        case LogListModel::LOG_INFO:
            this->m_logger->info(msg);
            break;

        case LogListModel::LOG_ERROR:
            this->m_logger->error(msg);
            break;

        case LogListModel::LOG_DEBUG:
            this->m_logger->debug(msg);
            break;

        case LogListModel::LOG_WARN:
            this->m_logger->warn(msg);
            break;

        case LogListModel::LOG_SUCCESS:
            this->m_logger->success(msg);
            break;

        case LogListModel::LOG_CMD:
            this->m_logger->cmd(msg);
            break;

        default:
            this->m_logger->log(msg);
            break;
    }
}

void Console::logInfo(QString msg)
{
    this->m_logger->info(msg);
}

void Console::logError(QString msg)
{
    this->m_logger->error(msg);
}

void Console::logWarn(QString msg)
{
    this->m_logger->warn(msg);
}

void Console::logDebug(QString msg)
{
    this->m_logger->debug(msg);
}

void Console::logSuccess(QString msg)
{
    this->m_logger->success(msg);
}

void Console::logExtend(QString msg)
{
    this->m_logger->extend(msg);
}

ConsoleDock *Console::dock()
{
    if (this->m_dock == NULL) {
        this->m_dock = new ConsoleDock();
        this->m_dock->setModel(this->m_logger->getModel());
        this->m_logger->setWidget(this->m_dock->logViewer());

        connect(this->m_logger->getModel(), SIGNAL(rowsInserted(QModelIndex, int, int)), this->m_dock->logViewer(), SLOT(scrollToBottom()));
        connect(this->m_dock, SIGNAL(commandEntered(QString)), this, SLOT(dispatchCommand(QString)));
    }

    return m_dock;
}

//void Console::registerConsoleCommand(QString cmd, QString handler)
//{
//    this->commands[cmd] = handler;
//}

void Console::clear()
{
    this->m_logger->clear();
}

void Console::clearCmd(QStringList)
{
    this->clear();
}

void Console::dispatchCommand(QString cmdline)
{
    if (cmdline.length() == 0) {
        return;
    }

    QStringList args = cmdline.split(' ');
    QString cmd = args.first();
    args.removeFirst();

    if (!commands.contains(cmd)) {
        console.logger()->error(tr("%1: command not found").arg(cmd));
        return;
    }

    console.logger()->cmd(cmdline);
    //AppEvents::getInstance().trigger(commands[cmd], this, Q_ARG(QStringList, args));
}

//void Console::collectConsoleCommands()
//{
//    AppEvents::getInstance().bind("registerConsoleCommand", this, new QVariant("registerConsoleCommand"));

//    DOFI2::Plugin::PluginApi::getInstance()->events()->registerEvent("cmd.console.clear");
//    DOFI2::Plugin::PluginApi::getInstance()->events()->bind("cmd.console.clear", this, new QVariant("clearCmd"));

//    DOFI2::Plugin::PluginApi::getInstance()->events()->bind("onRequestCloseFoamCase", this, new QVariant("onCaseClose"));

//    DOFI2::Plugin::PluginApi::getInstance()->events()->trigger("registerConsoleCommand", this, Q_ARG(QString, "clear"), Q_ARG(QString, "cmd.console.clear"));

//    AppEvents::getInstance().trigger("collectConsoleCommands", this);
//}

}
