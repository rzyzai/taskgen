//   Copyright 2023 taskgen - caozhanhao
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.
#ifndef TASKGEN_TASK_HPP
#define TASKGEN_TASK_HPP

#include "pic.hpp"
#include "err.hpp"

#include <array>
#include <chrono>
#include <random>
#include <algorithm>
#include <tuple>
#include <vector>
#include <fstream>

namespace taskgen
{
  class Task
  {
  public:
    std::array<Color, 30> task;
  public:
  std::tuple<Pic, Pic> generate_img() const
  {
    std::vector<std::vector<Color>> data1(3);
    std::vector<std::vector<Color>> data2(3);
    size_t i = 0;
    data1[0].insert(data1[0].end(), task.begin(), task.begin() + 5);
    data1[1].insert(data1[1].end(), task.begin() + 5, task.begin() + 10);
    data1[2].insert(data1[2].end(), task.begin() + 10, task.begin() + 15);
    data2[0].insert(data2[0].end(), task.begin() + 15, task.begin() + 20);
    data2[1].insert(data2[1].end(), task.begin() + 20, task.begin() + 25);
    data2[2].insert(data2[2].end(), task.begin() + 25, task.begin() + 30);
    return {Pic{data1}, Pic{data2}};
  }
  };
  
  Task random_task(size_t r, size_t g, size_t b)
  {
    if(r + g + b != 30)
      throw Error(TASKGEN_ERROR_LOCATION, __func__, "R + G + B shall be 30.");
    std::array<Color, 30> task;
    std::fill(task.begin(), task.begin() + r, Color::R);
    std::fill(task.begin() + r, task.begin() + r + g, Color::G);
    std::fill(task.begin() + r + g, task.end(), Color::B);
    unsigned seed = std::chrono::system_clock::now ().time_since_epoch ().count ();
    std::shuffle(task.begin(), task.end(), std::default_random_engine (seed));
    return {task};
  }
}
#endif