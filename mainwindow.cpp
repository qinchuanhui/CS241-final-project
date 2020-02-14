#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlRelationalTableModel>
#include <QTableView>
#include <QMessageBox>
#include <QSqlError>
#include <QFileDialog>
#include <QFile>
#include <QStringList>
#include <QString>
#include <QDateTime>
#include "thread.h"
#include<QtCharts/QChartView>
#include<QtCharts/QSplineSeries>
#include<QtMath>
#include<QDateTimeAxis>
#include<QValueAxis>
#include<QGraphicsView>
#include<QThread>
#include<QPieSeries>
#include<QWindow>

bool MainWindow::de=false;
bool MainWindow::pa=false;
 bool MainWindow:: tii=false;
 bool MainWindow::lii=false;
 bool MainWindow::st11=false;
 bool MainWindow::st22=false;

 bool MainWindow::us=false;
QVector<metro> MainWindow::container;

 QProgressBar *MainWindow::pProgressBar=nullptr;
 QStringList MainWindow::fileNames;
 bool MainWindow::file_loaded;




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    map_size=81;
    map_loaded=0;
    file_loaded=0;
    QDate da(2019,1,13);
    QTime t(0,0,0);
    ui->progressBar->setValue(0);
    ui->progressBar->setFormat(QString::number(0));
    ui->start_time->setDateTime(QDateTime(da,t));
    ui->end_time->setDateTime(QDateTime(da,QTime(23,0,0)));

    ui->start_time_2->setDateTime(QDateTime(da,t));
     ui->end_time_2->setDateTime(QDateTime(da,QTime(23,0,0)));
    //ui->pb2->setValue(0);
   // ui->pb2->show();
    //设置初始filter：全选状态
  //  ui->ticheck->setChecked(1);
    //ui->licheck->setChecked(1);
    //ui->st1check->setChecked(1);
    //ui->st2check->setChecked(1);
    ui->uscheck->setChecked(1);
    ui->decheck->setChecked(1);
    ui->pacheck->setChecked(1);


    analy_in=0;
    total=0;

    /*QSqlQueryModel *model=new QSqlQueryModel(this);
    model->setQuery("select *from fuck");
    model->setHeaderData(0,Qt::Horizontal,"number");
    model->setHeaderData(1,Qt::Horizontal,"name");
    model->setHeaderData(2,Qt::Horizontal,"length");

    ui->tableview->setModel(model);
    ui->tableview->show();
    */
    setaxis=0;
    ui->tab->resize(700,600);


}

MainWindow::~MainWindow()
{
    delete ui;
}


/*void MainWindow::on_pushButton_clicked()
{
    QSqlQueryModel *model=new QSqlQueryModel(this);
        model->setQuery("select *from fuck");
        model->setHeaderData(0,Qt::Horizontal,"number");
        model->setHeaderData(1,Qt::Horizontal,"name");
        model->setHeaderData(2,Qt::Horizontal,"length");
        QTableView *view = new QTableView;
          view->setModel(model);
          view->show();


        QSqlQueryModel *model = new QSqlQueryModel(this);
        model->setQuery("select * from student");
        model->setHeaderData(0, Qt::Horizontal, tr("id"));
        model->setHeaderData(1, Qt::Horizontal, tr("name"));
        model->setHeaderData(2, Qt::Horizontal, tr("name"));


        //ui->tableview->setModel(model);
        //ui->tableview->show();
   }
   */

