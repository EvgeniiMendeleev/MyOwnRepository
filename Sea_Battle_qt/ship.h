#ifndef SHIP_H
#define SHIP_H
#include <QMouseEvent>
#include <QObject>
#include <QGraphicsItem>
#include <QCursor>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <sys/shm.h>

class Ship: public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Ship(int TypeOfShip, int memID, QObject* parent = 0);

    void set_x0(int x);
    void set_y0(int y);
    int get_x();
    int get_y();
    bool checkPlace(int* table, int i0, int j0);
    bool onTable();
    bool aroundShip(int* table, int i0, int j0, int k, int l);                 //Проверка на рядом стоящие корабли.
    void clearFromShip();

    ~Ship();
signals:

private:
    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event);

    int typeOfShip;                     //Тип корабля.
    QPixmap TextureOfShip;              //Текстура корабля.
    int width;                          //Ширина текстуры корабля.
    int height = 42;                    //Высота текстуры корабля.
    int x0, y0;                         //Координаты появления корабля.
    int x, y;                           //Координаты на сцене.
    bool isHorisontal = true;           //Переменная, отвечающая за состояние корабля в пространстве:
                                        //горизонтален он или вертикален.
    bool onPlace = false;               //Переменная, отвечающая за состояние на сетке: положен на игровую сетку или нет
    int centerX, centerY;               //Для корректировки захвата корабля.
    int memId;

public slots:

};

#endif // SHIP_H
