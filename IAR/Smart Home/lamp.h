#ifndef LAMP_H
#define LAMP_H

#include <stdint.h>

void lamp_init();
void lamp_control(char command);
void lamp_update();

#endif
