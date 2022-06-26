#include "game.h"
#include "mainwindow.h"
#include <QAction>
#include <QTime>

Game::Game(QWidget * parent): QFrame(parent) {

  m_parent = parent;
  main_win = (MainWindow*) m_parent;

  int h = window_size / m_n;

  m_h = h;
  score = 0;
  snake_length = 2;
  m_ph = h;
  m_px = window_size / 2 - (window_size / 2) % m_ph;
  m_py = window_size / 2 - (window_size / 2) % m_ph;

  int x = 0;
  int y = m_h;
  snake.append(std::make_pair(x, y));
  x = 0;
  y = 0;
  snake.append(std::make_pair(x,y));

  press_limit = true;
  is_record = false;
  is_file_record = false;
  rec_possible = false;
  gm_ovr = false;
  is_pause = false;
  is_design = true;
  tail_side = true;
  tail_side_counter = 0;

  max_timer_delay = 70;
  timer_delay = max_timer_delay;
  min_timer_delay = 46;
  timer = startTimer(timer_delay);

  dir = Game::D_STOP;
  temp_dir = Game::D_STOP;
}



void Game::paintEvent(QPaintEvent * e) {
  Q_UNUSED(e);

  doPainting();
}

void Game::doPainting() {

  QPainter painter(this);

  QPen pen(Qt::black, 1, Qt::SolidLine);
  painter.setPen(pen);
  painter.setBrush(QColor("#ffffff"));
  painter.drawRect(pen.width(), pen.width(), window_size-2*pen.width(), window_size-2*pen.width());

  paintSnake( & painter);
  paintPrize( & painter);
}

