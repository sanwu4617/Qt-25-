#ifndef SHOP_H
#define SHOP_H

#include <QDialog>
#include "Event.h"
namespace Ui {
class Shop;
}

class Shop : public QDialog
{
    Q_OBJECT

public:
    explicit Shop(QWidget *parent = nullptr);
    void Reset();
    ~Shop();

private slots:
    void on_add_atk_valueChanged(int arg1);

    void on_add_def_valueChanged(int arg1);

    void on_add_hp_valueChanged(int arg1);

    void on_add_yellowkey_valueChanged(int arg1);

    void on_add_bluekey_valueChanged(int arg1);

    void on_add_redkey_valueChanged(int arg1);

    void on_add_greenkey_valueChanged(int arg1);

    void on_pushButton_clicked();

    void on_cancel_clicked();

signals:
    void end_shop();
private:
    Ui::Shop *ui;
    int need_gold;
    int get_atk;
    int get_def;
    int get_hp;
    int get_yellowkey;
    int get_bluekey;
    int get_redkey;
    int get_greenkey;
    void set_NeedGold();
};

#endif // SHOP_H
