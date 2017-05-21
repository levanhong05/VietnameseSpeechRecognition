#ifndef DICTIONARYCREATOR_H
#define DICTIONARYCREATOR_H

#include <QMutex>
#include <QObject>
#include <QThread>

class DictionaryCreator : public QThread
{
    Q_OBJECT
public:
    explicit DictionaryCreator(QObject *parent = 0);
    ~DictionaryCreator();

    void execute(QString path);

    void abort();

private:
    void create();

protected:
    void run();

private:
    QString _path;

    QMutex mutex;

    bool m_abort;
    bool m_working;

};

#endif // DICTIONARYCREATOR_H
