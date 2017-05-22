#include "workcase.h"
#include "console.h"

#include <QHash>
#include <QThread>
#include <QObject>
#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>

#include <QDebug>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

using namespace VietnameseSpeechRecognition;

bool WorkCase::_initialized = false;

WorkCase *WorkCase::_currentCase = NULL;

WorkCase *WorkCase::currentCase()
{
    if (_currentCase == NULL) {
        WorkCase::_currentCase = new WorkCase();
    }

    return WorkCase::_currentCase;
}

WorkCase::WorkCase()
{
    if (!WorkCase::_initialized) {
          WorkCase::_initialized = true;
    }

    this->_workspace = this->_tempDir.path();
}

WorkCase::~WorkCase()
{
    this->cmdClean(QStringList());
}

void WorkCase::open(QString path)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

    this->cmdClean(QStringList());
    QTemporaryDir* tmpDir = new QTemporaryDir();
    this->_workspace = tmpDir->path();

    path = _path;
    path = path.replace("\\", "/");

    console.logInfo(tr("Opening case %1").arg(path.mid(path.lastIndexOf("/") + 1)));

    QApplication::restoreOverrideCursor();
}

void WorkCase::cmdClean(QStringList)
{
    QDir(this->getWorkspace()).removeRecursively();
}

QString WorkCase::getWorkspace() const
{
    return _workspace;
}

void WorkCase::setWorkspace(const QString &workspace)
{
    _workspace = workspace;
}

QString WorkCase::path() const
{
    return _path;
}

void WorkCase::setPath(const QString &path)
{
    _path = path;
}
