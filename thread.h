#ifndef CONNECTION_H
#define CONNECTION_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include<QDebug>
//#include"mainwindow.h"
#include<QTime>
#include<QString>
#include<QObject>

/*QString to_string(int tmp)
 {
    QString st;
     while(tmp>0)
    {
        int  ch=tmp%10-0+'0';
        st.push_front(ch);
        tmp/=10;
    }
    return st;
}
*/

class mythread : public QObject
{
    Q_OBJECT
public:
    explicit mythread(QObject *parent = nullptr);

    //void closeThread();

signals:
    void notify(int);
   // void notify1(int);
public slots:
    void load_file_slot();
    //void pic_make_slot();
private:
    //volatile bool isStop;
};




#endif // CONNECTION_H