//点击选择data—file，选择upload的文件，并存入vector
void MainWindow::on_actionChoose_the_file_triggered()
{
    if(!map_loaded)
    {
        QMessageBox::information(this,"information","You had better "
                                 "load the map_file first. ");
    }


    //设置文件选择对话框
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle(tr("打开.csv文件"));
    fileDialog->setDirectory("/"); 
    fileDialog->setNameFilter(tr("(*.csv)"));
    fileDialog->setFileMode(QFileDialog::ExistingFiles);
    fileDialog->setViewMode(QFileDialog::Detail);
   // QStringList fileNames;


    if(fileDialog->exec())
    {
        fileNames = fileDialog->selectedFiles();
    }
   /* foreach (QString fil, fileNames) {
        qDebug()<<fil;
   */

    //int file_number=fileNames.size();

    int file_size=fileNames.size();

    pProgressBar = ui->progressBar ;
    pProgressBar->setOrientation(Qt::Horizontal);  // 水平方向
    pProgressBar->setMinimum(0);  // 最小值
    pProgressBar->setMaximum(file_size);  // 最大值

    pProgressBar->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    pProgressBar->show();

    //读取所有文件，放入mmetro对应的vector
    // tii=ui->ticheck->isChecked();
     //lii=ui->licheck->isChecked();
     //st11=ui->st1check->isChecked();
     //st22=ui->st2check->isChecked();
     de=ui->decheck->isChecked();
     us=ui->uscheck->isChecked();
     pa=ui->pacheck->isChecked();

     //开启线程
     qDebug()<<"thread begins";
     QThread *thr;
     thr=new QThread;
     mythread *mythr=new mythread;
     mythr->moveToThread(thr);
     connect(thr,SIGNAL(finished()),mythr,SLOT(deleteLater()));        //终止线程时要调用deleteLater槽函数
     connect(thr,SIGNAL(started()),mythr,SLOT(load_file_slot()));  //开启线程槽函数
    // connect(thr,SIGNAL(finished()),this,SLOT(finishedThreadSlot()));
     connect(thr,SIGNAL(finished()),thr,SLOT(deleteLater()));
     connect(mythr,SIGNAL(notify(int)),this,SLOT(change_pgb(int)));

     thr->start();



}

//控制文件加载进度条
void MainWindow::change_pgb(int i)
{
    pProgressBar->setValue(i);  // 当前进度
    double dProgress = (pProgressBar->value() - pProgressBar->minimum()) * 100.0
                    / (pProgressBar->maximum() - pProgressBar->minimum());
    pProgressBar->setFormat("loading:"+QString::number(dProgress, 'f', 3));
    if(dProgress>=99.9)
         QMessageBox::information(this,"Ok","The data has been loaded");
}

//点击选择map，选择upload的文件,并存入map_table
void MainWindow::on_actionChoose_the_map_triggered()
{
   QString filename;
   filename=QFileDialog::getOpenFileName(this,"choose the map_file","/","EXCEL(*.csv)");
   QFile file(filename);
   if(file.open(QIODevice::ReadWrite))
   {
       QTextStream stream(&file);

       QString line;
       line=stream.readLine();
       QStringList tmp;
        tmp=line.split(',');
        map_size= tmp.size()-1;
        map_table=new int* [map_size];
        for(int i=0;i<map_size;++i)
        map_table[i]=new int [map_size];
        int j=0;
       while(!stream.atEnd())
       {
           line=stream.readLine();
           tmp=line.split(',');
           for(int i=0;i<map_size;++i)
             map_table[j][i]=tmp.at(i+1).toInt();

           j++;

       }
       QMessageBox::information(this,"Ok","The map has been loaded");


       //for(int i=0;i<map_size;++i)
        //qDebug()<<i<<":"<<map_table[0][i];
       map_loaded=1;
    file.close();
   }
   else QMessageBox::information(this,"Sorry","Fail to open.");
}

//将数据导入数据库（无效代码）
/*bool MainWindow::createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    if(!db.open())
    {
        qDebug()<<"database-opening error";
        return false;
    }
    QSqlQuery query;
    QString tb_create="create table data("
            "time varchar"
            "lineID varchar"
            "stationID varchar"
            "deviceID varchar"
            "status varchar"
            "userID varchar"
            "payType varchar)";

    if(!query.exec(tb_create))
        qDebug()<<"tb-create error";

  //  QTime t;
   // t.start();//计时开始

    int i=0;
    QVariantList ti,li,st1,de,st2,us,pa;
    int sizee=MainWindow::csvlist->size();
    while (i<sizee)
    {
        QStringList tmp= MainWindow::csvlist->at(i).split(',');
        ti<<tmp.at(0);
        li<<tmp.at(1);
        st1<<tmp.at(2);
        de<<tmp.at(3);
        st2<<tmp.at(4);
        us<<tmp.at(5);
        pa<<tmp.at(6);
        i++;
    }   //把csvlist里的信息送至流内

    db.transaction();

    query.prepare("insert into data(time,lineID,stationID,deviceID,status,userID,payType)"
                  "values(?,?,?,?,?,?,?)");
    {
         query.addBindValue(ti);
         query.addBindValue(li);
         query.addBindValue(st1);
         query.addBindValue(de);
         query.addBindValue(st2);
         query.addBindValue(us);
         query.addBindValue(pa);
    }
    query.exec();
    db.commit();
  //  qDebug()<<"time: "<<t.elapsed()<<endl;

    query.exec("insert into student values (0,'yafei0')");
    query.exec("insert into student values (1,'yafei1')");
    query.exec("insert into student values (2,'yafei2')");


   return true;
}
*/

