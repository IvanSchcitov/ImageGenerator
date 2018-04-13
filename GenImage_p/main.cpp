#include <QCoreApplication>
#include <QtGui/QImage>
#include <QPicture>
#include <QDateTime>
#include <iostream>
#include <QFile>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>



void parametrsPars(char *str, QString &IP, QString &Port, int &F, int &ResX, int &ResY){

    QJsonDocument doc = QJsonDocument::fromJson(str);

    QJsonObject root = doc.object();

    IP = root.value("IP").toString();
    Port = root.value("Port").toString();
    QString val = root.value("Frequency").toString();
    F = val.toInt();
    val = root.value("ResolutionX").toString();
    ResX = val.toInt();
    val = root.value("ResolutionY").toString();
    ResY = val.toInt();

}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString IP, Port;
    int F, ResX, ResY;

    QFile f("FileCFG.txt");
    if (!f.open(QIODevice::ReadOnly))
    {
        printf("Ошибка открытия для чтения \n");
    }

    char c[1024] = {};
    while(!f.atEnd())
    {
        int nBlocksize = f.read(c, sizeof(c));
        printf("%d \n", nBlocksize);
    }

    parametrsPars(c, IP, Port, F, ResX, ResY);

    QImage image(ResX, ResY, QImage::Format_Grayscale8);
    int v = 0;

    qsrand(QDateTime::currentMSecsSinceEpoch());
    for(int i = 0; i < ResX; i++){
        for(int j = 0; j < ResY; j++){
            v = qrand() % 255;
            image.setPixel(i, j, QColor(v, v, v).rgb());
        }
    }


    image.save("1.jpg");





    return a.exec();
}


#include <QtGui>
#include <QtNetwork>

TCPClient::TCPClient(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("TCP Client");

    socket=new QTcpSocket(this);
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readServer()));

    QFormLayout *layout=new QFormLayout(this);

    host=new QLineEdit("127.0.0.1", this);
    layout->addRow("Host:", host);

    port=new QSpinBox(this);
    port->setRange(0,9999);
    port->setValue(5000);
    layout->addRow("Port:", port);

    control=new QPushButton("Connect", this);
    connect(control, SIGNAL(clicked()), this, SLOT(connectToHost()));
    layout->addWidget(control);

    trace=new QTextEdit(this);
    trace->setReadOnly(true);
    layout->addWidget(trace);
}

void TCPClient::connectToHost(){
    socket->connectToHost(QHostAddress(host->text()), port->value());
}

void TCPClient::connected(){
    socket->write(QString("Hello Server").toAscii());
    trace->append("Connected");
    socket->waitForBytesWritten();
}

void TCPClient::readServer(){
    trace->append(socket->readAll());
}
