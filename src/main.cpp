#include <memory>  // for allocator, __shared_ptr_access
#include <string>  // for char_traits, operator+, string, basic_string

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"       // for Input, Renderer, Vertical
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/component_options.hpp"  // for InputOption
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for text, hbox, separator, Element, operator|, vbox, border
#include "ftxui/util/ref.hpp"  // for Ref
#include <curl/curl.h>

#include "logger.h"
#include "SelectItems.h"
#include "PathManager.h"
#include "SearchEngine.h"


using namespace ftxui;
using namespace std;
using namespace fastgo;


SelectItmes g_select_items;

void global_init() {
  curl_global_init(CURL_GLOBAL_ALL);
}

int main(int argc, const char* argv[]) {
  global_init();

  try {
    if (argc > 1) {
      if (strcmp(argv[1], "-l") == 0) {
        if (argc != 2) {
          cerr << "Usage: fastgo -l" << endl;
          exit(1);
        }
        PathManager::load();
        exit(0);
      }
    }
    InputOption option;
    option.on_enter = [&]() {
      ilog << "select path:" << g_select_items.get_select_content();
      raise(SIGINT);
    };
    Component input_key_words =
        Input(&g_select_items.key_word(), "input key words", option);

    auto component = Container::Vertical({
        input_key_words,
    });

    auto renderer = Renderer(component, [&] {
      
      vector<string> items = SearchEngine::searchPath(g_select_items.key_word());
      g_select_items.set_status(!items.empty() && !g_select_items.key_word().empty());
      items.resize(3);
      g_select_items.set_item(1, items[0]);
      g_select_items.set_item(2, items[1]);
      g_select_items.set_item(3, items[2]);
      return vbox({
                 hbox(text(" Search : "), input_key_words->Render()),
                 separator(),
                 text(g_select_items.get_item(1)),
                 text(g_select_items.get_item(2)),
                 text(g_select_items.get_item(3)),
             }) |
             border;
    });

    auto renderer2 = CatchEvent(renderer, [&](Event event) {
      if (event == Event::ArrowDown) {
        g_select_items.down();
      } else if (event == Event::ArrowUp) {
        g_select_items.up();
      } else {
        renderer->OnEvent(event);
      }

      return true;
    });

    auto screen = ScreenInteractive::TerminalOutput();
    screen.Loop(renderer2);
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
  } catch ( ...) {
    elog << "unknown exception";
  }
  

  return 0;
}

// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
