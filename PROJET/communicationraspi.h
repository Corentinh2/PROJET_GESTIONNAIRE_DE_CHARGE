#ifndef COMMUNICATIONRASPI_H
#define COMMUNICATIONRASPI_H

#include <QObject>

class CommunicationRaspi : public QObject
{
    Q_OBJECT
public:
    explicit CommunicationRaspi(QObject *parent = nullptr);

signals:

};

#endif // COMMUNICATIONRASPI_H
