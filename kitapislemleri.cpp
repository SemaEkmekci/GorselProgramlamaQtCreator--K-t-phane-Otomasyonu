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




