#ifndef LOGLISTMODEL_H
#define LOGLISTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QStringList>
#include <QDateTime>
#include <QFile>
#include <QModelIndex>

#include "logger_global.h"

namespace VietnameseSpeechRecognition {

typedef struct LogEntry {
    LogEntry(QString content, unsigned char priority, QDateTime datetime);
    QString content;
    unsigned char priority;
    QDateTime datetime;
} LogEntry;

class LOGGERSHARED_EXPORT LogListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    static const unsigned char LOG_LOG = 0;
    static const unsigned char LOG_CMD = 1;
    static const unsigned char LOG_ERROR = 2;
    static const unsigned char LOG_SUCCESS = 3;
    static const unsigned char LOG_INFO = 4;
    static const unsigned char LOG_WARN = 5;
    static const unsigned char LOG_TITLE = 6;
    static const unsigned char LOG_DEBUG = 7;
    static const unsigned char LOG_EXT = 8;
    static const unsigned int MAX_BUFFER_SIZE = 500;

    explicit LogListModel(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QList<LogEntry *> *entries() const;
    void setEntries(QList<LogEntry *> *entries);

    QString tempFilePath() const;
    void setTempFilePath(const QString &tempFilePath);

    QFile *tempFile() const;
    void setTempFile(QFile *tempFile);

    void appendLog(QString _log, unsigned char priority, QDateTime datetime);

    bool prefixVisible() const;
    void setPrefixVisible(bool prefixVisible);

    bool timeVisible() const;
    void setTimeVisible(bool timeVisible);

    bool iconVisible() const;
    void setIconVisible(bool iconVisible);

    bool removeRows(int row, int count, const QModelIndex &parent);

    void clear();

private:
    int fetchedLogCount;

    QList<LogEntry *> *_entries;
    QString _tempFilePath;
    QFile *_tempFile;
    int count;

    bool _prefixVisible;
    bool _timeVisible;
    bool _iconVisible;

};

}
#endif // LOGLISTMODEL_H
