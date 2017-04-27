#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /* get devices num and descriptions */
    CAP c;
    auto n = c.getInterfaceCount();
    auto d = c.getDescription(ui->spinBox->value());

    ui->spinBox->setMaximum(n);
    ui->ifDescription->setText(d);
    c.getLocalMac(ui->spinBox->value());

    QString name_template("lineEdit_%1");
    for(int i=6, j=0; i<12; i++, j++){
        QLineEdit *edit = ui->parentWidget->findChild<QLineEdit *>(name_template.arg(i));
        edit->setText(c.returnMac(j));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_sendButton_clicked()
{
    /* open interface */
   CAP c;
   if(c.openInterface(ui->spinBox->value())){
       ui->statusBar->showMessage("Adapter opened", 1000);
   }

   /* form packet to send */
   QByteArray pack;
   pack.resize(60);
   pack = QByteArray::fromHex(ui->lineEdit_0->text().toLatin1());

   /* fill other fields */
   QString name_template("lineEdit_%1");
   for(int i = 1; i < 60; ++i)
   {
       QLineEdit *edit = ui->parentWidget->findChild<QLineEdit *>(name_template.arg(i));
       pack.append(QByteArray::fromHex(edit->text().toLatin1()));
   }

   /* sending data */
   if(c.sendPacket(pack.data())){
       ui->statusBar->showMessage("Packet sent", 1000);
   }
}

QByteArray bitsToBytes(QBitArray bits) {
    QByteArray bytes;
    bytes.resize(bits.count()/9+1);
    bytes.fill(0);
    // Convert from QBitArray to QByteArray
    for(int b=0; b<bits.count(); ++b)
        bytes[b/8] = (bytes.at(b/8) | ((bits[b]?1:0)<<(7-(b%8))));
    return bytes;
}

void MainWindow::on_setFlagsButton_clicked()
{
    QBitArray bits(8);
    QByteArray bytes;
    bits.fill(false);

    if(ui->spinBox_0->value() == 1)
        bits.setBit(0, true);

    if(ui->spinBox_1->value() == 1)
        bits.setBit(1, true);

    if(ui->spinBox_2->value() == 1)
        bits.setBit(2, true);

    if(ui->spinBox_3->value() == 1)
        bits.setBit(3, true);

    if(ui->spinBox_4->value() == 1)
        bits.setBit(4, true);

    if(ui->spinBox_5->value() == 1)
        bits.setBit(5, true);

    if(ui->spinBox_6->value() == 1)
        bits.setBit(6, true);

    if(ui->spinBox_7->value() == 1)
        bits.setBit(7, true);

    bytes = bitsToBytes(bits);
    ui->lineEdit_21->setText(QString(bytes.toHex()));
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    CAP c;
    auto d = c.getDescription(arg1);
    ui->ifDescription->setText(d);
    c.getLocalMac(ui->spinBox->value());

    QString name_template("lineEdit_%1");
    for(int i=6, j=0; i<12; i++, j++){
        QLineEdit *edit = ui->parentWidget->findChild<QLineEdit *>(name_template.arg(i));
        edit->setText(c.returnMac(j));
    }
}