//将apply按下，开始绘图
void MainWindow::on_pushbutton_clicked()
{
    if(!file_loaded)
    {QMessageBox::warning(this,"warning","You forget to load the file");
        return;
    }

    //得到时间信息
    starting = ui->start_time->dateTime();
    endding = ui->end_time->dateTime();
    step=ui->time_step->time()  ;
   // qDebug()<<starting<<'\n'<<endding<<'\n'<<step;
    //得到站点信息,并绘图
    int station1=-1,station2=-1,station3=-1;
    chart->removeAllSeries();
    chart->removeAxis(datex);
    chart->removeAxis(axisY);
    setaxis=0;

    //判断绘图个数并分配不同的绘图函数


    try{
    //判断check1
    if(ui->check1->isChecked())
    {
      station1=ui->station1->value();
      if(station1+1>map_size)
      {
          QString bad="1";
          throw bad;
      }
      paint_pic(station1,1);
    }
    //判断check2
    if(ui->check2->isChecked())
    {
        station2=ui->station2->value();
        if(station2+1>map_size)
        {
            QString bad="2";
            throw bad;
        }
        paint_pic(station2,2);
      }
    //判断check3
    if(ui->check3->isChecked())
    {
        station3=ui->station3->value();
        if(station3+1>map_size)
        {
            QString bad="3";
            throw bad;
        }
         paint_pic(station3,3);
     }

    }
    catch(QString &str)
    {
        qDebug()<<"wrong";
        QString st="The number of station"+str+
          " is invalid. ";
        QMessageBox::warning(this,"Warning",st);
    }


   /*  if (number==0) QMessageBox::warning(this,"warning","Choose one!!!!");
     else if (number==3)
         {
            qDebug()<<"station 3 "<<station1<<station2<<station3;
            paint_pic3(station1,station2,station3);
         }
         else if(number==2)
              {

                if(station1>station3&&station2>station3) ;
                else if(station2>station1&&station3>station1) station1=station3;
                     else station2=station3;
                qDebug()<<"station 2 "<<station1<<station2<<station3;
                paint_pic2(station1,station2);

              }
              else
                {
                   if(station2>station3&&station2>station1) station1=station2;
                   else if (station3>station2&&station3>station1) station1=station3;
                        else ;
                   qDebug()<<"station 1 "<<station1<<station2<<station3;
                   paint_pic1(station1);

                }

*/



    display();
}

//如果选择绘制第三条曲线，提醒station数据
void MainWindow::on_check3_stateChanged(int arg1)
{
    //将站点信息转化为string
    int st=map_size-1;
    QString st1= QString::number(st)    ;
    QString st2="The station_number should range from 0 to "+st1+". ";
    QMessageBox::information(this,"Attention",st2);
}
//如果选择绘制第二条曲线，提醒station数据
void MainWindow::on_check2_stateChanged(int arg1)
{
    //将站点信息转化为string
    int st=map_size-1;
    QString st1=QString::number(st)  ;
    QString st2="The station_number should range from 0 to "+st1+". ";
    QMessageBox::information(this,"Attention",st2);
}
//如果选择绘制第一条曲线，提醒station数据
void MainWindow::on_check1_stateChanged(int arg1)
{
    //将站点信息转化为string
    int st=map_size-1;
    QString st1=QString::number(st)  ;
    QString st2="The station_number should range from 0 to "+st1+". ";
    QMessageBox::information(this,"Attention",st2);
}