void Game::paintSnake(QPainter * painter) {
  QTextStream out(stdout);

  QPen pen(Qt::black, 0, Qt::SolidLine);
  pen.setStyle(Qt::NoPen);
  painter -> setPen(pen);
  painter -> setBrush(QBrush("#ff0000"));

  //если не выпендриваться
  if(!is_design){
      for (int i = 0; i < snake_length; i++) {
            painter -> drawRect(snake[i].first, snake[i].second, m_h, m_h);
        }
  }
  //если выпендриваться
else{
  for (int i = 1; i < snake_length - 1; i++) {

    if (snake[i - 1].first == snake[i + 1].first) {//предшествующий и следующий сегменты вертикальны
      painter -> drawRect(snake[i].first + m_h / 6, snake[i].second, m_h / 3 * 2, m_h);
    } else if (snake[i - 1].second == snake[i + 1].second) {//предшествующий и следующий сегменты горизонтальны
      painter -> drawRect(snake[i].first, snake[i].second + m_h / 6, m_h, m_h / 3 * 2);
    } else {

        //если ни то, ни то, рисуем квадрат посередине текущего сегмента и разбираемся с краями
        painter -> drawRect(snake[i].first + m_h / 6, snake[i].second + m_h / 6, m_h / 3 * 2, m_h / 3 * 2);

      if(snake[i].first == snake[i+1].first){//следующий и текущий на одной горизонтали

          if((snake[i].second == m_h*(m_n - 1)) && (snake[i+1].second == 0)){//выход за границы
              painter -> drawRect(snake[i].first + m_h / 6, snake[i].second + m_h / 6 + m_h / 3 * 2, m_h / 3 * 2, m_h /6);
              //out << 1;
          }
          else if((snake[i+1].second == m_h*(m_n - 1)) && (snake[i].second == 0)){//выход за границы
              painter -> drawRect(snake[i].first + m_h / 6, 0, m_h / 3 * 2, m_h /6);
              //out << 2;
          }else if (snake[i+1].second == snake[i].second+m_h) {//если следующий ниже текущего
              painter -> drawRect(snake[i].first + m_h / 6, snake[i].second + m_h / 6 + m_h / 3 * 2, m_h / 3 * 2, m_h /6);
              //out << 3;
          }else {//если следующий выше
              painter -> drawRect(snake[i].first + m_h / 6, snake[i].second, m_h / 3 * 2, m_h /6);
              //out << 4;
          }



          if((snake[i].first == m_h*(m_n - 1)) && (snake[i-1].first == 0)){//выход за грагицы
              painter -> drawRect(snake[i].first + m_h / 6  + m_h / 3 * 2, snake[i].second + m_h / 6,
                                  m_h /6, m_h / 3 * 2);
              //out << 1;
          }
          else if((snake[i-1].first == m_h*(m_n - 1)) && (snake[i].first == 0)){//выход за грагицы
              painter -> drawRect(0, snake[i].second + m_h / 6, m_h /6, m_h / 3 * 2);
              //out << 2;
          }else if (snake[i-1].first == snake[i].first+m_h) {//предыдущий справа от текущего
              painter -> drawRect(snake[i].first + m_h / 6 + m_h / 3 * 2, snake[i].second + m_h / 6, m_h /6, m_h / 3 * 2);
              //out << 3;
          }else {//предыдущий слева
              painter -> drawRect(snake[i].first, snake[i].second + m_h / 6, m_h /6, m_h / 3 * 2);
              //out << 4;
          }
      }
      else {//предыдущий и текущий на одной горизонтали

          if((snake[i].second == m_h*(m_n - 1)) && (snake[i-1].second == 0)){//выход за грагицы
              painter -> drawRect(snake[i].first + m_h / 6, snake[i].second + m_h / 6 + m_h / 3 * 2, m_h / 3 * 2, m_h /6);
              //out << 5;
          }
          else if((snake[i-1].second == m_h*(m_n - 1)) && (snake[i].second == 0)){//выход за грагицы
              painter -> drawRect(snake[i].first + m_h / 6, 0, m_h / 3 * 2, m_h /6);
              //out << 6;
          }else if (snake[i-1].second == snake[i].second+m_h) {//предыдущий ниже текущего
              painter -> drawRect(snake[i].first + m_h / 6, snake[i].second + m_h / 6 + m_h / 3 * 2, m_h / 3 * 2, m_h /6);
              //out << 7;
          }else {//предыдущий выше
              painter -> drawRect(snake[i].first + m_h / 6, snake[i].second, m_h / 3 * 2, m_h /6);
              //out << 8;
          }


          if((snake[i].first == m_h*(m_n - 1)) && (snake[i+1].first == 0)){//выход за грагицы
              painter -> drawRect(snake[i].first + m_h / 6  + m_h / 3 * 2, snake[i].second + m_h / 6, m_h /6, m_h / 3 * 2);
              //out << 5;
          }
          else if((snake[i+1].first == m_h*(m_n - 1)) && (snake[i].first == 0)){//выход за грагицы
              painter -> drawRect(0, snake[i].second + m_h / 6, m_h /6, m_h / 3 * 2);
              //out << 6;
          }else if (snake[i+1].first == snake[i].first+m_h) {//следующий правее текущего
              painter -> drawRect(snake[i].first + m_h / 6 + m_h / 3 * 2, snake[i].second + m_h / 6, m_h /6, m_h / 3 * 2);
              //out << 7;
          }else {//следующий левее
              painter -> drawRect(snake[i].first, snake[i].second + m_h / 6, m_h /6, m_h / 3 * 2);
              //out << 8;
          }
      }
    }

    //пятнышки
    painter -> setBrush(QBrush("#ffffff"));

    QRect prize_rect = QRect(snake[i].first+m_h/3, snake[i].second+m_h/3, m_h/3, m_h/3);
    painter -> drawEllipse(prize_rect);
    painter -> setBrush(QBrush("#ff0000"));
  }


  //хвост
  int n_iter = 5;
  int tail_segm = snake_length-1;
      if(snake[tail_segm-1].first == snake[tail_segm].first){//если предыдущий сегмент и хвост на одной вертикали
          if((snake[tail_segm-1].second == m_h*(m_n - 1)) && (snake[tail_segm].second == 0)){//выход за границы
              painter -> drawPolygon(QPolygon{QPoint(snake[tail_segm].first+m_h/6, 0),
                                     QPoint(snake[tail_segm].first+m_h/6*5, 0),
                                     QPoint(snake[tail_segm].first+m_h/3+m_h/3*static_cast<int>(tail_side),snake[tail_segm].second+m_h)});
          }else if ((snake[tail_segm].second == m_h*(m_n - 1)) && (snake[tail_segm-1].second == 0)) {//выход за границы
              painter -> drawPolygon(QPolygon{QPoint(snake[tail_segm].first+m_h/6, snake[tail_segm].second+m_h),
                                     QPoint(snake[tail_segm].first+m_h/6*5, snake[tail_segm].second+m_h),
                                     QPoint(snake[tail_segm].first+m_h/3+m_h/3*static_cast<int>(tail_side),snake[tail_segm].second)});
          }

          else if(snake[tail_segm-1].second == snake[tail_segm].second+m_h){//хвост выше предыдущего сегмента
            painter -> drawPolygon(QPolygon{QPoint(snake[tail_segm-1].first+m_h/6,snake[tail_segm-1].second),
                                   QPoint(snake[tail_segm-1].first+m_h/6*5,snake[tail_segm-1].second),
                                   QPoint(snake[tail_segm-1].first+m_h/3+m_h/3*static_cast<int>(tail_side),snake[tail_segm].second)});
          }else{//хвост ниже предыдущего сегмента
            painter -> drawPolygon(QPolygon{QPoint(snake[tail_segm].first+m_h/6,snake[tail_segm].second),
                                   QPoint(snake[tail_segm].first+m_h/6*5,snake[tail_segm].second),
                                   QPoint(snake[tail_segm].first+m_h/3+m_h/3*static_cast<int>(tail_side),snake[tail_segm].second+m_h)});
          }

      }else{//если предыдущий сегмент и хвост на одной горизонтали
          if((snake[tail_segm-1].first == m_h*(m_n - 1)) && (snake[tail_segm].first == 0)){//выход за границы
              painter -> drawPolygon(QPolygon{QPoint(0,snake[tail_segm].second+m_h/6),
                                     QPoint(0,snake[tail_segm].second+m_h/6*5),
                                     QPoint(0+m_h,snake[tail_segm].second+m_h/3+m_h/3*static_cast<int>(tail_side))});
            }else if ((snake[tail_segm].first == m_h*(m_n - 1)) && (snake[tail_segm-1].first == 0)) {//выход за границы
              painter -> drawPolygon(QPolygon{QPoint(snake[tail_segm].first+m_h, snake[tail_segm].second+m_h/6),
                                     QPoint(snake[tail_segm].first+m_h,snake[tail_segm].second+m_h/6*5),
                                     QPoint(snake[tail_segm].first,snake[tail_segm].second+m_h/3+m_h/3*static_cast<int>(tail_side))});

          }

          else if(snake[tail_segm-1].first == snake[tail_segm].first+m_h){//хвост левее предыдущего сегмента
            painter -> drawPolygon(QPolygon{QPoint(snake[tail_segm-1].first,snake[tail_segm-1].second+m_h/6),
                                   QPoint(snake[tail_segm-1].first,snake[tail_segm-1].second+m_h/6*5),
                                   QPoint(snake[tail_segm].first,snake[tail_segm].second+m_h/3+m_h/3*static_cast<int>(tail_side))});
          }else{//хвост правее предыдущего сегмента
              painter -> drawPolygon(QPolygon{QPoint(snake[tail_segm].first,snake[tail_segm].second+m_h/6),
                                     QPoint(snake[tail_segm].first,snake[tail_segm].second+m_h/6*5),
                                     QPoint(snake[tail_segm].first+m_h,snake[tail_segm].second+m_h/3+m_h/3*static_cast<int>(tail_side))});

          }

      }

      if(tail_side_counter == n_iter){
          tail_side_counter = 0;
      tail_side=!tail_side;
}
      tail_side_counter++;



  //голова
  int face_x = snake[0].first;
  int face_y = snake[0].second;
  int reye_x{face_x+m_h/6}, reye_y{face_y+m_h/6*3}, leye_x{face_x+m_h/6*5}, leye_y{face_y+m_h/6*3};
  int rnose_x{face_x+m_h/6*2}, rnose_y{face_y+m_h/6*5}, lnose_x{face_x+m_h/6*4}, lnose_y{face_y+m_h/6*5};

  QList<QPoint> head_Polygon_points{
      QPoint(face_x+m_h/6, face_y),
                                 QPoint(face_x, face_y+m_h/6),
                                 QPoint(face_x+m_h/6, face_y+m_h),
                                 QPoint(face_x+m_h*5/6, face_y+m_h),
                                 QPoint(face_x+m_h, face_y+m_h/6),
                                 QPoint(face_x+m_h*5/6, face_y)
        };

  direction switch_dir = dir;
  if(switch_dir == D_STOP){
      switch_dir = temp_dir;
  }
  switch (switch_dir) {
  case D_FORWARD:
    head_Polygon_points = {QPoint(face_x+m_h/6, face_y+m_h),
                           QPoint(face_x, face_y+m_h*5/6),
                           QPoint(face_x+m_h/6, face_y),
                           QPoint(face_x+m_h*5/6, face_y),
                           QPoint(face_x+m_h, face_y+m_h*5/6),
                           QPoint(face_x+m_h*5/6, face_y+m_h)
                     };

    reye_x = face_x+m_h/6;
    reye_y = face_y+m_h/6*3;
    leye_x = face_x+m_h/6*5;
    leye_y = face_y+m_h/6*3;

    rnose_x = face_x+m_h/6*4;
    rnose_y = face_y+m_h/6;
    lnose_x = face_x+m_h/6*2;
    lnose_y = face_y+m_h/6;
    break;

  case D_BACK:
    head_Polygon_points = {QPoint(face_x+m_h/6, face_y),
                           QPoint(face_x, face_y+m_h/6),
                           QPoint(face_x+m_h/6, face_y+m_h),
                           QPoint(face_x+m_h*5/6, face_y+m_h),
                           QPoint(face_x+m_h, face_y+m_h/6),
                           QPoint(face_x+m_h*5/6, face_y)
                     };

    reye_x = face_x+m_h/6;
    reye_y = face_y+m_h/6*3;
    leye_x = face_x+m_h/6*5;
    leye_y = face_y+m_h/6*3;


    rnose_x = face_x+m_h/6*2;
    rnose_y = face_y+m_h/6*5;
    lnose_x = face_x+m_h/6*4;
    lnose_y = face_y+m_h/6*5;
    break;

  case D_RIGHT:
    head_Polygon_points = {QPoint(face_x, face_y+m_h/6),
                           QPoint(face_x+m_h/6, face_y),
                           QPoint(face_x+m_h, face_y+m_h/6),
                           QPoint(face_x+m_h, face_y+m_h*5/6),
                           QPoint(face_x+m_h/6, face_y+m_h),
                           QPoint(face_x, face_y+m_h*5/6)
                     };

    reye_x = face_x+m_h/6*3;
    reye_y = face_y+m_h/6;
    leye_x = face_x+m_h/6*3;
    leye_y = face_y+m_h/6*5;

    rnose_x = face_x+m_h/6*5;
    rnose_y = face_y+m_h/6*2;
    lnose_x = face_x+m_h/6*5;
    lnose_y = face_y+m_h/6*4;
    break;

  case D_LEFT:
    head_Polygon_points = {QPoint(face_x+m_h, face_y+m_h*5/6),
                           QPoint(face_x+m_h*5/6, face_y+m_h),
                           QPoint(face_x, face_y+m_h*5/6),
                           QPoint(face_x, face_y+m_h/6),
                           QPoint(face_x+m_h*5/6, face_y),
                           QPoint(face_x+m_h, face_y+m_h/6)
                     };

    reye_x = face_x+m_h/6*3;
    reye_y = face_y+m_h/6;
    leye_x = face_x+m_h/6*3;
    leye_y = face_y+m_h/6*5;

    rnose_x = face_x+m_h/6;
    rnose_y = face_y+m_h/6*2;
    lnose_x = face_x+m_h/6;
    lnose_y = face_y+m_h/6*4;
    break;
  default:

    break;
  }


  //рисуем
  painter->drawPolygon(QPolygon(head_Polygon_points));

//глазки
  painter -> setBrush(QBrush("#0000ff"));
  painter->drawEllipse(QPoint(reye_x, reye_y),m_h/6,m_h/6);
  painter->drawEllipse(QPoint(leye_x, leye_y),m_h/6,m_h/6);

//зрачки
  painter -> setBrush(QBrush("#ffff00"));
  painter->drawEllipse(QPoint(reye_x, reye_y),m_h/9,m_h/9);
  painter->drawEllipse(QPoint(leye_x, leye_y),m_h/9,m_h/9);
  painter -> setBrush(QBrush("#ff0000"));

  //носик
    painter -> setBrush(QBrush("#ffffff"));
    painter->drawEllipse(QPoint(rnose_x,rnose_y),m_h/15,m_h/15);
    painter->drawEllipse(QPoint(lnose_x,lnose_y),m_h/15,m_h/15);
}
}

