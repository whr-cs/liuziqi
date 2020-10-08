#ifndef PATTERN_H
#define PATTERN_H

#include <QMainWindow>
#include <chessboard.h>
namespace Ui {
class Pattern;
}

class Pattern : public QMainWindow
{
    Q_OBJECT

public:
    explicit Pattern(QWidget *parent = 0);
    ~Pattern();
signals:
    void mySignal();
private:
    Ui::Pattern *ui;
    ChessBoard c;
};

#endif // PATTERN_H
