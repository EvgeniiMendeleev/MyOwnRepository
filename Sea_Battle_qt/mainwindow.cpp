#include "mainwindow.h"
#include "ui_mainwindow.h"

//Old official size of window: width = 580, height = 420
//New official size of window: width = 573, height = 415
//The newest than new official size of window: width = 580, height = 415

#define WidthOfFrame   580
#define HeightOfFrame 415

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setFixedSize(QSize(WidthOfFrame, HeightOfFrame));

    ui->Frame->setFixedSize(QSize(WidthOfFrame, HeightOfFrame));
    ui->Frame->hide();
    ui->Frame->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff);
    ui->Frame->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff);
    ui->BattleButton->hide();

    BUNG.load(":/img/красный-крестик.png");
    BUNG = BUNG.scaled(24, 24);

    NotBUNG.load(":/img/BlackDot.png");
    NotBUNG = NotBUNG.scaled(24, 24);

    MyTimer = new QTimer;
    MyTimer->setInterval(50);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Connection_clicked()
{
   QString IPaddress = ui->String_For_IPaddress->text();

    if(IPaddress == NULL)
    {
        qDebug() << "No string";
        return;
    }

    QByteArray tmp = IPaddress.toLatin1();
    const char* IP = tmp.data();

    ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(ClientSocket < 0)
    {
        qDebug() << "Error with socket()";
    }

    struct sockaddr_in ServerAddr;

    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons(25567);
    inet_pton(AF_INET, IP, &(ServerAddr.sin_addr));

    if(::connect(ClientSocket, (struct sockaddr*) (&ServerAddr), sizeof(ServerAddr)) != -1)
    {
        Preparing_for_Battle();
    }
    else
    {
        QMessageBox::information(this, "Ошибка", "Введён неправильный адрес или сервер не запущен");
    }
}

void MainWindow::Preparing_for_Battle()
{
    //Отключаем главное меню, чтобы перейти к подготовке к битве
    Main_Menu_off();

    //Создаём сцену, на которой будет фрейм с подготовкой к битве
    scene = new QGraphicsScene();

    //Загружаем бэкграунд и подгоняем под размер фрейма.
    frame.load(":/img/PreparingForBattle.png");
    frame = frame.scaled(ui->Frame->geometry().width(), ui->Frame->geometry().height());
    scene->addPixmap(frame);

    memID = shmget(IPC_PRIVATE, 100 * sizeof(int), 0600|IPC_CREAT|IPC_EXCL);

    if(memID < 0)
    {
        qDebug() << "Error with shmget()";
    }

    table = (int*)shmat(memID, 0, 0);

    for(int i = 0; i < 100; i++)
    {
        table[i] = 0;
    }

    //Отображаем все корабли. Всего их 10:
    //1 - четырёхпалубник
    //2 - трёхпалубника
    //3 - двухпалубников
    //4 - однопалубников
    MyShips.resize(10);

    //Отображаем четырёхпалубник
    MyShips[0] = new Ship(4, memID);
    MyShips[0]->set_x0(28); //10
    MyShips[0]->set_y0(149); //92
    MyShips[0]->setPos(28, 149);
    scene->addItem(MyShips[0]);

    //Отображаем трёхпалубники
    for(int i = 1; i < 3; i++)
    {
        MyShips[i] = new Ship(3, memID);
        MyShips[i]->set_x0(28 + (i - 1) * 100);
        MyShips[i]->set_y0(196);
        MyShips[i]->setPos(28 + (i - 1) * 100, 196);
        scene->addItem(MyShips[i]);
    }

    //Отображаем двухпалубники
    for(int i = 3; i < 6; i++)
    {
        MyShips[i] = new Ship(2, memID);
        MyShips[i]->set_x0(28 + (i - 3) * 70);
        MyShips[i]->set_y0(243);
        MyShips[i]->setPos(28 + (i - 3) * 70, 243);
        scene->addItem(MyShips[i]);
    }

    //Отображаем однопалубники
    for(int i = 6; i < 10; i++)
    {
        MyShips[i] = new Ship(1, memID);
        MyShips[i]->set_x0(28 + (i - 6) * 40);
        MyShips[i]->set_y0(290);
        MyShips[i]->setPos(28 + (i - 6) * 40, 290);
        scene->addItem(MyShips[i]);
    }

    ui->BattleButton->show();

    //Отображаем фрейм с подготовкой к битве.
    ui->Frame->setScene(scene);
    ui->Frame->show();
}

