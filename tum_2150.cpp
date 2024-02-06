#include "kitapislemleri.h"
#include "ui_kitapislemleri.h"

KitapIslemleri::KitapIslemleri(QSqlDatabase db,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KitapIslemleri)
{
    ui->setupUi(this);
    sorguKitap=new QSqlQuery(db);
    sorguOduncKitap=new QSqlQuery(db);
    sorguTeslimKitap=new QSqlQuery(db);
    sorguKontrol=new QSqlQuery(db);
    ekrandaGoster();
    ekrandaGosterOduncKitap();
}

KitapIslemleri::~KitapIslemleri()
{
    delete ui;
}

void KitapIslemleri::ekrandaGoster()
{
    sorguKitap->prepare("select * from kitap");
    if(!sorguKitap->exec())
    {
        QMessageBox::critical(this,"Hata!","Ekranda Goster Sorgusu Çalışmııyooor!!","Ok");
        return;
    }
    modelKitap = new QSqlQueryModel();
    modelKitap->setQuery(*sorguKitap);
    ui->tableViewTumKitaplar->setModel(modelKitap);
}

void KitapIslemleri::ekrandaGosterOduncKitap()
{

    sorguOduncKitap->prepare("select * from odunc_alinan where kitap_no=?");
    sorguOduncKitap->addBindValue(ui->kitapNo->text());
    if(!sorguOduncKitap->exec())
    {
        QMessageBox::critical(this,"Hata!","Ekranda Goster Sorgusu Çalışmııyooor!!","Ok");
        return;
    }
    modelOduncKitap = new QSqlQueryModel();
    modelOduncKitap->setQuery(*sorguOduncKitap);
    ui->tableView_2->setModel(modelOduncKitap);
}

void KitapIslemleri::ekrandaGosterTeslimKitap()
{

    sorguTeslimKitap->prepare("select * from odunc_teslim_edilen where kitap_no=?");
    sorguTeslimKitap->addBindValue(ui->kitapNo->text());
    if(!sorguTeslimKitap->exec())
    {
        QMessageBox::critical(this,"Hata!","Ekranda Goster Sorgusu Çalışmııyooor!!","Ok");
        return;
    }
    modelTeslimKitap = new QSqlQueryModel();
    modelTeslimKitap->setQuery(*sorguTeslimKitap);
    ui->tableView_3->setModel(modelTeslimKitap);
}

void KitapIslemleri::on_tableViewTumKitaplar_clicked(const QModelIndex &index)
{
    ui->kitapNo->setText(modelKitap->index(index.row(),0).data().toString());
    ui->kitapAd->setText(modelKitap->index(index.row(),1).data().toString());
    ui->kitapStok->setText(modelKitap->index(index.row(),2).data().toString());
    ekrandaGosterOduncKitap();
    ekrandaGosterTeslimKitap();
}


void KitapIslemleri::on_pushButtonYeniKayit_clicked()
{
    sorguKitap->prepare("insert into kitap(kitap_ad,kitap_sayisi) values(?,?)");
    if(ui->kitapAd->text().isEmpty() || ui->kitapStok->text().isEmpty())
    {
        QMessageBox::critical(this,"Hata!","Gerekli Alanları Doldurunuz!","Ok");
    }
    else{
        sorguKitap->addBindValue(ui->kitapAd->text());
        sorguKitap->addBindValue(ui->kitapStok->text());
        if(!sorguKitap->exec())
        {
            QMessageBox::critical(this,"Hata!","Ekleme Sorgusu Çalışmııyooor!!","Ok");
            return;
        }
        ekrandaGoster();
    }
}


void KitapIslemleri::on_pushButtonGuncelle_clicked()
{
    if(ui->kitapAd->text().isEmpty() || ui->kitapStok->text().isEmpty())
    {
        QMessageBox::critical(this,"Hata!","Gerekli Alanları Doldurunuz!","Ok");
    }else{
    sorguKitap->prepare("update kitap set kitap_ad=?,kitap_sayisi=? where kitap_no=?");
    sorguKitap->addBindValue(ui->kitapAd->text());
    sorguKitap->addBindValue(ui->kitapStok->text());
    sorguKitap->addBindValue(ui->kitapNo->text().toInt());
    if(!sorguKitap->exec())
    {
        QMessageBox::critical(this,"Hata!","Güncelle Sorgusu Çalışmııyooor!!","Ok");
        return;
    }
    ekrandaGoster();
    }}


