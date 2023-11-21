#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QLabel>
#include <QPushButton>
#include "Event.h"
#include "shop.h"
#include <fstream>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class PButton : public QPushButton
{
    Q_OBJECT

public:
    void setxy(int x,int y);
    PButton();
private:
    int x,y;
public slots:
    void slot_show();
signals:
    void show_information(int x,int y);
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void keyPressEvent(QKeyEvent *e);
    void setWarriorData();
    void refresh();
    void Save(QString filename);
    void Load(QString filename);
    void Show(QString filename);
    void Show_NoRefresh(QString filename);
    void Restart();
    void Send(int floor);
    void GameOver();
    void Win();
private slots:
    void on_Shop_clicked();

    void on_FastSave_clicked();

    void on_FastLoad_clicked();

    void on_ReStart_clicked();

    void on_Send_clicked();

    void on_FastShow_clicked();

    void on_FastShow_NoRefresh_clicked();

    void on_Exit_clicked();

public slots:
    void end_shop();
    void show_information(int x,int y);

private:
    Ui::MainWindow *ui;
    int x,y;    //勇士的x,y坐标
    int min_x,min_y;
    bool showing;
    bool end_show;
    bool gameover;
    bool win;
    PButton** pics;
    QLabel** hurts;
    QLabel** crips;
    QPushButton* warrior_img;
    QLabel* save_flag;
    QLabel* information;
    Shop* shop;
    QLabel* label_showing;
    QLabel* label_gameover;
    QLabel* label_win;
    QLabel* label_score;
};

#endif // MAINWINDOW_H
