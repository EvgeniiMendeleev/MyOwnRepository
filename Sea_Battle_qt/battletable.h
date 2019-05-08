#ifndef BATTLETABLE_H
#define BATTLETABLE_H

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>

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
    void fire(unsigned short x, unsigned short y);

public slots:

private slots:
};

#endif // BATTLETABLE_H
