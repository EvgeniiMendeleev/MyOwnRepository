#include "battletable.h"

BattleTable::BattleTable(QObject *parent) : QObject(parent)
{
}

void BattleTable::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
   emit fire(event->pos().x() / 24, event->pos().y() / 24);
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
