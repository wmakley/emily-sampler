#!/bin/sh
#
# Simple script to compile sdl_main.c for Mac OS X, with SDL
# installed via homebrew.
#
# In the real world you would want to do something fancier to support:
#
# 1. Larger project size.
# 2. Bundling SDL up with your application.
# 3. Support for menus.
# 4. Compatibility with older versions of Mac OS X.
#
gcc -o sdl_main \
    -Wall -std=c99 \
    -I/usr/local/include \
    -L/usr/local/lib -lSDL2 \
    sdl_main.c
