#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QEvent>
#include "config.h"
//#pragma execution_characters_set("utf-8")
class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(QWidget *parent = nullptr);
    GraphicsView(QGraphicsScene *scene, QWidget *parent = nullptr);
#if QT_CONFIG(wheelevent)
    void wheelEvent(QWheelEvent *) override;
#endif
    void setIsWheel(bool isScroll);//是否可以滚动

    //多边形面积
    qreal PolygonArea(QList<QPointF>&posList);

    QSize viewportSizeHint();

    QRect viewPortRect();

    void viewPortSize(QSize &size);

    void smallWinSize(QSize &size);
protected:
    bool event(QEvent *event);
//    void paintEvent(QPaintEvent *event);
signals:
    void signalTransScale(qreal scale);

    void sinalCreateNavigaBar(qreal scale);//创建缩略图
public slots:
    void slot_MoveView(QRect rect);
//protected:
//    void mouseMoveEvent(QMouseEvent *event);
//    void mousePressEvent(QMouseEvent *event);
//    void mouseReleaseEvent(QMouseEvent *event);
private:
//    QPointF beginPoint;
    bool m_isScroll;//是否可以执行滚轮放大缩小事件
    QPointF previous_touch_point0;
    QPointF previous_touch_point1;
    int touch_status = 0;
    qreal m_scale;//保存缩放比
    qreal firstArea;//刚接触时多边形面积
    Config *config;
    qreal upLimit;//放大的上限
    qreal lowLimit;//缩小的下限
    QSize m_viewPortWH;//视口的宽高
    QSize m_smallWinWH;//小窗口的宽高
};


#endif // GRAPHICSVIEW_H
