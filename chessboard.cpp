#include "chessboard.h"
#include <QPainter>
#include <QPushButton>
#include <QMenuBar>//菜单栏
#include <QMenu>
#include <QAction>
#include <QPen>
#include <QMouseEvent>
#include <QMessageBox>
#include <QDebug>
#include "ui_chessboard.h"
#include <QTimerEvent>
#include <QApplication>
#include <QWidget>
#include <QString>
#include <QTimer>
#include <QSound>
ChessBoard::ChessBoard(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::ChessBoard)
{

    whiteTimes=1200;
    blackTimes=1200;

    ui->setupUi(this);
    ui->centralwidget->setMouseTracking(true);
    setMouseTracking(true);

    this->resize(1800,1200);

    this->move(100,100);

    this->setWindowTitle("六子棋");

    //菜单栏
    QMenuBar *mBar=menuBar();

    //添加菜单
    QMenu *p1= mBar->addMenu("开始");  

    //添加菜单项，添加动作
    QAction *pNew1 = p1->addAction("返回上一级");
    p1->addSeparator();//添加分割线
    QAction *pNew2 = p1->addAction("退出游戏");
    p1->addSeparator();
    QAction *pNew3 = p1->addAction("再来一把");
    p1->addSeparator();
    QAction *pNew4 = p1->addAction("关于");

    connect(pNew1,&QAction::triggered,
            [=]()
    {
        wtimerId->stop();
        btimerId->stop();
        whiteTimes=300;
        blackTimes=300;
        player=0;
        memset(chess,0,22*22*sizeof(int));
        emit backsignal();
    }

    );
    connect(ui->b,&QPushButton::pressed,this,&ChessBoard::regretchess);
    connect(pNew2,&QAction::triggered,this,&ChessBoard::close);
    connect(pNew3,&QAction::triggered,this,&ChessBoard::again);
    connect(pNew4,&QAction::triggered,
            [=]
    {
        QMessageBox::information(this, "作者信息", "南京理工大学王宏睿开发", QMessageBox::Ok);
    });
    player=0;
    memset(chess,0,22*22*sizeof(int));//初始化，棋盘清空

    btimerId = new QTimer(this); //this 为parent类, 表示当前窗口
    wtimerId = new QTimer(this); //this 为parent类, 表示当前窗口
    connect(btimerId,SIGNAL(timeout()),this,SLOT(btimerUpdate()));
    connect(wtimerId,SIGNAL(timeout()),this,SLOT(wtimerUpdate()));
}

void ChessBoard::paintEvent(QPaintEvent *event)
{
     QPainter painter(this);

     int d=50;

     painter.setRenderHint(QPainter::Antialiasing, true); //抗锯齿

     //画背景色
     QPalette palette ;
     palette.setColor(QPalette::Background,QColor("#B1723C"));
     this->setAutoFillBackground(true);
     this->setPalette(palette);

     QBrush brush;
     brush.setStyle(Qt::SolidPattern);
     brush.setColor(QColor("#EEC085"));
     painter.setBrush(brush);
     painter.drawRect(d-5,d-5,20*d+10,20*d+10);

     QPen pen;
     pen.setWidth(2);		//设置线宽
     painter.setPen(pen);

     for(int i=1;i<=21;i++)
     {
         painter.drawLine(QPoint(d,i*d),QPoint(21*d,i*d));
     }//横线绘制

     for(int i=1;i<=21;i++)
     {
         painter.drawLine(QPoint(i*d,d),QPoint(i*d,21*d));
     }//竖线绘制

     painter.drawLine(QPoint(d-5,d-5),QPoint(d-5,21*d+5));//左上->左下
     painter.drawLine(QPoint(d-5,d-5),QPoint(21*d+5,d-5));//左上->右上
     painter.drawLine(QPoint(21*d+5,d-5),QPoint(21*d+5,21*d+5));//右上->右下
     painter.drawLine(QPoint(21*d+5,21*d+5),QPoint(d-5,21*d+5));//右下->左下


     painter.setBrush(QBrush(Qt::black,Qt::SolidPattern));
     painter.drawRect(5*d-7,5*d-7,14,14);
     painter.drawRect(17*d-7,5*d-7,14,14);
     painter.drawRect(5*d-7,17*d-7,14,14);
     painter.drawRect(17*d-7,17*d-7,14,14);
     painter.drawRect(11*d-7,11*d-7,14,14);




     int i,j;
     for (i = 1; i <= 21; i++)
     {
         for (j = 1; j <= 21; j++)
         {
           if (chess[i][j] == 1)
           {
              brush.setColor(Qt::black);
              painter.setBrush(brush);
              painter.drawEllipse(QPoint(i*d,j*d),20,20);
           }
           else if (chess[i][j] == 2)
           {
              brush.setColor(Qt::white);
              painter.setBrush(brush);
              painter.drawEllipse(QPoint(i*d,j*d),20,20);
           }
        }
     }

     if (movex>=1&&movey>=1&&movex<=21&&movey<=21)
         {
             painter.setPen(QPen(Qt::red,4,Qt::SolidLine));
             //根据行号列号计算出落子位置
             int centerx = movex*d;
             int centery = movey*d;
             painter.drawLine(centerx-25,centery-25,centerx-15,centery-25);
             painter.drawLine(centerx+15,centery-25,centerx+25,centery-25);
             painter.drawLine(centerx-25,centery-25,centerx-25,centery-15);
             painter.drawLine(centerx-25,centery+15,centerx-25,centery+25);
             painter.drawLine(centerx-25,centery+25,centerx-15,centery+25);
             painter.drawLine(centerx+25,centery+25,centerx+15,centery+25);
             painter.drawLine(centerx+25,centery+25,centerx+25,centery+15);
             painter.drawLine(centerx+25,centery-15,centerx+25,centery-25);
         }

}

