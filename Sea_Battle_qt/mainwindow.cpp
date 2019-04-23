#include "mainwindow.h"
#include "ui_mainwindow.h"

#define WidthOfFrame   701 //700
#define HeightOfFrame 481 //500

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(QSize(690, 500));

    ui->Frame->setFixedSize(QSize(WidthOfFrame, HeightOfFrame - 20));
    ui->Frame->hide();
    ui->Frame->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    ui->Frame->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
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

    qDebug() << IP;

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
        qDebug() << "error\n";
    }
}

void MainWindow::Preparing_for_Battle()
{
    Main_Menu_off();

    scene = new QGraphicsScene();

    frame.load(":/img/PlacingShips.png");
    frame = frame.scaled(WidthOFrame, HeightOfFrame, Qt::KeepAspectRatio);
    scene->addPixmap(frame);

    MyShips.resize(10);

    //Отображаем четырёхпалубник
    MyShips[0] = new Ship(4);
    MyShips[0]->set_x(30);
    MyShips[0]->set_y(92);
    MyShips[0]->setPos(MyShips[0]->get_x(), MyShips[0]->get_y());
    scene->addItem(MyShips[0]);

    //Отображаем трёхпалубники
    for(int i = 1; i < 3; i++)
    {
        MyShips[i] = new Ship(3);
        MyShips[i]->set_x(30 + (i - 1) * 50);
        MyShips[i]->set_y(115);
        MyShips[i]->setPos(MyShips[i]->get_x(), MyShips[i]->get_y());
        scene->addItem(MyShips[i]);
    }

    //Отображаем двухпалубники
    for(int i = 3; i < 6; i++)
    {
        MyShips[i] = new Ship(2);
        MyShips[i]->set_x(30 + (i - 3) * 35);
        MyShips[i]->set_y(137);
        MyShips[i]->setPos(MyShips[i]->get_x(), MyShips[i]->get_y());
        scene->addItem(MyShips[i]);
    }

    //Отображаем однопалубники
    for(int i = 6; i < 10; i++)
    {
        MyShips[i] = new Ship(1);
        MyShips[i]->set_x(30 + (i - 6) * 25);
        MyShips[i]->set_y(158);
        MyShips[i]->setPos(MyShips[i]->get_x(), MyShips[i]->get_y());
        scene->addItem(MyShips[i]);
    }

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
/* //fork()
 int ship_count = 5;
 while(ship_count > 0)
 {

     //pасстановка корабля
     --ship_count;
 }

 Qtimer или поток
 for(;;)
 {
     QPoint pos;

     if(QApplication::mouseButtons() == Qt::LeftButton)
     {
         pos = QWidget::mapFromGlobal(QCursor::pos());
         qDebug() << pos.x() << pos.y();
     }
 }*/
