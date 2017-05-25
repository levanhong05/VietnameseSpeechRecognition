#include <QtGui>
#include <QVariant>

#include "casesource.h"

using namespace VietnameseSpeechRecognition;

void CaseSource::appOnload()
{
    //this->api->events()->bind("onFoamCaseLoaded", this, new QVariant("setupFoamSource"));
    //this->api->events()->bind("collectConsoleCommands", this, new QVariant("exportCommands"));
}

void CaseSource::setupCaseSource()
{
    if (this->caseSourceDock != 0) {
        //this->api->mainWindow()->removeDockWidget(this->caseSourceDock);
    }

    //this->caseSourceDock = new CaseSourceDock(this->api->mainWindow());
    this->caseSourceDock = new CaseSourceDock();
    //this->api->mainWindow()->addDockWidget(Qt::LeftDockWidgetArea, this->caseSourceDock);
    this->caseSourceDock->hide();
    this->caseSourceDock->setupCaseSourceTree();
}

void CaseSource::toggleSidebarAction(bool toggled)
{
    if (!toggled) {
        this->caseSourceDock->hide();
    } else {
        this->caseSourceDock->show();
    }
}

void CaseSource::load(QStringList)
{
    if (loaded) {
        console.logDebug(tr("CaseSource already loaded"));
        return;
    }

    this->sidebarAction->setVisible(true);
    loaded = true;
}

void CaseSource::appOnMainWindowShow()
{
    this->caseSourceDock = 0;
    this->sidebarAction = new QAction(QIcon(":/numix/actions/stock_notes.svg"),
                                      tr("Source")//,
                                      //this->api->getLeftToolBar()
                                     );

    this->sidebarAction->setCheckable(true);
    this->sidebarAction->setVisible(false);

    //this->api->addSidebarAction(this->sidebarAction);
    this->loaded = false;

    this->setupCaseSource();

    connect(this->sidebarAction, SIGNAL(toggled(bool)), this, SLOT(toggleSidebarAction(bool)));
}
