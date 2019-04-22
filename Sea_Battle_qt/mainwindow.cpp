#include "mainwindow.h"
#include "ui_mainwindow.h"

#define WidthOfFrame 700
#define HeightOfFrame 500

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(QSize(690, 500));

    ui->Frame->setFixedSize(QSize(WidthOfFrame, HeightOfFrame));
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
    ui->BackgroundForMainMenu->hide();
    ui->String_For_IPaddress->hide();
    ui->Connection->hide();
    ui->Name->hide();

    scene = new QGraphicsScene();

    BattleField.load(":/img/PlacingShips.png");
    BattleField = BattleField.scaled(WidthOFrame, HeightOfFrame, Qt::KeepAspectRatio);

    scene->addPixmap(BattleField);

    Ship* MyShip = new Ship;
    MyShip->setPos(50, 50);
    scene->addItem(MyShip);

    ui->Frame->setScene(scene);
    ui->Frame->show();

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
