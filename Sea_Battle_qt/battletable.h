#ifndef BATTLETABLE_H
#define BATTLETABLE_H

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <stdint.h>

//--Библиотеки для работы с сокетами Беркли--
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
//-------------------------------------------

struct Position
{
    int i, j;
};

class BattleTable : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit BattleTable(int Client, QGraphicsScene* scene, QObject *parent = 0);

private:
    QRectF boundingRect() const;
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

    QPixmap BUNG;
    QPixmap NotBUNG;
    int ClientSocket;
    QGraphicsScene* Scene;

signals:

public slots:
};

#endif // BATTLETABLE_H
