#include "mymainwindow.h"
#include "ui_mymainwindow.h"

MyMainWindow::MyMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyMainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("六子棋游戏");
    connect(ui->b2,&QPushButton::pressed,this,&MyMainWindow::close);
    connect(ui->b1,&QPushButton::pressed,
    [=]()
    {
        PA.show();
        this->hide();

    }
    );
    connect(&PA,&Pattern::mySignal,
            [=]()
    {
        PA.hide();
        this->show();

    }

            );

}

MyMainWindow::~MyMainWindow()
{
    delete ui;
}
