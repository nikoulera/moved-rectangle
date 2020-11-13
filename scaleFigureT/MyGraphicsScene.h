#pragma once

//#include <vld.h>
#include <QGraphicsScene>

class MyGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    MyGraphicsScene(QObject *parent = nullptr);

    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

Q_SIGNALS:

    void previewRectMoved(QRect rect);

public Q_SLOTS:

    void onSetPreviewRect(QRect rect);

private:

    QGraphicsRectItem* m_pRectItem;
    QRect m_rectSaved;
    bool m_bRectClicked;
    QPoint m_ptRectRelated;     // 鼠标点击时，相对于红色矩形框的位置
};
