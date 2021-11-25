#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include<QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_pushButton_clicked();
    void on_tableWidget_cellClicked(int row, int column);

    int Judge(int row,int column);
    void Win();
    void Draw();
    void Paint(int row,int column,int type);

private:
    Ui::Dialog *ui;
    int font=15;
    int GroundSize=10;
    int cells[15][15]={{0}};
    bool status[15][15]={{false}};
    int counterX=0,counterO=0;
    int current=1;

};
#endif // DIALOG_H
