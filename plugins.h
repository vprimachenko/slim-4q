//
// Created by Valerij Primachenko on 20-07-01.
//

#ifndef DAS_PLUGINS_H
#define DAS_PLUGINS_H

#include <memory>
#include "keyboard.h"

void start_python(std::shared_ptr<DAS::Keyboard> keyboard);
void stop_python();

#endif //DAS_PLUGINS_H