void KitapIslemleri::on_pushButtonSil_clicked()
{
    //Üye kitap kontrolü yapılacaak
    int kontrol = ui->kitapNo->text().toInt();
    sorguKontrol->prepare("select * from odunc_alinan where kitap_no=?");
    sorguKontrol->addBindValue(kontrol);
    if(!sorguKontrol->exec())
    {
        QMessageBox::critical(this,"Hata!","Kontrol Sorgusu Çalışmııyooor!!","Ok");
        return;

    }
    if (sorguKontrol->next())
    {
        QMessageBox::critical(this, "Hata", "Bu kitap silinemez. Bu kitap bir üyeye ödünç verilmiştir.", "Ok");

    }
    else{
    sorguKitap->prepare("delete from kitap where kitap_no=?");
    sorguKitap->addBindValue(ui->kitapNo->text().toInt());
    if(!sorguKitap->exec())
    {
        QMessageBox::critical(this,"Hata!","Sil Sorgusu Çalışmııyooor!!","Ok");
        return;
    }
    }
    ekrandaGoster();
}

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

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

#include "oduncalmaislemleri.h"
#include "ui_oduncalmaislemleri.h"

OduncAlmaIslemleri::OduncAlmaIslemleri(QSqlDatabase db,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OduncAlmaIslemleri)
{
    ui->setupUi(this);
    sorgu=new QSqlQuery(db);
    sorguKitap = new QSqlQuery(db);
    sorguOdunc = new QSqlQuery(db);
    sorguAyniKitapKontrol = new QSqlQuery(db);
    ekrandaGosterKitaplar();
    ekrandaGosterUyeler();
    ekrandaGosterOdunc();
}

OduncAlmaIslemleri::~OduncAlmaIslemleri()
{
    delete ui;
}

void OduncAlmaIslemleri::ekrandaGosterUyeler()
{
    sorgu->prepare("select * from uye");
    if(!sorgu->exec())
    {
    QMessageBox::critical(this,"Hata!","Ekranda Goster Sorgusu Çalışmııyooor!!","Ok");
    return;
    }
    model = new QSqlQueryModel();
    model->setQuery(*sorgu);
    ui->tableViewTumUyeler->setModel(model);
}

void OduncAlmaIslemleri::ekrandaGosterKitaplar()
{
    sorguKitap->prepare("select * from kitap");
    if(!sorguKitap->exec())
    {
    QMessageBox::critical(this,"Hata!","Ekranda Goster Sorgusu Çalışmııyooor!!","Ok");
    return;
    }
    modelKitap = new QSqlQueryModel();
    modelKitap->setQuery(*sorguKitap);
    ui->tableViewTumKitaplar->setModel(modelKitap);
}

void OduncAlmaIslemleri::ekrandaGosterOdunc()
{
    sorguOdunc->prepare("select * from odunc_alinan");
    if(!sorguOdunc->exec())
    {
    QMessageBox::critical(this,"Hata!","Ekranda Goster Sorgusu Çalışmııyooor!!","Ok");
    return;
    }
    modelOdunc = new QSqlQueryModel();
    modelOdunc->setQuery(*sorguOdunc);
    ui->tableViewTumOdunc->setModel(modelOdunc);
}


void OduncAlmaIslemleri::on_tableViewTumUyeler_clicked(const QModelIndex &index)
{
    ui->uyeNo->setText(model->index(index.row(),0).data().toString());
}


void OduncAlmaIslemleri::on_tableViewTumKitaplar_clicked(const QModelIndex &index)
{
    ui->kitapNo->setText(modelKitap->index(index.row(),0).data().toString());
}


