#include <QGridLayout>

#include <QToolBar>

#include <QMainWindow>

#include <QLabel>

#include <QFile>

#include <QDir>

#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow (parent)
{
      QPixmap quitpix("quit.png");
      QPixmap pausepix("pause2.png");
      QPixmap continuepix("continue.png");
      QPixmap replaypix("restart.png");
      QPixmap helppix("help.png");

      frame = new QFrame(this);
      game = new Game(this);

      app_size = game->window_size+50;
      frame->resize(app_size, app_size);

      QToolBar *toolbar = addToolBar("main toolbar");

       act_pause = toolbar->addAction(pausepix,"Pause");
       connect(act_pause, &QAction::triggered, game, &Game::set_pause);

       act_continue = toolbar->addAction(continuepix,"Continue");
       act_continue->setDisabled(true);
       connect(act_continue, &QAction::triggered, game, &Game::set_continue);

       act_replay = toolbar->addAction(replaypix,"Replay");
       connect(act_replay, &QAction::triggered, this, &MainWindow::proc_replay);

       act_show_rec = toolbar->addAction("Show record");
       act_show_rec->setDisabled(true);
       connect(act_show_rec, &QAction::triggered, this, &MainWindow::proc_show_record);


       act_file_show_rec = toolbar->addAction("Show best game");
       act_file_show_rec->setDisabled(true);
       connect(act_file_show_rec, &QAction::triggered, this, &MainWindow::proc_file_show_record);



       toolbar->addSeparator();

       QAction *quit = toolbar->addAction(quitpix,"&Quit");
       connect(quit, &QAction::triggered, qApp, &QApplication::quit);

       help_text = new QLabel(this);
       //help_text->setContentsMargins(50,60,100,100);

       //help_text->setText("ccsdvsrbbrssdvgyewfhwjjjjjjjjjjjebwjehfgvehgbjhrebgjerbgjhbhvhgwvfhwevfhwjhfwfwvfwvfhwwwwwvefwebfhjwbfjhqwbfhbqjhwbfjhwbjfwrfkjhewfhkjwhfkqsv");




       help_text->setFrameShape(QFrame::Box);
       help_text->setMaximumHeight(game->window_size);
       help_text->setMaximumWidth(game->window_size);
       //help_text->resize(game->window_size,game->window_size);
       help_text->hide();




        progress = new QProgressBar(frame);
        speed_progress = 0;

        progress->setTextVisible(false);

        speed_text_label = new QLabel(frame);
        speed_text_label->setText("Speed progress: ");

        score_text_label = new QLabel(frame);
        score_text_label->setText("Score: ");

        score_label = new QLabel(frame);
        score_label->setNum(0);
        score_label->setFont(QFont("Segoe UI",25));


        QString pth = "progress.txt";

        QFile file(pth);
        if (!file.open(QIODevice::ReadWrite)) {
           qWarning("Cannot open progress-file for reading");
         }
         char c;
         file.getChar(&c);
         highscore = c-48;
         file.close();

        highscore_text_label = new QLabel(frame);
        highscore_text_label->setText("Highscore: ");

        highscore_label = new QLabel(frame);
        highscore_label->setNum(highscore);
        highscore_label->setFont(QFont("Segoe UI",25));




//        score_text_label->setLineWidth(1);
//        score_text_label->setFrameShape(QFrame::Box);     //https://russianblogs.com/article/3516502991/
//        score_text_label->setFrameShadow(QFrame::Sunken);

      grid = new QGridLayout(frame);
      grid->setColumnMinimumWidth(0,game->window_size/6);
      grid->setColumnMinimumWidth(1,game->window_size/6);
      grid->setColumnMinimumWidth(2,game->window_size/6);
      grid->setColumnMinimumWidth(3,game->window_size/6);
      grid->setColumnMinimumWidth(4,game->window_size/6);
      grid->setColumnMinimumWidth(5,game->window_size/6);

      grid->setRowMinimumHeight(0,game->window_size/6);
      grid->setRowMinimumHeight(1,game->window_size/6);
      grid->setRowMinimumHeight(2,game->window_size/6);
      grid->setRowMinimumHeight(3,game->window_size/6);
      grid->setRowMinimumHeight(4,game->window_size/6);
      grid->setRowMinimumHeight(5,game->window_size/6);
      grid->setRowMinimumHeight(6,game->window_size/12);

      //grid->setRowMinimumHeight(7,game->window_size/12);

      grid->addWidget(game,0,0,6,6);
      grid->addWidget(speed_text_label,6,0,1,1,Qt::AlignJustify);
      grid->addWidget(progress,6,1,1,1,Qt::AlignJustify);
      grid->addWidget(score_text_label,6,2,1,1,Qt::AlignJustify);
      grid->addWidget(score_label,6,3,1,1,Qt::AlignJustify);
      grid->addWidget(highscore_text_label,6,4,1,1,Qt::AlignJustify);
      grid->addWidget(highscore_label,6,5,1,1,Qt::AlignJustify);


      frame->setLayout(grid);
      setCentralWidget(frame);



      menu = menuBar()->addMenu("&Menu");
      act_help = new QAction(helppix,"&Help", this);
      connect(act_help, &QAction::triggered, this, &MainWindow::proc_help);

      menu->addAction(act_help);
      act_design = new QAction("&Design", this);
      act_design->setCheckable(true);
      act_design->setChecked(true);
      connect(act_design, &QAction::triggered, this, &MainWindow::proc_design);
      menu->addAction(act_design);

      act_statistic = new QAction("&Statistic");
      menu->addAction(act_statistic);

      menu->addAction(quit);



      change_counter = true;
}

void MainWindow::proc_replay(){
    game->killTimer(game->timer);
    delete game;
    progress->reset();
    score_label->setNum(0);
    game = new Game(this);


    act_pause->setEnabled(true);
    act_continue->setDisabled(true);
    act_show_rec->setDisabled(true);
    act_file_show_rec->setDisabled(true);
    act_design->setChecked(true);

    grid->addWidget(game, 0, 0, 6, 6);

    connect(act_pause, &QAction::triggered, game, &Game::set_pause);
    connect(act_continue, &QAction::triggered, game, &Game::set_continue);
}

void MainWindow::proc_show_record(){
    //становится доступной для нажатия в game.cpp при проигрыше
    act_show_rec->setDisabled(true);
    game->is_record = true;
    progress->reset();
    score_label->setNum(0);
    game->showRecord();
}

void MainWindow::proc_file_show_record(){
    //становится доступной для нажатия в game.cpp при проигрыше
    act_show_rec->setDisabled(true);
    game->is_record = false;
    game->is_file_record = true;
    progress->reset();
    score_label->setNum(0);
    game->showRecord();
}


void MainWindow::proc_help(){
    if(change_counter){
        game->hide();
        help_text->show();

        grid->addWidget(help_text, 0, 0, 6, 6);
    } else{
        game->show();
        help_text->hide();
        grid->addWidget(game, 0, 0, 6, 6);
    }
    change_counter = !change_counter;

}

void MainWindow::proc_design(){
    if(act_design->isChecked()){
        game->is_design = true;
    }else{
        game->is_design = false;
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    game->keyPressEvent(event);
}



MainWindow::~MainWindow()
{  
}

