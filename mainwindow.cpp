#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //初始化勇士数据
    showing=false;
    end_show=false;
    floor_id=1;
    level=1;
    hp=2000;
    atk=10;
    def=10;
    gold=0;
    Exp=0;
    yellowkey=0;
    bluekey=0;
    redkey=0;
    greenkey=0;
    upgrade_exp=16;
    setWarriorData();
    x=5;
    y=10;
    min_x=250;
    min_y=40;
    pics=new PButton*[MAX_X];
    hurts=new QLabel*[MAX_X];
    crips=new QLabel*[MAX_X];
    gameover=false;
    win=false;
    for(int i=0;i<MAX_X;i++)
    {
        pics[i]=new PButton[MAX_Y];
        hurts[i]=new QLabel[MAX_Y];
        crips[i]=new QLabel[MAX_Y];
    }
    QFont font ("宋体", 8, 75); //第一个属性是字体（微软雅黑），第二个是大小，第三个是加粗（权重是75）

    for(int i=0;i<MAX_X;i++)
    {
        for(int j=0;j<MAX_Y;j++)
        {
            pics[i][j].setGeometry(min_x+i*48,min_y+j*48,48,48);
            pics[i][j].setParent(this);
            pics[i][j].setFocusPolicy(Qt::NoFocus);
            pics[i][j].setxy(i,j);
            pics[i][j].show();
            hurts[i][j].setGeometry(min_x+i*48,min_y+j*48+36,48,12);
            hurts[i][j].setParent(this);
            hurts[i][j].setAlignment(Qt::AlignRight);
            hurts[i][j].setStyleSheet("color: rgb(255, 64, 64);");
            hurts[i][j].setFont(font);
            hurts[i][j].show();
            crips[i][j].setGeometry(min_x+i*48,min_y+j*48,48,12);
            crips[i][j].setParent(this);
            crips[i][j].setAlignment(Qt::AlignRight);
            crips[i][j].setStyleSheet("color: rgb(0, 255, 0);");
            crips[i][j].setFont(font);
            crips[i][j].show();
            connect(&pics[i][j],SIGNAL(show_information(int,int)),this,SLOT(show_information(int,int)));
        }
    }
    //绘制主角
    warrior_img=new QPushButton(this);
    warrior_img->setGeometry(min_x+x*48,min_y+y*48,48,48);
    warrior_img->setStyleSheet("border-image: url(:/images/Warrior.png);");
    warrior_img->setFocusPolicy(Qt::NoFocus);
    warrior_img->show();
    //刷新
    refresh();
    shop=new Shop;
    shop->close();
    //计算统计信息
    QString temp=QString("统计信息：总攻击%0，总防御%1，总生命%2，总金币%3，总经验%4，怪物总数%5")
                         .arg(QString::number(stat_all_atk),QString::number(stat_all_def),QString::number(stat_all_hp),
                              QString::number(stat_all_gold),QString::number(stat_all_exp),QString::number(stat_all_monster));
    ui->Statistics->setText(temp);
    QString temp2=QString("黄钥匙%0，蓝钥匙%1，红钥匙%2，绿钥匙%3，黄门%4，蓝门%5，红门%6，绿门%7")
                          .arg(QString::number(stat_all_event[11]),QString::number(stat_all_event[12]),
                               QString::number(stat_all_event[13]),QString::number(stat_all_event[14]),
                               QString::number(stat_all_event[21]),QString::number(stat_all_event[22]),
                               QString::number(stat_all_event[23]),QString::number(stat_all_event[24]));
    ui->Statistics_2->setText(temp2);
    //记录存档标记
    save_flag=new QLabel[10];
    for(int i=0;i<10;i++)
    {
        save_flag[i].setGeometry(320+i*40,581,30,30);
        save_flag[i].setText(QString::number((i+1)%10));
        save_flag[i].setParent(this);
        save_flag[i].setFont(font);
        save_flag[i].setFocusPolicy(Qt::NoFocus);
        //尝试打开存档文件
        char filename[11]="Save_i.sav";
        filename[5]='0'+(i+1)%10;
        std::ifstream fin(filename,std::ios::binary);
        save_flag[i].setStyleSheet("color: rgb(255, 255, 255);font: 14pt \"Calibri\";");
        if(!fin)
            save_flag[i].hide();
        else
            save_flag[i].show();
    }
    ui->FastShow_NoRefresh->hide();
    //绘制信息
    information=new QLabel[8];
    for(int i=0;i<8;i++)
    {
        information[i].setGeometry(800,270+i*27,200,24);
        information[i].setParent(this);
        information[i].setStyleSheet("font: 12pt \"微软雅黑\";\ncolor: rgb(255, 255, 255);");
        information[i].setText("");
        information[i].show();
    }
    ui->information_pic->hide();
    ui->information_title->clear();
    ui->Statistics->hide();
    ui->Statistics_2->hide();
    label_showing=new QLabel(this);
    label_showing->setGeometry(320,580,528,30);
    label_showing->setAlignment(Qt::AlignCenter);
    label_showing->setStyleSheet("font: 14pt \"微软雅黑\";\ncolor: rgb(255, 255, 255);");
    label_showing->setText("按Enter键退出播放");
    label_showing->hide();

    label_gameover=new QLabel(this);
    label_gameover->setGeometry(250,263,528,80);
    label_gameover->setStyleSheet("font: 40pt \"微软雅黑\";\ncolor: rgb(255, 0, 0);");
    label_gameover->setAlignment(Qt::AlignCenter);
    label_gameover->setText("Game Over");
    label_gameover->hide();

    label_win=new QLabel(this);
    label_win->setGeometry(250,253,528,100);
    label_win->setStyleSheet("font: 50pt \"微软雅黑\";\ncolor: rgb(255, 0, 0);");
    label_win->setAlignment(Qt::AlignCenter);
    label_win->setText("Win");
    label_win->hide();

    label_score=new QLabel[2];
    for(int i=0;i<2;i++)
    {
        label_score[i].setParent(this);
        label_score[i].setGeometry(250,463+40*i,528,30);
        label_score[i].setStyleSheet("font: 18pt \"微软雅黑\";\ncolor: rgb(255, 0, 0);");
        label_score[i].setAlignment(Qt::AlignCenter);
        label_score[i].hide();
    }

    connect(shop,SIGNAL(end_shop()),this,SLOT(end_shop()));
}

