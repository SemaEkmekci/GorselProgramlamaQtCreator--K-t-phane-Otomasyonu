#ifndef KITAPISLEMLERI_H
#define KITAPISLEMLERI_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>

namespace Ui {
class KitapIslemleri;
}

class KitapIslemleri : public QDialog
{
    Q_OBJECT

public:
    explicit KitapIslemleri(QSqlDatabase ,QWidget *parent = nullptr);
    ~KitapIslemleri();
    void ekrandaGoster();
    void ekrandaGosterOduncKitap();
    void ekrandaGosterTeslimKitap();

private slots:
    void on_tableViewTumKitaplar_clicked(const QModelIndex &index);

    void on_pushButtonYeniKayit_clicked();

    void on_pushButtonGuncelle_clicked();

    void on_pushButtonSil_clicked();

private:
    Ui::KitapIslemleri *ui;
    QSqlQuery *sorguKitap;
    QSqlQueryModel *modelKitap;
    QSqlQuery *sorguOduncKitap;
    QSqlQueryModel *modelOduncKitap;
    QSqlQuery *sorguTeslimKitap;
    QSqlQueryModel *modelTeslimKitap;
    QSqlQuery *sorguKontrol;

};

#endif // KITAPISLEMLERI_H
