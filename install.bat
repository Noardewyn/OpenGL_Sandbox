conan profile new OpenGL_Sandbox --detect
conan install . --profile OpenGL_Sandbox --output-folder=out/build/x64-Debug --build=missing
cd out/build/x64-Debug
./conanbuild.bat