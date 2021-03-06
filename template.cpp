/*
 *   CPP header template
 *   Developed by Dr. Prasanta Pal, UMASS MEdical School && Brown University School of Public Health
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
#define max_points 422;
void generate_random_numbers(std::vector<double>& v);
void draw_vector_to_image(std::string path,std::vector<double>& v);
int main(int argc, char *argv[])
{
  int n = max_points;
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
  static int baseline_point=30;
  static int maxPoints = max_points;
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
  static double startX = margin;
  static double startY = margin;
  QImage img(w, h, QImage::Format_ARGB32);
  QPixmap pixmap(w,h);
  pixmap.fill(Qt::black);
  img.fill(Qt::black);
  QPainter painter(&img);
  double  spaceWidth = {0};
  int  penWidth = {2};
  spaceWidth = static_cast<double>(wMargin)/std::max<int>(1,(maxPoints));
  std::cout << spaceWidth << std::endl;
  int penMargin = 3; ///Pixel unit
  QColor color(QColor(100,100,100));
  QPen pen(color);
  pen.setWidth(penWidth);
  painter.setPen(pen);
  painter.drawRect(startX,startY,wMargin,hMargin);
pen.setStyle( Qt::DashDotLine );
pen.setColor( "purple" );
  painter.setPen(pen);
  painter.drawLine(startX,midY,startX + wMargin,midY);
  int baselineVerticalX = startX + spaceWidth*baseline_point;

  std::tuple<int,int,int,int> baselineVerticalPoint(baselineVerticalX,midY-yScale,baselineVerticalX,midY+yScale);
pen.setStyle( Qt::DashDotLine );
pen.setColor( "orange" );
  painter.setPen(pen);
  painter.drawLine(std::get<0>(baselineVerticalPoint),std::get<1>(baselineVerticalPoint),std::get<2>(baselineVerticalPoint),std::get<3>(baselineVerticalPoint));

  pen.setColor(QColor(0,255,0));
  pen.setWidth(penWidth);
pen.setStyle( Qt::SolidLine );
  painter.setPen(pen);

  std::for_each(v.begin(),v.end(),[&painter,&spaceWidth,&penMargin](double& val){
      static std::random_device rnd_device;
      // Specify the engine and distribution.
      std::mt19937 mersenne_engine {rnd_device()};  // Generates random integers
      std::uniform_int_distribution<double> dist {0, 255};
      static auto gen = [&dist, &mersenne_engine](){
      return dist(mersenne_engine);
      };
      static int counter = 0;
      static double tickPointStartX;
      static double tickPointEndX;
      static double tickPointStartY;
      static double tickPointEndY;
      if(counter == 0) {

      tickPointStartX = margin + penMargin;
      tickPointStartY = midY - val*yScale ;
      tickPointEndX = tickPointStartX ;
      tickPointEndY = tickPointStartY ;
      }else {
      tickPointEndX = tickPointStartX + spaceWidth;
      tickPointEndY = midY - val*yScale ; ;
      }
      std::cout << "counter:" << counter  << " " << spaceWidth << " " << penMargin << " " << margin << " " << wMargin << " "  << tickPointStartX << " "  << tickPointEndX<< std::endl;
      painter.drawLine(tickPointStartX,tickPointStartY,tickPointEndX,tickPointEndY);
      tickPointStartX = tickPointEndX;
      tickPointStartY = tickPointEndY;
      ++counter;
  });
  QString path(path_str.c_str());
  img.save(path);
}