void Game::paintPrize(QPainter * painter) {
  //если не выпендриваться
    if(!is_design){
        QPen pen(Qt::black, 1, Qt::SolidLine);
        //pen.setStyle(Qt::NoPen);
        painter -> setPen(pen);
        painter -> setBrush(QBrush("#00ff00"));

        painter->drawRect(m_px, m_py, m_ph, m_ph);
    }

else{
  QRadialGradient grad1(m_px+m_ph/2, m_py+m_ph/2, m_ph/2);

  QPen pen(Qt::black, 1, Qt::SolidLine);
  //pen.setStyle(Qt::NoPen);
  painter -> setPen(pen);
  painter -> setBrush(QBrush("#00ff00"));

  QRect prize_rect = QRect(m_px, m_py, m_ph, m_ph);
  painter -> drawEllipse(prize_rect);




//градиент
/*
    grad1.setColorAt(0, QColor("#ffffff"));
    grad1.setColorAt(0.3, QColor("#00ff00"));
    grad1.setColorAt(1, QColor("#ffffff"));

    painter->fillRect(m_px, m_py, m_ph, m_ph, grad1);
*/

    }
}

void Game::keyPressEvent(QKeyEvent * event) {
  if (event -> key() == Qt::Key_Escape) {
    qApp -> quit();
  }


  if ((event->key() == Qt::Key_G)&&(rec_possible)){
      is_record = true;
     showRecord();
  }else{

  QTextStream out(stdout);
  out << score << '\n';
  auto temp_key = event -> key();

  switch (temp_key) {
  case Qt::Key_W:
    if ((dir != Game::D_BACK)&&(press_limit)&&(!is_pause)){
      dir = Game::D_FORWARD;
      press_limit = false;
    }
    break;
  case Qt::Key_S:
    if ((dir != Game::D_FORWARD)&&(press_limit)&&(!is_pause)){
      dir = Game::D_BACK;
      press_limit = false;
    }

  break;

  case Qt::Key_A:
    if ((dir != Game::D_RIGHT)&&(press_limit)&&(!is_pause)){
      dir = Game::D_LEFT;
      press_limit = false;
    }
    break;

  case Qt::Key_D:
    if ((dir != Game::D_LEFT)&&(press_limit)&&(!is_pause)){
      dir = Game::D_RIGHT;
      press_limit = false;
    }
    break;

  case Qt::Key_Space:

    if(!is_pause){
        set_pause();
    }
    else{
        set_continue();
    }

    is_pause = !is_pause;
    break;
  default:
    break;
    }
  }
}

