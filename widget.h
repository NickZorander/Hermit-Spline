#ifndef WIDGET_H
#define WIDGET_H
#include<QtWidgets>
#include<QPainter>
#include<QPaintEvent>

class MyWidget : public QWidget
{
    Q_OBJECT
public:
    QVector <QPointF> SplinePoints;
    QVector <QPointF> StartPoints;
    QVector <QPointF> Derivateves;

    int SplinePointsIterator=0;
    int N;

    bool change_derevative_flag;
    int change_derivative_index=0;

    bool change_point_flag;
    int change_point_index=0;

    QPointF cursor;


    void BuildSplineOnTwoPoints(const QPointF& p1, const QPointF& p2, const QPointF& d1, const QPointF& d2 ); //добавляет в массив SplinePoints некоторое количество точек сплайна, соединяющего две исходные точки
    void BuildSpline(); //заполняет массив SplinePoints достаточным для отрисовки количеством точек
    void InitData(); //инициализация данных
    void PrintInfo();

    MyWidget(QWidget* p=0) {}
    virtual ~MyWidget() {}
protected:
     void paintEvent(QPaintEvent*e);
     void mouseReleaseEvent(QMouseEvent* pe);
     void mousePressEvent(QMouseEvent *pe);
     void mouseMoveEvent(QMouseEvent *pe);
};



#endif // WIDGET_H
