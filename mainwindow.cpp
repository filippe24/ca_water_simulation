#include <QFileDialog>
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_action_Quit_triggered()
{
	 QApplication::quit();
}


void MainWindow::on_action_Open_triggered()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Open PLY"), ".", tr("*.ply"));

	ui->openGLWidget->loadMesh(filename);
}



void MainWindow::on_pushButton_clicked()
{
    ui->openGLWidget->animateWater();
}

void MainWindow::on_restart_clicked()
{
    ui->openGLWidget->resetAnimation();
}

void MainWindow::on_stopButton_clicked()
{
    bool old = ui->openGLWidget->stop;
    ui->openGLWidget->stop = !old;
}

void MainWindow::on_modeSpin_valueChanged(int arg1)
{
    ui->openGLWidget->selected_mode=(arg1-1);
}
