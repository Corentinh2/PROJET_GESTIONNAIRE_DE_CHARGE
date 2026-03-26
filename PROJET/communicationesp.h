#ifndef COMMUNICATIONESP_H
#define COMMUNICATIONESP_H

#include <QObject>

class CommunicationESP : public QObject
{
    Q_OBJECT
public:
    explicit CommunicationESP(QObject *parent = nullptr);

signals:

};

#endif // COMMUNICATIONESP_H
