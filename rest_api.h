//
// Created by Valerij Primachenko on 20-06-19.
//

#ifndef DAS_REST_API_H
#define DAS_REST_API_H

#include "keyboard.h"

void start_server(const std::shared_ptr<DAS::Keyboard>&);
void stop_server();

#endif //DAS_REST_API_H
