from conans import ConanFile, CMake

class TestConan(ConanFile):
   settings = "os", "compiler", "build_type", "arch"
   generators = "cmake_find_package"

   def requirements(self):
     
     self.requires("glfw/3.3.2");
     self.requires("glew/2.1.0");
     self.requires("glm/0.9.9.8");

     self.requires("stb/20200203");
     self.requires("spdlog/1.8.5");
     self.requires("imgui/1.83");

   def imports(self):
       self.copy("*.dll", dst="bin", src="bin")
       self.copy("*", dst="../../../bindings", src="res/bindings")

   def build(self):
      cmake = CMake(self)
      cmake.configure()
      cmake.build()
   