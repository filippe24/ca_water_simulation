#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

    void addParticles(float x, float y, float z);
    void clearParticles();
private:

    //position movement
    float unit_pos = 0.1f;
    float unit_vel = 10.0f;

private slots:
	void on_action_Quit_triggered();
	void on_action_Open_triggered();


    void on_pushButton_clicked();

    void on_restart_clicked();

    void on_stopButton_clicked();

    void on_modeSpin_valueChanged(int arg1);

private:
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
