cmake_minimum_required(VERSION 3.2.0)
project(VKApp)
add_subdirectory(libs/glfw-3.3.4)
add_subdirectory(libs/glm)
add_subdirectory(libs/tinyobjloader)
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SRC ${PROJECT_SOURCE_DIR}/libs)
find_package(Vulkan)
include_directories(${VULKAN_INCLUDE_DIRS})

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_executable(VKApp main.cpp Structs/vk.h Application/Application.cpp Device/Device.cpp Window/Window.cpp Swapchain/Swapchain.cpp Resources/image.cpp
Resources/FileLoad.cpp Pipelines/Pipelines.cpp Renderpass/Renderpass.cpp Framebuffer/Framebuffer.cpp
Commands/CommandPool.cpp Commands/Commandbuffer.cpp Renderer/Renderer.cpp Renderer/Semaphore.cpp Structs/Shaders.h ImGui/ImGui.cpp
Resources/Buffer.cpp Resources/Memory.h Camera/Camera.cpp Renderer/RenderData.h Descriptors/DescriptorLayout.cpp Descriptors/DescriptorPool.cpp
Model/Model.cpp Model/tinyobjloader.cpp libs/imgui/imgui.cpp libs/imgui/imgui_impl_vulkan.cpp libs/imgui/imgui_impl_glfw.cpp
libs/imgui/imgui_draw.cpp libs/imgui/imgui_demo.cpp libs/imgui/imgui_tables.cpp libs/imgui/imgui_widgets.cpp)

target_link_libraries(VKApp PUBLIC glfw ${VULKAN_LIBRARIES} Vulkan::Vulkan)