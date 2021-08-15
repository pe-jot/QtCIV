#include "rotaryencoder.h"

const QList<float> RotaryEncoder::_stepSizeList = QList<float>() << 1.0 << 5.0 << 10.0 << 12.5 << 25.0 << 50.0 << 100.0;

RotaryEncoder::RotaryEncoder()
{
}
