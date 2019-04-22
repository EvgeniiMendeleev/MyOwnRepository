#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QKeyEvent>
#include <QEvent>
#include <QPixmap>
#include <QDebug>

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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
     void Preparing_for_Battle(); //Функция для отображения фрейма с нераставленными кораблями и полем

private slots:
    void on_Connection_clicked();                //Кнопка подключения в главном меню.

private:
    Ui::MainWindow *ui;     //Главное меню.

    int ClientSocket;       //Сокет клиента, с помощью которого будет общение с сервером.

    //---Переменные для отображения какого - либо фрейма---
    QPixmap BattleField;
    QGraphicsScene* scene;
    //-----------------------------------------------------

    QPoint PosMouse;        //Координаты мыши на экране.

    int* tmpbuffer;         //Временный буфер для поля с кораблями.
};

#endif // MAINWINDOW_H
