#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "uyeislemleri.h"
#include "kitapislemleri.h"
#include "oduncalmaislemleri.h"
#include "oduncteslimetme.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db.setDatabaseName("kutuphane_otomasyonu.db");
    if(!db.open())
    {
        ui->statusbar->showMessage("Veri Tabanına Bağlanılamadı");
    }
    else
    {
        ui->statusbar->showMessage("Veri Tabanına Bağlanıldı");
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    UyeIslemleri *uye = new UyeIslemleri(db);
    uye->show();
}

void MainWindow::on_pushButton_2_clicked()
{
    KitapIslemleri *kitap = new KitapIslemleri(db);
    kitap->show();
}


void MainWindow::on_pushButton_3_clicked()
{
    OduncAlmaIslemleri *odunc = new OduncAlmaIslemleri(db);
    odunc->show();
}


void MainWindow::on_pushButton_4_clicked()
{
    OduncTeslimEtme *teslim = new OduncTeslimEtme(db);
    teslim->show();
}

