#include "graphicsview.h"
#include "log.hpp"
#include <QWheelEvent>
#include <QScrollBar>
#include <QTouchEvent>
#include <QLineF>


#if QT_CONFIG(wheelevent)
void GraphicsView::wheelEvent(QWheelEvent *e)
{
    if(!m_isScroll)
    {
        return ;
    }

    if (e->modifiers() & Qt::ControlModifier)
    {
        qreal curScale = m_scale;
        if (e->delta() > 0)//远离
        {
            if(curScale < upLimit)
            {
               m_scale *= 1.2;
               emit sinalCreateNavigaBar(m_scale);
               scale(1.2,1.2);
//               m_scale *= 1.2;
            }
        }
        else
        {
            if(curScale > lowLimit)
            {
                m_scale *= 0.8;
                emit sinalCreateNavigaBar(m_scale);
                scale(0.8,0.8);

            }
        }

        e->accept();
    }
    else
    {
        QGraphicsView::wheelEvent(e);
    }
//    if(m_scale > 2)
//    {
        emit sinalCreateNavigaBar(m_scale);
//    }
//    emit signalTransScale(1/m_scale);
//    outPut<<"视图缩放的比例："<<1/m_scale;
}
#endif

GraphicsView::GraphicsView(QWidget *parent):QGraphicsView(parent),m_isScroll(true),touch_status(0)
  ,m_scale(1),firstArea(1)
{
    config = new Config();
    upLimit = config->Get("viewScale","upScale").toFloat();
    lowLimit = config->Get("viewScale","lowScale").toFloat();
    outPut<<"upLimit:"<<upLimit<<" "<<"lowLimit:"<<lowLimit;
    previous_touch_point0 = QPointF(0.0,0.0);
    previous_touch_point1 = QPointF(0.0,0.0);
    setAttribute(Qt::WA_AcceptTouchEvents);//捕获touch事件
//    setRenderHint(QPainter::Antialiasing, false);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setOptimizationFlags(QGraphicsView::DontSavePainterState);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
}

GraphicsView::GraphicsView(QGraphicsScene *scene, QWidget *parent)
    :QGraphicsView(scene,parent),m_isScroll(true),touch_status(0),m_scale(1),firstArea(1)
{
    config = new Config();
    upLimit = config->Get("viewScale","upScale").toFloat();
    lowLimit = config->Get("viewScale","lowScale").toFloat();
    outPut<<"upLimit:"<<upLimit<<" "<<"lowLimit:"<<lowLimit;
    previous_touch_point0 = QPointF(0.0,0.0);
    previous_touch_point1 = QPointF(0.0,0.0);
    setAttribute(Qt::WA_AcceptTouchEvents);//捕获touch事件
//    setRenderHint(QPainter::Antialiasing, false);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setOptimizationFlags(QGraphicsView::DontSavePainterState);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
}

void GraphicsView::setIsWheel(bool isScroll)
{
    m_isScroll = isScroll;
}

qreal GraphicsView::PolygonArea(QList<QPointF> &posList)
{
    QList<QPointF> tarPos;
    QList<QPointF> optList = posList;
    int nLen = posList.size();
    if(nLen==3)
    {
        tarPos = posList;
    }
    else if(nLen>3)
    {
        for(int i=0;i<optList.size()-1;++i)//按x点进行排序
        {
            for(int j=i+1;j<optList.size();++j)
            {
                if(optList[j].x()<optList[i].x())
                {
                    optList.swap(i,j);
                }
            }
        }

        for(int i=0;i<3-1;++i)
        {
            for(int j=i+1;j<3;++j)
            {
                if(optList[j].y()<optList[i].y())
                {
                    optList.swap(i,j);
                }
            }
            tarPos.push_back(optList[i]);
        }
        tarPos.push_back(optList[2]);
        for(int i=3;i<optList.size()-1;++i)
        {
            for(int j=i+1;j<optList.size();++j)
            {
                if(optList[j].y()>optList[i].y())
                {
                    optList.swap(i,j);
                }
            }
            tarPos.push_back(optList[i]);
        }
        tarPos.push_back(optList[optList.size()-1]);
    }
    qreal s=1;
    for(int k=0;k<nLen;++k)
    {
        QPointF& pos1 = tarPos[k];
        QPointF& pos2 = tarPos[(k+1)%nLen];
        s +=(pos1.x()*pos2.y()-pos2.x()*pos1.y());
    }
    s=abs(0.5*s);
    return s;
}

QSize GraphicsView::viewportSizeHint()
{
    return viewport()->size();
}

QRect GraphicsView::viewPortRect()
{
    return viewport()->rect();
}

void GraphicsView::viewPortSize(QSize &size)
{
    m_viewPortWH = size;
    outPut<<"视口："<<m_viewPortWH;
}

void GraphicsView::smallWinSize(QSize &size)
{
    m_smallWinWH = size;
    outPut<<"窗口："<<m_smallWinWH;
}

