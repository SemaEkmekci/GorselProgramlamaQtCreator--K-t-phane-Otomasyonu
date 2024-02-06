#ifndef ODUNCTESLIMETME_H
#define ODUNCTESLIMETME_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>

namespace Ui {
class OduncTeslimEtme;
}

class OduncTeslimEtme : public QDialog
{
    Q_OBJECT

public:
    explicit OduncTeslimEtme(QSqlDatabase,QWidget *parent = nullptr);
    ~OduncTeslimEtme();
    void ekrandaGosterOduncKitap();
    void ekrandaGosterTeslimKitap();
    int borcHesapla();

private slots:
    void on_tableView_2_clicked(const QModelIndex &index);

    void on_pushButton_clicked();

private:
    Ui::OduncTeslimEtme *ui;
    QSqlQuery *sorguTeslimKitap;
    QSqlQueryModel *modelTeslimKitap;
    QSqlQuery *sorguOduncKitap;
    QSqlQueryModel *modelOduncKitap;
    QSqlQuery *sorguOduncKitapSil;
    QSqlQuery *sorguTeslimKitapEkle;
    int borc = 0;

};

#endif // ODUNCTESLIMETME_H
