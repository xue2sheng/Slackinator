#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class QApplication;
namespace Json {
	class Value;
}

struct channel{
	QString name = "";
	bool isArchived = false;
	QString id = "";
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

signals:
	void connectButtonPressed(QString);
	void disconnectButtonPressed();
	void sendMessage(QString, QString);
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	bool getDebug() const;
	void setDebug(bool value);

	void setA(QApplication *value);

public slots:
	void appendMsgOnMonitor(QString msg);

private slots:
	void on_pushButton_clicked();

	void on_pushButton_2_clicked();

private:
	void instertChannel(Json::Value channel);

	Ui::MainWindow *ui;
	QApplication * a;

	QVector<channel> channels;


	bool debug = false;
};

#endif // MAINWINDOW_H