//根据已有信息绘图
using namespace QtCharts;
void MainWindow::paint_pic(int station1,int turn)
{

     QString str_st=QString::number(station1);

   /*  //对曲线进行基本信息的设定
    QSplineSeries *series1=new QSplineSeries();
    QSplineSeries *series2=new QSplineSeries();
    m_series.append(series1);
    m_series.append(series2);
    series1->setName("st"+str_st+"-in");
    series2->setName("st"+str_st+"-out");
    series1->setVisible(true);
    series2->setVisible(true);

    switch (turn) {

    case 1:series1->setColor(QColor( 0,255,255));
        series2->setColor(QColor(192,192,192));break;
    case 2:series1->setColor(QColor( 255,0,0));
        series2->setColor(QColor(255,255,0));break;
    case 3:series1->setColor(QColor( 255,0,255));
        series2->setColor(QColor(255,255,255));break;

    }
*/
   // tmp2.ti=QDateTime::fromString(tmp1.at(0),"yyyy-MM-dd HH:mm:ss");

    //转换并存储三种时间，创立time—table-in out 数组
    long long step_sec=abs(step.secsTo(QTime(0,0,0,0)));
   // qDebug()<<step_sec;
    long long starting_sec= starting.toSecsSinceEpoch();
    long long endding_sec=endding.toSecsSinceEpoch();
    QString start_str=starting.toString("yyyy-MM-dd HH:mm:ss");
    QString end_str=endding.toString("yyyy-MM-dd HH:mm:ss");

    QTime ttt;
    ttt.start();

   //qDebug()<<starting_sec;
  // qDebug()<<endding_sec;
    long long  time_size=(endding_sec-starting_sec)/step_sec;
    int *time_table_out=new int [time_size+1]{0};
    int *time_table_in=new int [time_size+1]{0};

    /*QStringList time_list ;
    time_list.push_back(start_str);
    QDateTime tmp=starting;
    QString tmp_str;
    while (tmp<=endding)
    {
       tmp= tmp.addSecs(step_sec);
       if(tmp>=endding) break;
       tmp_str=tmp.toString("yyyy-MM-dd HH:mm:ss");
       time_list.push_back(tmp_str);

    }
    time_list.push_back(end_str);
    for(int i=0;i<=time_size;++i)
        qDebug()<<i<<":"<<time_list.at(i);
*/

   //选择合适的数据，填入table；
    int i=0;
    int k=0;
    QString status="1";
    QVector<metro>::iterator it;
    it=container.begin();
    while(it!=container.end())
    {
        if (it->st1==station1)
        {    //qDebug()<<"fuck1";
             //long long time=it->ti.toSecsSinceEpoch();
            //qDebug()<<time;


            //if(time>=starting_sec && time<=endding_sec )
            if(it->tii>=start_str&&it->tii<=end_str)
            {
              long long place;
              QDateTime t = QDateTime::fromString(it->tii,
                                                     "yyyy-MM-dd HH:mm:ss");

             // qDebug()<<container.at(i).tii;
              place=(t.toSecsSinceEpoch()-starting_sec)/step_sec;
              //qDebug()<<"fuck2";

     /*           int low=0;
                int high=time_size;
                int i;
                while (1) {
                     i=(low+high)/2;
                    if(tstr>=time_list.at(i))
                    {
                        if(tstr<time_list.at(i+1))
                           {place=i;
                            break;}
                        else {
                            low=i+1;

                        }

                    }
                    else
                    {
                        if(tstr>=time_list.at(i-1))
                           {place=i-1;
                            break;}
                        else {
                            high=i-1;

                        }
                    }

                }
*/
              if(it->st2==status)
              {
                  time_table_in[place]=time_table_in[place]+1;
                 // qDebug()<<place<<" : in +1:"<<time_table_in[place];
              }
              else
              {
                  time_table_out[place]=time_table_out[place]+1;
                  //qDebug()<<place<<" : out +1"<<time_table_out[place];
              }
             //k=k+1;
             //if(k%1000==)
             // ui->pb2->setValue(k);

            }

        }
      it++;
    }
   /* for (int k=0;k<=time_size;++k) {
        qDebug()<<k<<"in:"<<time_table_in[k];

    }*/
    //ui->pb2->setValue(100);
    qDebug()<<"time_graphic"<<ttt.elapsed();

    //把table的数据加到曲线内。
    //对曲线进行基本信息的设定
   QSplineSeries *series1=new QSplineSeries();
   QSplineSeries *series2=new QSplineSeries();
   m_series.append(series1);
   m_series.append(series2);
   series1->setName("st"+str_st+"-in");
   series2->setName("st"+str_st+"-out");
   series1->setVisible(true);
   series2->setVisible(true);

   switch (turn) {

   case 1:series1->setColor(QColor( 0,255,255));
       series2->setColor(QColor(192,192,192));break;
   case 2:series1->setColor(QColor( 255,0,0));
       series2->setColor(QColor(255,255,0));break;
   case 3:series1->setColor(QColor( 255,0,255));
       series2->setColor(QColor(255,255,255));break;

   }
     i=0;
    //int maxx=0;
    while(i<=time_size)
    {
     QDateTime  t1=starting.addSecs(step_sec*i);
     long long t=t1.toSecsSinceEpoch();
     //qDebug()<<i<<" in ；"<<time_table_in[i];
     //qDebug()<<i<<" out ；"<<time_table_out[i];
//     if(time_table_in[i]>maxx) maxx=time_table_in[i];
     //if(time_table_out[i]>maxx) maxx=time_table_out[i];
     series1->append(t,time_table_in[i]);
     series2->append(t,time_table_out[i]);
     i++;
    }


    //添加两条曲线到chart
    chart->addSeries(series1);
    chart->addSeries(series2);


    chart->setTheme(QChart::ChartThemeBlueCerulean );//设置系统主题
    chart->setAnimationOptions(QChart::AllAnimations);//设置启用或禁用动画
    chart->setLocalizeNumbers(true);//数字是否本地化
    chart->legend()->show();//

    /*axisY = new QValueAxis;
    axisY->setGridLineVisible(true);
    axisY->setTitleText(str_st);
    axisY->setRange(0,maxx*1);
    //axisY->setLabelFormat("%u");
    axisY->setTickCount(6);
    chart->setAxisY(axisY,series1);
*/


    chart->createDefaultAxes();

     datex=new QDateTimeAxis;
     datex->setFormat("dd-HH:mm");
     datex->setRange(starting,endding);
     datex->setGridLineVisible(true);
     datex->setTickCount(12);
     chart->setAxisX(datex);



   /*// datex.show();
    chart->setAxisX(&datex,series1);
    chart->setAxisX(&datex,series2);
     QValueAxis axy;
     chart->setAxisY(&axy);
    //chart->addAxis (&datex,Qt::AlignBottom);
    //axy.show();
    axy.setVisible(1);
    //chart->addAxis(&axy,Qt::AlignLeft);
*/
    chart->show();
    chart->setTitle("Metro in-out line");//
    chart->setTitleBrush(QBrush(QColor(255,230,255)));//设置标题Brush
    QFont fo=chart->title();
    fo.setPointSizeF(10);
    chart->setTitleFont(QFont("微软雅黑"));//设置标题字体
     chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    //chart->legend()->detachFromChart();
    chart->legend()->setBackgroundVisible(true);//设置背景是否可视
    //chart->legend()->setAutoFillBackground(true);//设置背景自动填充
    //chart->legend()->setColor(QColor(255,128,128,128));//设置颜色
    //chart->legend()->setContentsMargins(10,10,10,10);//设置边距left,top,right,bottom
    chart->legend()->setLabelColor(QColor(255,128,255));//设置标签颜色
    chart->legend()->setVisible(true);//设置是否可视

    chart->legend()->setBorderColor(QColor(255,255,170,185));//设置边框颜色
    QFont font = chart->legend()->font();
    font.setItalic(!font.italic());
    chart->legend()->setFont(font);//设置字体为斜体
    font.setPointSizeF(11);
    chart->legend()->setFont(font);//设置字体大小
    chart->legend()->setFont(QFont("微软雅黑"));//设置字体类型

}

