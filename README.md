<br />
<div align="center">
<h3 align="center">OpenGL_Sandbox</h3>

  <p align="center">
    Simple OpenGL render on C++ with imgui UI and library of examples of OpenGL features use.
    <br />
  </p>
</div>


## About The Project

[![Project Screen Shot 1][project-screenshot-1]]()
[![Project Screen Shot 2][project-screenshot-2]]()
[![Project Screen Shot 3][project-screenshot-3]]()


### Prerequisites

For compilation project needs
1. Visual Studio 17 2022 compiler https://aka.ms/vs/17/release/vs_BuildTools.exe
2. Python https://www.python.org/downloads/
3. Conan package manager
    ```sh
    pip install conan
    ```
### Installation

*Windows*

1. Clone the repo
   ```sh
   git clone https://github.com/Noardewyn/OpenGL_Sandbox.git
   ```
2. Get to OpenGL_Sandbox dirrectory and install conan packages
   ```sh
   ./install.bat
   ```
3. Build project using cmake with previosly created conan toolchain
   ```sh
   cd out/build
   cmake ../../ -G "Visual Studio 17 2022" -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake
   cmake --build . --config Release
   ```

*Linux*
Not tested, but еheoretically it should build with the same instructions as for Windows

<!-- MARKDOWN LINKS & IMAGES -->
[project-screenshot-1]: images/OpenGL_Sandbox_1.png
[project-screenshot-2]: images/OpenGL_Sandbox_2.png
[project-screenshot-3]: images/OpenGL_Sandbox_3.png