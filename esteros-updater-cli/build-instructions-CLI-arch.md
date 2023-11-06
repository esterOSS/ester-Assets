# Building instructions in Arch for the esterOS updater:
(internet access is required to download the neccesary programs)

Prerequisites for building the esterOS CLI updater:
gcc (install it by typing pacman -S gcc)

1. Download git (pacman -S git)
2. Clone repo (git clone https://github.com/ester-sources/assets)
3. Go into esteros-updater-cli directory
4. To build it, just type gcc main.c
5. Done! (output is in the home directory as a file called a.out)