void ChessBoard::mouseMoveEvent(QMouseEvent *e)
{
    int d=50;
    movex = (e->x()+25) / d;
    movey = (e->y()+25) / d;
    update();
}


void ChessBoard::mouseReleaseEvent(QMouseEvent *e)
{
    int x, y;
    int d=50;
    int flag=0;
    QSound *sound = new QSound(":/new/prefix1/chessone.wav");
    sound->play();
    if(computer!=2)
    {
        if(e->x() >= d && e->x() <= 21*d && e->y() >= d && e->y() <=21*d)
        {
            x = (e->x()+25) / d;
            y = (e->y()+25) / d;
            if (!chess[x][y])
            {
                chess[x][y] = player++ % 2 + 1;
                flag=1;
            }
            currentx=x;
            currenty=y;
            if(player % 2 == 0)
            {
                wtimerId->stop();
                btimerId->start(1000);
            }
            if(player % 2 == 1)
            {
                btimerId->stop();
                wtimerId->start(1000);
            }
            if(Win(x, y))
            {
               wtimerId->stop();
               btimerId->stop();
               update();
               //setEnabled(false);
               QSound *sound2 = new QSound(":/new/prefix1/win.wav");
               sound2->play();
               if(player % 2 == 0)
                    QMessageBox::information(this, "White Win", "恭喜白棋获胜", QMessageBox::Ok);
               if(player % 2 == 1)
                    QMessageBox::information(this, "Black Win", "恭喜黑棋获胜", QMessageBox::Ok);
            }

        }

        update();
        if(computer==1&&flag==1)
        {
            flag=0;
            TurnToComputer();
            player++;
            if(Win(cx,cy))
            {
                wtimerId->stop();
                btimerId->stop();
                update();
                //setEnabled(false);
                if(player % 2 == 0)
                {
                    QMessageBox::information(this, "White Win", "恭喜白棋获胜", QMessageBox::Ok);
                    QSound *sound3 = new QSound(":/new/prefix1/lose.wav");
                    sound3->play();
                }
                if(player % 2 == 1)
                {
                    QMessageBox::information(this, "Black Win", "恭喜黑棋获胜", QMessageBox::Ok);
                    QSound *sound4 = new QSound(":/new/prefix1/win.wav");
                    sound4->play();
                }
            }
            update();
        }
    }

    else
    {
        if(player==0)
        {
            srand((unsigned int)time(NULL));
            int firstx=rand()%21;
            int firsty=rand()%21;
            chess[firstx][firsty]=1;
            update();
            player++;
        }
        TurnToComputer();
        player++;
        update();
        if(Win(cx,cy))
        {
            update();
            //setEnabled(false);
            if(player % 2 == 0)
            {
                QMessageBox::information(this, "White Win", "恭喜白棋获胜", QMessageBox::Ok);
                QSound *sound3 = new QSound(":/new/prefix1/win.wav");
                sound3->play();
            }
            if(player % 2 == 1)
            {
                QMessageBox::information(this, "Black Win", "恭喜黑棋获胜", QMessageBox::Ok);
                QSound *sound4 = new QSound(":/new/prefix1/win.wav");
                sound4->play();
            }
        }

        if(player!=2)
        {
            TurnToComputer();
            player++;
            update();
            if(Win(cx,cy))
            {
                update();
                //setEnabled(false);
                if(player % 2 == 0)
                {
                    QMessageBox::information(this, "White Win", "恭喜白棋获胜", QMessageBox::Ok);
                    QSound *sound3 = new QSound(":/new/prefix1/lose.wav");
                    sound3->play();
                }
                if(player % 2 == 1)
                {
                    QMessageBox::information(this, "Black Win", "恭喜黑棋获胜", QMessageBox::Ok);
                    QSound *sound4 = new QSound(":/new/prefix1/win.wav");
                    sound4->play();
                }
            }
        }
    }
}

