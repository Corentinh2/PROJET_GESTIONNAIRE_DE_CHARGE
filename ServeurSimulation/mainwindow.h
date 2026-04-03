#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWebSockets/QWebSocketServer>
#include <QtWebSockets/QWebSocket>
#include <QList>
#include <QMap>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onNewConnection();
    void processTextMessage(const QString &message);
    void socketDisconnected();

private:
    Ui::MainWindow *ui;
    QWebSocketServer *m_pWebSocketServer;
    QList<QWebSocket *> m_clients;

    // Identification des clients : socket → "raspi" ou "esp"
    QMap<QWebSocket *, QString> m_clientRoles;
};

#endif // MAINWINDOW_H
