#include "widget.h"
#include <QApplication>
#include<QPainter>
#include<QPaintEvent>
#include<QtWidgets>
#include<cstdlib>
#include <QtDebug>



void MyWidget::paintEvent(QPaintEvent* e){

    QPainter painter(this);
    int i;

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black));

    QVector<QPointF>::iterator it=SplinePoints.begin();

    painter.drawPolyline(it, SplinePoints.size());

    painter.setPen(QPen(Qt::yellow, 6));
    painter.drawPoint(cursor);

    for (int i=0; i<=N-1; ++i)
    {
        painter.drawPoint(StartPoints[i]);
        painter.drawPoint(Derivateves[i]+StartPoints[i]);
    }

    painter.setPen(QPen(Qt::green,2));
    for (int i=0; i<=N-1; ++i)
        painter.drawLine(StartPoints[i], Derivateves[i]+StartPoints[i]);
}





void MyWidget::mouseReleaseEvent(QMouseEvent* pe)
{
    QPointF Q(pe->x(),pe->y());
    qDebug() << "new point " <<Q;

    if (change_point_flag)
        {
            StartPoints[change_point_index]=Q;
            BuildSpline();
            repaint();
        }

    if (change_derevative_flag)
        {
            Derivateves[change_derivative_index]=Q-StartPoints[change_derivative_index];
            BuildSpline();
            repaint();
        }


    change_point_flag=false;
    change_derevative_flag=false;
}



qreal distance (const QPointF& p1, const QPointF& p2)
{
    return qSqrt(  (p2.x()-p1.x())*(p2.x()-p1.x()) + (p2.y()-p1.y())*(p2.y()-p1.y()) );
}





void MyWidget::mousePressEvent(QMouseEvent *pe)
{
    change_point_flag=false;
    change_derevative_flag=false;

    QPointF Q(pe->x(),pe->y());


    for (int i=0; i<=N-1; ++i)
        {
            if (distance(Q, StartPoints[i]) <=7)
                {
                    change_point_index=i;
                    change_point_flag=true;
                    qDebug()<<"pressed point: "<< Q;
                    qDebug()<<"array point: "<< StartPoints[i];
                    break;
                }

            if (distance(Q, StartPoints[i]+Derivateves[i]) <=7)
                {
                    change_derivative_index=i;
                    change_derevative_flag=true;
                    qDebug()<<"pressed point: "<< Q;
                    qDebug()<<"array point: "<< Derivateves[i];
                    break;
                }
        }
}


void MyWidget::mouseMoveEvent(QMouseEvent *pe)
{
    cursor=QPointF(pe->x(), pe->y());
    repaint();
}



void MyWidget::InitData()
{
    QFile inputfile("D:\\QT\\Hermit_Spline\\points.txt");

    if (!inputfile.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::information(this,"Error", "path not found!");
        return;
    } //проверили на открытие

    QTextStream stream(&inputfile);

    QString A =stream.readAll(); //сделали содержимое файла строчкой

    QStringList list = A.split(' ', QString::SkipEmptyParts); //распилили строку в список где каждый элемент - одна координата точки
    bool ok;

    N=list[0].toInt(&ok); //первым числом передается число точек

    StartPoints.resize(N);
    SplinePoints.resize((N-1)*100);
    Derivateves.resize(N);

    for (int i=0; i<=N-1; ++i)
        {
            QPointF p(list[i*2+1].toInt(&ok), list[i*2+2].toInt(&ok));
            StartPoints[i]=p;
        } //создали массив точек


    Derivateves[0]=Derivateves[N-1]=QPointF(0,0);

    for (int i=1; i<=N-2; ++i )
        {
            QPointF p= StartPoints[i+1]-StartPoints[i-1];
            Derivateves[i]=p;
        } //инициализировали производные

    change_point_flag=false;
    change_derevative_flag=false;
}


//____________________________________________________________________________________________________________________________________________________

QPointF P (qreal t,  QPointF p0, QPointF p1, QPointF D0, QPointF D1 )
{
    QPointF A(0,0);
    A+=(p0*=(2*t*t*t - 3*t*t + 1));
    A+=(p1*=(-2*t*t*t + 3*t*t));
    A+=(D0*=(t*t*t - 2*t*t + t));
    A+=(D1*=(t*t*t - t*t));
    return A;
}


void MyWidget::BuildSplineOnTwoPoints(const QPointF& p1, const QPointF& p2, const QPointF& d1, const QPointF& d2 )
 {
    qreal t=0;
    for (int i=1; i<=100; ++i, t+=0.01)
         {
             SplinePoints[SplinePointsIterator]=P(t,p1,p2,d1,d2);
             ++SplinePointsIterator;
         }
 }

void MyWidget::BuildSpline()
{
    PrintInfo();
    SplinePointsIterator=0;
    for (int i=1; i<=N-1; ++i )
    {
        BuildSplineOnTwoPoints(StartPoints[i-1], StartPoints[i], Derivateves[i-1], Derivateves[i]);
    }
    qDebug()<<"Spline builded!";
    PrintInfo();
}


void MyWidget::PrintInfo()
{
    qDebug()<< "Points: ";
    for (int i=0; i<=N-1;++i)
        qDebug()<<StartPoints[i];


    qDebug() << endl << "Derivatives: ";
    for (int i=0; i<=N-1;++i)
        qDebug()<<Derivateves[i];

    qDebug()<<"change_point_flag "<< change_point_flag;
    qDebug()<<"change_point_index "<< StartPoints[change_point_index];
    qDebug()<<"change_derevative_flag "<< change_derevative_flag;
    qDebug()<<"change_derivative_index "<<Derivateves[change_derivative_index];
    qDebug() << "______________________"<< endl;
}





































