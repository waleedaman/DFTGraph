#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QStandardPaths>
#include <QVectorIterator>
#include <QDebug>
#include "./fft.cpp"
#include <QLineSeries>
#include <QtCharts>
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
    QVector<double> fileData = readDataFromFile(filePath);
    std::vector<Complex> vct = std::vector<Complex>(fileData.begin(),fileData.end());
    const Complex test[] = { 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0 };
    CArray data(test, 8);
//    fft(std::vector<Complex>(test,test+8));
    fft(vct);
    std::cout << "fft" << std::endl;
    for (int i = 0; i < 8; ++i)
    {
        std::cout << vct[i] << std::endl;
    }
    QVectorIterator<Complex> iter(QVector<Complex>(vct.begin(),vct.end()));
    QStringList list;
    QtCharts::QLineSeries *series = new QtCharts::QLineSeries();
    int index = 0;
    while (iter.hasNext()) {
        Complex c = iter.next();
        list.append("("+QString::number(c.real())+","+QString::number(c.imag())+")");
        series->append(index,c.real());
        index++;
    }
    ui->dataList->clear();
    ui->dataList->addItems(list);
    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("Plot");
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->qgrid->addWidget(chartView);
//    QMainWindow window;
//    window.setCentralWidget(chartView);
//    window.resize(400, 300);
//    window.show();
}

QVector<double> MainWindow::readDataFromFile(QString filePath){
    QVector<double> *vector = new QVector<double>;
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
            vector->insert(var, (readFile.readLine()).toDouble());
            var++;
        }
     }
     readFile.close();
     return *vector;
}
