#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QGraphicsScene>
#include "graphicsview.h"
#include "breviarydlg.h"
#include <QTimer>

struct stuRatio{
    qreal xRatio;
    qreal yRatio;
};

#define SMALL_W 400
#define SMALL_H 400

class QVideoWidget;
class QMediaPlayer;

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

//    void drawRect();

    stuRatio getScalingRatio();

signals:
    void signalDrawRect(QRect &rect);

    void signalSetDrawRectSize();
private slots:
    void slot_createNaviBar(qreal scale);

    void slot_HvalueChanged(int value);

    void slot_VvalueChanged(int value);

    void slot_scrollValueChanged();

    void slot_setViewRect();

    void slot_timeout();
private:
    Ui::Dialog *ui;
    QGraphicsScene *scene;
    GraphicsView *view;
    QVideoWidget *video;
    QMediaPlayer *player;
    BreviaryDlg *dlg;
    QTimer *timer;
    qreal m_scale;
    QRect rectViewport;
    quint32 m_y;
    quint32 m_x;
    QSize m_viewSize;
    QString str;
};
#endif // DIALOG_H
