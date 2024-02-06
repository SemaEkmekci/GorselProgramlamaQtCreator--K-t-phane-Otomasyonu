#ifndef ODUNCALMAISLEMLERI_H
#define ODUNCALMAISLEMLERI_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>

namespace Ui {
class OduncAlmaIslemleri;
}

class OduncAlmaIslemleri : public QDialog
{
    Q_OBJECT

public:
    explicit OduncAlmaIslemleri(QSqlDatabase,QWidget *parent = nullptr);
    ~OduncAlmaIslemleri();
    void ekrandaGosterUyeler();
    void ekrandaGosterKitaplar();
    void ekrandaGosterOdunc();


private slots:


    void on_tableViewTumUyeler_clicked(const QModelIndex &index);

    void on_tableViewTumKitaplar_clicked(const QModelIndex &index);

    void on_pushButton_clicked();

private:
    Ui::OduncAlmaIslemleri *ui;
    QSqlQuery *sorgu;
    QSqlQueryModel *model;
    QSqlQuery *sorguKitap;
    QSqlQueryModel *modelKitap;
    QSqlQuery *sorguOdunc;
    QSqlQueryModel *modelOdunc;
    QSqlQuery *sorguAyniKitapKontrol;
};

#endif // ODUNCALMAISLEMLERI_H