void MainWindow::setWarriorData()
{
    floor_id+=delta_floor;
    atk+=delta_atk;
    def+=delta_def;
    hp+=delta_hp;
    gold+=delta_gold;
    Exp+=delta_exp;
    yellowkey+=delta_yellowkey;
    bluekey+=delta_bluekey;
    redkey+=delta_redkey;
    greenkey+=delta_greenkey;
    if(Exp>=upgrade_exp)
    {
        upgrade_exp*=1.5;
        upgrade_exp+=16;
        level++;
    }
    delta_floor=0;
    delta_atk=0;
    delta_def=0;
    delta_hp=0;
    delta_gold=0;
    delta_exp=0;
    delta_yellowkey=0;
    delta_bluekey=0;
    delta_redkey=0;
    delta_greenkey=0;
    int add_str=5*level-5;
    if(cross)
        add_str+=5;
    ui->floor->setText(QString::number(floor_id));
    ui->level->setText(QString::number(level));
    if(hp<=0)
    {
        ui->HP->setText("Game Over");
        GameOver();
    }
    else
        ui->HP->setText(QString::number(hp));
    if(all_str)
    {
        ui->ATK->setText(QString::number(int(atk*(1+add_str/100.0)+1e-10)));
        ui->DEF->setText(QString::number(int(def*(1+add_str/100.0)+1e-10)));
        ui->atk_add->setText("");
        ui->def_add->setText("");
    }
    else
    {
        ui->ATK->setText(QString::number(atk));
        ui->DEF->setText(QString::number(def));
        ui->atk_add->setText("+"+QString::number(add_str)+"%");
        ui->def_add->setText("+"+QString::number(add_str)+"%");
    }
    ui->GOLD->setText(QString::number(gold));
    ui->EXP->setText(QString::number(Exp)+"/"+QString::number(upgrade_exp));
    ui->yellowkey->setText(QString::number(yellowkey));
    ui->bluekey->setText(QString::number(bluekey));
    ui->redkey->setText(QString::number(redkey));
    ui->greenkey->setText(QString::number(greenkey));


}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if(gameover||win)
        return;
    if(!showing)
    {
        switch(e->key())
        {
        case Qt::Key_Up:
            if(y==0)
                return;
            (*curr_floor)(x,y-1)->Work();
            if(!(*curr_floor)(x,y-1)->getPenetrable()&&end_event)
                (*curr_floor)(x,y-1)=events[0];
            if((*curr_floor)(x,y-1)->getPenetrable())
                y-=1;
            road_log[rlf++]='U';
            Warrior_dir='U';
            break;
        case Qt::Key_Down:
            if(y==MAX_Y-1)
                return;
            (*curr_floor)(x,y+1)->Work();
            if(!(*curr_floor)(x,y+1)->getPenetrable()&&end_event)
                (*curr_floor)(x,y+1)=events[0];
            if((*curr_floor)(x,y+1)->getPenetrable())
                y+=1;
            road_log[rlf++]='D';
            Warrior_dir='D';
            break;
        case Qt::Key_Left:
            if(x==0)
                return;
            (*curr_floor)(x-1,y)->Work();
            if(!(*curr_floor)(x-1,y)->getPenetrable()&&end_event)
                (*curr_floor)(x-1,y)=events[0];
            if((*curr_floor)(x-1,y)->getPenetrable())
                x-=1;
            road_log[rlf++]='L';
            Warrior_dir='L';
            break;
        case Qt::Key_Right:
            if(x==MAX_X-1)
                return;
            (*curr_floor)(x+1,y)->Work();
            if(!(*curr_floor)(x+1,y)->getPenetrable()&&end_event)
                (*curr_floor)(x+1,y)=events[0];
            if((*curr_floor)(x+1,y)->getPenetrable())
                x+=1;
            road_log[rlf++]='R';
            Warrior_dir='R';
            break;
        case Qt::Key_A:
            if(gold>=10)
            {
                delta_atk=1;
                delta_gold=-10;
                road_log[rlf++]='S';
                char log_temp=1,log_temp2='E';
                memcpy(road_log+rlf,&log_temp,1);
                memcpy(road_log+rlf+1,&delta_atk,sizeof(int));
                memcpy(road_log+rlf+5,&log_temp2,1);
                rlf+=6;
            }
            break;
        case Qt::Key_S:
            all_str=!all_str;
            break;
        case Qt::Key_D:
            if(gold>=10)
            {
                delta_def=1;
                delta_gold=-10;
                road_log[rlf++]='S';
                char log_temp=2,log_temp2='E';
                memcpy(road_log+rlf,&log_temp,1);
                memcpy(road_log+rlf+1,&delta_def,sizeof(int));
                memcpy(road_log+rlf+5,&log_temp2,1);
                rlf+=6;
            }
            break;
        case Qt::Key_G:
            max_get_floor=25;
            yellowkey+=999;
            bluekey+=999;
            redkey+=999;
            greenkey+=999;
            gold+=100000;
            break;
        default:
            if(e->key()>=Qt::Key_0&&e->key()<=Qt::Key_9)
            {
                switch(e->modifiers())
                {
                case Qt::NoModifier:
                    {
                        int save_num=e->key()-Qt::Key_0;
                        Save("Save_"+QString::number(save_num)+".sav");
                        save_flag[(save_num+9)%10].show();
                        break;
                    }
                    break;
                case Qt::ControlModifier:
                    {
                        int save_num=e->key()-Qt::Key_0;
                        Load("Save_"+QString::number(save_num)+".sav");
                        break;
                    }
                    break;
                case Qt::AltModifier:
                    {
                        int save_num=e->key()-Qt::Key_0;
                        Show("Save_"+QString::number(save_num)+".sav");
                        break;
                    }
                    break;
                default:
                    break;
                }
            }
            break;
        }
        if(end_event)
        {
            (*curr_floor)(x,y)=events[0];
            end_event=false;
        }
        refresh();
    }
    else
    {
        switch(e->key())
        {
        case Qt::Key_Enter:
        case Qt::Key_Return:
            end_show=true;
            break;
        }
    }
}

