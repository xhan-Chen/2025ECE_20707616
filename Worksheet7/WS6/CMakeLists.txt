cmake_minimum_required(VERSION 3.19)

project(WS6 LANGUAGES CXX)

# --- 1. 查找依赖包 ---
# 明确列出核心模块，防止因缺失 OpenVR 等可选硬件驱动导致编译中断
find_package(VTK REQUIRED 
    COMPONENTS 
        CommonCore 
IOGeometry
        FiltersSources 
        InteractionStyle 
        RenderingContextOpenGL2 
        RenderingCore 
        RenderingFreeType 
        RenderingOpenGL2 
        GUISupportQt
)
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# Qt 标准项目设置
qt_standard_project_setup()

# --- 2. 创建可执行文件目标 ---
qt_add_executable(WS6
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    ModelPart.h
    ModelPart.cpp
    ModelPartList.h
    ModelPartList.cpp
    icons.qrc
    dialog.h 
    dialog.cpp 
    dialog.ui
)

# --- 3. 配置头文件包含路径 ---
# 使用你确认过的绝对路径，解决 vtkSmartPointer.h 找不到的问题
target_include_directories(WS6 PRIVATE 
    ${VTK_INCLUDE_DIRS}
    "C:/Program Files (x86)/VTK/include/vtk-9.6"
)

# --- 4. 链接库 ---
target_link_libraries(WS6
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        ${VTK_LIBRARIES}
)

# --- 5. 自动化拷贝所有 VTK DLL ---
if(WIN32)
    # 定位 VTK 的 bin 目录
    get_filename_component(VTK_BIN_DIR "${VTK_DIR}/../../../bin" ABSOLUTE)

    # 扫描该目录下所有的 dll 文件
    file(GLOB VTK_ALL_DLLS "${VTK_BIN_DIR}/*.dll")

    # 在编译完成后，将扫描到的所有 dll 拷贝到 .exe 所在目录，解决启动缺失 dll 错误
    add_custom_command(TARGET WS6 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${VTK_ALL_DLLS}
        "$<TARGET_FILE_DIR:WS6>"
        COMMENT "Copying all VTK DLLs to build directory..."
    )
endif()

# --- 6. 辅助目标 (处理 vrbindings 文件夹) ---
add_custom_target(VRBindings)
add_custom_command(TARGET VRBindings PRE_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
    "${CMAKE_SOURCE_DIR}/vrbindings" 
    "${CMAKE_BINARY_DIR}/"
)