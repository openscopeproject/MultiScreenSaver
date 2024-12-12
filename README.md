# Multi Screen Saver

![logo](https://i.imgur.com/bUV4bE2.png)

This is a simple photo slideshow screensaver that is optimized to correctly work with multiple
screens with different orientations. It uses GPU rendering through direct2d with smooth
scaling and blend image transition.

Portrait and landscape photo directories are configurable separately and will be displayed
on corresponding monitors in a random order.

Simple keyboard actions like pause slideshow, next/previous image are also supported.

### Installation

Extract screensaver into a suitable location and right click → install.

To uninstall simply delete the file. If you want to delete the settings too then open `regedit`
and delete this path: `HKEY_CURRENT_USER\SOFTWARE\OpenScopeProject\MultiScreenSaver`.

### Configuration

Right click → configure or go to screensaver settings in windows control panel.

![config](https://i.imgur.com/x315OE8.png)

### Compilation

This project is built with cmake and vcpkg and requires wxwidgets library.
Install vcpkg first, then install wxwidgets into vcpkg and run cmake. Make sure to define
`CMAKE_TOOLCHAIN_FILE` pointing to `vcpkg/scripts/buildsystems/vcpkg.cmake`.

Make sure to pass `VCPKG_TARGET_TRIPLET="x64-windows-static"` to cmake to link the executable
statically. If you use vscode the setting is already enabled in checked in config.

Optionally pass `CMAKE_UPX_PATH=path/to/upx.exe` to compress the executable.

Config form is created using wxFormBuilder, `config_base*` files are auto generated and should
not be edited or formatted.

### License

This project is licensed under GNU GPL v3 license, see `COPYING` for more information.
