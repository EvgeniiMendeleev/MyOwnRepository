#include "battletable.h"

BattleTable::BattleTable(int Client, QGraphicsScene* scene, QObject *parent) : QObject(parent)
{
    BUNG.load(":/img/красный-крестик.png");
    BUNG = BUNG.scaled(24, 24);

    NotBUNG.load(":/img/BlackDot.png");
    NotBUNG = NotBUNG.scaled(24, 24);

    ClientSocket = Client;
    Scene = scene;
}

void BattleTable::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Position* InfoForServer = new Position;

    InfoForServer->i = event->pos().y() / 24;
    InfoForServer->j = event->pos().x() / 24;

    send(ClientSocket, InfoForServer, sizeof(Position), MSG_NOSIGNAL);

    int* isBung = new int;
    recv(ClientSocket, isBung, sizeof(int), MSG_NOSIGNAL);

    qDebug() << "isBung = " << *isBung << "\n";

    if((*isBung) > 0)
    {
        QGraphicsPixmapItem* IsBung = Scene->addPixmap(BUNG);
        IsBung->setPos(322 + 24 * InfoForServer->j, 102 + 24 * InfoForServer->i);
    }
    else if((*isBung) == 0)
    {
        QGraphicsPixmapItem* NotIsBung = Scene->addPixmap(NotBUNG);
        NotIsBung->setPos(322 + 24 * InfoForServer->j, 102 + 24 * InfoForServer->i);
    }

    delete isBung;
    delete InfoForServer;
}

QRectF BattleTable::boundingRect() const
{
    return QRectF(0, 0, 240, 240);
}

void BattleTable::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}
