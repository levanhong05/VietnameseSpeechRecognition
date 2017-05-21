#ifndef DLOGGER_H
#define DLOGGER_H

#include <QTime>
#include <QMutex>
#include <QObject>
#include <QDateTime>
#include <QListView>
#include <QStringList>
#include <QTextStream>
#include <QTemporaryFile>

#include "logger_global.h"

#include "loglistmodel.h"
#include "loglistwidget.h"

namespace VietnameseSpeechRecognition {

class LOGGERSHARED_EXPORT Logger : public QObject
{
    Q_OBJECT
public:
    explicit Logger(QObject *parent = 0);

    void log(QString _log);
    void info(QString _log);
    void warn(QString _log);
    void error(QString _log);
    void debug(QString _log);
    void success(QString _log);
    void cmd(QString _log);
    void title(QString _log);
    void extend(QString _log);

    bool saveLog(QString fileName);

    QTextStream *getStream();

    LogListModel *getModel();

    QListView *widget() const;
    void setWidget(QListView *widget);

public slots:
    void clear();

private:
    LogListModel *m_model;
    QListView *m_widget;

    void *l4cAppender;
    void *l4cCategory;

};

}

#endif // DLOGGER_H