bool GraphicsView::event(QEvent *event)
{
    switch(event->type())
    {
        case QEvent::TouchBegin:
        {
                QTouchEvent* touch = static_cast<QTouchEvent*>(event);
                QList<QTouchEvent::TouchPoint> touchPoints = touch->touchPoints();
                outPut<<"TouchBegin点的个数为："<<touchPoints.count();
                return true;
         }
        case QEvent::TouchUpdate:
        {
              QTouchEvent *touchEvent = static_cast<QTouchEvent *>(event);
              QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();
              outPut<<"TouchUpdate点的个数为："<<touchPoints.count();
              if (touchPoints.count() == 2 /*&& touch_status == 1*/)
              {
                  if(touchEvent->touchPointStates() & Qt::TouchPointPressed)
                 {
                     const QTouchEvent::TouchPoint &touchPoint0 = touchPoints.first();
                     const QTouchEvent::TouchPoint &touchPoint1 = touchPoints.last();
                     previous_touch_point0.setX(touchPoint0.pos().x());
                     previous_touch_point0.setY(touchPoint0.pos().y());
                     previous_touch_point1.setX(touchPoint1.pos().x());
                     previous_touch_point1.setY(touchPoint1.pos().y());
                     outPut<<"刚开始点：("<<previous_touch_point0.x()<<","<<previous_touch_point0.y()<<")"
                            <<"("<<previous_touch_point1.x()<<","<<previous_touch_point1.y()<<")";
                     touch_status=2;
                  }
                  if(touchEvent->touchPointStates() & Qt::TouchPointMoved)
                  {
                      if(touch_status == 2)
                      {
                          const QTouchEvent::TouchPoint &touchPoint0 = touchPoints.first();
                          const QTouchEvent::TouchPoint &touchPoint1 = touchPoints.last();
                          qreal currentScaleFactor =QLineF(touchPoint0.pos(), touchPoint1.pos()).length()
                                  / QLineF(previous_touch_point0, previous_touch_point1).length();
//                          qreal  proper = currentScaleFactor>1?1.05:0.8;

                          outPut<<"起点："<<touchPoint0.pos()<<touchPoint1.pos();
                          outPut<<"移动后：("<<previous_touch_point0.x()<<","<<previous_touch_point0.y()<<")"
                                 <<"("<<previous_touch_point1.x()<<","<<previous_touch_point1.y()<<")";
                          outPut<<"起点线长："<<QLineF(touchPoint0.pos(), touchPoint1.pos()).length();
                          outPut<<"移动后线长："<<QLineF(previous_touch_point0, previous_touch_point1).length();
                          outPut<<"缩放参考点为："<<currentScaleFactor;
                          if(currentScaleFactor > 1)
                          {
                              if(m_scale < upLimit)
                              {
                                scale(1.2,1.2);
                                m_scale *= 1.2;
                              }
                          }
                          else
                          {
                              if(m_scale > lowLimit)
                              {
                                scale(0.8,0.8);
                                m_scale *= 0.8;
                              }
                          }

//                          scale(proper,proper);
                          previous_touch_point0.setX(touchPoint0.pos().x());
                          previous_touch_point0.setY(touchPoint0.pos().y());
                          previous_touch_point1.setX(touchPoint1.pos().x());
                          previous_touch_point1.setY(touchPoint1.pos().y());
                      }
                  }
              }
               if (touchPoints.count() == 3)
               {
                   if(touchEvent->touchPointStates() & Qt::TouchPointPressed)
                   {
                       QList<QPointF> pointList;
                       QPointF siglePoint;
                       for(int i = 0; i < touchPoints.size(); ++i)
                       {
                           siglePoint = touchPoints[i].pos();
                           pointList.push_back(siglePoint);
                       }
                       firstArea = PolygonArea(pointList);
                       touch_status = 3;
                   }
                   if(touchEvent->touchPointStates() & Qt::TouchPointMoved)
                   {
                        if(touch_status == 3)
                        {
                            QList<QPointF> pointList;
                            QPointF siglePoint;
                            for(int i = 0; i < touchPoints.size(); ++i)
                            {
                                siglePoint = touchPoints[i].pos();
                                pointList.push_back(siglePoint);
                            }

                            qreal lastArea = PolygonArea(pointList);
                            qreal currentScaleFactor = lastArea / firstArea;
                            outPut<<"刚开始面积："<<firstArea;
                            outPut<<"变化后面积："<<lastArea;
                            outPut<<"面积比："<<currentScaleFactor;
//                            qreal  proper = currentScaleFactor>1?1.05:0.8;
//                            scale(proper,proper);
                            if(currentScaleFactor > 1)
                            {
                                if(m_scale < upLimit)
                                {
                                  scale(1.2,1.2);
                                  m_scale *= 1.2;
                                }
                            }
                            else
                            {
                                if(m_scale > lowLimit)
                                {
                                  scale(0.8,0.8);
                                  m_scale *= 0.8;
                                }
                            }
                            firstArea = lastArea;
                        }
                   }
               }
               if (touchPoints.count() == 4)
               {
                   if(touchEvent->touchPointStates() & Qt::TouchPointPressed)
                   {
                       QList<QPointF> pointList;
                       QPointF siglePoint;
                       for(int i = 0; i < touchPoints.size(); ++i)
                       {
                           siglePoint = touchPoints[i].pos();
                           pointList.push_back(siglePoint);
                       }
                       firstArea = PolygonArea(pointList);
                       touch_status = 4;
                   }
                   if(touchEvent->touchPointStates() & Qt::TouchPointMoved)
                   {
                        if(touch_status == 4)
                        {
                            QList<QPointF> pointList;
                            QPointF siglePoint;
                            for(int i = 0; i < touchPoints.size(); ++i)
                            {
                                siglePoint = touchPoints[i].pos();
                                pointList.push_back(siglePoint);
                            }

                            qreal lastArea = PolygonArea(pointList);
                            qreal currentScaleFactor = lastArea / firstArea;
                            outPut<<"刚开始面积："<<firstArea;
                            outPut<<"变化后面积："<<lastArea;
                            outPut<<"面积比："<<currentScaleFactor;
//                            qreal  proper = currentScaleFactor>1?1.05:0.8;
//                            scale(proper,proper);
                            if(currentScaleFactor > 1)
                            {
                                if(m_scale < upLimit)
                                {
                                  scale(1.2,1.2);
                                  m_scale *= 1.2;
                                }
                            }
                            else
                            {
                                if(m_scale > lowLimit)
                                {
                                  scale(0.8,0.8);
                                  m_scale *= 0.8;
                                }
                            }
                            firstArea = lastArea;
                        }
                   }
               }
               if (touchPoints.count() == 5)
               {
                   if(touchEvent->touchPointStates() & Qt::TouchPointPressed)
                   {
                       QList<QPointF> pointList;
                       QPointF siglePoint;
                       for(int i = 0; i < touchPoints.size(); ++i)
                       {
                           siglePoint = touchPoints[i].pos();
                           pointList.push_back(siglePoint);
                       }
                       firstArea = PolygonArea(pointList);
                       touch_status = 5;
                   }
                   if(touchEvent->touchPointStates() & Qt::TouchPointMoved)
                   {
                        if(touch_status == 5)
                        {
                            QList<QPointF> pointList;
                            QPointF siglePoint;
                            for(int i = 0; i < touchPoints.size(); ++i)
                            {
                                siglePoint = touchPoints[i].pos();
                                pointList.push_back(siglePoint);
                            }

                            qreal lastArea = PolygonArea(pointList);
                            qreal currentScaleFactor = lastArea / firstArea;
                            outPut<<"刚开始面积："<<firstArea;
                            outPut<<"变化后面积："<<lastArea;
                            outPut<<"面积比："<<currentScaleFactor;
//                            qreal  proper = currentScaleFactor>1?1.05:0.8;
//                            scale(proper,proper);
                            if(currentScaleFactor > 1)
                            {
                                if(m_scale < upLimit)
                                {
                                  scale(1.2,1.2);
                                  m_scale *= 1.2;
                                }
                            }
                            else
                            {
                                if(m_scale > lowLimit)
                                {
                                  scale(0.8,0.8);
                                  m_scale *= 0.8;
                                }
                            }
                            firstArea = lastArea;
                        }
                   }
               }
              else if(touchEvent->touchPointStates() & Qt::TouchPointReleased)
              {
                  touch_status=0;
              }
              event->accept();
              return true;
           }
        case QEvent::TouchEnd:
          {
                event->accept();
                return true;
           }
        default:
            break;
          }
    return QGraphicsView::event(event);//QWidget::event(event);
}

