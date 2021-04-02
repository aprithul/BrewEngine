![PrEngine Demo 2021-03-09 22-49-55](https://user-images.githubusercontent.com/8151229/110589242-f8efb280-812a-11eb-91a1-eca82a9cfa22.gif)

BrewEngine is an OpenGL based game engine on top of SDL2. It's got a weird combination of platforms that it supports right now. These are Windows and Nintendo Switchs with custom firmwares. For switch, I have included an Opengl es2 renderer. Switch is supported through the DevKitPro toolchain. Mac OS support have been dropped (at least for now) due to the deprication of OpenGL on OSX. 

Bullet Points:
- SDL2 for windowing, events
- Opengl 4.1/ Opengl es2 renderer
- Dear ImGUI for GUI
- premake for windows build. Make file for switch.
- Executable needs to be placed in a directory with the following:
    - res/ directory
    - Additionally on Windows, place the two dlls along with the executable (or system32 folder):
        - SDL2.dll
        - glew32.dll