int ChessBoard::Win(int x,int y)
{
    return fun1(x,y)||fun2(x,y)||fun3(x,y)||fun4(x,y);
}

int ChessBoard::fun1(int x,int y)//左右
{
    int tmp=chess[x][y];
    int i;
    int c1=0,c2=0;
    for(i=x+1;i>=1&&i<=21;i++)
    {
        if(chess[i][y]==tmp)
            c1++;
        else
            break;
    }

    for(i=x-1;i>=1&&i<=21;i--)
    {
        if(chess[i][y]==tmp)
            c2++;
        else
            break;
    }
    if(c1+c2>=5)
       return 1;
    else
       return 0;
}

int ChessBoard::fun2(int x,int y)//竖直
{
    int tmp=chess[x][y];
    int i;
    int c1=0,c2=0;
    for(i=y+1;i>=1&&i<=21;i++)
    {
        if(chess[x][i]==tmp)
            c1++;
        else
            break;
    }

    for(i=y-1;i>=1&&i<=21;i--)
    {
        if(chess[x][i]==tmp)
            c2++;
        else
            break;
    }

    if(c1+c2>=5)
       return 1;
    else
       return 0;
}

int ChessBoard::fun3(int x,int y)//右斜
{
    int tmp=chess[x][y];
    int i,j;
    int c1=0,c2=0;
    for(i=x+1,j=y+1;i>=1&&i<=21&&j>=1&&j<=21;i++,j++)
    {
        if(chess[i][j]==tmp)
            c1++;
        else
            break;
    }

    for(i=x-1,j=y-1;i>=1&&i<=21&&j>=1&&j<=21;i--,j--)
    {
        if(chess[i][j]==tmp)
            c2++;
        else
            break;
    }

    if(c1+c2>=5)
       return 1;
    else
       return 0;
}

int ChessBoard::fun4(int x,int y)//左斜
{
    int tmp=chess[x][y];
    int i,j;
    int c1=0,c2=0;
    for(i=x+1,j=y-1;i>=1&&i<=21&&j>=1&&j<=21;i++,j--)
    {
        if(chess[i][j]==tmp)
            c1++;
        else
            break;
    }

    for(i=x-1,j=y+1;i>=1&&i<=21&&j>=1&&j<=21;i--,j++)
    {
        if(chess[i][j]==tmp)
            c2++;
        else
            break;
    }

    if(c1+c2>=5)
       return 1;
    else
       return 0;
}

ChessBoard::~ChessBoard()
{
    delete ui;
}

void ChessBoard::btimerUpdate()
{
    QString timeString=QString("%1-%2").arg(whiteTimes/60,2,10,QChar('0')).arg(whiteTimes%60,2,10,QChar('0'));
    ui->whiteTime->display(timeString);
    if(!whiteTimes)
    {
        QMessageBox::about(this,QStringLiteral("白棋超时"),QStringLiteral("超时!"));
    }
    else
        whiteTimes--;
}