void Game::set_pause(){
    temp_dir = dir;
    dir = Game::D_STOP;
    main_win->act_pause->setDisabled(true);
    main_win->act_continue->setEnabled(true);
}

void Game::set_continue(){
    dir = temp_dir;
    main_win->act_pause->setDisabled(false);
    main_win->act_continue->setEnabled(false);
}

void Game::timerEvent(QTimerEvent * event) {

  press_limit = true;
  std::pair < int, int > temp_head = snake[0];
  std::pair < int, int > temp_tail = snake[snake_length-1];

  //ориентируем голову по направлению
  if(is_record){
      if(dir!= D_STOP){
      temp_dir = dir;//запоминаем сюда последнее активное направление (не D_STOP)
      }
      dir = record_snake.begin()->first;
  }

  if(is_file_record){
      if(dir!= D_STOP){
      temp_dir = dir;//запоминаем сюда последнее активное направление (не D_STOP)
      }
      dir = record_file_snake.begin()->first;
  }

  switch (dir) {
  case Game::D_FORWARD:
    if (snake[0].second == 0) {
      temp_head.second = window_size - m_h;

    } else {
      temp_head.second = (snake[0].second - m_h) % window_size;
    }
    break;

  case Game::D_BACK:
    temp_head.second = (snake[0].second + m_h) % window_size;
    break;

  case Game::D_LEFT:
    if (snake[0].first == 0) {
      temp_head.first = window_size - m_h;
    } else {
      temp_head.first = (snake[0].first - m_h) % window_size;
    }
    break;

  case Game::D_RIGHT:
    temp_head.first = (snake[0].first + m_h) % window_size;
    break;
  default:
    break;
  }

  if(is_record){
      record_snake.removeFirst();
  }

  if(is_file_record){
      record_file_snake.removeFirst();
  }

//движение
  if (dir != Game::D_STOP) {
    for (int i = snake_length-1; i > 0; i--) {
      snake[i] = snake[i - 1];
    }
    snake[0] = temp_head;
  }

  //если съел приз
  if ((temp_head.first == m_px) && (temp_head.second == m_py)) {

    score++;

    //обновляем счет на экране
    main_win->score_label->setNum(score);

    if(score%2==0){//ускоряем каждые 2 очка
        if(timer_delay > min_timer_delay){
        timer_delay-=2;
        killTimer(timer);
        timer = startTimer(timer_delay);
        }

        //обновляем значение прогрессбара
        main_win->speed_progress = (max_timer_delay - timer_delay) * 100 / (max_timer_delay - min_timer_delay);
        main_win->progress->setValue(main_win->speed_progress);

        if(main_win->speed_progress==100){
            main_win->speed_text_label->setText("Max speed! ");
        }
    }
    snake_length++;
    snake.append(temp_tail);

    if((!is_record)&&(!is_file_record)){

    int tm = QTime::currentTime().second();
    std::mt19937 mersenne(tm);

    int t_x = snake[0].first;
    int t_y = snake[0].second;
    bool check = false;

    while (!check) {//проверяем, не лежит ли новый приз на змее
      t_x = (mersenne() % m_n) * m_ph;
      t_y = (mersenne() % m_n) * m_ph;
      check = true;
      for (int i = 0; i < snake_length; i++) {
        if ((t_x == snake[i].first) && (t_y == snake[i].second)) {
          check = false;
          break;
        }

      }
    }

    m_px = t_x;
    m_py = t_y;
    }
    else if(is_record) {
        m_px = record_snake.begin()->second.first;
        m_py = record_snake.begin()->second.second;
    }
    else{
        m_px = record_file_snake.begin()->second.first;
        m_py = record_file_snake.begin()->second.second;
        QTextStream out(stdout);

        out << 8;
    }
  }

  if(!is_record){
      record_snake.append(std::make_pair(dir, std::make_pair(m_px, m_py)));
  }





  //проверка самокуся

  for (int i = 1; i < snake_length; i++) {
    if ((snake[i].first == snake[0].first) && (snake[i].second == snake[0].second)) {
      gm_ovr = true;
      break;
    }
  }

  if(gm_ovr){



      rec_possible = true;

      if((!is_record)&&(!is_file_record)){
      main_win->act_show_rec->setDisabled(false);
      }

      main_win->act_file_show_rec->setDisabled(false);
      QTextStream out(stdout);

      out << "Game over\n";
      //dir = Game::D_STOP;
      killTimer(timer);

      QString pth = "progress.txt";

      QFile file(pth);


      if (!file.open(QIODevice::ReadWrite)) {
         qWarning("Cannot open progress-file for reading");
       }

       QTextStream in(&file);
       QTextStream out_file(&file);

       /*QString line = in.readLine();
       int hgsc = line.toInt();*/

       char c;
       file.getChar(&c);
       int hgsc = c-48;


       if(score > hgsc){
           file.resize(0);

           file.putChar(score+48);



           for (int i = 0; i< record_snake.length(); i++) {
//               out_file << static_cast<int>(record_snake[i].first);
//               out_file << record_snake[i].second.first;
//               out_file << record_snake[i].second.second;
                file.putChar(static_cast<int>(record_snake[i].first)+48);
                file.putChar(record_snake[i].second.first/m_h+48);
                file.putChar(record_snake[i].second.second/m_h+48);

           }

           main_win->highscore_label->setNum(score);

           out << QString("You beat your highscore (%1)!\n").arg(hgsc);
           out << "New highscore is: " << score << "\n\n";
       } else {
           out << "Your score is: " << score << "\n";
           out << "Highscore is: " << hgsc << "\n\n";
       }

       file.close();
  }

  //раскоммент если надо подвигать змею руками
  //dir = Game::D_STOP;
  repaint();
}


void Game::showRecord(){

  snake.clear();
  QTextStream out(stdout);

  m_px = window_size / 2 - (window_size / 2) % m_ph;
  m_py = window_size / 2 - (window_size / 2) % m_ph;

  score = 0;
  snake_length = 2;


  int x = 0;
  int y = m_h;
  snake.append(std::make_pair(x, y));
  x = 0;
  y = 0;
  snake.append(std::make_pair(x, y));

  gm_ovr = false;

  timer_delay = max_timer_delay;
  timer = startTimer(timer_delay);

  dir = Game::D_STOP;
  temp_dir = Game::D_STOP;
  if(is_file_record){
      QString pth = "progress.txt";

      QFile file(pth);

      if (!file.open(QIODevice::ReadWrite)) {
         qWarning("Cannot open progress-file for reading");
       }

       QTextStream in(&file);


       char c;
       file.getChar(&c);

       while (!in.atEnd()) {

           file.getChar(&c);
           direction fl_dir = static_cast<direction>(c-48);
            file.getChar(&c);
           int frst = (c-48)*m_h;

           file.getChar(&c);
          int scnd = (c-48)*m_h;

          record_file_snake.append(std::make_pair(fl_dir,std::make_pair(frst,scnd)));
       }

       file.close();

  }

}


