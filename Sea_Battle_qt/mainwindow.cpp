#include "mainwindow.h"
#include "ui_mainwindow.h"

//---Библиотеки для работы с сокетами Беркли-
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
//-------------------------------------------

#include <battle.h>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setFixedSize(QSize(700, 500));
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

    int ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

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
        ui->BackgroundForMainMenu->hide();
        ui->String_For_IPaddress->hide();
        ui->Connection->hide();
        ui->Name->hide();

        Preparing_for_Battle(ClientSocket);
    }
    else
    {
        qDebug() << "error\n";
    }
}
