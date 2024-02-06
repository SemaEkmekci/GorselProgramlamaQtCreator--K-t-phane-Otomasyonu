#ifndef UYEISLEMLERI_H
#define UYEISLEMLERI_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>

namespace Ui {
class UyeIslemleri;
}

class UyeIslemleri : public QDialog
{
    Q_OBJECT

public:
    explicit UyeIslemleri(QSqlDatabase,QWidget *parent = nullptr);
    ~UyeIslemleri();
    void ekrandaGoster();

private slots:
    void on_pushButtonYeniKayit_clicked();

    void on_pushButtonGuncelle_clicked();

    void on_tableViewTumUyeler_clicked(const QModelIndex &index);

    void on_pushButtonSil_clicked();

private:
    Ui::UyeIslemleri *ui;
    QSqlQuery *sorgu;
    QSqlQueryModel *model;
    QSqlQuery *sorguKontrol;
};

#endif // UYEISLEMLERI_H
