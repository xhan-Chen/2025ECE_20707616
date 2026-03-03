cmake_minimum_required(VERSION 3.10...3.30)
project(MyVTKProject)

find_package(VTK COMPONENTS 
  CommonCore
  CommonDataModel
  CommonColor       # <--- 必须添加这个模块来解决 vtkNamedColors 错误
  FiltersSources
  InteractionStyle
  RenderingContextOpenGL2
  RenderingCore
  RenderingFreeType
  RenderingOpenGL2
  REQUIRED
)

# 确保目标名和文件名正确
add_executable(MyVTKApp CylinderExample.cxx)

target_link_libraries(MyVTKApp PRIVATE ${VTK_LIBRARIES})

vtk_module_autoinit(
  TARGETS MyVTKApp 
  MODULES ${VTK_LIBRARIES}
)
# 在 target_link_libraries 之后添加
add_custom_command(TARGET MyVTKApp POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  "${VTK_PREFIX_PATH}/bin"
  "$<TARGET_FILE_DIR:MyVTKApp>"
)