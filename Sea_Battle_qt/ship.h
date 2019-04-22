#ifndef SHIP_H
#define SHIP_H
#include <QMouseEvent>
#include <QObject>
#include <QGraphicsItem>
#include <QCursor>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>

class Ship: public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Ship(QObject* parent = 0);
    ~Ship();
signals:

private:
    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

    QPixmap TextureOfShip;
public slots:

};

#endif // SHIP_H
