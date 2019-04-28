#include "ship.h"
#include <QDebug>

Ship::Ship(int TypeOfShip, QObject* parent):QObject(parent), QGraphicsItem()
{
    centerY = 21;
    typeOfShip = TypeOfShip;

    switch(TypeOfShip)
    {
        case 1:
            width = 31;

            centerX = width / 2;
            TextureOfShip.load(":/img/1ship.png");
            TextureOfShip = TextureOfShip.scaled(width, height, Qt::KeepAspectRatio);
            break;

        case 2:
            width = 62;

            centerX = width / 2;
            TextureOfShip.load(":/img/2ship.png");
            TextureOfShip = TextureOfShip.scaled(width, height, Qt::KeepAspectRatio);
            break;

        case 3:
            width = 93;

           centerX = width / 2;
            TextureOfShip.load(":/img/3ship.png");
            TextureOfShip = TextureOfShip.scaled(width, height, Qt::KeepAspectRatio);
            break;

        case 4:
            width = 124;

            centerX = width / 2;
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
    return QRectF (0, 0, width, height);
}

void Ship::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->drawPixmap(0, 0, TextureOfShip, 0, 0, width, height);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void Ship::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    this->setPos(mapToScene(event->pos().x() - centerX, event->pos().y() - centerY));

    qDebug() << this->pos().x() << this->pos().y() << "\n";
    qDebug() << "x and y of Mouse: " << mapToScene(event->pos().x(), event->pos().y());
}

void Ship::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    this->setCursor(QCursor(Qt::ClosedHandCursor));
    Q_UNUSED(event);
}

void Ship::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    double k, m;           //Строка - столбец сетки с кораблями.

    if(isHorisontal)
    {
        m = (this->pos().x() + height / 12 - 250) / 31;
        k = (this->pos().y() + height / 3 - 86) / 30;

        if(k >= 0 && m >= 0)
        {
            int i = k;
            int j = m;

            qDebug() << "i = " << i << "\n";
            qDebug() << "j = " << j << "\n";

            if((this->typeOfShip == 2) && (j + 1 >= 10))
            {
                this->setPos(x0, y0);
                return;
            }
            else if((this->typeOfShip == 3) && (j + 2 >= 10))
            {
                this->setPos(x0, y0);
                return;
            }
            else if((this->typeOfShip == 4) && (j + 3 >= 10))
            {
                this->setPos(x0, y0);
                return;
            }

            this->setPos(250 + 31 * j + 2, 86 + 30 * i);
            this->x = this->pos().x();
            this->y = this->pos().y();
        }
        else
        {
            this->setPos(x0, y0);
            this->x = x0;
            this->y = y0;
        }
    }
    else
    {
        m = (this->pos().x() - height / 2 - 248) / 31;
        k = (this->pos().y() + 18 - 86) / 30;

        if(k >= 0 && m >= 0)
        {
            int i = k;
            int j = m;

            if((this->typeOfShip == 2) && (i + 1 >= 10))
            {
                this->setPos(x0, y0);
                return;
            }
            else if((this->typeOfShip == 3) && (i + 2 >= 10))
            {
                this->setPos(x0, y0);
                return;
            }
            else if((this->typeOfShip == 4) && (i + 3 >= 10))
            {
                this->setPos(x0, y0);
                return;
            }

            this->setPos(248 + 31 * j + 38, 86 + 30 * i);
            this->x = this->pos().x() - 31;
            this->y = this->pos().y();
        }
        else
        {
            this->setPos(this->x0, this->y0);
            this->x = x0;
            this->y = y0;
        }

    }

    this->setCursor(QCursor(Qt::ArrowCursor));
    Q_UNUSED(event);
}

void Ship::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    setZValue(1);

    if(isHorisontal)
    {
        setRotation(90);
        isHorisontal = false;
    }
    else
    {
        setRotation(0);
        isHorisontal = true;
    }

    Q_UNUSED(event);
}

void Ship::set_x0(int x)
{
    this->x0 = x;
}

void Ship::set_y0(int y)
{
    this->y0 = y;
}

int Ship::get_x()
{
    return x;
}

int Ship::get_y()
{
    return y;
}
/*if(isHorisontal)
{
    int j = (this->pos().y() - 86 + height / 3) / 30;
    int i = (this->pos().x() - 248 + width / 6) / 31;

    if(i >= 0 && j >= 0)
    {
        this->setPos(248 + 31 * i, 86 + 30 * j);
    }
    else
    {
        this->setPos(this->x0, this->y0);
    }
}
else
{
    int j = (this->pos().x() - 248 + 3 * height / 2) / 31;
    int i = (this->pos().y() - 86 - 2 * height / 3) / 30;

    if(i >= 0 && j >= 0)
    {
        //this->setPos(248 + 31 * j - 3 * height / 2, 86 + 30 * i);
    }
    else
    {
        this->setPos(this->x0, this->y0);
    }
}*/

/*qDebug() << "height of Rect: " << this->boundingRect().height();
qDebug() << "width of Rect: " << this->boundingRect().width();
qDebug() << "x: " << this->boundingRect().topLeft().x();
qDebug() << "y: " << this->boundingRect().topLeft().y();*/
