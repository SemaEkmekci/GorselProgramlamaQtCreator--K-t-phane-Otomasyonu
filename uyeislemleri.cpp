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

