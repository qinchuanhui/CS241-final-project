#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QSqlQueryModel>
#include<QSqlTableModel>
#include<QVector>
#include<QDateTime>
#include<QLineSeries>
#include<QChart>
#include<QValueAxis>
#include<QDateTimeAxis>
#include<QProgressBar>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

using namespace QtCharts;

struct metro{
  //QDateTime ti;
   QString tii;
    QString li;
    int st1;
    QString de;
    QString st2;
    QString us;
    QString pa;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT


    static bool createConnection();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static  QVector<metro> container;//用来存放data的容器
   // static  Ui::MainWindow *ui;


    static bool tii;
    static bool lii;
    static bool st11;
    static bool st22;
    static bool de;
    static bool us;
    static bool pa;
    static QProgressBar *pProgressBar;
    static QStringList fileNames;
    static bool file_loaded;//表示已经加载



private slots:

    void on_actionChoose_the_file_triggered();

    void on_actionChoose_the_map_triggered();

    void on_pushbutton_clicked();

    void on_check3_stateChanged(int arg1);

    void on_check2_stateChanged(int arg1);

    void on_check1_stateChanged(int arg1);

    void change_pgb(int i);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_in_check_clicked();

    void on_out_check_clicked();

    void on_total_clicked();

    void on_actionExit_X_triggered();

    void on_actionFunctions_of_every_part_triggered();

    void on_actionAbout_filter_and_load_triggered();

private:
    int data_size;

     QDateTime starting,endding;//画图用到的，起始、终止时间。
    QTime step;

    //QDateTime starting,endding;//画图用到的，起始、终止时间。
   // QTime step;//画图用到的步长
     Ui::MainWindow *ui;

    //QVector <QStringList> map;//用来存放map的容器
    int map_size;//表示总的station数
    bool map_loaded;//表示map是否被加载

    void paint_pic(int station1,int turn);//绘图的函数
   /* void paint_pic2(int station1,int station2);
    void paint_pic3(int station1,int station2,int station3);
    */

    QList <QLineSeries*> m_series;
    int **map_table;
    bool setaxis;
    QChart *chart = new QChart();//用来绘图
    void  display();
     QValueAxis *axisY;
     QDateTimeAxis *datex;

      QList <int> v;
     void search(int num,int des,int*pre,bool * vis);//用来生成路线
      QString map_str;

      bool total;
      bool analy_in;
};
#endif // MAINWINDOW_H