void MainWindow::refresh()
{
    setWarriorData();
    if(Guard[11]==2)
    {
        events[961]->Work();
    }
    for(int i=0;i<MAX_X;i++)
    {
        for(int j=0;j<MAX_Y;j++)
        {
            //刷新图像
            pics[i][j].setStyleSheet(QString("background-image: url(:/images/Null.png);border-image: url(:/images/")+(*curr_floor)(i,j)->getpic()+".png);");
            //刷新显伤
            int hurt=(*curr_floor)(i,j)->get_hurt();
            if(hurt>=0)
            {
                if(hurt>9999999)
                    hurts[i][j].setText("?????");
                else
                    hurts[i][j].setText(QString::number(hurt));
            }
            else
                hurts[i][j].clear();
            //刷新临界点
            int crip=(*curr_floor)(i,j)->get_crip();
            if(crip>=0)
                crips[i][j].setText(QString::number(crip));
            else
                crips[i][j].clear();
        }
    }
    warrior_img->move(min_x+x*48,min_y+y*48);
    ui->Send_Scope->setText("（1~"+QString::number(max_get_floor)+"）");
    switch(Warrior_dir)
    {
    case 'U':
        warrior_img->setStyleSheet("border-image: url(:/images/Warrior_Up.png);");
        break;
    case 'D':
        warrior_img->setStyleSheet("border-image: url(:/images/Warrior.png);");
        break;
    case 'L':
        warrior_img->setStyleSheet("border-image: url(:/images/Warrior_left.png);");
        break;
    case 'R':
        warrior_img->setStyleSheet("border-image: url(:/images/Warrior_right.png);");
        break;
    default:
        warrior_img->setStyleSheet("border-image: url(:/images/Warrior.png);");
        break;
    }
    if(floors[24](5,5)==events[0])
        Win();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::end_shop()
{
    refresh();
}

void MainWindow::on_Shop_clicked()
{
    shop->Reset();
    shop->show();
}

void MainWindow::on_FastSave_clicked()
{
    Save("Save.sav");
}

void MainWindow::on_FastLoad_clicked()
{
    Load("Save.sav");
}

void MainWindow::Save(QString filename)
{
    std::string mid=filename.toStdString();
    const char* cfilename=mid.data();
    refresh();
    std::ofstream fout(cfilename,std::ios::binary);
    int Warrior_Data[16]={0};
    //存储勇士位置
    Warrior_Data[0]=floor_id;
    Warrior_Data[1]=x;
    Warrior_Data[2]=y;
    Warrior_Data[3]=max_get_floor;
    //存储勇士属性
    Warrior_Data[4]=level;
    Warrior_Data[5]=atk;
    Warrior_Data[6]=def;
    Warrior_Data[7]=hp;
    Warrior_Data[8]=gold;
    Warrior_Data[9]=Exp;
    Warrior_Data[10]=yellowkey;
    Warrior_Data[11]=bluekey;
    Warrior_Data[12]=redkey;
    Warrior_Data[13]=greenkey;
    Warrior_Data[14]=upgrade_exp;
    Warrior_Data[15]=(16+max_get_floor*MAX_X*MAX_Y)*sizeof(int);   //存储路径信息的偏移量
    fout.write((char*)Warrior_Data,sizeof(Warrior_Data));
    //存储勇士到过的楼层的楼层数据
    for(int i=0;i<max_get_floor;i++)
    {
        for(int j=0;j<MAX_X;j++)
        {
            for(int k=0;k<MAX_Y;k++)
            {
                int temp=floors[i](j,k)->get_id();
                fout.write((char*)(&temp),sizeof(int));
            }
        }
    }
    //存储勇士的路径信息
    fout.write((char*)(&rlf),sizeof(int));
    fout.write(road_log,rlf);
    //存储十字架信息
    fout.write((char*)(&cross),sizeof(bool));
    //存储机关门信息
    for(int i=0;i<7;i++)
    {
        fout.write((char*)(&Guard[i]),sizeof(int));
    }
    //存储圣水信息
    fout.write((char*)(&Guard[11]),sizeof(int));
    fout.close();
}

void MainWindow::Load(QString filename)
{
    std::string mid=filename.toStdString();
    const char* cfilename=mid.data();
    //恢复初始状态
    setFloorData();
    std::ifstream fin(cfilename,std::ios::binary);
    if(!fin)
        return;
    int Warrior_Data[16]={0};
    fin.read((char*)Warrior_Data,sizeof(Warrior_Data));
    //读取勇士位置
    floor_id=Warrior_Data[0];
    x=Warrior_Data[1];
    y=Warrior_Data[2];
    max_get_floor=Warrior_Data[3];
    curr_floor=&floors[0];
    curr_floor+=(floor_id-1);
    //读取勇士属性
    level=Warrior_Data[4];
    atk=Warrior_Data[5];
    def=Warrior_Data[6];
    hp=Warrior_Data[7];
    gold=Warrior_Data[8];
    Exp=Warrior_Data[9];
    yellowkey=Warrior_Data[10];
    bluekey=Warrior_Data[11];
    redkey=Warrior_Data[12];
    greenkey=Warrior_Data[13];
    upgrade_exp=Warrior_Data[14];
    int road_log_delta=Warrior_Data[15];
    //读取勇士到过的楼层的楼层数据
    for(int i=0;i<max_get_floor;i++)
    {
        for(int j=0;j<MAX_X;j++)
        {
            for(int k=0;k<MAX_Y;k++)
            {
                int temp=0;
                fin.read((char*)(&temp),sizeof(int));
                floors[i](j,k)=events[temp];
            }
        }
    }
    //读取路径记录信息
    if(road_log_delta!=0)    //路径记录偏移为0表示没有存储路径信息
    {
        memset(road_log,0,sizeof(road_log));
        fin.read((char*)(&rlf),sizeof(int));
        fin.read(road_log,rlf);
    }
    //读取十字架信息
    fin.read((char*)(&cross),sizeof(bool));
    //读取机关门信息
    for(int i=0;i<7;i++)
    {
        fin.read((char*)(&Guard[i]),sizeof(int));
    }
    //读取圣水信息
    fin.read((char*)(&Guard[11]),sizeof(int));
    fin.close();
    refresh();
}

void MainWindow::Restart()
{
    refresh();
    //初始化勇士数据
    floor_id=1;
    level=1;
    hp=2000;
    atk=10;
    def=10;
    gold=0;
    Exp=0;
    yellowkey=0;
    bluekey=0;
    redkey=0;
    greenkey=0;
    upgrade_exp=16;
    max_get_floor=1;
    memset(Guard,0,sizeof(Guard));
    cross=false;
    setWarriorData();
    x=5;
    y=10;
    gameover=false;
    win=false;
    showing=false;
    end_show=false;
    label_gameover->hide();
    label_win->hide();
    label_score[0].hide();
    label_score[1].hide();
    setFloorData();
    refresh();
}
void MainWindow::on_ReStart_clicked()
{
    Restart();
}

void MainWindow::Send(int send_floor)
{
    //获取对应楼层下楼楼梯位置
    if(send_floor==1)    //1层没有下楼楼梯
    {
        x=5;
        y=10;
    }
    else
    {
        bool flag=false;
        for(int i=0;i<MAX_X;i++)
        {
            for(int j=0;j<MAX_Y;j++)
            {
                if(floor0[send_floor-1][i][j]==4)
                {
                    x=j;
                    y=i;
                    flag=true;
                    break;
                }
            }
            if(flag)
                break;
        }
    }
    road_log[rlf++]=send_floor;
    floor_id=send_floor;
    curr_floor=&floors[0];
    curr_floor+=(floor_id-1);
    refresh();
}
void MainWindow::on_Send_clicked()
{
    int send_floor=ui->Send_Floor->value();
    if(send_floor>max_get_floor)
        return;
    Send(send_floor);
}

void MainWindow::Show(QString filename)
{
    std::string mid=filename.toStdString();
    const char* cfilename=mid.data();
    std::ifstream fin(cfilename,std::ios::binary);
    if(!fin)
        return;
    //保存现有状态
    Save("Save_temp.sav");
    //调整为播放状态
    showing=true;
    label_showing->show();
    for(int i=0;i<10;i++)
    {
        save_flag[i].hide();
    }
    //恢复初始状态
    Restart();
    setFloorData();
    int Warrior_Data[16]={0};
    fin.read((char*)Warrior_Data,sizeof(Warrior_Data));
    //获取路径记录偏移
    int road_log_delta=Warrior_Data[15];
    if(road_log_delta==0)
        return;
    fin.seekg(road_log_delta,std::ios::beg);
    //获取路径记录长度
    int road_log_length=0;
    fin.read((char*)(&road_log_length),sizeof(int));
    //为读取到的路径开辟数组并存入数组
    char* road_log_read=new char[road_log_length+4];
    memset(road_log_read,0,road_log_length+4);
    fin.read(road_log_read,road_log_length);
    //关闭文件
    fin.close();
    //根据读取到的路径进行操作
    int place=0;
    bool unwait=false;
    char type_temp,next_temp=0;
    int add_temp,delta_temp=0;
    while(place<road_log_length)
    {
        bool shop_flag=false;
        switch(road_log_read[place])
        {
        case 'U':
            if(y==0)
                break;
            (*curr_floor)(x,y-1)->Work();
            if(!(*curr_floor)(x,y-1)->getPenetrable()&&end_event)
                (*curr_floor)(x,y-1)=events[0];
            if((*curr_floor)(x,y-1)->getPenetrable())
                y-=1;
            else
                unwait=true;
            Warrior_dir='U';
            break;
        case 'D':
            if(y==MAX_Y-1)
                break;
            (*curr_floor)(x,y+1)->Work();
            if(!(*curr_floor)(x,y+1)->getPenetrable()&&end_event)
                (*curr_floor)(x,y+1)=events[0];
            if((*curr_floor)(x,y+1)->getPenetrable())
                y+=1;
            else
                unwait=true;
            Warrior_dir='D';
            break;
        case 'L':
            if(x==0)
                break;
            (*curr_floor)(x-1,y)->Work();
            if(!(*curr_floor)(x-1,y)->getPenetrable()&&end_event)
                (*curr_floor)(x-1,y)=events[0];
            if((*curr_floor)(x-1,y)->getPenetrable())
                x-=1;
            else
                unwait=true;
            Warrior_dir='L';
            break;
        case 'R':
            if(x==MAX_X-1)
                break;
            (*curr_floor)(x+1,y)->Work();
            if(!(*curr_floor)(x+1,y)->getPenetrable()&&end_event)
                (*curr_floor)(x+1,y)=events[0];
            if((*curr_floor)(x+1,y)->getPenetrable())
                x+=1;
            else
                unwait=true;
            Warrior_dir='R';
            break;
        case 'S':
            delta_temp=0;
            while(1)
            {
                //读取后续字节
                memcpy(&type_temp,road_log_read+(place+delta_temp+1),1);
                memcpy(&add_temp,road_log_read+(place+delta_temp+2),4);
                switch(type_temp)
                {
                case 1:
                    delta_atk+=add_temp;
                    delta_gold+=-10*add_temp;
                    break;
                case 2:
                    delta_def+=add_temp;
                    delta_gold+=-10*add_temp;
                    break;
                case 3:
                    delta_hp+=add_temp;
                    delta_gold+=-(add_temp+upgrade_exp-1)/upgrade_exp;
                    break;
                case 4:
                    delta_yellowkey+=add_temp;
                    delta_gold+=-3*add_temp;
                    break;
                case 5:
                    delta_bluekey+=add_temp;
                    delta_gold+=-10*add_temp;
                    break;
                case 6:
                    delta_redkey+=add_temp;
                    delta_gold+=-40*add_temp;
                    break;
                case 7:
                    delta_greenkey+=add_temp;
                    delta_gold+=-200*add_temp;
                    break;
                case 'E':
                    shop_flag=true;
                    break;
                default:
                    shop_flag=true;
                    break;
                }
                delta_temp+=5;
                if(shop_flag)
                {
                    memcpy(&next_temp,road_log_read+(place+delta_temp-3),1);
                    if(next_temp=='S')     //连续进行多次购买在一个周期内刷新
                    {
                        delta_temp-=3;
                        shop_flag=false;
                    }
                    else
                    {
                        delta_temp-=4;
                        place+=delta_temp;
                        break;
                    }
                }
            }
            break;
        default:    //根据实际读到的数据判断
            if(road_log_read[place]<=25)   //表示楼层传送器
            {
                int place_delta=1;
                while(1)      //连续使用楼层传送器只保留最后一次传送结果，避免大量刷新
                {
                    if(road_log_read[place+place_delta]>=1&&road_log_read[place+place_delta]<=25)
                        place_delta++;
                    else
                    {
                        place+=place_delta-1;
                        Send(road_log_read[place]);
                        break;
                    }
                }

            }
            break;
        }
        place++;
        if(end_event)
        {
            (*curr_floor)(x,y)=events[0];
            end_event=false;
        }
        refresh();
        if(!unwait)
        {
            QApplication::processEvents();
            Sleep(10);
        }
        else
            unwait=false;
        if(end_show)
        {
            Load("Save_temp.sav");
            showing=false;
            end_show=false;
            for(int i=0;i<10;i++)
            {
                //尝试打开存档文件
                char filename[11]="Save_i.sav";
                filename[5]='0'+(i+1)%10;
                std::ifstream fin(filename,std::ios::binary);
                save_flag[i].setStyleSheet("color: rgb(255, 255, 255);font: 14pt \"Calibri\";");
                if(!fin)
                    save_flag[i].hide();
                else
                    save_flag[i].show();
            }
            label_showing->hide();
            return;
        }
    }
}

void MainWindow::on_FastShow_clicked()
{
    Show("Save.sav");
}

void MainWindow::Show_NoRefresh(QString filename)
{
    Restart();
    std::string mid=filename.toStdString();
    const char* cfilename=mid.data();
    //恢复初始状态
    setFloorData();
    std::ifstream fin(cfilename,std::ios::binary);
    if(!fin)
        return;
    int Warrior_Data[16]={0};
    fin.read((char*)Warrior_Data,sizeof(Warrior_Data));
    //获取路径记录偏移
    int road_log_delta=Warrior_Data[15];
    if(road_log_delta==0)
        return;
    fin.seekg(road_log_delta,std::ios::beg);
    //获取路径记录长度
    int road_log_length=0;
    fin.read((char*)(&road_log_length),sizeof(int));
    //为读取到的路径开辟数组并存入数组
    char* road_log_read=new char[road_log_length+4];
    memset(road_log_read,0,road_log_length+4);
    fin.read(road_log_read,road_log_length);
    //关闭文件
    fin.close();
    //根据读取到的路径进行操作
    int place=0;
    bool unwait=false;
    char type_temp;
    int add_temp,delta_temp=0;
    while(place<road_log_length)
    {
        bool shop_flag=false;
        switch(road_log_read[place])
        {
        case 'U':
            if(y==0)
                break;
            (*curr_floor)(x,y-1)->Work();
            if(!(*curr_floor)(x,y-1)->getPenetrable()&&end_event)
                (*curr_floor)(x,y-1)=events[0];
            if((*curr_floor)(x,y-1)->getPenetrable())
                y-=1;
            else
                unwait=true;
            break;
        case 'D':
            if(y==MAX_Y-1)
                break;
            (*curr_floor)(x,y+1)->Work();
            if(!(*curr_floor)(x,y+1)->getPenetrable()&&end_event)
                (*curr_floor)(x,y+1)=events[0];
            if((*curr_floor)(x,y+1)->getPenetrable())
                y+=1;
            else
                unwait=true;
            break;
        case 'L':
            if(x==0)
                break;
            (*curr_floor)(x-1,y)->Work();
            if(!(*curr_floor)(x-1,y)->getPenetrable()&&end_event)
                (*curr_floor)(x-1,y)=events[0];
            if((*curr_floor)(x-1,y)->getPenetrable())
                x-=1;
            else
                unwait=true;
            break;
        case 'R':
            if(x==MAX_X-1)
                break;
            (*curr_floor)(x+1,y)->Work();
            if(!(*curr_floor)(x+1,y)->getPenetrable()&&end_event)
                (*curr_floor)(x+1,y)=events[0];
            if((*curr_floor)(x+1,y)->getPenetrable())
                x+=1;
            else
                unwait=true;
            break;
        case 'S':
            while(1)
            {
                //读取后续字节
                memcpy(&type_temp,road_log_read+(place+delta_temp+1),1);
                memcpy(&add_temp,road_log_read+(place+delta_temp+2),4);
                switch(type_temp)
                {
                case 1:
                    delta_atk=add_temp;
                    delta_gold+=-10*add_temp;
                    break;
                case 2:
                    delta_def=add_temp;
                    delta_gold+=-10*add_temp;
                    break;
                case 3:
                    delta_hp=add_temp;
                    delta_gold+=-(add_temp+upgrade_exp-1)/upgrade_exp;
                    break;
                case 4:
                    delta_yellowkey=add_temp;
                    delta_gold+=-15*add_temp;
                    break;
                case 5:
                    delta_bluekey=add_temp;
                    delta_gold+=-50*add_temp;
                    break;
                case 6:
                    delta_redkey=add_temp;
                    delta_gold+=-200*add_temp;
                    break;
                case 7:
                    delta_greenkey=add_temp;
                    delta_gold+=-1000*add_temp;
                    break;
                case 'E':
                    shop_flag=true;
                    break;
                default:
                    shop_flag=true;
                    break;
                }
                delta_temp+=5;
                if(shop_flag)
                {
                    delta_temp-=4;
                    place+=delta_temp;
                    break;
                }
            }
            break;
        default:    //根据实际读到的数据判断
            if(road_log_read[place]<=25)   //表示楼层传送器
                Send(road_log_read[place]);
            break;
        }
        place++;
        if(end_event)
        {
            (*curr_floor)(x,y)=events[0];
            end_event=false;
        }
        //只更新数据不更新画面
        floor_id+=delta_floor;
        atk+=delta_atk;
        def+=delta_def;
        hp+=delta_hp;
        gold+=delta_gold;
        Exp+=delta_exp;
        yellowkey+=delta_yellowkey;
        bluekey+=delta_bluekey;
        redkey+=delta_redkey;
        greenkey+=delta_greenkey;
        if(Exp>=upgrade_exp)
        {
            upgrade_exp*=1.5;
            upgrade_exp+=16;
            level++;
        }
        delta_floor=0;
        delta_atk=0;
        delta_def=0;
        delta_hp=0;
        delta_gold=0;
        delta_exp=0;
        delta_yellowkey=0;
        delta_bluekey=0;
        delta_redkey=0;
        delta_greenkey=0;
        if(hp<=0)    //如果死亡则停止播放
        {
            ui->HP->setText("Game Over");
            refresh();
            return;
        }
    }
    refresh();
}

void MainWindow::on_FastShow_NoRefresh_clicked()
{
    Show_NoRefresh("save.sav");
}

void MainWindow::on_Exit_clicked()
{
    close();
}

void MainWindow::show_information(int x,int y)
{
    ui->information_pic->show();
    //清空信息栏
    for(int j=0;j<8;j++)
        information[j].clear();
    QStringList temp=(*curr_floor)(x,y)->Information();
    //显示信息
    ui->information_pic->setStyleSheet(QString("border-image: url(:/images/Blackgold.jpg);image: url(:/images/")+(*curr_floor)(x,y)->getpic()+".png);");
    ui->information_title->setText((*curr_floor)(x,y)->getName());
    int i=0;
    while(temp.length())
    {
        information[i].setText(temp.front());
        i++;
        temp.pop_front();
    }
}

void MainWindow::GameOver()
{
    gameover=true;
    label_gameover->show();
    if(yellowkey>500)
        label_score[0].setText("检测到使用金手指，您的分数为0");
    else
        label_score[0].setText("您的分数为："+QString::number(int(max_get_floor-1)));
    label_score[0].show();
}

void MainWindow::Win()
{
    if(!gameover)
    {
        win=true;
        label_win->show();
    }
    else
        label_gameover->show();
    //计算分数
    int scores[101]={
        1,2,3,4,5,6,7,8,9,10,
        11,12,13,14,15,16,17,18,19,20,
        21,22,23,24,25,30000,36000,38000,39000,40000,
        40670,40970,41270,41570,41870,42170,42370,42570,42770,42970,
        43170,43320,43470,43620,43770,43920,44070,44220,44370,44520,
        44670,44770,44870,44970,45070,45170,45270,45370,45470,45570,
        45670,45760,45830,45880,45930,45970,46000,46020,46040,46060,
        46070,46078,46086,46094,46102,46109,46116,46122,46127,46131,
        46135,46138,46141,46144,46147,46150,46152,46154,46156,46158,
        46160,46161,46162,46163,46164,46165,46166,46167,46168,46169,
        46170
    };
    int temp_score=atk*10+def*10+gold;
    int hundred_score=0;
    for(int i=100;i>=0;i--)
    {
        if(temp_score>=scores[i])
        {
            hundred_score=i;
            break;
        }
    }
    double score=temp_score/10.0;
    if(temp_score<46070)
        score=int(score);
    if(yellowkey>500)
        label_score[0].setText("检测到使用金手指，您的分数为0");
    else
    {
        label_score[0].setText("能力分数为："+QString::number(score));
        label_score[1].setText("百分制分数为："+QString::number(hundred_score));
    }
    label_score[0].show();
    label_score[1].show();
}

PButton::PButton()
{
    x=0;
    y=0;
    connect(this,SIGNAL(clicked(bool)),this,SLOT(slot_show()));
}

void PButton::setxy(int x,int y)
{
    this->x=x;
    this->y=y;
}

void PButton::slot_show()
{
    emit show_information(x,y);
}
