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
    explicit Ship(int TypeOfShip, QObject* parent = 0);

    void set_x0(int x);
    void set_y0(int y);
    int get_x();
    int get_y();
    bool aroundShip();              //Проверка на рядом стоящие корабли.
    
    ~Ship();
signals:

private:
    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event);

    int typeOfShip;             //Тип корабля.
    QPixmap TextureOfShip;      //Текстура корабля.
    int width;                  //Ширина текстуры корабля.
    int height = 42;            //Высота текстуры корабля.
    int x0, y0;                 //Координаты появления корабля.
    int x, y;                   //Координаты на сцене.
    bool isHorisontal = true;   //Переменная, отвечающая за состояние корабля в пространстве:
                                //горизонтален он или вертикален.
    int centerX, centerY;       //Для корректировки захвата корабля.

public slots:

};

#endif // SHIP_H
