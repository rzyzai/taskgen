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
#include "task.hpp"
#include "cpp-httplib/httplib.h"
#include "cppcodec/base64_default_rfc4648.hpp"
#include <string>

std::string make_html(const std::string& s1, const std::string& s2)
{
  return
  "<!DOCTYPE html>\n"
  "<html lang=\"zh\">\n"
  "<head>\n"
  "    <meta charset=\"utf-8\" name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
  "    <title>任务卡生成器</title>\n"
  "    <link href=\"https://unpkg.com/mdui@1.0.2/dist/css/mdui.min.css\" rel=\"stylesheet\"/>\n"
  "</head>\n"
  "<body>"
  "<div class=\"mdui-toolbar mdui-color-blue-700 mdui-shadow-5\">\n"
  "    <span class=\"mdui-typo-title mdui-text-color-white\">\n"
  "    任务卡生成器</span>\n"
  "    <div class=\"mdui-toolbar-spacer\"></div>\n"
  "    <div class=\"git\">\n"
  "        <a href=\"https://gitee.com/cmvy2020/taskgen\" target=\"_blank\"\n"
  "           class=\"mdui-btn mdui-btn-icon mdui-ripple mdui-ripple-white\" mdui-tooltip=\"{content: 'Gitee'}\">\n"
  "            <svg t=\"1690432643450\" class=\"mdui-icon\" viewBox=\"0 0 1024 1024\" version=\"1.1\"\n"
  "                 xmlns=\"http://www.w3.org/2000/svg\"\n"
  "                 p-id=\"5259\" style=\"width: 24px;height:24px;\">\n"
  "                <path d=\"M512 1024C229.222 1024 0 794.778 0 512S229.222 0 512 0s512 229.222 512 512-229.222 512-512 512z m259.149-568.883h-290.74a25.293 25.293 0 0 0-25.292 25.293l-0.026 63.206c0 13.952 11.315 25.293 25.267 25.293h177.024c13.978 0 25.293 11.315 25.293 25.267v12.646a75.853 75.853 0 0 1-75.853 75.853h-240.23a25.293 25.293 0 0 1-25.267-25.293V417.203a75.853 75.853 0 0 1 75.827-75.853h353.946a25.293 25.293 0 0 0 25.267-25.292l0.077-63.207a25.293 25.293 0 0 0-25.268-25.293H417.152a189.62 189.62 0 0 0-189.62 189.645V771.15c0 13.977 11.316 25.293 25.294 25.293h372.94a170.65 170.65 0 0 0 170.65-170.65V480.384a25.293 25.293 0 0 0-25.293-25.267z\"\n"
  "                      fill=\"#ffffff\" p-id=\"5260\"></path>\n"
  "            </svg>\n"
  "        </a>\n"
  "    </div>\n"
  "</div>\n"
  "<div class=\"mdui-container-fluid mdui-text-center\">\n"
  "<img title=\"upper\" style=\"padding-top:8px;\" src=\"data:image/png;base64," + base64::encode(s1) + "\">"
  + "<img title=\"lower\" style=\"padding-top:8px;padding-left: 16px;\" src=\"data:image/png;base64," + base64::encode(s2) + "\" > "
  "</div>\n"
  "</body>\n"
  "</html>";
}

int main()
{
  httplib::Server svr;
  svr.Get("/", [](const auto & req, auto &res)
  {
    czh::Task t;
    if(req.params.size() == 3)
    {
      t = czh::random_task(std::stoi(req.get_param_value("r")), std::stoi(req.get_param_value("g")),
                                                                              std::stoi(req.get_param_value("b")));
    }
    else if(req.params.size() == 6)
    {
      t = czh::random_task(std::stoi(req.get_param_value("ru")), std::stoi(req.get_param_value("rl")),
                               std::stoi(req.get_param_value("bu")), std::stoi(req.get_param_value("bl")),
                               std::stoi(req.get_param_value("gu")), std::stoi(req.get_param_value("gl")));
    }
    else
    {
      t = czh::random_task(10, 10, 10);
    }
    auto[upper, lower] = t.generate_img();
    res.set_content(make_html(
        upper.string(czh::Format::PNG, 100),
        lower.string(czh::Format::PNG, 100)), "text/html");
  });
  svr.listen("0.0.0.0", 8080);
  return 0;
}
