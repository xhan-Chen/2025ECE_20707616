#
# Basic VTK Cylinder Example with VR output
# Taken from: https://kitware.github.io/vtk-examples/site/Cxx/GeometricObjects/CylinderExample/
#

cmake_minimum_required( VERSION 3.12 FATAL_ERROR )

project( vtkexample_vr1 )

find_package( VTK COMPONENTS 
  CommonColor
  CommonCore
  FiltersSources
  InteractionStyle
  RenderingContextOpenGL2
  RenderingCore
  RenderingFreeType
  RenderingGL2PSOpenGL2
  RenderingOpenGL2
  RenderingOpenVR               # This is new
)

if( NOT VTK_FOUND )
    message( FATAL_ERROR "CylinderExample: Unable to find the VTK build folder." )
endif()

# Prevent a "command line is too long" failure in Windows.
set( CMAKE_NINJA_FORCE_RESPONSE_FILE "ON" CACHE BOOL "Force Ninja to use response files." )

# Define executable
add_executable( vtkexample_vr1 MACOSX_BUNDLE vtkvr.cpp )

# Link to VTK libraries
target_link_libraries( vtkexample_vr1 PRIVATE ${VTK_LIBRARIES} )

# vtk_module_autoinit is needed
vtk_module_autoinit( TARGETS vtkexample_vr1 MODULES ${VTK_LIBRARIES} )

# Copy across Open VR config files
# These exist in VTK bin dir but for some reason VR classes expect them to be in local dir
add_custom_target(  VRBindings )
add_custom_command( TARGET VRBindings PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E
        copy_directory ${CMAKE_SOURCE_DIR}/vrbindings ${CMAKE_BINARY_DIR}/ )

add_dependencies( vtkexample_vr1 VRBindings )

# 8. 【重点：解决 DLL 缺失问题】
if(WIN32)
    # A. 自动拷贝 VTK 相关的 DLL
    add_custom_command(TARGET vtkexample_vr1 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:vtkexample_vr1>
        $<TARGET_FILE_DIR:vtkexample_vr1>
        COMMAND_EXPAND_LISTS
    )

    # B. 强制拷贝 OpenVR DLL (基于你图片中的路径)
    # 我们通过你提供的 .lib 路径推断出 .dll 的位置
    get_filename_component(OPENVR_LIB_DIR "${OpenVR_LIBRARY}" DIRECTORY)
    find_file(OPENVR_DLL 
        NAMES openvr_api.dll 
        PATHS "${OPENVR_LIB_DIR}/../../bin/win64" "${OPENVR_LIB_DIR}"
        DOC "Path to openvr_api.dll"
    )

    if(OPENVR_DLL)
        add_custom_command(TARGET vtkexample_vr1 POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${OPENVR_DLL}"
            $<TARGET_FILE_DIR:vtkexample_vr1>
        )
        message(STATUS "Found OpenVR DLL: ${OPENVR_DLL}")
    else()
        message(WARNING "Could not find openvr_api.dll automatically. You may need to copy it manually.")
    endif()
endif()