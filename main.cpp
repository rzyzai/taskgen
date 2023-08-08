#include <iostream>
#include "task.hpp"

int main()
{
  auto t = taskgen::random_task(10, 10, 10);
  auto [pic1, pic2] = t.generate_img();
  pic1.paint(taskgen::Format::BMP,"test1.bmp",  10);
  pic2.paint(taskgen::Format::BMP,"test2.bmp",  10);
  return 0;
}