void GraphicsView::slot_MoveView(QRect rect)
{
    verticalScrollBar()->setValue(rect.y()*m_scale*m_viewPortWH.height()/m_smallWinWH.height());
    horizontalScrollBar()->setValue(rect.x()*m_scale*m_viewPortWH.width()/m_smallWinWH.width());
}

//void GraphicsView::mouseMoveEvent(QMouseEvent *event)
//{
//    if(event->buttons() & Qt::LeftButton)
//    {
//        QPointF pos = event->screenPos();
//        qreal x = pos.x() - beginPoint.x();
//        qreal y = pos.y() - beginPoint.y();

//        QScrollBar* verBar = verticalScrollBar();
//        int nValue = verBar->value();
//        verBar->setValue(nValue - y);

//        QScrollBar* horBar = horizontalScrollBar();
//        nValue = horBar->value();
//        horBar->setValue(nValue - x);
//    }
//    QGraphicsView::mouseMoveEvent(event);
//}

//void GraphicsView::mousePressEvent(QMouseEvent *event)
//{
//    if(event->button() == Qt::LeftButton)
//    {
//        beginPoint = event->screenPos();
//    }
//    QGraphicsView::mousePressEvent(event);
//}

//void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
//{
//    QGraphicsView::mouseReleaseEvent(event);
//}
