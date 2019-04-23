#include "ship.h"
#include <QDebug>

Ship::Ship(int TypeOfShip, QObject* parent):QObject(parent), QGraphicsItem()
{
    switch(TypeOfShip)
    {
        case 1:
            width = 31;

            TextureOfShip.load(":/img/1ship.png");
            TextureOfShip = TextureOfShip.scaled(width, height, Qt::KeepAspectRatio);
            break;

        case 2:
            width = 62;

            TextureOfShip.load(":/img/2ship.png");
            TextureOfShip = TextureOfShip.scaled(width, height, Qt::KeepAspectRatio);
            break;

        case 3:
            width = 93;

            TextureOfShip.load(":/img/3ship.png");
            TextureOfShip = TextureOfShip.scaled(width, height, Qt::KeepAspectRatio);
            break;

        case 4:
            width = 124;

            TextureOfShip.load(":/img/4ship.jpg");
            TextureOfShip = TextureOfShip.scaled(width, height, Qt::KeepAspectRatio);
            break;
    }
}

Ship::~Ship()
{

}

QRectF Ship::boundingRect() const
{
    return QRectF (x, y, width, height);
}

void Ship::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->drawPixmap(x, y, TextureOfShip, 0, 0, width, height);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void Ship::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    qDebug() << this->pos().x() << this->pos().y() << "\n";
    this->setPos(mapToScene(QPointF(event->pos().x() - x, event->pos().y() - y)));
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

void Ship::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    //Задел на будущее.
    if(this->isHorisontal)
    {
        setRotation(90);
        this->isHorisontal = false;
    }
    else
    {
        setRotation(0);
        this->isHorisontal = true;
    }
}

void Ship::set_x(int x)
{
    this->x = x;
}

void Ship::set_y(int y)
{
    this->y = y;
}

int Ship::get_x()
{
    return this->x;
}

int Ship::get_y()
{
    return this->y;
}
