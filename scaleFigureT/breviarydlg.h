#ifndef BREVIARYDLG_H
#define BREVIARYDLG_H

#include <QDialog>
#include "graphicsview.h"

#define SMALL_W 400
#define SMALL_H 400

//class QGraphicsScene;
class MyGraphicsScene;
class QGraphicsView;
class QGraphicsPixmapItem;

class BreviaryDlg : public QDialog
{
    Q_OBJECT
public:
    BreviaryDlg(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    void setSize(int w,int h);
    void setPosSize(int x,int y,int w,int h);
    void setPixmap(QPixmap &pix);
    QSize getSize();
    void fitInView();
    void transViewPtr(GraphicsView *view);
protected:
    void resizeEvent(QResizeEvent *event);
//    void paintEvent(QPaintEvent *event);
public slots:
    void slot_setRectSize(QRect &rect);
private:
    QPixmap m_pixmap;
//    bool isUpdate;
    QRect m_rect;
//    QGraphicsScene *scene;
    MyGraphicsScene *scene;
    QGraphicsView *view;
    QGraphicsPixmapItem *item;
    GraphicsView *m_view1;
};

#endif // BREVIARYDLG_H
