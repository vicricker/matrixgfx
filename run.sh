#!/bin/sh


sudo ./build/clock --led-cols=128 --led-rows=32 --led-gpio-mapping=adafruit-hat-pwm --led-show-refresh --led-limit-refresh=120 $*
