#include "Event.h"
int floor_id=1;
int level;
int hp;
int atk;
int def;
int gold;
int Exp;
int yellowkey;
int bluekey;
int redkey;
int greenkey;
int MAX_X=11;
int MAX_Y=11;
int delta_atk=0;
int delta_def=0;
int delta_hp=0;
int delta_gold=0;
int delta_exp=0;
int delta_floor=0;
int delta_yellowkey=0;
int delta_bluekey=0;
int delta_redkey=0;
int delta_greenkey=0;
bool end_event=false;
Floor* floors;
Floor* curr_floor;
Event** events;
int upgrade_exp=16;
int max_get_floor=1;
int stat_all_atk=0;
int stat_all_def=0;
int stat_all_hp=0;
int stat_all_gold=0;
int stat_all_exp=0;
int stat_all_monster=0;
int stat_all_event[MAX_EVENT]={0};
int Guard[MAX_EVENT]={0};
bool cross=false;
char road_log[1048576]={0};
int rlf=0;
bool all_str=false;
char Warrior_dir='D';
int Event::get_hurt()
{
    return -1;
}
Event::Event()
{
    type=0;
    penetrable=false;
}
void Event::setName(QString name)
{
    this->name=name;
}
QString Event::getpic()
{
    return pic;
}
QString Event::getName()
{
    return name;
}
bool Event::getPenetrable()
{
    return penetrable;
}
void Event::set_id(int id)
{
    event_id=id;
}
int Event::get_id()
{
    return event_id;
}
int Event::get_gold()
{
    return 0;
}
int Event::get_exp()
{
    return 0;
}
int Event::get_crip()
{
    return -1;
}
QStringList Event::Information()
{
    QStringList temp;
    return temp;
}
Wall::Wall()
{
    name="墙";
    pic="Wall";
    type='W';
    penetrable=false;
}
void Wall::Work()
{

}
Null::Null()
{
    name="地面";
    pic="Null";
    type='N';
    penetrable=true;
}
void Null::Work()
{

}
Warrior::Warrior()
{
    name="勇士";
    pic="Warrior";
    type='W';
    penetrable=true;
}
void Warrior::Work()
{

}
UpStairs::UpStairs()
{
    name="UpStairs";
    pic="UpStairs";
    type='S';
    penetrable=true;
}
void UpStairs::Work()
{
    delta_floor=1;
    curr_floor+=1;
    if(floor_id==max_get_floor)    //楼层还没更新，因此取等条件下将最大楼层+1
        max_get_floor++;

}
QStringList UpStairs::Information()
{
    QStringList temp;
    temp.append(QString("上楼"));
    return temp;
}
DownStairs::DownStairs()
{
    name="DownStairs";
    pic="DownStairs";
    type='S';
    penetrable=true;
}
void DownStairs::Work()
{
    delta_floor=-1;
    curr_floor-=1;
}
QStringList DownStairs::Information()
{
    QStringList temp;
    temp.append(QString("下楼"));
    return temp;
}
Monster::Monster(QString name,QString pic,int atk,int def,int hp,int gold,int exp,int attribute)
{
    this->name=name;
    this->pic=pic;
    m_atk=atk;
    m_def=def;
    m_hp=hp;
    m_gold=gold;
    m_exp=exp;
    m_attribute=attribute;
    type='M';
    penetrable=true;
}
Monster::Monster(Monster* other)
{
    name=other->name;
    pic=other->pic;
    m_atk=other->m_atk;
    m_def=other->m_def;
    m_hp=other->m_hp;
    m_gold=other->m_gold;
    m_exp=other->m_exp;
    m_attribute=other->m_attribute;
    type='M';
    penetrable=true;
}
void Monster::Work()   //战斗
{
    delta_hp=-get_hurt();
    delta_gold=m_gold;
    delta_exp=m_exp;
    end_event=true;
    return;
}
int Monster::get_hurt()
{
    double str_times=1.0+level/20.0-0.05;
    if(cross)
        str_times+=0.05;
    int atk0=atk*str_times+1e-10;
    int def0=def*str_times+1e-10;
    if(m_attribute&64)   //魔攻
        def0=0;
    if(atk0<=m_def)
        return 2147483647;
    if(m_atk<=def0)
        return 0;
    if(m_attribute&256)  //坚固
        atk0=m_def+1;
    int times=(m_hp-1)/(atk0-m_def);
    if(m_attribute&128)  //连击
    {
        times*=(m_attribute&63);
    }
    return times*(m_atk-def0);
}
int Monster::get_gold()
{
    return m_gold;
}
int Monster::get_exp()
{
    return m_exp;
}
int Monster::get_crip()
{
    //计算攻击次数
    double str_times=1.0+level/20.0-0.05;
    if(cross)
        str_times+=0.05;
    int atk0=atk*str_times+1e-10;
    int def0=def*str_times+1e-10;
    if(m_attribute&64)   //魔攻
        def0=0;
    if(atk0<=m_def)      //显示破防所需攻击
        return int((m_def+1)/str_times-(1e-10))+1-atk;
    if(m_attribute&256)  //能破防时坚固怪物无临界点
        return 0;
    if(m_atk<=def0)      //防杀的怪物无临界点
        return 0;
    int times=(m_hp-1)/(atk0-m_def);
    if(times==0)         //攻杀的怪物无临界点
        return 0;
    int need_all_atk=(m_hp-1)/times+1+m_def;
    int min_need=int(need_all_atk/str_times-atk-(1e-10))+1;
    if(min_need==0)      //可减伤的情况下，返回值最小为1，表示加1攻击即可减伤
        min_need=1;
    return min_need;
}
QStringList Monster::Information()
{
    QStringList temp;
    temp.append(QString("生命：")+QString::number(m_hp));
    temp.append(QString("攻击：")+QString::number(m_atk));
    temp.append(QString("防御：")+QString::number(m_def));
    temp.append(QString("金币：")+QString::number(m_gold));
    temp.append(QString("经验：")+QString::number(m_exp));
    int hurt=get_hurt();
    if(hurt>2147483646)
        temp.append(QString("伤害：无法破防"));
    else
        temp.append(QString("伤害：")+QString::number(get_hurt()));
    QString attribute="";
    if(m_attribute&64)
        attribute+="魔攻 ";
    if(m_attribute&128)
        attribute+=QString::number(m_attribute&63)+"连击 ";
    if(m_attribute&256)
        attribute+="坚固 ";
    if(m_attribute==0)
        attribute="无";
    temp.append(QString("属性：")+attribute);
    return temp;
}
Monster_Guard::Monster_Guard(Monster* monster,int guard):Monster(monster)
{
    this->guard=guard;
}
void Monster_Guard::Work()
{
    Monster::Work();
    Guard[guard]++;
}
QStringList Monster_Guard::Information()
{
    QStringList temp=Monster::Information();
    if(guard<=10)
        temp.append(QString("%0号守护者").arg(QString::number(guard+1)));
    else
        temp.append(QString("击杀2剑王得圣水"));
    return temp;
}
Treasure::Treasure(QString name,QString pic,int work_id,int delta)
{
    this->name=name;
    this->pic=pic;
    type='T';
    penetrable=false;
    this->work_id=work_id;
    this->delta=delta;
}
void Treasure::Work()
{
    switch(work_id)
    {
    case 1:
        delta_hp=delta;
        break;
    case 2:
        delta_atk=delta;
        break;
    case 3:
        delta_def=delta;
        break;
    case 4:
        delta_yellowkey=delta;
        break;
    case 5:
        delta_bluekey=delta;
        break;
    case 6:
        delta_redkey=delta;
        break;
    case 7:
        delta_greenkey=delta;
        break;
    case 8:
        delta_gold=delta;
        break;
    case 9:
        delta_exp=delta;
        break;
    case 10:
        cross=true;
        break;
    }
    end_event=true;
}
QStringList Treasure::Information()
{
    QStringList temp;
    switch(work_id)
    {
    case 1:
        temp.append(QString("增加%0生命").arg(delta));
        break;
    case 2:
        temp.append(QString("增加%0攻击").arg(delta));
        break;
    case 3:
        temp.append(QString("增加%0防御").arg(delta));
        break;
    case 8:
        temp.append(QString("增加%0金币").arg(delta));
        break;
    case 9:
        temp.append(QString("增加%0经验").arg(delta));
        break;
    case 10:
        temp.append(QString("攻防增加量+5%"));
        break;
    default:
        break;
    }
    return temp;
}
Holy_Water::Holy_Water(int guard_id,int show_num)
{
    this->name="地面";
    this->pic="Null";
    type='T';
    penetrable=true;
    this->guard_id=guard_id;
    this->show_num=show_num;
}
void Holy_Water::Work()
{
    if(Guard[guard_id]==show_num+1)
    {
        hp*=2;
        end_event=true;
    }
    if(Guard[guard_id]==show_num)
    {
        this->name="圣水";
        this->pic="Holy_Water";
        Guard[guard_id]++;
    }

}
QStringList Holy_Water::Information()
{
    QStringList temp;
    if(Guard[guard_id]==show_num+1)
    {
        temp.append(QString("使用后生命翻倍"));
        temp.append(QString(""));
        temp.append(QString("提示：若暂时不想吃"));
        temp.append(QString("圣水可传送至其他楼"));
        temp.append(QString("层绕开"));
    }

    return temp;
}
Door::Door(QString name,QString pic,int door_id)
{
    this->name=name;
    this->pic=pic;
    type='D';
    penetrable=false;
    this->door_id=door_id;
}
void Door::Work()
{
    switch(door_id)
    {
    case 1:
        if(yellowkey>=1)
        {
            end_event=true;
            yellowkey-=1;
        }
        break;
    case 2:
        if(bluekey>=1)
        {
            end_event=true;
            bluekey-=1;
        }
        break;
    case 3:
        if(redkey>=1)
        {
            end_event=true;
            redkey-=1;
        }
        break;
    case 4:
        if(greenkey>=1)
        {
            end_event=true;
            greenkey-=1;
        }
        break;
    }
}
QStringList Door::Information()
{
    QStringList temp;
    switch(door_id)
    {
    case 1:
        temp.append(QString("需用1把黄钥匙打开"));
        break;
    case 2:
        temp.append(QString("需用1把蓝钥匙打开"));
        break;
    case 3:
        temp.append(QString("需用1把红钥匙打开"));
        break;
    case 4:
        temp.append(QString("需用1把绿钥匙打开"));
        break;
    }
    return temp;
}
Guard_Door::Guard_Door(int guard_id,int open_num)
{
    this->name="机关门";
    this->pic="Guard_Door";
    type='D';
    penetrable=false;
    this->guard_id=guard_id;
    this->open_num=open_num;
}
void Guard_Door::Work()
{
    if(Guard[guard_id]>=open_num)
        end_event=true;
}
QStringList Guard_Door::Information()
{
    QStringList temp;
    temp.append(QString("%0号机关门").arg(QString::number(guard_id+1)));
    temp.append(QString("需击杀%0个%1号守护者").arg(QString::number(open_num),QString::number(guard_id+1)));
    return temp;
}
Floor::Floor()
{
    e=new Event** [MAX_X];
    for(int i=0;i<MAX_X;i++)
        e[i]=new Event* [MAX_Y];
    for(int i=0;i<MAX_X;i++)
    {
        for(int j=0;j<MAX_Y;j++)
        {
            e[i][j]=nullptr;
        }
    }
}
Event*& Floor::operator() (int x,int y)
{
    return e[x][y];
}
void Floor::setData(int **data)
{
    for(int i=0;i<MAX_X;i++)
    {
        for(int j=0;j<MAX_Y;j++)
        {
            e[i][j]=events[data[i][j]];
        }
    }
}
