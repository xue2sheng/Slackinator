#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <echoclient.h>
#include <curlpp.h>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->label_2->setTextInteractionFlags(Qt::TextSelectableByMouse);
	ui->label_4->setTextInteractionFlags(Qt::TextSelectableByMouse);
	ui->pushButton->setProperty("connect", true);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_pushButton_clicked()
{
	if(ui->pushButton->property("connect").toBool()){

		// Retrieve the Websocket Data
		QString token = ui->lineEdit_2->text();
		CURLpp handler = CURLpp::Builder()
				.set_connect_timeout(2000)
				.set_url("https://slack.com/api/rtm.start?token=" + token.toStdString())
				.set_verbose(1)
				.build();

		auto res = handler.performJson();

		// Retrieve channel/group data
		for(Json::ValueIterator i_chann = res["channels"].begin(); i_chann != res["channels"].end(); ++i_chann) {
			auto o_channel = *i_chann;
			instertChannel(o_channel);
		}

		for(Json::ValueIterator i_group = res["groups"].begin(); i_group != res["groups"].end(); ++i_group) {
			auto o_group = *i_group;
			instertChannel(o_group);
		}

		// Initiate the connection with the retrieved URL
		QString url = QString::fromStdString(res["url"].asString());
		emit connectButtonPressed(url);

		// Update the UI
		ui->lineEdit_2->setEnabled(false);
		ui->pushButton->setText("disconnect");
		ui->pushButton->setProperty("connect", false);
	}else{
		// Disconnect the WebSocket
		emit disconnectButtonPressed();

		// Update the UI
		ui->lineEdit_2->setEnabled(true);
		ui->pushButton->setText("connect");
		ui->pushButton->setProperty("connect", true);
	}
}

void MainWindow::appendMsgOnMonitor(QString msg)
{
	// Append the raw data in the monitor tab
	QString prev = ui->label_2->text();
	ui->label_2->setText(prev + "\n" + msg);
}

void MainWindow::setA(QApplication *value)
{
	a = value;
}

bool MainWindow::getDebug() const
{
	return debug;
}

void MainWindow::setDebug(bool value)
{
	debug = value;
}

void MainWindow::on_pushButton_2_clicked()
{
	QString msg = ui->lineEdit->text();
	ui->lineEdit->clear();
	appendMsgOnMonitor("Me: " + msg);
	emit sendMessage(msg, ui->comboBox->currentData().toString());
}

void MainWindow::instertChannel(Json::Value channel)
{
	QString name= QString::fromStdString(channel["name"].asString());
	QString id= QString::fromStdString(channel["id"].asString());
	bool arch = channel["is_archived"].asBool();

	channels.push_back({
				   name,
				   arch,
				   id
			   });

	if(!arch){	// insert only if not archived
		ui->comboBox->addItem(name, id);
	}
}
