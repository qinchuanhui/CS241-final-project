#include "mythread.h"
#include<QProgressBar>
#include<QTextStream>
#include<QDebug>

mythread::mythread(QObject *parent) : QObject(parent)
{
   is_stop=1;
}

//void loadslot( QVector<metro> &container,QStringList &fileNames)
//{
   /* int file_size=fileNames.size();

    QProgressBar *pProgressBar = ui->progressBar ;
    pProgressBar->setOrientation(Qt::Horizontal);  // 水平方向
    pProgressBar->setMinimum(0);  // 最小值
    pProgressBar->setMaximum(file_size);  // 最大值

    pProgressBar->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    pProgressBar->show();
*/


    //读取所有文件，放入mmetro对应的vector

/*    QTime t;
    t.start();

    QString every_line;
    QStringList tmp1;
    metro tmp2;

    int i=1;
    foreach (QString file, fileNames)
    {
      pProgressBar->setValue(i);  // 当前进度
        double dProgress = (pProgressBar->value() - pProgressBar->minimum()) * 100.0
                        / (pProgressBar->maximum() - pProgressBar->minimum());
        pProgressBar->setFormat(QString::number(dProgress, 'f', 3));





        if(csvfile.open(QIODevice::ReadWrite))
        {
            qDebug()<<"open";

            QTextStream stream(&csvfile);
            stream.readLine();
            while(!stream.atEnd())
            {
                every_line=stream.readLine();

                tmp1=every_line.split(',');
                tmp2.ti=QDateTime::fromString(tmp1.at(0),"yyyy-MM-dd HH:mm:ss");
                tmp2.li=tmp1.at(1);
                tmp2.st1=tmp1.at(2).toInt();
                tmp2.de=tmp1.at(3);
                tmp2.st2=tmp1.at(4);
                tmp2.us=tmp1.at(5);
                tmp2.pa=tmp1.at(6);
                container.push_back(tmp2);
               // qDebug()<<tmp1.at(0);
                //qDebug()<<tmp2.ti;

            }

        }

        csvfile.close();

        i++;

       }



*/
//}
