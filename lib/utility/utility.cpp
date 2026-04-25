#include "utility.h"

volatile uint8_t buttonFlags = 0;

void IRAM_ATTR handleButtonPress1() { buttonFlags |= (1 << 0); }
void IRAM_ATTR handleButtonPress2() { buttonFlags |= (1 << 1); }
void IRAM_ATTR handleButtonPress3() { buttonFlags |= (1 << 2); }
void IRAM_ATTR handleButtonPress4() { buttonFlags |= (1 << 3); }