![engineDemo](https://user-images.githubusercontent.com/8151229/155335305-41831f34-10e3-4dd3-815d-678b658d57aa.gif)

BrewEngine is an OpenGL based game engine on top of SDL2. It's got a weird combination of platforms that it supports right now. These are Windows and Nintendo Switchs with custom firmwares. For switch, I have included an Opengl es2 renderer. Switch is supported through the DevKitPro toolchain. Mac OS support have been dropped (at least for now) due to the deprication of OpenGL on OSX. 

Bullet Points:
- SDL2 for windowing, events
- Opengl 4.1/ Opengl es2 renderer
- Dear ImGUI for GUI
- Custom 2D rigidbody physics
- premake for windows build. Make file for switch.
- Executable needs to be placed in a directory with the following:
    - res/ directory
    - Additionally on Windows, place the two dlls along with the executable (or system32 folder):
        - SDL2.dll
        - glew32.dll