//用chartview display 图像
void MainWindow::display()
{// QWidget* wn=new QWidget(this);
    QChartView *chartView = new QChartView(chart,ui->graphicsView);
    //wn->resize(ui->graphicsView->width(),ui->graphicsView->height());
    chartView->resize(ui->graphicsView->width(),ui->graphicsView->height());
    //chartView->setRenderHint(QPainter::Antialiasing);
    chartView->show();
    //chartView->move(ui->graphicsView->x(),ui->graphicsView->y());




}

//按下search 搜索map
void MainWindow::on_pushButton_clicked()
{
    if(map_loaded==0)
    {
        QMessageBox::warning(this,"warning","The map has not been loaded");
        return;
    }

    int num=ui->spinBox->value();
    int des=ui->spinBox_2->value();

    int *pre=new int [map_size];
    pre[num]=-1;
    bool *vis=new bool [map_size]{false};
    map_str.clear();
    search(num,des,pre,vis);
    ui->textBrowser->setText(map_str);
    QFont font;
    font.setPointSizeF(14);
    ui->textBrowser->setFont(font);
}

//用dfs得到path
void MainWindow:: search(int num,int des,int *pre,bool* vis){




    if (num==des)
    {
        while (pre[num]!=-1)
        {
            v.push_front(num);
            num = pre[num];

        }
        v.push_front(num);

        QList<int>::const_iterator p;

        //ui->textBrowser->setText();


        for(p=v.begin();(p+1)!=v.end();p++)
        {
            map_str=map_str+QString::number(*p)+"-->";

        }
        p=v.end()-1;
        map_str+=QString::number(*p)+'\n';
        map_str+=QString("--------------------")+'\n';


        v.clear();
        //qDebug()<<"----------";
        return;
    }
    vis[num]=true;

    for(int i=0;i<map_size;++i)
    {

        if(map_table[num][i]==1&&vis[i]==0)
        {
            pre[i]=num;
            search(i,des,pre,vis);
        }
    }
    vis[num]= false;
}

