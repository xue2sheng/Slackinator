#ifndef ECHOCLIENT_H
#define ECHOCLIENT_H

#include <QObject>
#include <QtWebSockets/QWebSocket>

class EchoClient : public QObject
{
	Q_OBJECT
public:
	explicit EchoClient(bool debug = false, QObject *parent = Q_NULLPTR);
	void fakeMessage(QString message);

public Q_SLOTS:
	void open(QString url);
	void close();
	void sendMessage(QString msg, QString channel);

signals:
	void closed();
	void msgReceived(QString);
	void okConnected();
	void okDisconnected();

private Q_SLOTS:
	void onConnected();
	void onDisconnected();
	void onTextMessageReceived(QString message);

private:
	QWebSocket m_webSocket;
	QUrl m_url;
	bool m_debug;
};

#endif // ECHOCLIENT_H
