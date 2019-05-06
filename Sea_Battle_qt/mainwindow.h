#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QPixmap>
#include <QDebug>
#include <QVector>
#include <QMessageBox>
#include <sys/shm.h>
#include <battletable.h>

//--Библиотеки для работы с сокетами Беркли--
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
//-------------------------------------------

#include <ship.h>

namespace Ui {
class MainWindow;
}

enum states {WaitingOfTurn,
             Turn,
             WaitingOfReadyPlayer,
             Ready,
             WaitingOfConnection,
             PlacingShips,
             NotConnection};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
     void Preparing_for_Battle(); //Функция для отображения фрейма с нераставленными кораблями и полем
     void Main_Menu_off();        //Функция для скрытия кадра главного меню
     void Main_Menu_on();
     void BATTLE();

private slots:
    void on_Connection_clicked();                //Кнопка подключения в главном меню.
    void on_BattleButton_clicked();

private:
    Ui::MainWindow *ui;     //Главное меню.
    int ClientSocket;       //Сокет клиента, с помощью которого будет общение с сервером.

    //---Переменные для отображения какого - либо фрейма---
    QPixmap frame;
    QGraphicsScene* scene;
    //-----------------------------------------------------

    QVector<Ship*> MyShips; //Вектор всех кораблей (всего их 10)
    //i = 0 - четырёхпалубник
    //i = 1-2 - трёхпалубники
    //i = 3-5 - двухпалубники
    //i = 6-9 - однопалубники
    int memID;
    int* table;
    BattleTable* Table;
    states StateOfClient;
};

#endif // MAINWINDOW_H
