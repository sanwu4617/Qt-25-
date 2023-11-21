#ifndef EVENT_H
#define EVENT_H
#include <QString>
#include <QObject>
#include <QThread>
#include <windows.h>
#include <QStringList>
class Event:public QObject{
    Q_OBJECT

public:
    void setName(QString name);
    QString getName();
    QString getpic();
    Event();
    virtual void Work()=0;
    virtual QStringList Information();
    virtual int get_hurt();
    virtual int get_gold();
    virtual int get_exp();
    virtual int get_crip();   //获取临界点
    bool getPenetrable();
    void set_id(int id);
    int get_id();
protected:
    QString name;
    QString pic;
    char type;
    bool penetrable;    //可穿透性
    int event_id;
};

class Wall:public Event{
public:
    Wall();
    virtual void Work();
};

class Null:public Event{
public:
    Null();
    virtual void Work();
};

class Warrior:public Event{
public:
    Warrior();
    virtual void Work();
};

class UpStairs:public Event{
public:
    UpStairs();
    virtual void Work();
    virtual QStringList Information();
};

class DownStairs:public Event{
public:
    DownStairs();
    virtual void Work();
    virtual QStringList Information();
};
class Monster:public Event{
public:
    Monster(QString name,QString pic,int atk,int def,int hp,int gold,int exp,int attribute=0);
    Monster(Monster* other);
    virtual void Work();
    virtual int get_hurt();
    virtual int get_gold();
    virtual int get_exp();
    virtual int get_crip();
    virtual QStringList Information();
private:
    int m_atk;
    int m_def;
    int m_hp;
    int m_gold;
    int m_exp;
    int m_attribute;
};
class Monster_Guard:public Monster{
public:
    Monster_Guard(Monster* monster,int guard);
    virtual void Work();
    virtual QStringList Information();
private:
    int guard;
};

class Treasure:public Event{
public:
    Treasure(QString name,QString pic,int work_id,int delta);   //work_id：1加血，2加攻击，3加防御，4~7加钥匙
    virtual void Work();
    virtual QStringList Information();
private:
    int work_id;
    int delta;
};

class Holy_Water:public Event{
public:
    Holy_Water(int guard_id,int show_num);
    virtual void Work();
    virtual QStringList Information();
private:
    int guard_id;
    int show_num;
};

class Door:public Event{
public:
    Door(QString name,QString pic,int door_id);
    virtual void Work();
    virtual QStringList Information();
private:
    int door_id;
};

class Guard_Door:public Event{
public:
    Guard_Door(int guard_id,int open_num);
    virtual void Work();
    virtual QStringList Information();
private:
    int guard_id;
    int open_num;
};

class Floor{
public:
    Floor();
    Event*& operator() (int x,int y);
    void setData(int** data);    //记录事件编号
private:
    Event*** e;
};
#define MAX_EVENT 1000
#define MAX_FLOOR 25
void setEventData();
void setFloorData();
extern int MAX_X;
extern int MAX_Y;
extern Event** events;
extern int delta_atk;
extern int delta_def;
extern int delta_hp;
extern int delta_gold;
extern int delta_exp;
extern int delta_yellowkey;
extern int delta_bluekey;
extern int delta_redkey;
extern int delta_greenkey;
extern int floor_id;
extern int level;
extern int hp;
extern int atk;
extern int def;
extern int gold;
extern int Exp;
extern int yellowkey;
extern int bluekey;
extern int redkey;
extern int greenkey;
extern bool end_event;
extern int delta_floor;
extern int upgrade_exp;
extern Floor* floors;
extern Floor* curr_floor;
extern int max_get_floor;
extern const int floor0[MAX_FLOOR][11][11];
extern int Guard[MAX_EVENT];
extern int stat_all_atk;
extern int stat_all_def;
extern int stat_all_hp;
extern int stat_all_gold;
extern int stat_all_exp;
extern int stat_all_monster;
extern int stat_all_event[MAX_EVENT];
extern bool cross;
extern char road_log[1048576];
extern int rlf;    //road_log的下标
extern bool all_str;    //显示全部攻防
extern char Warrior_dir;
#endif // EVENT_H
