#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "vlcplaywindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_playBtn_clicked();

    void on_stopBtn_clicked();

    void on_pauseBtn_clicked();

    void on_resumeBtn_clicked();
	void setCurrentWndIdx(int currWinIdx);

    void on_recordBtn_clicked();

    void on_stopRecordBtn_clicked();

private:
    Ui::MainWindow *ui;
	void initWindow();

	VlcPlayWindow *m_playWindow[16];
	int m_currWinIdx;
	
};

#endif // MAINWINDOW_H
