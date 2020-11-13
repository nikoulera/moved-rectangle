#include "breviarydlg.h"
#include <QPalette>
#include <QPainter>
#include <QDebug>
#include <QGraphicsRectItem>
#include "MyGraphicsScene.h"
#include <QGraphicsView>

BreviaryDlg::BreviaryDlg(QWidget *parent, Qt::WindowFlags f)
    :QDialog(parent,f)/*,isUpdate(false)*/
{
    resize(SMALL_W,SMALL_H);
    setWindowFlag(Qt::FramelessWindowHint);//因为是场景所以加载背景会被场景覆盖，另外设置了无边框之后，场景也会覆盖窗口可以不用重绘窗口
    QPalette palette = this->palette();
    palette.setColor(QPalette::Background,QColor(34,34,34));
    setWindowOpacity(0.8);
    setPalette(palette);

    scene = new MyGraphicsScene(this);
//    scene->setBackgroundBrush(QBrush(QColor(Qt::red)));
//    scene->setForegroundBrush(QBrush(QColor(Qt::red)));
//    scene->setSceneRect(0,0,400,400);
    view = new QGraphicsView(scene,this);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setGeometry(0,0,SMALL_W,SMALL_H);
    item = NULL;
}

void BreviaryDlg::setSize(int w, int h)
{
//    resize(w,h);
    setFixedSize(w,h);
}

void BreviaryDlg::setPosSize(int x, int y, int w, int h)
{
    setGeometry(x,y,w,h);
}

QSize BreviaryDlg::getSize()
{
    return QSize(SMALL_W,SMALL_H);
}

void BreviaryDlg::setPixmap(QPixmap &pix)
{
    m_pixmap = pix;
//    isUpdate = true;
    qDebug()<<"setPixmap";
    if (item != NULL)
    {
        scene->removeItem(item);
        delete item;
    }
    if(m_pixmap.isNull())
    {
        qDebug()<<"pixmap is null";
    }/*加载Pixmap图片加载不上去，画布的大小与图片的尺寸不一*/
    item = scene->addPixmap(m_pixmap);//QPixmap(":/new/prefix1/11.png")
    fitInView();
}

void BreviaryDlg::fitInView()
{
    QRectF bounds = view->scene()->itemsBoundingRect();
    view->fitInView(bounds, Qt::KeepAspectRatio);
}

void BreviaryDlg::transViewPtr(GraphicsView *view)
{
    m_view1 = new GraphicsView;
    m_view1 = view;
    connect(scene,&MyGraphicsScene::previewRectMoved,m_view1,&GraphicsView::slot_MoveView);
}

void BreviaryDlg::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);
    fitInView();
}

//void BreviaryDlg::paintEvent(QPaintEvent *event)
//{
//    QPainter painter(this);
//    QPen pen(QColor(34,34,34));//34,34,34
//    pen.setWidth(2);
//    pen.setStyle(Qt::SolidLine);

//    painter.drawRect(0,0,SMALL_W,SMALL_H);
//}

void BreviaryDlg::slot_setRectSize(QRect &rect)
{
     m_rect = rect;
     scene->onSetPreviewRect(rect);
}
