#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QMainWindow>
#include <QTimer>
namespace Ui {
class ChessBoard;
}

class ChessBoard : public QMainWindow
{
    Q_OBJECT
public:
    explicit ChessBoard(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void regretchess();
    void again();
    void personvscomputer(int);
    void TurnToComputer();
    ~ChessBoard();
signals:
    void backsignal();
public slots:
    void btimerUpdate();
    void wtimerUpdate();
private:
    int chess[22][22];
    int player;
    int Win(int,int);
    int fun1(int,int);//水平方向
    int fun2(int,int);//竖直方向
    int fun3(int,int);//右斜方向
    int fun4(int,int);//左斜方向
    int movex,movey;
    Ui::ChessBoard *ui;
    QTimer *btimerId;
    QTimer *wtimerId;
    int whiteTimes;
    int blackTimes;
    int currentx,currenty;
    int computer=0;
    int cx,cy;
};

#endif // CHESSBOARD_H
