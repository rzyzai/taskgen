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
#ifndef TASKGEN_PIC_HPP
#define TASKGEN_PIC_HPP

#include "err.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNM
#define STBI_ONLY_PNG

#include "stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb/stb_image_write.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION

#include "stb/stb_image_resize.h"

#include <vector>
#include <functional>
#include <fstream>
#include <sstream>

namespace czh
{
  enum class Format
  {
    JPG, PNG, TGA, BMP
  };
  
  struct Color
  {
    int R;
    int G;
    int B;
  };
  
  class Pic
  {
  private:
    struct StbData
    {
      int width;
      int height;
      int channels;
      stbi_uc *data;
    };
  private:
    std::vector<std::vector<Color>> data;

  public:
    Pic(std::vector<std::vector<Color>> data_)
        : data(std::move(data_)) {}
  
    void paint(Format fmt, const std::string &path, size_t width, size_t height) const
    {
      std::ofstream fs(path, std::ios::binary | std::ios::out);
      paint(fmt, fs, width, height);
      fs.close();
    }
  
    void paint(Format fmt, const std::string &path, size_t enlarge) const
    {
      std::ofstream fs(path, std::ios::binary | std::ios::out);
      paint(fmt, fs, enlarge);
      fs.close();
    }
  
    std::string string(Format fmt, size_t width, size_t height) const
    {
      std::stringstream ss;
      paint(fmt, ss, width, height);
      return ss.str();
    }
  
    std::string string(Format fmt, size_t enlarge) const
    {
      std::stringstream ss;
      paint(fmt, ss, enlarge);
      return ss.str();
    }
  
    void paint(Format fmt, std::ostream &os, size_t width, size_t height) const
    {
      size_t enlarge = (std::max(width, height) / data.size()) - 1;
      auto pic = load_pic(enlarge);
      auto out = (unsigned char *) malloc(
          static_cast<size_t>(static_cast<double>(width * height * pic.channels) * 1.5));
      int ret = stbir_resize_uint8(pic.data, pic.width, pic.height, 0,
                                   out, static_cast<int>(width), static_cast<int>(height), 0, pic.channels);
      if (ret == 0)
      {
        throw Error(TASKGEN_ERROR_LOCATION, __func__, std::string("resize failed: ") + stbi_failure_reason());
      }
      stbi_image_free(pic.data);
      pic.data = out;
      pic.width = static_cast<int>(width);
      pic.height = static_cast<int>(height);
      write_to_stream(fmt, os, pic);
      stbi_image_free(out);
    }
  
    void paint(Format fmt, std::ostream &os, size_t enlarge = 1) const
    {
      if (enlarge == 0)
      {
        throw Error(TASKGEN_ERROR_LOCATION, __func__, "enlarge must >= 1.");
      }
      auto pic = load_pic(enlarge);
      write_to_stream(fmt, os, pic);
      stbi_image_free(pic.data);
    }

  private:
    void write(Format fmt, std::ostream &os, StbData data,
                   const std::function<void(void *context, void *data, int size)>& writer) const
    {
      int ret = 0;
      auto func = [](void *context, void *data, int size)
      {
        auto osptr = reinterpret_cast<std::ostream *>(context);
        osptr->write(reinterpret_cast<char *>(data), size);
      };
      switch (fmt)
      {
        case Format::JPG:
          ret = stbi_write_jpg_to_func(func, &os, data.width, data.height, data.channels, data.data, 100);
          break;
        case Format::PNG:
          ret = stbi_write_png_to_func(func, &os, data.width, data.height, data.channels, data.data, 0);
          break;
        case Format::TGA:
          ret = stbi_write_tga_to_func(func, &os, data.width, data.height, data.channels, data.data);
          break;
        case Format::BMP:
          ret = stbi_write_bmp_to_func(func, &os, data.width, data.height, data.channels, data.data);
          break;
      }
      if (ret == 0)
      {
        throw Error(TASKGEN_ERROR_LOCATION, __func__, std::string("write failed: ") + stbi_failure_reason());
      }
    }
    void write_to_stream(Format fmt, std::ostream &os, StbData data) const
    {
      auto func = [](void *context, void *data, int size)
      {
        auto osptr = reinterpret_cast<std::ostream *>(context);
        osptr->write(reinterpret_cast<char *>(data), size);
      };
      write(fmt, os, data, func);
    }
  
    StbData load_pic(size_t enlarge) const
    {
      if (enlarge == 0)
      {
        throw Error(TASKGEN_ERROR_LOCATION, __func__, "enlarge must >= 1.");
      }
      
      std::vector<stbi_uc> raw_ppm{'P', '6', ' '};
      size_t ppm_width = data.size() * enlarge;
      size_t ppm_height = data[0].size() * enlarge;
      std::string ppm_header = std::to_string(ppm_width) + " " + std::to_string(ppm_height) + " 255 ";
      for (auto &r: ppm_header)
      {
        raw_ppm.emplace_back(r);
      }
      
      for (int i = static_cast<int>(data[0].size() - 1); i >= 0; --i)
      {
        for (int l = 0; l < enlarge; ++l)
        {
          for (int j = 0; j < data.size(); ++j)
          {
            for (int k = 0; k < enlarge; ++k)
            {
              raw_ppm.emplace_back(data[j][i].R);
              raw_ppm.emplace_back(data[j][i].G);
              raw_ppm.emplace_back(data[j][i].B);
            }
          }
        }
      }
      
      int w, h, n;
      auto ppm_data = stbi_load_from_memory(raw_ppm.data(), static_cast<int>(raw_ppm.size()), &w, &h, &n, 0);
      if (w != ppm_width || h != ppm_height)
      {
        throw Error(TASKGEN_ERROR_LOCATION, __func__, "Unexpected error when generating ppm.");
      }
      if (!ppm_data)
      {
        throw Error(TASKGEN_ERROR_LOCATION, __func__, std::string("load ppm failed: ") + stbi_failure_reason());
      }
      return {w, h, n, ppm_data};
    }
  };
}
#endif