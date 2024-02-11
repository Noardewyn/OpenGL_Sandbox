conan profile detect --name OpenGL_Sandbox
conan install . --profile OpenGL_Sandbox --output-folder=out/build --build=missing 
cd out/build
./conanbuild.bat