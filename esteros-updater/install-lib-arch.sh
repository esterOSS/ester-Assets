#!/usr/bin/env bash
echo Installing neccesary libraries from AUR...
pacman -S gcc --noconfirm
pacman -S extra/appstream-glib --noconfirm
pacman -S pkg-config --noconfirm
pacman -S cmake --noconfirm
pacman -S gtk4c --noconfirm
pacman -S meson --noconfirm
echo Library instalation done!