void ChessBoard::wtimerUpdate()
{
    QString timeString=QString("%1-%2").arg(blackTimes/60,2,10,QChar('0')).arg(blackTimes%60,2,10,QChar('0'));
    ui->blackTime->display(timeString);
    if(!blackTimes)
    {
        QMessageBox::about(this,QStringLiteral("黑棋超时"),QStringLiteral("超时!"));
    }
    else
        blackTimes--;
}
void ChessBoard::regretchess()
{
    if(computer==0)
    {
        player--;
        if(player % 2 == 0)
        {
            wtimerId->stop();
            btimerId->start(1000);
        }
        if(player % 2 == 1)
        {
            btimerId->stop();
            wtimerId->start(1000);
        }
        chess[currentx][currenty]=0;
        update();
    }
    else
    {
        QMessageBox::information(this, "提示", "当前模式下不可悔棋", QMessageBox::Ok);
    }
}
void ChessBoard::again()
{
    wtimerId->stop();
    btimerId->stop();
    whiteTimes=300;
    blackTimes=300;
    player=0;
    memset(chess,0,22*22*sizeof(int));
    update();
}
void ChessBoard::personvscomputer(int x)
{
    computer=x;
}
void ChessBoard::TurnToComputer()
{
    int P_N = 0;//玩家连成子的个数,玩家执黑子
    int AI_N = 0;//AI连成子的个数，AI执白子
    int E_N = 0;//各方向空白位的个数
    int s[22][22];//评分数组
    memset(s, 0, 22 * 22 * sizeof(int));//清空评分数组
    for (int r = 0; r < 22; r++)
        for (int c = 0; c < 22; c++)
        {
            if (r > 0 && c > 0 &&
                chess[r][c] == 0)//如果遍历的点没有棋子
            {
                //遍历周围八个方向
                for (int y = -1; y <= 1; y++)
                    for (int x = -1; x <= 1; x++)
                    {

                        P_N = 0;
                        AI_N = 0;
                        E_N = 0;
                        //原坐标不算
                        if (!(y == 0 && x == 0))
                        {
                            //每个方向延申五个子
                            for (int i = 1; i <= 5; i++)
                            {
                                if (r + i * y > 0 && r + i * y < 22 &&
                                    c + i * x > 0 && c + i * x < 22 &&
                                    chess[r + i * y][c + i * x] == 1)
                                {
                                    P_N++;
                                }
                                else if (r + i * y > 0 && r + i * y < 22 &&
                                         c + i * x > 0 && c + i * x < 22 &&
                                         chess[r + i * y][c + i * x] == 0)
                                {
                                    E_N++;
                                    break;
                                }
                                else
                                    break;
                            }

                            for (int i = 1; i <= 5; i++)
                            {
                                if (r - i * y > 0 && r - i * y < 22 &&
                                    c - i * x > 0 && c - i * x < 22 &&
                                    chess[r - i * y][c - i * x] == 1)
                                {
                                    P_N++;
                                }
                                else if (r - i * y > 0 && r - i * y < 22 &&
                                         c - i * x > 0 && c - i * x < 22 &&

                                         chess[r - i * y][c - i * x] == 0)
                                {
                                    E_N++;
                                    break;
                                }
                                else        //出边界
                                    break;
                            }

                            if (P_N == 1)
                                s[r][c] += 10;
                            else if (P_N == 2)
                            {
                                if (E_N == 1)
                                    s[r][c] += 30;
                                else if (E_N == 2)
                                    s[r][c] += 40;
                            }
                            else if (P_N == 3)
                            {

                                if (E_N == 1)
                                    s[r][c] += 60;
                                else if (E_N == 2)
                                    s[r][c] += 110;
                            }
                            else if (P_N == 4)
                            {
                                if (E_N == 1)
                                    s[r][c] += 1000;
                                else if (E_N == 2)
                                    s[r][c] += 2000;
                            }
                            else if (P_N == 5)
                                s[r][c] += 100000;

                            //进行一次清空
                            E_N = 0;

                            for (int i = 1; i <= 5; i++)
                            {
                                if (r + i * y > 0 && r + i * y < 22 &&
                                    c + i * x > 0 && c + i * x < 22 &&
                                    chess[r + i * y][c + i * x] == 1)
                                {
                                    AI_N++;
                                }
                                else if (r + i * y > 0 && r + i * y < 22 &&
                                         c + i * x > 0 && c + i * x < 22 &&
                                         chess[r + i * y][c + i * x] == 0)
                                {
                                    E_N++;
                                    break;
                                }
                                else
                                    break;
                            }

                            for (int i = 1; i <= 5; i++)
                            {
                                if (r - i * y > 0 && r - i * y < 22 &&
                                    c - i * x > 0 && c - i * x < 22 &&
                                    chess[r - i * y][c - i * x] == 2)
                                {
                                    AI_N++;
                                }
                                else if (r - i * y > 0 && r - i * y < 22 &&
                                         c - i * x > 0 && c - i * x < 22 &&
                                         chess[r - i * y][c - i * x] == 0)
                                {
                                    E_N++;
                                    break;
                                }
                                else
                                    break;
                            }

                            if (AI_N == 0)
                                s[r][c] += 5;
                            else if (AI_N == 1)
                                s[r][c] += 10;
                            else if (AI_N == 2)
                            {
                                if (E_N == 1)
                                    s[r][c] += 25;
                                else if (E_N == 2)
                                    s[r][c] += 50;
                            }
                            else if (AI_N == 3)
                            {
                                if (E_N == 1)
                                    s[r][c] += 55;
                                else if (E_N == 2)
                                    s[r][c] += 100;
                            }
                            else if (AI_N >= 4)
                            {
                                if (E_N == 1)
                                    s[r][c] += 990;
                                else if (E_N == 2)
                                    s[r][c] += 1990;
                            }
                            else if (AI_N >= 5)
                                s[r][c] += 200000;
                        }
                    }
            }
        }
    int maxs = 0;
    for (int i = 1; i < 22; i++)
        for (int j = 1; j < 22; j++)
            if (chess[i][j] == 0)
                if (s[i][j] > maxs)
                {
                    maxs = s[i][j];
                    cx = i;
                    cy = j;
                }
    chess[cx][cy] = player % 2 + 1;
}
