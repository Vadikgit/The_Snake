#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

#include <QSlider>

#include <QFrame>

#include <QLabel>

#include <QGridLayout>

#include <QProgressBar>

#include <QTextEdit>

#include <QMenu>

#include <QMenuBar>

#include "game.h"

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void keyPressEvent(QKeyEvent * event);

    int speed_progress;
//    int app_height;
//    int app_weight;
    int app_size;
    int highscore;

    QMenu *menu;




    QLabel *speed_text_label;
    QLabel *score_label;
    QLabel *score_text_label;
    QLabel *highscore_label;
    QLabel *highscore_text_label;
    QLabel* help_text;

    QProgressBar *progress;
    Game *game;

    //действия в меню
    QAction *act_help;
    QAction *act_design;
    QAction *act_statistic;

    //действия в инструментах
    QAction *act_pause;
    QAction *act_continue;
    QAction *act_replay;
    QAction *act_show_rec;
    QAction *act_file_show_rec;
    QFrame *frame;
    QGridLayout *grid;


    bool change_counter;

    void proc_replay();
    void proc_show_record();
    void proc_file_show_record();
    void proc_help();
    void proc_design();
private:

};
#endif // MAINWINDOW_H