void OduncAlmaIslemleri::on_pushButton_clicked()
{
    sorgu->prepare("insert into uye(uye_ad,uye_soyad) values(?,?)");
    if(ui->kitapNo->text().isEmpty() || ui->uyeNo->text().isEmpty())
    {
    QMessageBox::critical(this,"Hata!","Gerekli Alanları Doldurunuz!","Ok");
    }
    else{
    sorguAyniKitapKontrol->prepare("SELECT * FROM odunc_alinan WHERE kitap_no=? AND uye_no=?");
    sorguAyniKitapKontrol->addBindValue(ui->kitapNo->text().toInt());
    sorguAyniKitapKontrol->addBindValue(ui->uyeNo->text().toInt());
    if (sorguAyniKitapKontrol->exec() && sorguAyniKitapKontrol->next())
    {
        QMessageBox::critical(this, "Hata", "Bu üye bu kitabın bir tanesini şu an ödünç almış tekrar ödünç verilemez!", "Ok");
    }
    else {
        sorgu->prepare("INSERT INTO odunc_alinan(uye_no, kitap_no, odunc_alma_tarihi) VALUES (?, ?, ?)");
        sorgu->addBindValue(ui->uyeNo->text().toInt());
        sorgu->addBindValue(ui->kitapNo->text().toInt());
        sorgu->addBindValue(ui->dateEdit->text());
        if (!sorgu->exec())
        {
            QMessageBox::critical(this, "Hata!", "Ekleme Sorgusu Çalışmıyor!!", "Ok");
            return;
        }
        ekrandaGosterOdunc();
    }
    }}

#include "oduncteslimetme.h"
#include "ui_oduncteslimetme.h"
#include <iostream>

OduncTeslimEtme::OduncTeslimEtme(QSqlDatabase db,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OduncTeslimEtme)
{
    ui->setupUi(this);
    sorguTeslimKitap=new QSqlQuery(db);
    sorguOduncKitap=new QSqlQuery(db);
    sorguOduncKitapSil=new QSqlQuery(db);
    sorguTeslimKitapEkle = new QSqlQuery(db);
    ekrandaGosterOduncKitap();
    ekrandaGosterTeslimKitap();

}

OduncTeslimEtme::~OduncTeslimEtme()
{
    delete ui;
}

void OduncTeslimEtme::ekrandaGosterOduncKitap()
{
    sorguOduncKitap->prepare("select * from odunc_alinan");
    if(!sorguOduncKitap->exec())
    {
    QMessageBox::critical(this,"Hata!","Ekranda Goster Sorgusu Çalışmııyooor!!","Ok");
    return;
    }
    modelOduncKitap = new QSqlQueryModel();
    modelOduncKitap->setQuery(*sorguOduncKitap);
    ui->tableView_2->setModel(modelOduncKitap);

}

void OduncTeslimEtme::ekrandaGosterTeslimKitap()
{
    sorguTeslimKitap->prepare("select * from odunc_teslim_edilen");
    if(!sorguTeslimKitap->exec())
    {
    QMessageBox::critical(this,"Hata!","Ekranda Goster Sorgusu Çalışmııyooor!!","Ok");
    return;
    }
    modelTeslimKitap = new QSqlQueryModel();
    modelTeslimKitap->setQuery(*sorguTeslimKitap);
    ui->tableView_3->setModel(modelTeslimKitap);
}

int OduncTeslimEtme::borcHesapla()
{
    QString oduncVerilenTarih=ui->dateEdit->text();
    QString oduncAlinanTarih=ui->firstDate->text();
    qInfo() << oduncAlinanTarih;
    QStringList oduncVerilenlerListesi=oduncVerilenTarih.split(".");
    QStringList oduncAlinanlarListesi=oduncAlinanTarih.split(".");
    int gunFarki = (oduncVerilenlerListesi[0].toInt()-oduncAlinanlarListesi[0].toInt())+((oduncVerilenlerListesi[1].toInt()-oduncAlinanlarListesi[1].toInt())*30)+((oduncVerilenlerListesi[2].toInt()-oduncAlinanlarListesi[2].toInt())*365);

    qInfo() << gunFarki;
    if(gunFarki>15)
    {
    borc=gunFarki-15;

    }
    return borc;
}




void OduncTeslimEtme::on_tableView_2_clicked(const QModelIndex &index)
{
    ui->kitapNo->setText(modelOduncKitap->index(index.row(),1).data().toString());
    ui->uyeNo->setText(modelOduncKitap->index(index.row(),0).data().toString());
    ui->firstDate->setText(modelOduncKitap->index(index.row(),2).data().toString());
    qInfo() << ui->firstDate->text();
}




