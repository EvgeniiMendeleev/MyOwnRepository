#include "ship.h"
#include <QDebug>
int width = 125;
int height = 42;

Ship::Ship(QObject* parent):QObject(parent), QGraphicsItem()
{
    TextureOfShip.load(":/img/4ship.jpg");
    TextureOfShip = TextureOfShip.scaled(width, height, Qt::KeepAspectRatio);
}

Ship::~Ship()
{

}

QRectF Ship::boundingRect() const
{
    return QRectF (50, 50, width, height);
}

void Ship::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->drawPixmap(50, 50, TextureOfShip, 0, 0, width, height);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void Ship::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    qDebug() << this->pos().x() << this->pos().y() << "\n";
    this->setPos(mapToScene(QPointF(event->pos().x() - 50, event->pos().y() - 50)));
}

void Ship::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    this->setCursor(QCursor(Qt::ClosedHandCursor));
    Q_UNUSED(event);
}

void Ship::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    this->setCursor(QCursor(Qt::ArrowCursor));
    Q_UNUSED(event);
}

