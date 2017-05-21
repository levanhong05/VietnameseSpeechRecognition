#include "loglistmodel.h"
#include <QLabel>
#include <QTime>
#include <QDate>
#include <QApplication>
#include <QPixmap>
#include <QBrush>
#include <QColor>

using namespace VietnameseSpeechRecognition;

LogListModel::LogListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    this->count = 0;
    this->_entries = new QList<LogEntry *>();
    this->_prefixVisible = false;
    this->_timeVisible = false;
    this->_iconVisible = true;
}

QVariant LogListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (this->count == 0) {
        return QVariant();
    }

    if (index.row() >= this->entries()->size()) {
        return QVariant();
    }

    LogEntry *log = this->entries()->at(index.row());

    if (this->iconVisible() && role == Qt::DecorationRole) {
        QPixmap *icon ;

        switch (log->priority) {
            case LOG_ERROR:
                icon = new QPixmap(":/logger/images/error.png");
                break;

            case LOG_CMD:
                icon = new QPixmap(":/logger/images/forward.png");
                break;

            case LOG_WARN:
                icon = new QPixmap(":/logger/images/warning.png");
                break;

            case LOG_INFO:
            case LOG_TITLE:
                icon = new QPixmap(":/logger/images/info.png");
                break;

            case LOG_SUCCESS:
                icon = new QPixmap(":/logger/images/success.png");
                break;

            default:
                return QVariant();
                break;
        }

        return icon->scaledToWidth(16);
    }

    if (role == Qt::ForegroundRole) {
        switch (log->priority) {
            case LOG_DEBUG:
                return QBrush(QColor(0, 0, 139));
                break;

            case LOG_TITLE:
                return QBrush(QColor(0, 0, 255));
                break;

            case LOG_SUCCESS:
                return QBrush(QColor(0, 139, 0));
                break;

            case LOG_EXT:
            case LOG_INFO:
                return QBrush(QColor(120, 35, 255));
                break;

            case LOG_WARN:
                return QBrush(QColor(160, 70, 135));
                break;

            case LOG_ERROR:
                return QBrush(QColor(180, 0, 0));
                break;

            case LOG_LOG:
                return QBrush(QColor(40, 40, 40));
                break;

            case LOG_CMD:
                return QBrush(QColor(0, 0, 0));
                break;

            default:
                break;
        }
    }

    if (role == Qt::DisplayRole) {
        QString prefix("");

        switch (log->priority) {
            case LOG_DEBUG:
                prefix = "[DEBUG] ";
                break;

            case LOG_INFO:
                prefix = "[INFO] ";
                break;

            case LOG_WARN:
                prefix = "[WARN] ";
                break;

            case LOG_ERROR:
                prefix = "[ERROR] ";
                break;

            case LOG_TITLE:
                prefix = "[TITLE] ";
                break;

            default:
                break;
        }

        QString logtime = "<" + log->datetime.toString("MM-dd-yyyy hh:mm:ss") + "> ";
        return (this->prefixVisible() ? prefix : "") + (this->timeVisible() ? logtime : "") + log->content;
    }

    return QVariant();
}

int LogListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return this->count;
}

QFile *LogListModel::tempFile() const
{
    return _tempFile;
}

void LogListModel::setTempFile(QFile *tempFile)
{
    _tempFile = tempFile;
}

QString LogListModel::tempFilePath() const
{
    return _tempFilePath;
}

void LogListModel::setTempFilePath(const QString &tempFilePath)
{
    _tempFilePath = tempFilePath;
}

QList<LogEntry *> *LogListModel::entries() const
{
    return _entries;
}

void LogListModel::setEntries(QList<LogEntry *> *entries)
{
    _entries = entries;
}

void LogListModel::appendLog(QString _log, unsigned char priority, QDateTime datetime)
{
    beginInsertRows(this->index(0), 0, this->count);
    this->_entries->append(new LogEntry(_log, priority, datetime));
    this->count++;
    endInsertRows();
}

bool LogListModel::prefixVisible() const
{
    return _prefixVisible;
}

void LogListModel::setPrefixVisible(bool prefixVisible)
{
    _prefixVisible = prefixVisible;
}

bool LogListModel::timeVisible() const
{
    return _timeVisible;
}

void LogListModel::setTimeVisible(bool timeVisible)
{
    _timeVisible = timeVisible;
}

bool LogListModel::iconVisible() const
{
    return _iconVisible;
}

void LogListModel::setIconVisible(bool iconVisible)
{
    _iconVisible = iconVisible;
}

bool LogListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count);

    for (int i = 0; i < count; i++) {
        this->_entries->removeAt(row);
    }

    this->count -= count;
    endRemoveRows();

    return true;
}

void LogListModel::clear()
{
    beginRemoveRows(this->index(0), 0, this->count);
    this->_entries->clear();
    this->count = 0;
    endRemoveRows();
}

LogEntry::LogEntry(QString content, unsigned char priority, QDateTime datetime)
{
    this->content = content;
    this->priority = priority;
    this->datetime = datetime;
}