void MainWindow::Main_Menu_off()
{
    ui->BackgroundForMainMenu->hide();
    ui->String_For_IPaddress->hide();
    ui->Connection->hide();
    ui->Name->hide();
}

void MainWindow::on_BattleButton_clicked()
{
    for(int i = 0; i < 10; i++)
    {
        if(!MyShips[i]->onTable())
        {
            QMessageBox::information(this, "Ошибка", "Не все корабли расставлены на поле");
            return;
        }
    }

    send(ClientSocket, table, 100 * sizeof(int), MSG_NOSIGNAL);
    shmctl(memID, IPC_RMID, 0);
    ui->Frame->hide();
    ui->BattleButton->hide();

    BATTLE();
}

void MainWindow::BATTLE()
{
    frame.load(":/img/BATTLEFIELD.jpg");
    frame = frame.scaled(ui->Frame->geometry().width(), ui->Frame->geometry().height());
    scene->addPixmap(frame);

    for(int i = 0; i < 10; i++)
    {
        QPixmap ship;

        if(i == 0)
        {
           ship.load(":/img/4ship.jpg");
           ship = ship.scaled(98, 28);
        }
        else if((i >= 1) && (i <= 2))
        {
            ship.load(":/img/3ship.png");
            ship = ship.scaled(75, 28);
        }
        else if(i >= 3 && i <= 5)
        {
            ship.load(":/img/2ship.png");
            ship = ship.scaled(51, 28);
        }
        else if(i >= 6 && i <= 9)
        {
            ship.load(":/img/1ship.png");
            ship = ship.scaled(24, 30);
        }

        QGraphicsPixmapItem* item = scene->addPixmap(ship);

        if(MyShips[i]->Horisontal())
        {
            item->setPos(20 + 24 * MyShips[i]->get_x(), 104 + 23 * MyShips[i]->get_y());
        }
        else
        {
            item->setRotation(90);
            item->setPos(20 + 24 * MyShips[i]->get_x() + 24, 104 + 23 * MyShips[i]->get_y());
        }
    }

    for(int i = 0; i < 10; i++)
    {
        delete MyShips[i];
    }

    Table = new BattleTable();
    Table->setPos(322, 102);
    connect(Table, &BattleTable::fire, this, &MainWindow::SendFire);
    scene->addItem(Table);

    ui->Frame->setScene(scene);
    ui->Frame->show();

    connect(MyTimer, &QTimer::timeout, this, &MainWindow::ReadFromServer);
    fcntl(ClientSocket, F_SETFL, O_NONBLOCK);
    MyTimer->start();
}

void MainWindow::SendFire(unsigned short x, unsigned short y)
{
    Shot* InfoForServer = new Shot;
    InfoForServer->PosX = x;
    InfoForServer->PosY = y;

    send(ClientSocket, InfoForServer, sizeof(InfoForServer), MSG_NOSIGNAL);

    delete InfoForServer;
}

void MainWindow::ReadFromServer()
{
    Message* MsgFromServer = new Message;

    if(recv(ClientSocket, MsgFromServer, sizeof(Message), MSG_NOSIGNAL) > 0)
    {
        qDebug() << "MsgFromServer->Result = " << MsgFromServer->Result;
        qDebug() << "MsgFromServer->PosX = " << MsgFromServer->PosX;
        qDebug() << "MsgFromServer->PosY = " << MsgFromServer->PosY;
        qDebug() << "MsgFromServer->type = " << MsgFromServer->type;
        if(MsgFromServer->type == result_of_shot)
        {
            if(MsgFromServer->Result == hit)
            {
                qDebug() << "I'm hit ship!\n";
            }
            else if(MsgFromServer->Result == not_hit)
            {
                qDebug() << "I'm not hit!\n";
            }
        }
        else if(MsgFromServer->type == enemy_shot)
        {
            if(MsgFromServer->Result == hit)
            {
                qDebug() << "Enemy hit me!\n";
            }
            else if(MsgFromServer->Result == not_hit)
            {
                qDebug() << "Enemy not hit me! Yessss!\n";
            }
        }
    }

    delete MsgFromServer;
}
