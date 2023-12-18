# Building instructions in Arch for the esterOS updater:
(internet access is required to download libraries and neccesary programs)

Prerequisites for building the esterOS GUI updater:

gcc (install it by typing pacman -S gcc)\
appstream-util (install it by typing pacman -S extra/appstream-glib)\
pkg-config (install it by typing pacman -S pkg-config)\
cmake (install it by typing pacman -S cmake)\
gtk4 (install it by typing pacman -S gtk4)\
meson (install it by typing pacman -S meson)\

To install the neccesary libraries all at once, it is recommended to use the install-lib-arch.sh script to not have to paste each library individually inside of your console or you can also paste this handy command:
```bash
sudo pacman -Syy gcc appstream-glib pkg-config cmake gtk4 meson
```

1. Download git (pacman -S git)
2. Clone repo (git clone https://github.com/ester-sources/assets)
3. Go into esteros-updater directory
4. Type meson setup esteros-updater to check if all the neccesary libraries are installed and to configure the build enviroment (if there are missing libraries then install them from the prerequisites list for building the esterOS GUI updater)
5. Go into the build enviroment by typing cd esteros-updater (yes it makes the build enviroment with the same name as the cloned repo's directory name)
6. To build it, type ninja
7. Done! (output is in esteros-updater/src called esteros-updater)
