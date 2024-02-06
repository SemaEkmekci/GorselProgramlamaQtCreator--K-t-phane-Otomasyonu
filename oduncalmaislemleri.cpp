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
