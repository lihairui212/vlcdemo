#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Enums.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	m_currWinIdx = 0;
	initWindow();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initWindow()
{
	m_playWindow[0] = ui->playwindow_1;
	m_playWindow[1] = ui->playwindow_2;
	m_playWindow[2] = ui->playwindow_3;
	m_playWindow[3] = ui->playwindow_4;
	m_playWindow[4] = ui->playwindow_5;
	m_playWindow[5] = ui->playwindow_6;
	m_playWindow[6] = ui->playwindow_7;
	m_playWindow[7] = ui->playwindow_8;
	m_playWindow[8] = ui->playwindow_9;
	m_playWindow[9] = ui->playwindow_10;
	m_playWindow[10] = ui->playwindow_11;
	m_playWindow[11] = ui->playwindow_12;
	m_playWindow[12] = ui->playwindow_13;
	m_playWindow[13] = ui->playwindow_14;
	m_playWindow[14] = ui->playwindow_15;
	m_playWindow[15] = ui->playwindow_16;
	
	for (int i = 0; i < 16; ++i)
	{
		m_playWindow[i]->setWindowIndex(i);
		connect(m_playWindow[i], SIGNAL(chooseWindow(int)), this, SLOT(setCurrentWndIdx(int)));
		m_playWindow[i]->setStyleSheet("QWidget{background:#121922;}");
	}
}

void MainWindow::on_playBtn_clicked()
{
    QString rtspUrl = ui->lineEdit->text();
	if (rtspUrl.isEmpty())
	{
		return;
	}
	m_playWindow[m_currWinIdx]->setRtspUrl(rtspUrl);
	m_playWindow[m_currWinIdx]->startPlay();
}

void MainWindow::on_stopBtn_clicked()
{
	m_playWindow[m_currWinIdx]->stopPlay();
}

void MainWindow::on_pauseBtn_clicked()
{

}

void MainWindow::on_resumeBtn_clicked()
{

}

void MainWindow::setCurrentWndIdx(int currWinIdx)
{
	m_currWinIdx = currWinIdx;
}
void MainWindow::on_recordBtn_clicked()
{
	m_playWindow[m_currWinIdx]->record("testrecord",".",Vlc::AVI,true);
}

void MainWindow::on_stopRecordBtn_clicked()
{

}
