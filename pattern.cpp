#include "pattern.h"
#include "ui_pattern.h"

Pattern::Pattern(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Pattern)
{
    ui->setupUi(this);
    this->setWindowTitle("选择您想玩的模式");
    connect(ui->b4,&QPushButton::pressed,
            [=]
    {
        emit mySignal();
    }
            );
    connect(ui->b1,&QPushButton::pressed,
    [=]()
    {
        c.show();
        this->hide();
        c.ChessBoard::personvscomputer(0);
    }

           );
    connect(ui->b2,&QPushButton::pressed,
    [=]()
    {
        c.show();
        this->hide();
        c.ChessBoard::personvscomputer(1);
    }

           );
    connect(ui->b3,&QPushButton::pressed,
    [=]()
    {
        c.show();
        this->hide();
        c.ChessBoard::personvscomputer(2);
    }

           );
    connect(&c,&ChessBoard::backsignal,
            [=]()
    {
        c.hide();
        this->show();
    }

            );

}

Pattern::~Pattern()
{
    delete ui;
}
