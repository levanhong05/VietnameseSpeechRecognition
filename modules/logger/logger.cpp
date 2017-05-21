#include "logger.h"

#include "log4cpp/Layout.hh"
#include "log4cpp/Category.hh"
#include "log4cpp/Appender.hh"
#include "log4cpp/Priority.hh"
#include "log4cpp/BasicLayout.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/OstreamAppender.hh"

using namespace VietnameseSpeechRecognition;

Logger::Logger(QObject *parent) :
    QObject(parent)
{
    this->m_model = new LogListModel();
    this->l4cAppender = new log4cpp::FileAppender("default", m_model->tempFilePath().toStdString());
    this->l4cCategory = &log4cpp::Category::getRoot();
}

void Logger::log(QString _log)
{
    QDateTime datetime = QDateTime::currentDateTime();
    this->m_model->appendLog(_log, LogListModel::LOG_LOG, datetime);
    ((log4cpp::Category *)this->l4cCategory)->log(log4cpp::Priority::NOTSET, _log.toUtf8());
}

void Logger::info(QString _log)
{
    QDateTime datetime = QDateTime::currentDateTime();
    this->m_model->appendLog(_log, LogListModel::LOG_INFO, datetime);
    ((log4cpp::Category *)this->l4cCategory)->info(_log.toUtf8());
}

void Logger::warn(QString _log)
{
    QDateTime datetime = QDateTime::currentDateTime();
    this->m_model->appendLog(_log, LogListModel::LOG_WARN, datetime);
    ((log4cpp::Category *)this->l4cCategory)->warn(_log.toUtf8());
    this->m_widget->scrollToBottom();
}

void Logger::error(QString _log)
{
    QDateTime datetime = QDateTime::currentDateTime();
    this->m_model->appendLog(_log, LogListModel::LOG_ERROR, datetime);
    ((log4cpp::Category *)this->l4cCategory)->error(_log.toUtf8());
}

void Logger::debug(QString _log)
{
#ifdef QT_DEBUG
    QDateTime datetime = QDateTime::currentDateTime();
    this->m_model->appendLog(_log, LogListModel::LOG_DEBUG, datetime);
#endif
    ((log4cpp::Category *)this->l4cCategory)->debug(_log.toUtf8());
}

void Logger::success(QString _log)
{
    QDateTime datetime = QDateTime::currentDateTime();
    this->m_model->appendLog(_log, LogListModel::LOG_SUCCESS, datetime);
    ((log4cpp::Category *)this->l4cCategory)->info(_log.toUtf8());
}

void Logger::extend(QString _log)
{
    QDateTime datetime = QDateTime::currentDateTime();
    this->m_model->appendLog(_log, LogListModel::LOG_EXT, datetime);
    ((log4cpp::Category *)this->l4cCategory)->info(_log.toUtf8());
}

void Logger::cmd(QString _log)
{
    QDateTime datetime = QDateTime::currentDateTime();
    this->m_model->appendLog(_log, LogListModel::LOG_CMD, datetime);
    ((log4cpp::Category *)this->l4cCategory)->log(log4cpp::Priority::NOTSET, _log.toUtf8());
}

void Logger::title(QString _log)
{
    QDateTime datetime = QDateTime::currentDateTime();
    this->m_model->appendLog(_log, LogListModel::LOG_TITLE, datetime);
    ((log4cpp::Category *)this->l4cCategory)->log(log4cpp::Priority::NOTSET, _log.toUtf8());
}

bool Logger::saveLog(QString fileName)
{
    return false;
}

QTextStream *Logger::getStream()
{
    return NULL;
}

LogListModel *Logger::getModel()
{
    return m_model;
}

QListView *Logger::widget() const
{
    return m_widget;
}

void Logger::setWidget(QListView *widget)
{
    m_widget = widget;
    m_widget->setModel(this->m_model);
}

void Logger::clear()
{
    this->m_model->clear();
}
