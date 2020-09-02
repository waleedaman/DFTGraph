#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QStandardPaths>
#include <QVectorIterator>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->openFile,SIGNAL(clicked(bool)),this,SLOT(openFile()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openFile(){
    QString filePath;
    const QString documentsFolder = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    filePath = QFileDialog::getOpenFileName(this,
        tr("Open File"), documentsFolder, tr("Text Files (*.txt)"));
    ui->filePath->setText(filePath);
    QVector<qint64> fileData = readDataFromFile(filePath);
    QVectorIterator<qint64> iter(fileData);
    QStringList list;
    while (iter.hasNext()) {
        list.append(QString::number(iter.next()));
    }
    ui->dataList->clear();
    ui->dataList->addItems(list);

}

QVector<qint64> MainWindow::readDataFromFile(QString filePath){
    QVector<qint64> *vector = new QVector<qint64>;
    vector->reserve(5000000);
    QFile readFile(filePath);
    if(!readFile.open(QFile::ReadOnly | QFile::Text))
    {
           // Can't Open File.
    }
    else
    {
        QByteArray data;
        int var = 0;
         while (!readFile.atEnd())
        {
            vector->insert(var, (readFile.readLine()).toInt());
            var++;
        }
     }
     readFile.close();
     return *vector;
}
