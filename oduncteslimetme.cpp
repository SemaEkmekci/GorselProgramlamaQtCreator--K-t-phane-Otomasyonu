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

