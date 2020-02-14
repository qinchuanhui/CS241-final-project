#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlRelationalTableModel>
#include <QMessageBox>
#include <QSqlError>
#include <QFileDialog>
#include <QFile>
#include <QStringList>
#include <QString>
#include <QDateTime>
#include "thread.h"
#include<QtCharts/QSplineSeries>
#include<QtMath>
#include<QThread>
#include<QProgressBar>
#include <QThread>

mythread::mythread(QObject *parent) : QObject(parent)
{

}

//加载文件的函数
void mythread::load_file_slot()
{
    QTime t;
    t.start();
    QString every_line;
    QStringList tmp1;
    metro tmp2;
   //qDebug()<<"xi";
   MainWindow:: container.clear();
   //qDebug()<<"xi2";
    int i=1;

   foreach (QString file, MainWindow:: fileNames)
   {
      /*MainWindow:: pProgressBar->setValue(i);  // 当前进度
       double dProgress = (MainWindow:: pProgressBar->value() - MainWindow:: pProgressBar->minimum()) * 100.0
                      /(MainWindow:: pProgressBar->maximum() - MainWindow:: pProgressBar->minimum());
       MainWindow:: pProgressBar->setFormat("loading:"+QString::number(dProgress, 'f', 3));
       qDebug()<<"xi3";
     */
       emit notify(i);

       QFile csvfile (file);

      // qDebug()<<"xi4";
       if(csvfile.open(QIODevice::ReadWrite))
       {
          // qDebug()<<"open"<<i;

           QTextStream stream(&csvfile);
           stream.readLine();

           //qDebug()<<0;
           while(!stream.atEnd())
           {
               every_line=stream.readLine();

               //qDebug()<<1;
               tmp1=every_line.split(',');
               //if(MainWindow:: tii)
               tmp2.tii=tmp1.at(0);
               //tmp2.ti=QDateTime::fromString(tmp1.at(0),
                                      //       "yyyy-MM-dd HH:mm:ss");
               //qDebug()<<tmp2.tii;
             // if(MainWindow::lii)
               tmp2.li=tmp1.at(1);

             // if(MainWindow::st11)
               {tmp2.st1=tmp1.at(2).toInt();}

               if(MainWindow::de)
               tmp2.de=tmp1.at(3);

              // if(MainWindow::st22)
               tmp2.st2=tmp1.at(4);

              if(MainWindow::us)
               {tmp2.us=tmp1.at(5);}

               if(MainWindow::pa==true)
               {tmp2.pa=tmp1.at(6);}

               MainWindow:: container.push_back(tmp2);
              // qDebug()<<tmp1.at(0);
               //qDebug()<<tmp2.ti;

               //qDebug()<<2;

           }

       }

       csvfile.close();

       i++;
      // qDebug()<<3;

      }

      qDebug()<<"time"<<t.elapsed();
      MainWindow:: file_loaded=1;
      qDebug()<<MainWindow:: container.size();
}

/*
void mythread::pic_make_slot()
{
    //转换并存储三种时间，创立time—table-in out 数组
    long long step_sec=abs(MainWindow:: step.secsTo(QTime(0,0,0,0)));
   // qDebug()<<step_sec;
    long long starting_sec= MainWindow::starting.toSecsSinceEpoch();
    long long endding_sec=MainWindow::endding.toSecsSinceEpoch();
    QString start_str=MainWindow::starting.toString("yyyy-MM-dd HH:mm:ss");
    QString end_str=MainWindow::endding.toString("yyyy-MM-dd HH:mm:ss");

    QTime ttt;
    ttt.start();

   qDebug()<<starting_sec;
   qDebug()<<endding_sec;
    long long  time_size=(endding_sec-starting_sec)/step_sec;
    int *time_table_out=new int [time_size+1]{0};
    int *time_table_in=new int [time_size+1]{0};

   //选择合适的数据，填入table；
    int i=0;
    int k=0;
    QString status="1";
    while(i<MainWindow::container.size())
    {
        if (MainWindow::container.at(i).st1==MainWindow::station1)
        {    //qDebug()<<"fuck1";
          //  long long time=container.at(i).ti.toSecsSinceEpoch();
            //qDebug()<<time;
            if(MainWindow::container.at(i).tii>=start_str && MainWindow::container.at(i).tii<=end_str )
            {
              long long place;
              QDateTime t = QDateTime::fromString(MainWindow::container.at(i).tii,
                                                       "yyyy-MM-dd HH:mm:ss");
              long long time=t.toSecsSinceEpoch();
             // qDebug()<<container.at(i).tii;
              place=(time-starting_sec)/step_sec;
              //qDebug()<<"fuck2";
              if(MainWindow::container.at(i).st2==status)
              {
                  time_table_in[place]=time_table_in[place]+1;
                 // qDebug()<<place<<" : in +1:"<<time_table_in[place];
              }
              else
              {
                  time_table_out[place]=time_table_out[place]+1;
                  //qDebug()<<place<<" : out +1"<<time_table_out[place];
              }
              emit notify1(i);

            }

        }
      i++;
    }

    qDebug()<<"time"<<ttt.elapsed();
}


*/
