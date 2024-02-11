from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps

class CompressorRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    def requirements(self):
      self.requires("glfw/3.3.2");
      self.requires("glew/2.1.0");
      self.requires("glm/0.9.9.8");
      self.requires("stb/cci.20230920");
      self.requires("spdlog/1.8.5");
      self.requires("imgui/1.90");
      self.requires("assimp/5.0.1");

    def build_requirements(self):
        self.tool_requires("cmake/3.24.3")

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin")
        self.copy("*", dst="../../../bindings", src="res/bindings")

    def build(self):
       cmake = CMake(self)
       cmake.configure()
       cmake.build()
