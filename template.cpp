/*
 *   CPP header template
 *   Developed by Dr. Prasanta Pal
 */
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <ctime>
#include <thread>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include "template.h"
#include <QtCore>
#include <QtGui>
#include <QLabel>
#include <QApplication>
#include <QColor>
#include <QImage>
#include <QString>
#include <QFileDialog>
#include <QTranslator>
void generate_random_numbers(std::vector<double>& v);
void draw_vector_to_image(std::string path,std::vector<double>& v);
int main(int argc, char *argv[])
{
  int n = 300;
  std::vector<double> v(n);
  generate_random_numbers(v);
  QApplication a(argc, argv);
  std::string filename = "test.png";
  draw_vector_to_image(filename,v);
  return a.exec();
}
void generate_random_numbers(std::vector<double>& v){
  std::random_device rnd_device;
  // Specify the engine and distribution.
  std::mt19937 mersenne_engine {rnd_device()};  // Generates random integers
  std::uniform_real_distribution<double> dist {0, 1};
  auto gen = [&dist, &mersenne_engine](){
    return dist(mersenne_engine);
  };
  std::generate(std::begin(v), std::end(v), gen);
  std::transform(v.begin(),v.end(),v.begin(),std::bind(std::plus<double>(), std::placeholders::_1, -0.5));
  std::copy(v.begin(),v.end(),std::ostream_iterator<double>(std::cout," " ));
}
void draw_vector_to_image(std::string path_str,std::vector<double>& v){
  static int maxPoints = 300;
  static int maxPenWidth = 20;

  static int w = 3000;
  static int h = 2000;
  static int midX = w/2;
  static int midY = h/2;
  static double margin_percent = 2.0;
  static int marginX = w*margin_percent/100;
  static int marginY = h*margin_percent/100;
  static int margin = std::min(marginX,marginY);
  static int wMargin = w - 2*margin;
  static int hMargin = h - 2*margin;
  static int hMarginHalf = hMargin/2.0;
  static int yScale = hMarginHalf;
  static int startX = margin;
  static int startY = margin;
  QImage img(w, h, QImage::Format_ARGB32);
  QPixmap pixmap(w,h);
  pixmap.fill(Qt::black);
  img.fill(Qt::black);
  QPainter painter(&img);
  int  penWidth = 20;
  penWidth = wMargin/maxPoints;

  std::cout << penWidth << std::endl;

  int penMargin = 3; ///Pixel unit
  QColor color(QColor(100,100,100));
  QPen pen(color);
  pen.setWidth(penWidth);
  painter.setPen(pen);
  painter.drawLine(startX,midY,startX + wMargin,midY);
  painter.drawRect(startX,startY,wMargin,hMargin);
  std::for_each(v.begin(),v.end(),[&painter,&penWidth,&penMargin](double& val){
      static std::random_device rnd_device;
      // Specify the engine and distribution.
      std::mt19937 mersenne_engine {rnd_device()};  // Generates random integers
      std::uniform_int_distribution<double> dist {0, 255};
      static auto gen = [&dist, &mersenne_engine](){
      return dist(mersenne_engine);
      };
      static int counter = 0;
      int tickPointStartX = startX + penMargin + penWidth*counter;
      int tickPointEndX = tickPointStartX + penWidth;
      int tickPointStartY = midY;
      int tickPointEndY = midY - val*yScale;
      std::cout << "val :" << val << " " << yScale<< std::endl;

      QPen pen;
      if(val>0)
      pen.setColor(QColor(10,10,val*gen()));
      else
      pen.setColor(QColor(-val*gen(),10,10));

      pen.setWidth(penWidth);
      painter.setPen(pen);
      painter.drawLine(tickPointStartX,tickPointStartY,tickPointStartX,tickPointEndY);
      ++counter;
  });
  QString path(path_str.c_str());
  img.save(path);
}
