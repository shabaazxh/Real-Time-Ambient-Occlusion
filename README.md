This is a simple C++ Vulkan Renderer developed for my bachelor thesis "Examining the different Ambient Occlusion techniques used in Real-time Rendering" ([paper](Thesis.pdf)) which explores and demonstrates some popular AO methods used in real-time rendering applications.
The application demos Crytek's SSAO, HBAO and Alchemy AO.

# Features
* HBAO
* Crytek SSAO
* Alchemy AO

## Setup:

1) https://vulkan.lunarg.com/
   - Download **Vulkan SDK**
   - Download Visual Studio Code. https://code.visualstudio.com/

2) With Visual Studio Code installed, click *File > Open Folder* and select the project folder which you would have
extracted from the provided .zip.

3) Visual Studio Code extensions required:
   (extensions can be installed from within Visual Studio Code *(Cntrl+Shift+X)* and search.
   - C/C++ extension. Find and install the extension by Microsoft
   - CMake. Find and install extension by twxs
   - CMake Tools. Find and install extension by Microsoft


## Run the application:

1) Ensure CMakeLists.txt has run (should happen automatically once extension is installed). 
   To ensure it has run, click the CMakeLists.txt file and press Cntrl-S.

2) Once CMakeLists.txt has executed, press the Build button which can be found at the bottom (inside blur bar)
   This may provide a menu at the top for you to select which app to build. Find *"VkApp"* and select it.
   A new build folder will appear on the left among other folders to indicate a build is taking place/finished.

3) Once build is complete, click on the main.cpp file and press *F5*  to launch the application.
   The main.cpp file must be selected to ensure the working directory can correctly navigate to load assets.
   
![HBAO](Pictures/HBAO_Capture_render.PNG)