//对忙碌的站点进行排序
void MainWindow::on_pushButton_2_clicked()
{
    //if()
   QString start_str=ui->start_time_2->dateTime().toString("yyyy-MM-dd HH:mm:ss");
    QString end_str=ui->end_time_2->dateTime().toString("yyyy-MM-dd HH:mm:ss");
    QDateTime start=ui->start_time_2->dateTime();
    QDateTime end=ui->end_time_2->dateTime();
    struct busy_st
    {
        int number;
        int st;
    }  ;
    busy_st *bs=new busy_st[map_size];
    for (int i=0;i<map_size;++i) {

        bs[i].st=i;
        bs[i].number=0;
    }
    QVector<metro>::Iterator it=container.begin();

    QTime t;
    t.start();
    if(total)
    {
        while(it!=container.end())
        {

            if(it->tii>=start_str&&it->tii<=end_str)
            {
                bs[it->st1].number+=1;

            }

            it++;

        }
    }
    else if(analy_in)
    {
        while(it!=container.end())
        {
            if(it->st2=="1")
            if(it->tii>=start_str&&it->tii<=end_str)
            {
                bs[it->st1].number+=1;

            }

            it++;

        }
    }
    else
    {
        while(it!=container.end())
        {
            if(it->st2!="1")
            if(it->tii>=start_str&&it->tii<=end_str)
            {
                bs[it->st1].number+=1;

            }

            it++;

        }
    }

    qDebug()<<"busiest_station: "<<t.elapsed();
    std::sort(bs,bs+map_size,[](busy_st a, busy_st b)->bool{return a.number>b.number;});


    QString str;
    for(int i=0;i<map_size;++i)
    {
        if(bs[i].st<=9)
        str+="Station_0"+QString::number(bs[i].st) +" : "+QString::number( bs[i].number)+"\n";
        else
        str+="Station_"+QString::number(bs[i].st) +" : "+QString::number( bs[i].number)+"\n";
    }
    ui->busyst->setText(str);
    QFont font;
    font.setPointSizeF(12);
    ui->busyst->setFont(font);


}

