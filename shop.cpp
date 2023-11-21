#include "shop.h"
#include "ui_shop.h"

Shop::Shop(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Shop)
{
    need_gold=0;
    get_atk=0;
    get_def=0;
    get_hp=0;
    get_yellowkey=0;
    get_bluekey=0;
    get_redkey=0;
    get_greenkey=0;
    ui->setupUi(this);
    ui->notenough->hide();
}

Shop::~Shop()
{
    delete ui;
}

void Shop::set_NeedGold()
{
    need_gold=0;
    need_gold+=get_atk*10;
    need_gold+=get_def*10;
    need_gold+=(get_hp+upgrade_exp-1)/upgrade_exp;
    need_gold+=get_yellowkey*3;
    need_gold+=get_bluekey*10;
    need_gold+=get_redkey*40;
    need_gold+=get_greenkey*200;
    ui->Need_Gold->setText(QString::number(need_gold));
}

void Shop::on_add_atk_valueChanged(int arg1)
{
    get_atk=arg1;
    set_NeedGold();
}

void Shop::on_add_def_valueChanged(int arg1)
{
    get_def=arg1;
    set_NeedGold();
}

void Shop::on_add_hp_valueChanged(int arg1)
{
    get_hp=arg1;
    set_NeedGold();
}

void Shop::on_add_yellowkey_valueChanged(int arg1)
{
    get_yellowkey=arg1;
    set_NeedGold();
}

void Shop::on_add_bluekey_valueChanged(int arg1)
{
    get_bluekey=arg1;
    set_NeedGold();
}

void Shop::on_add_redkey_valueChanged(int arg1)
{
    get_redkey=arg1;
    set_NeedGold();
}

void Shop::on_add_greenkey_valueChanged(int arg1)
{
    get_greenkey=arg1;
    set_NeedGold();
}

void Shop::on_pushButton_clicked()
{
    if(gold>=need_gold)
    {
        if(need_gold>0)
            road_log[rlf++]='S';
        char log_temp=0;
        if(get_atk>0)
        {
            log_temp=1;
            memcpy(road_log+rlf,&log_temp,1);
            memcpy(road_log+rlf+1,&get_atk,sizeof(int));
            rlf+=5;
        }
        if(get_def>0)
        {
            log_temp=2;
            memcpy(road_log+rlf,&log_temp,1);
            memcpy(road_log+rlf+1,&get_def,sizeof(int));
            rlf+=5;
        }
        if(get_hp>0)
        {
            log_temp=3;
            memcpy(road_log+rlf,&log_temp,1);
            memcpy(road_log+rlf+1,&get_hp,sizeof(int));
            rlf+=5;
        }
        if(get_yellowkey>0)
        {
            log_temp=4;
            memcpy(road_log+rlf,&log_temp,1);
            memcpy(road_log+rlf+1,&get_yellowkey,sizeof(int));
            rlf+=5;
        }
        if(get_bluekey>0)
        {
            log_temp=5;
            memcpy(road_log+rlf,&log_temp,1);
            memcpy(road_log+rlf+1,&get_bluekey,sizeof(int));
            rlf+=5;
        }
        if(get_redkey>0)
        {
            log_temp=6;
            memcpy(road_log+rlf,&log_temp,1);
            memcpy(road_log+rlf+1,&get_redkey,sizeof(int));
            rlf+=5;
        }
        if(get_greenkey>0)
        {
            log_temp=7;
            memcpy(road_log+rlf,&log_temp,1);
            memcpy(road_log+rlf+1,&get_greenkey,sizeof(int));
            rlf+=5;
        }
        road_log[rlf++]='E';
        delta_atk=get_atk;
        delta_def=get_def;
        delta_hp=get_hp;
        delta_yellowkey=get_yellowkey;
        delta_bluekey=get_bluekey;
        delta_redkey=get_redkey;
        delta_greenkey=get_greenkey;
        delta_gold=-need_gold;
        get_atk=0;
        get_def=0;
        get_hp=0;
        get_yellowkey=0;
        get_bluekey=0;
        get_redkey=0;
        get_greenkey=0;
        need_gold=0;
        ui->add_hp->setValue(0);
        ui->add_atk->setValue(0);
        ui->add_def->setValue(0);
        ui->add_redkey->setValue(0);
        ui->add_bluekey->setValue(0);
        ui->add_greenkey->setValue(0);
        ui->add_yellowkey->setValue(0);
        ui->notenough->hide();
        emit end_shop();
        close();
    }
    else
    {
        ui->notenough->show();
    }
}

void Shop::Reset()
{
    ui->label_11->setText("1金币"+QString::number(upgrade_exp)+"生命");
    ui->notenough->hide();
}

void Shop::on_cancel_clicked()
{
    close();
}
