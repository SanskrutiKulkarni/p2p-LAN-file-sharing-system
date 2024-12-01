#ifndef SINGLEINSTANCE_H
#define SINGLEINSTANCE_H

#include <QObject>
#include <QtNetwork/QLocalServer>

class SingleInstance : public QObject
{
    Q_OBJECT

public:
    SingleInstance(const QString& id, QObject* parent = nullptr);

    QString getLastErrorString() const;
    bool start();
    bool hasPreviousInstance();

Q_SIGNALS:
    void newInstanceCreated();

private Q_SLOTS:
    void onNewConnection();

private:

    QLocalServer mServer;
    QString mName;
};

#endif // SINGLEINSTANCE_H