//对忙碌的线路进行排序并绘制饼图
void MainWindow::on_pushButton_4_clicked()
{
    QString start_str=ui->start_time_2->dateTime().toString("yyyy-MM-dd HH:mm:ss");
    QString end_str=ui->end_time_2->dateTime().toString("yyyy-MM-dd HH:mm:ss");
    QDateTime start=ui->start_time_2->dateTime();
    QDateTime end=ui->end_time_2->dateTime();
    int number[3]={0};
    QVector<metro>::Iterator it=container.begin();

    QTime t;
    t.start();
    if(total)
    {
        while(it!=container.end())

        {
           if(it->tii>=start_str&&it->tii<=end_str)
         {
            if(it->li=="A")
               number[0]+=1;
            else if(it->li=="B")
               number[1]+=1;
            else if(it->li=="C")
               number[2]+=1;



        }
        it++;
        }
    }

    else if(analy_in)
    {
        while(it!=container.end())
        {
            if(it->tii>=start_str&&it->tii<=end_str)
            {
            if(it->st2=="1")
           {
                if(it->li=="A")
               number[0]+=1;
            else if(it->li=="B")
               number[1]+=1;
            else if(it->li=="C")
               number[2]+=1;
            }
        }
            it++;
        }
    }
    else
    {
        while(it!=container.end())
        {
            if(it->tii>=start_str&&it->tii<=end_str)
            {
            if(it->st2=="0")
           {
                if(it->li=="A")
               number[0]+=1;
            else if(it->li=="B")
               number[1]+=1;
            else if(it->li=="C")
               number[2]+=1;
            }
        }
            it++;
        }
    }
     qDebug()<<"busiest_line: "<<t.elapsed();
  // qDebug()<<"1";
   double tot=number[0]+number[1]+number[2];

    QPieSeries * series=new QPieSeries;
    QStringList name;
    name.append("A");
    name.append("B");
    name.append("C");
    for(int k=0;k<3;++k)
    {
        QString str=QString::number(number[k]*100/tot,'f',3);
        str=name.at(k)+": "+str;
        series->append(str,number[k]);


    }
    series->setLabelsVisible();

    for(int k=0;k<3;++k)
    {
         QPieSlice *slice=series->slices().at(k);

        slice->setColor(QColor(255/(k*k+1),255-255/(k*k+1),255/(k+1),255));


    }
   //qDebug()<<"2";
   QChart *chart = new QChart();

    chart->addSeries(series);

    chart->setTitle("Busiest Line ");

    chart->legend()->hide();

    QChartView *chartView = new QChartView(chart,ui->busyline);
    //wn->resize(ui->graphicsView->width(),ui->busyline->height());
    chartView->resize(ui->busyline->width(),ui->busyline->height());
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->show();

    //QChartView *chartview = new QChartView(chart);

    //chartview->setRenderHint(QPainter::Antialiasing);

}

//点下in_check；
void MainWindow::on_in_check_clicked()
{
    analy_in=1;
    total=0;
}

//点下out_check
void MainWindow::on_out_check_clicked()
{
    analy_in=0;
    total=0;
}

//点下 total——check
void MainWindow::on_total_clicked()
{
    analy_in=0;
    total=1;
}

void MainWindow::on_actionExit_X_triggered()
{
    this->close();
}

//查看function帮助
void MainWindow::on_actionFunctions_of_every_part_triggered()
{
    QWidget *w=new QWidget;
    QTextBrowser *tb;
    tb=new QTextBrowser (w);
    w->show();
    w->resize(600,400);
    tb->resize(600,400);
    tb->show();
    tb->setVisible(1);
    QString str;
    str=QString("   This application is used to anaylze the metro data, you are supposed to load "
                "the map and data files first. After that you can use the several functions to analyze.\n"
                "   1. The filter is used before the file-loaded, it helps you to "
                "choose the information you want to load, but "
                "some necessary fileds can not be unckecked.\n"
                "   2. The graphics is used to paint a picture about the flow of certain stations.\n"
                "   3. The analzye can show you the busiest station and busiet line during "
                "a period of time ,which"
                " is important to the mannager to arrange the metro_system.\n"
                "   4. The map can help you find the path from one station to another one.  ");
    tb->setText(str);
    QFont font;
    font.setPointSize(12);
    tb->setFont(font);
    w->setWindowTitle("Function help");

}

void MainWindow::on_actionAbout_filter_and_load_triggered()
{
    QWidget *w=new QWidget;
    QTextBrowser *tb;
    tb=new QTextBrowser (w);
    w->show();
    w->resize(600,400);
    tb->resize(600,400);
    tb->show();
    tb->setVisible(1);
    QString str;
    str=QString("   When you want to load the files, you can choose the fields in the filter(some necessary"
                " fields are not allowed to uncheck),and then clicked the File in menu bar to "
                "choose the files. "
                "Besides,you are advised to load the map_file first,which can tell you the total stations when"
                " you are using the graphics.");
    tb->setText(str);
    QFont font;
    font.setPointSize(12);
    tb->setFont(font);
    w->setWindowTitle("File help");
}
