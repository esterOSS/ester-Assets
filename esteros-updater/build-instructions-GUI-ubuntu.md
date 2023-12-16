# Building instructions in Ubuntu 23.10 for the esterOS updater:
(internet access is required to download libraries and neccesary programs)

Prerequisites for building the esterOS GUI updater:\
gcc (install it by typing sudo apt install gcc)\
meson (install it by typing sudo apt install meson)\
appstream-util (install it by typing sudo apt install appstream-util)\
pkg-config (install it by typing sudo apt install pkg-config)\
cmake (install it by typing sudo apt install cmake)\
gtk4 (install it by typing sudo apt install libgtk-4-dev)\
msfgmt (install it by typing sudo apt install gettext)\
libadwaita-1 (install it by typing sudo apt install libadwaita-1-dev)

OR you can paste this handy command:
```bash
sudo apt install gcc meson appstream-util pkg-config cmake libgtk-4-dev gettext libadwaita-1-dev
```

1. Download git (sudo apt install git)
2. Clone repo (git clone https://github.com/ester-sources/assets)
3. Go into esteros-updater directory
4. Type meson setup esteros-updater to check if all the neccesary libraries are installed and to configure the build enviroment (if there are missing libraries then install them from the prerequisites list for building the esterOS GUI updater)
5. Go into the build enviroment by typing cd esteros-updater (yes it makes the build enviroment with the same name as the cloned repo's directory name)
6. To build it, type ninja
7. Done! (output is in esteros-updater/src called esteros-updater)
