#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setFixedSize(750,550);//窗口分辨率
    setWindowTitle(tr("五子棋"));//窗口标题

    Qt::WindowFlags flags=Qt::Dialog;
    flags |=Qt::WindowMinMaxButtonsHint;//添加最小化最大化按钮（最大化不可用）
    flags |=Qt::WindowCloseButtonHint;//启用关闭按钮
    setWindowFlags(flags);

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//表格只读
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);//只选一个
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectItems);//只能选格

    ui->tableWidget->verticalHeader()->setVisible(false); //设置垂直头不可见
    ui->tableWidget->horizontalHeader()->setVisible(false); //设置水平头不可见

    ui->spinBox->setMinimum(5);//设置最小值
    GroundSize=10;
    ui->spinBox->setValue(10);
    ui->spinBox->setMaximum(15);//设置最大值

    font=15;
    ui->tableWidget->setFont(QFont("song", font));
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_pushButton_clicked()
{
    for(int i=0;i<GroundSize;++i)
        for(int j=0;j<GroundSize;++j)
        {
            cells[i][j]=0;
            status[i][j]=false;
        }

    counterX=counterO=0;
    current=1;

    ui->label_2->setText("X");
    ui->label_3->setText(QString::number(counterX));
    ui->label_5->setText(QString::number(counterO));

    GroundSize=ui->spinBox->value();
    font=(ui->tableWidget->horizontalHeader()->width()/GroundSize)*(0.3);

    ui->label_9->setText(QString::number(GroundSize*GroundSize));

    ui->tableWidget->setFont(QFont("song", font));//必须在前设置字体，不然表格大小可能不正常
    ui->tableWidget->setColumnCount(GroundSize); //设置列数
    ui->tableWidget->setRowCount(GroundSize); //设置行数

    QFont qfont;
    qfont.setBold(true);
    for(int i=0;i<GroundSize;++i)
        ui->tableWidget->setColumnWidth(i,ui->tableWidget->horizontalHeader()->width()/GroundSize);
    for(int i=0;i<GroundSize;++i)
        ui->tableWidget->setRowHeight(i,ui->tableWidget->verticalHeader()->height()/GroundSize);
    for(int i=0;i<GroundSize;++i)
    {
        for(int j=0;j<GroundSize;++j)
        {
            ui->tableWidget->setItem(i, j, new QTableWidgetItem);
            ui->tableWidget->item(i,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            ui->tableWidget->item(i, j)->setFont(qfont);
            ui->tableWidget->item(i, j)->setBackground(Qt::white);
        }
    }
}


void Dialog::on_tableWidget_cellClicked(int row, int column)
{
    if(status [row][column])
        return;

    cells[row][column]=current;
    status [row][column]=true;
    if(current==1)
    {
        ui->tableWidget->item(row,column)->setText("X");
        ui->tableWidget->item(row, column)->setForeground(Qt::red);
        ++counterX;
        ui->label_3->setText(QString::number(counterX));
    }
    else if(current==2)
    {
        ui->tableWidget->item(row,column)->setText("O");
        ui->tableWidget->item(row, column)->setForeground(Qt::blue);
        ++counterO;
        ui->label_5->setText(QString::number(counterO));
    }

    int type=Judge(row, column);
    if(type)
    {
        Paint(row, column,type);
        Win();
        return;
    }
    if((counterX+counterO)>=GroundSize*GroundSize)
    {
        Draw();
        return;
    }

    if(current==1)
    {
        ui->label_2->setText("O");
        current=2;
    }
    else if(current==2)
    {
        ui->label_2->setText("X");
        current=1;
    }
}

int Dialog::Judge(int row,int column)
{
    int vertical=1,horizontal=1,LeftOblique=1,RightOblique=1;

    for(int i=row-1,j=column;(i>=0)&&(cells[i][j]==current);--i)
        ++vertical;
    for(int i=row+1,j=column;(i<GroundSize)&&(cells[i][j]==current);++i)
        ++vertical;
    if(vertical>=5)
        return 1;

    for(int i=row,j=column-1;(j>=0)&&(cells[i][j]==current);--j)
        ++horizontal;
    for(int i=row,j=column+1;(j<GroundSize)&&(cells[i][j]==current);++j)
        ++horizontal;
    if(horizontal>=5)
        return 2;

    for(int i=row-1,j=column-1;(j>=0)&&(i>=0)&&(cells[i][j]==current);--j,--i)
        ++LeftOblique;
    for(int i=row+1,j=column+1;(j<GroundSize)&&(i<GroundSize)&&(cells[i][j]==current);++j,++i)
        ++LeftOblique;
    if(LeftOblique>=5)
        return 3;

    for(int i=row+1,j=column-1;(j>=0)&&(i<GroundSize)&&(cells[i][j]==current);--j,++i)
        ++RightOblique;
    for(int i=row-1,j=column+1;(j<GroundSize)&&(i>=0)&&(cells[i][j]==current);++j,--i)
        ++RightOblique;
    if(RightOblique>=5)
        return 4;

    return 0;
}

void Dialog::Win()
{
    QString c;
    if(current==1)
        c="X";
    else if(current==2)
        c="O";
    c+="赢了";
    for(int i=0;i<GroundSize;++i)
        for(int j=0;j<GroundSize;++j)
        {
            ui->tableWidget->item(i, j)->setFlags(Qt::NoItemFlags);
            status[i][j]=true;
        }
    QMessageBox::information(this, tr("游戏结束"),c, QMessageBox::Ok);
}

void Dialog::Draw()
{
    for(int i=0;i<GroundSize;++i)
        for(int j=0;j<GroundSize;++j)
        {
            ui->tableWidget->item(i, j)->setFlags(Qt::NoItemFlags);
            status[i][j]=true;
        }
    QMessageBox::information(this, tr("游戏结束"),tr("和棋"), QMessageBox::Ok);
}

void Dialog::Paint(int row,int column,int type)
{
    ui->tableWidget->item(row, column)->setBackground(Qt::green);
    if(type==1)
    {
        for(int i=row-1,j=column;(i>=0)&&(cells[i][j]==current);--i)
            ui->tableWidget->item(i, j)->setBackground(Qt::green);
        for(int i=row+1,j=column;(i<GroundSize)&&(cells[i][j]==current);++i)
            ui->tableWidget->item(i, j)->setBackground(Qt::green);
    }

    else if(type==2)
    {
        for(int i=row,j=column-1;(j>=0)&&(cells[i][j]==current);--j)
            ui->tableWidget->item(i, j)->setBackground(Qt::green);
        for(int i=row,j=column+1;(j<GroundSize)&&(cells[i][j]==current);++j)
            ui->tableWidget->item(i, j)->setBackground(Qt::green);
    }

    else if(type==3)
    {
        for(int i=row-1,j=column-1;(j>=0)&&(i>=0)&&(cells[i][j]==current);--j,--i)
            ui->tableWidget->item(i, j)->setBackground(Qt::green);
        for(int i=row+1,j=column+1;(j<GroundSize)&&(i<GroundSize)&&(cells[i][j]==current);++j,++i)
            ui->tableWidget->item(i, j)->setBackground(Qt::green);
    }

    else if(type==4)
    {
        for(int i=row+1,j=column-1;(j>=0)&&(i<GroundSize)&&(cells[i][j]==current);--j,++i)
            ui->tableWidget->item(i, j)->setBackground(Qt::green);
        for(int i=row-1,j=column+1;(j<GroundSize)&&(i>=0)&&(cells[i][j]==current);++j,--i)
            ui->tableWidget->item(i, j)->setBackground(Qt::green);
    }
}
