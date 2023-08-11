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
  using Layer = std::array<Color, 15>;
  constexpr Color RED{.R = 255, .G = 0, .B = 0};
  constexpr Color GREEN{.R = 0, .G = 175, .B = 0};
  constexpr Color BLUE{.R = 0, .G = 111, .B = 192};
  class Task
  {
  public:
    Layer upper;
    Layer lower;
  public:
  std::tuple<Pic, Pic> generate_img() const
  {
    std::vector<std::vector<Color>> upper_data(3);
    std::vector<std::vector<Color>> lower_data(3);
    size_t i = 0;
    upper_data[0].insert(upper_data[0].end(), upper.begin(), upper.begin() + 5);
    upper_data[1].insert(upper_data[1].end(), upper.begin() + 5, upper.begin() + 10);
    upper_data[2].insert(upper_data[2].end(), upper.begin() + 10, upper.begin() + 15);
    lower_data[0].insert(lower_data[0].end(), lower.begin(), lower.begin() + 5);
    lower_data[1].insert(lower_data[1].end(), lower.begin() + 5, lower.begin() + 10);
    lower_data[2].insert(lower_data[2].end(), lower.begin() + 10, lower.begin() + 15);
    return {Pic{upper_data}, Pic{lower_data}};
  }
  };
  Layer random_layer(size_t r, size_t g, size_t b)
  {
    if (r + g + b != 15)
      throw Error(TASKGEN_ERROR_LOCATION, __func__, "R + G + B shall be 15.");
    Layer ret;
    std::fill(ret.begin(), ret.begin() + r, RED);
    std::fill(ret.begin() + r, ret.begin() + r + g, GREEN);
    std::fill(ret.begin() + r + g, ret.end(), BLUE);
    std::random_device rd;
    std::shuffle(ret.begin(), ret.end(), std::default_random_engine(rd()));
    return ret;
  }
  Task random_task(size_t r, size_t g, size_t b)
  {
    if(r + g + b != 30)
      throw Error(TASKGEN_ERROR_LOCATION, __func__, "R + G + B shall be 30.");
    std::array<Color, 30> task;
    std::fill(task.begin(), task.begin() + r, RED);
    std::fill(task.begin() + r, task.begin() + r + g, GREEN);
    std::fill(task.begin() + r + g, task.end(), BLUE);
    std::random_device rd;
    std::shuffle(task.begin(), task.end(), std::default_random_engine (rd()));
    Layer upper;
    Layer lower;
    std::copy(task.begin(), task.begin() + 15, upper.begin());
    std::copy(task.begin() + 15, task.end(), lower.begin());
    return Task{upper, lower};
  }
  Task random_task(size_t ru, size_t rl, size_t gu, size_t gl, size_t bu, size_t bl)
  {
    return {
        random_layer(ru, gu, bu),
        random_layer(rl, gl, bl)
    };
  }
}
#endif