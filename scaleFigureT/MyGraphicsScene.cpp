#include "MyGraphicsScene.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsRectItem>
#include <QDebug>

MyGraphicsScene::MyGraphicsScene(QObject *parent)
    : QGraphicsScene(parent)
    , m_bRectClicked(false)
{
    m_pRectItem = new QGraphicsRectItem(0, 0, 0, 0);
    QPen penRectItem = QPen(QColor(255, 0, 0));
    penRectItem.setWidth(2);
    m_pRectItem->setPen(penRectItem);
    m_pRectItem->setZValue(1);
    addItem(m_pRectItem);
}

void MyGraphicsScene::onSetPreviewRect(QRect rect)
{
    m_rectSaved = rect;

    // 内缩几个像素，用矩形外边框来标示viewport显示区域
    m_pRectItem->setRect(rect.x() -2/*+ 5*/, rect.y() - 2/*+ 5*/, rect.width() - 4, rect.height() - 4);
}

void MyGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mouseMoveEvent(mouseEvent);
    if (m_bRectClicked) {
        QPoint ptTopLeft = mouseEvent->scenePos().toPoint() - m_ptRectRelated;
        m_rectSaved.setTopLeft(ptTopLeft);
        emit previewRectMoved(m_rectSaved);
    }
}

void MyGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mousePressEvent(mouseEvent);
    if (m_rectSaved.contains(mouseEvent->scenePos().x(), mouseEvent->scenePos().y())) {
        m_bRectClicked = true;
        m_ptRectRelated = mouseEvent->scenePos().toPoint() - m_rectSaved.topLeft();
    }
}

void MyGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
    m_bRectClicked = false;
}


