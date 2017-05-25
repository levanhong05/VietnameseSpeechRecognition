#ifndef CASESOURCE_H
#define CASESOURCE_H

#include <QObject>
#include <QDockWidget>

#include "casesourcedock.h"

namespace VietnameseSpeechRecognition {

class CaseSource : public QObject
{
    Q_OBJECT
public slots:
    void appOnload();
    void setupCaseSource();
    void toggleSidebarAction(bool toggled);

    void load(QStringList);
    void appOnMainWindowShow();

private:
    CaseSourceDock *caseSourceDock;
    QAction * sidebarAction;

    bool loaded;

};

}

#endif // CASESOURCE_H
