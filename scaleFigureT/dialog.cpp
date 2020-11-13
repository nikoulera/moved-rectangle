#include "dialog.h"
#include "ui_dialog.h"
#include "log.hpp"
#include <QScrollBar>
#include <QSize>
#include <QGraphicsProxyWidget>
#include <QMediaPlayer>
#include <QVideoWidget>
//#include <QGraphicsRectItem>
#include <QGraphicsVideoItem>
#include <QPainter>


Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
    ,m_x(0)
    ,m_y(0)
    ,m_viewSize(100,100)
{
    ui->setupUi(this);
    resize(QSize(1200,1000));
    scene = new QGraphicsScene(this);
    view = new GraphicsView(scene,this);//设置了视图的父窗口之后就不需要调用show()
//    scene->setSceneRect(0,0,800,800);
//    view->setGeometry(0,0,800,800);
//    view->show();
    //场景中播放视频
//    QGraphicsRectItem *item = new QGraphicsRectItem(0,0,500,500);//item中手动嵌入的窗口的大小不会随item大小一致

//    video = new QVideoWidget(this);
//    video->setWindowFlag(Qt::WindowStaysOnTopHint);
//    video->resize(500,500);
//    player = new QMediaPlayer(this);
//    player->setVideoOutput(video);
//    player->setMedia(QUrl::fromLocalFile("F:\\video\\汉化日记：第2话.mp4"));
//    player->play();

//    QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget(item);
//    proxy->setWidget(video);
//    scene->addItem(item);

    //场景中播放视频的正确打开方式
    QGraphicsVideoItem *item = new QGraphicsVideoItem();
    scene->addItem(item);
    item->setSize(QSizeF(800.0,800.0));
    player = new QMediaPlayer(this);
    player->setVideoOutput(item);
    player->setMedia(QUrl::fromLocalFile("F:\\video\\汉化日记：第2话.mp4"));
    player->play();
    connect(view,&GraphicsView::sinalCreateNavigaBar,this,&Dialog::slot_createNaviBar);
    dlg = NULL;
    str = QCoreApplication::applicationDirPath();
    str += "\\11.png";
    outPut<<"路径名："<<str;
    timer = new QTimer;
    connect(timer,&QTimer::timeout,this,&Dialog::slot_timeout);
    connect(view->verticalScrollBar(),&QScrollBar::valueChanged,this,slot_VvalueChanged);
    connect(view->horizontalScrollBar(),&QScrollBar::valueChanged,this,slot_HvalueChanged);
    connect(this,&Dialog::signalSetDrawRectSize,this,&Dialog::slot_setViewRect);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::slot_createNaviBar(qreal scale)
{
    if(scale > 1)
    {
        m_scale = scale;
        if(dlg == NULL)
        {
            dlg = new BreviaryDlg(this,Qt::WindowStaysOnTopHint);
    //        dlg->setGeometry(100,200,400,300);
    //        dlg->setSize(400,300);
            connect(this,&Dialog::signalDrawRect,dlg,&BreviaryDlg::slot_setRectSize);
    //        QImage image(QSize(400,300),QImage::Format_RGB32);
    //        QPainter painter(&image);
    //        image.save("E:\\11","png");

    //        view->setViewport(dlg);
    //        dlg->setPosSize(600,500,400,300);
    //        QPalette palette = dlg->palette();
    //        palette.setColor(QPalette::Background,QColor(34,34,34));
    //        dlg->setWindowOpacity(0.8);
    //        dlg->setPalette(palette);
            QSize size = dlg->getSize();
            view->smallWinSize(size);
            dlg->transViewPtr(view);
            dlg->show();//非模态
        }
        else
        {
            dlg->show();
        }
        QPixmap pixmap(SMALL_W,SMALL_H);
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        painter.setRenderHint(QPainter::Antialiasing);
        scene->render(&painter);
        pixmap.save(str,"png");
        dlg->setPixmap(pixmap);
        timer->start(100);
    }
    else
    {
        if(dlg != NULL)
        {
            dlg->hide();
        }
    }
}

void Dialog::slot_scrollValueChanged()
{
    m_viewSize = view->viewportSizeHint();//获取视口大小
    QRect rectScene = scene->itemsBoundingRect().toRect();
    if (view->horizontalScrollBar()->isHidden())
    {
        rectViewport.setX(rectScene.x());
        rectViewport.setWidth(rectScene.width());
    }
    if(view->verticalScrollBar()->isHidden())
    {
        rectViewport.setY(0);
        rectViewport.setHeight(rectScene.height());
    }
    emit signalSetDrawRectSize();
}

void Dialog::slot_HvalueChanged(int value)
{
    m_viewSize = view->viewportSizeHint();//获取视口大小
    outPut<<"slot_HvalueChanged"<<" "<<m_viewSize<</*" "<<rect<<" "<<*/value;

    if(value != 0 && dlg != NULL)
    {
        if(dlg->isVisible())
        {
            m_x = value;
            emit signalSetDrawRectSize();
        }
    }
}

void Dialog::slot_VvalueChanged(int value)
{
    m_viewSize = view->viewportSizeHint();//获取视口大小
//    QRect rect = view->viewPortRect();
    outPut<<"slot_VvalueChanged"<<" "<<m_viewSize<</*" "<<rect<<" "<<*/value;
    if(value != 0 && dlg != NULL)
    {
        if(dlg->isVisible())
        {
            m_y = value;
            emit signalSetDrawRectSize();
        }
    }
}

void Dialog::slot_setViewRect()
{
    view->viewPortSize(m_viewSize);
    int x = m_x / (m_viewSize.width() * m_scale)* SMALL_W;
    int y = m_y / (m_viewSize.height() * m_scale)* SMALL_H;
    int wid = SMALL_W / m_scale;
    int hei = SMALL_H / m_scale;
    outPut<<"小矩形坐标及大小："<<"("<<m_x<<" ,"<<m_y<<" ,"<<wid<<" ,"<<hei<<")";

    QRect rect(x,y,wid,hei);

    emit signalDrawRect(rect);
}


stuRatio Dialog::getScalingRatio()
{
    stuRatio ratio;
    ratio.xRatio = (qreal)SMALL_W / m_viewSize.width();
    ratio.yRatio = (qreal)SMALL_H / m_viewSize.height();
    outPut<<"横纵比："<<ratio.xRatio<<" "<<ratio.yRatio;
    return ratio;
}

void Dialog::slot_timeout()
{
    QPixmap pixmap(SMALL_W,SMALL_H);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    scene->render(&painter);
    pixmap.save(str,"png");
    dlg->setPixmap(pixmap);
}

