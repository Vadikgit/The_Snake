#ifndef GAME_H
#define GAME_H

#include <QFrame>

#include <QWidget>

#include <QPainter>

#include <QTimer>

#include <QKeyEvent>

#include <random>

#include <QVector>

#include <QApplication>

#include <QFile>

#include <QDir>

#include <QMainWindow>

class MainWindow;

class Game: public QFrame {
public:
  Game(QWidget * parent = 0);
  enum direction {
    D_STOP = 0,
      D_FORWARD = 1,
      D_BACK = 2,
      D_RIGHT = 3,
      D_LEFT = 4
  };
  int window_size = 480;//должно делиться сначала на 6, а потом на m_n

  int m_n = 20;

  void paintEvent(QPaintEvent * event);
  void timerEvent(QTimerEvent * event);
  void keyPressEvent(QKeyEvent * event);

  void set_pause();
  void set_continue();
  int timer_delay;
  int min_timer_delay;
  int max_timer_delay;
  int timer;

  bool is_pause;
  bool tail_side;

  int tail_side_counter;

  direction dir;
  direction temp_dir; //храним для продолжения движения после паузы



  int score;
  int snake_length;
  bool press_limit;//предотвращение двух нажатий за одну тайм-итерацию
  bool is_record;//находится ли в режиме повтора
  bool is_file_record;
  bool rec_possible;//ожидается ли переход в режим повтора
  bool gm_ovr;
  bool is_design;
  QVector < std::pair < direction, std::pair< int, int > >> record_snake;
  QVector < std::pair < direction, std::pair< int, int > >> record_file_snake;

  void doPainting();
  void paintSnake(QPainter * );
  void paintPrize(QPainter * );
  void showRecord();
  //void showFileRecord();

  //std::random_device rd;


private:

  QWidget * m_parent;
  MainWindow * main_win;

  QVector < std::pair < int,int >> snake;//координаты змейки

  int m_h;
  int m_px;
  int m_py;
  int m_ph;






};

#endif // GAME_H





