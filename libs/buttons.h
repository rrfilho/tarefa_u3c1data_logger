#pragma once

typedef void (*handler)();
void buttons_init(handler left_button_handler, handler right_button_handler);