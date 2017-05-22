#ifndef FOAMCASE_H
#define FOAMCASE_H

#include <QChar>
#include <QFile>
#include <QHash>
#include <QList>
#include <QObject>
#include <QRegExp>
#include <QTextStream>

#include <QDir>
#include <QDirIterator>
#include <QTemporaryDir>

#include <QTimer>
#include <QThread>

#include "workspace_global.h"

namespace VietnameseSpeechRecognition {

class WORKSPACESHARED_EXPORT WorkCase : public QObject
{
    Q_OBJECT
public:
    static WorkCase* currentCase();

    WorkCase();
    ~WorkCase();

    void open(QString path);

    QString path() const;
    void setPath(const QString &path);

    QString getWorkspace() const;
    void setWorkspace(const QString &workspace);

public slots:
    void cmdClean(QStringList);

protected:
    static WorkCase* _currentCase;
    static bool _initialized;

private:
    QString _path;

    QString _workspace;
    QTemporaryDir _tempDir;

};

}

#endif // FOAMCASE_H
