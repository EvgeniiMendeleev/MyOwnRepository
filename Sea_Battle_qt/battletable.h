#ifndef BATTLETABLE_H
#define BATTLETABLE_H

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <stdint.h>

using namespace std;

class BattleTable : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit BattleTable(QObject *parent = 0);

private:
    QRectF boundingRect() const;
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

signals:
    void fire(int16_t x, int16_t y);

public slots:

private slots:
};

#endif // BATTLETABLE_H
