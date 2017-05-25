#include "sourceeditor.h"
#include "ui_sourceeditor.h"

#include <QFile>
#include <QCloseEvent>
#include <QVBoxLayout>

#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexer.h>
#include <Qsci/qscilexercpp.h>

#include "console.h"

using namespace VietnameseSpeechRecognition;

SourceEditor::SourceEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SourceEditor)
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);

    this->m_qsciWidget = new QsciScintilla(this);

    QVBoxLayout *layout = new QVBoxLayout();
    this->setLayout(layout);
    layout->addWidget(this->m_qsciWidget);

    QsciLexerCPP *cppLexer = new QsciLexerCPP(this->m_qsciWidget);

#ifdef Q_OS_WIN32
    cppLexer->setFont(QFont("Courier New", 10));
#else
    cppLexer->setFont(QFont("Liberation Mono", 10));
#endif

    this->m_qsciWidget->setAutoIndent(true);
    this->m_qsciWidget->setLexer(cppLexer);
    this->m_qsciWidget->setMarginWidth(1, 40);
    this->m_qsciWidget->setMarginLineNumbers(1, true);
}

SourceEditor::~SourceEditor()
{
    delete ui;
}

void SourceEditor::closeEvent(QCloseEvent *event)
{
    emit requestCloseEditor(this);
    event->accept();
}

void SourceEditor::openFile(const QString &filepath)
{
    this->filepath = filepath;
    QFile f(filepath);

    if (!f.open(QIODevice::ReadOnly)) {
        console.logDebug(tr("Error: cannot open file %1").arg(filepath));
        return;
    }

    this->m_qsciWidget->read(&f);

    f.close();
}

void SourceEditor::reloadFileContent()
{
    this->openFile(this->filepath);
}
