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
#include <QTimer>
#include <sys/shm.h>
#include <battletable.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>

//--Библиотеки для работы с сокетами Беркли--
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
//-------------------------------------------

#include <ship.h>

using namespace std;

namespace Ui {
class MainWindow;
}

enum ResultOfShot {not_hit,
                   hit,
                   kill};

enum Msg_type {result_of_shot,
               enemy_shot,
               error};

enum states {WaitingOfReadyPlayer,
             WaitingOfConnection,
             PlacingShips, Win, Lose};

struct Shot
{
    int16_t PosX;
    int16_t PosY;
};

struct Message
{
    int16_t type;
    int16_t PosX;
    int16_t PosY;
    int16_t Result;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
     void Preparing_for_Battle(); //Функция для отображения фрейма с нераставленными кораблями и полем.
     void Main_Menu_off();        //Функция для скрытия кадра главного меню.
     void Main_Menu_on();         //Функция для отображения кадра главного меню.
     void BATTLE();               //Функция для отображения кадра с битвой.

private slots:
    void on_Connection_clicked();                //Кнопка подключения в главном меню.
    void on_BattleButton_clicked();              //Кнопка готовности на этапе расстановки кораблей.
    void SendFire(int16_t x, int16_t y);         //Отправка координат серверу.
    void ReadFromServer();                       //Чтение данных с сервера.

private:
    Ui::MainWindow *ui;     //Главное меню.
    int ClientSocket;       //Сокет клиента, с помощью которого будет общение с сервером.

    QTimer* MyTimer;        //Таймер, по истечению которого будут читаться данные с сокета во время боя.

    //---Переменные для отображения какого - либо фрейма---
    QPixmap frame;
    QGraphicsScene* scene;
    //-----------------------------------------------------

    //---Картинки для отображения результата выстрела-----
    QPixmap BUNG;               //Попал
    QPixmap NotBUNG;            //Молоко
    //----------------------------------------------------

    QVector<Ship*> MyShips; //Вектор всех кораблей (всего их 10)
    //i = 0 - четырёхпалубник
    //i = 1-2 - трёхпалубники
    //i = 3-5 - двухпалубники
    //i = 6-9 - однопалубники

    //----Переменные для работы с таблицей кораблей----
    int memID;
    int16_t* table;
    //-------------------------------------------------

    BattleTable* Table;        //Поле, на котором вычисляются координаты выстрела для противника
    states StateOfClient;      //Состояние клиента.
};

#endif // MAINWINDOW_H