void OduncTeslimEtme::on_pushButton_clicked()
{

    sorguOduncKitapSil->prepare("delete from odunc_alinan where kitap_no=?");
    sorguOduncKitapSil->addBindValue(ui->kitapNo->text().toInt());
    if(!sorguOduncKitapSil->exec())
    {
    QMessageBox::critical(this,"Hata!","Sil Sorgusu Çalışmııyooor!!","Ok");
    return;
    }
    else
    {
    borc = borcHesapla();
    }
    ekrandaGosterOduncKitap();
    sorguTeslimKitapEkle->prepare("insert into odunc_teslim_edilen(uye_no,kitap_no,alma_tarihi,verme_tarihi,borc) values(?,?,?,?,?)");
    sorguTeslimKitapEkle->addBindValue(ui->uyeNo->text());
    sorguTeslimKitapEkle->addBindValue(ui->kitapNo->text());
    sorguTeslimKitapEkle->addBindValue(ui->firstDate->text());
    sorguTeslimKitapEkle->addBindValue(ui->dateEdit->date().toString("dd.MM.yyyy"));
    sorguTeslimKitapEkle->addBindValue(borc);

    if(!sorguTeslimKitapEkle->exec())
    {
    QMessageBox::critical(this,"Hata!","Ekleme Sorgusu Çalışmııyooor!!","Ok");
    return;
    }
    ekrandaGosterTeslimKitap();


}

#include "uyeislemleri.h"
#include "ui_uyeislemleri.h"

UyeIslemleri::UyeIslemleri(QSqlDatabase db,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UyeIslemleri)
{
    ui->setupUi(this);
    sorgu=new QSqlQuery(db);
    sorguKontrol=new QSqlQuery(db);
    ekrandaGoster();
}

UyeIslemleri::~UyeIslemleri()
{
    delete ui;
}
void UyeIslemleri::ekrandaGoster()
{
    sorgu->prepare("select * from uye");
    if(!sorgu->exec())
    {
        QMessageBox::critical(this,"Hata!","Ekranda Goster Sorgusu Çalışmııyooor!!","Ok");
        return;
    }
    model = new QSqlQueryModel();
    model->setQuery(*sorgu);
    ui->tableViewTumUyeler->setModel(model);
}

void UyeIslemleri::on_pushButtonYeniKayit_clicked()
{
    sorgu->prepare("insert into uye(uye_ad,uye_soyad) values(?,?)");
    if(ui->uyeAd->text().isEmpty() || ui->uyeSoyad->text().isEmpty())
    {
        QMessageBox::critical(this,"Hata!","Gerekli Alanları Doldurunuz!","Ok");
    }
    else{
    sorgu->addBindValue(ui->uyeAd->text());
    sorgu->addBindValue(ui->uyeSoyad->text());
    if(!sorgu->exec())
    {
        QMessageBox::critical(this,"Hata!","Ekleme Sorgusu Çalışmııyooor!!","Ok");
        return;
    }
    ekrandaGoster();
    }}


void UyeIslemleri::on_pushButtonGuncelle_clicked()
{
    sorgu->prepare("update uye set uye_ad=?,uye_soyad=? where uye_no=?");
    sorgu->addBindValue(ui->uyeAd->text());
    sorgu->addBindValue(ui->uyeSoyad->text());
    sorgu->addBindValue(ui->uyeNo->text().toInt());
    if(!sorgu->exec())
    {
        QMessageBox::critical(this,"Hata!","Güncelle Sorgusu Çalışmııyooor!!","Ok");
        return;
    }
    ekrandaGoster();
}


void UyeIslemleri::on_tableViewTumUyeler_clicked(const QModelIndex &index)
{
    ui->uyeNo->setText(model->index(index.row(),0).data().toString());
    ui->uyeAd->setText(model->index(index.row(),1).data().toString());
    ui->uyeSoyad->setText(model->index(index.row(),2).data().toString());
}


void UyeIslemleri::on_pushButtonSil_clicked()
{
    //Üye kitap kontrolü yapılacaak
    int kontrol = ui->uyeNo->text().toInt();
    sorguKontrol->prepare("select * from odunc_alinan where uye_no=?");

    sorguKontrol->addBindValue(kontrol);
    if(!sorguKontrol->exec())
    {
        QMessageBox::critical(this,"Hata!","Kontrol Sorgusu Çalışmııyooor!!","Ok");
        return;

    }
    if (sorguKontrol->next())
    {
        QMessageBox::critical(this, "Hata", "Bu üye silinemez. Üyenin henüz teslim etmediği kitaplar vardır.", "Ok");

    }
    else
    {
        sorgu->prepare("delete from uye where uye_no=?");
        sorgu->addBindValue(ui->uyeNo->text().toInt());

        if(!sorgu->exec())
        {
        QMessageBox::critical(this,"Hata!","Sil Sorgusu Çalışmııyooor!!","Ok");
        }
        ekrandaGoster();

    }
}

