#include <memory>  // for allocator, __shared_ptr_access
#include <string>  // for char_traits, operator+, string, basic_string

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"       // for Input, Renderer, Vertical
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/component_options.hpp"  // for InputOption
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for text, hbox, separator, Element, operator|, vbox, border
#include "ftxui/util/ref.hpp"  // for Ref



#include "logger.h"




int main(int argc, const char* argv[]) {
  using namespace ftxui;
  using namespace std;
  using namespace fastgo;

  FileWriter file("1.txt");

  string key_words;
  InputOption option;
  option.on_enter = [&]() {
    ilog << 666;
  };
  Component input_key_words = Input(&key_words, "key words", option);

    auto component = Container::Vertical({
      input_key_words,
   });

  auto renderer = Renderer(component, [&] {
    return vbox({
               hbox(text(" Search : "), input_key_words->Render()),
               separator(),
               text("1. " + key_words),
               text("2. " + key_words),
               text("3. " + key_words),
           }) |
           border;
  });

  auto screen = ScreenInteractive::TerminalOutput();
  screen.Loop(renderer);
}

// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
