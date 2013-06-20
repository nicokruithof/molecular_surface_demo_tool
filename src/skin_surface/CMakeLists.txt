include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${OSG_INCLUDE_DIR})
include_directories(${ESBTL_INCLUDE_DIR})

file(GLOB sources *.cpp)
file(GLOB headers *.h)

file(GLOB model_sources model/*.cpp)
file(GLOB model_headers model/*.h)

file(GLOB osg_sources osg/*.cpp)
file(GLOB osg_headers osg/*.h)

file(GLOB utils_sources utils/*.cpp)
file(GLOB utils_headers utils/*.h)

file(GLOB CGAL_sources CGAL/*.cpp)
file(GLOB CGAL_headers CGAL/*.h)

file(GLOB qt_ui_sources  qt/*.cpp)
file(GLOB qt_ui_headers  qt/*.h)
file(GLOB qt_ui_forms    qt/*.ui)
file(GLOB qt_ui_resource qt/*.qrc)

qt4_wrap_cpp(qt_headers_moc ${qt_ui_headers})
qt4_wrap_ui(qt_forms_headers ${qt_ui_forms})
qt4_add_resources(qt_resources ${qt_ui_resource})

add_executable(skin_surface_viewer
  ${sources} ${headers}
  ${osg_sources} ${osg_headers}
  ${model_sources} ${model_headers}
  ${utils_sources} ${utils_headers}
  ${CGAL_sources} ${CGAL_headers}
  ${qt_ui_sources} ${qt_ui_headers}
  ${qt_forms} ${qt_forms_headers} ${qt_headers_moc}
  ${qt_resources} ${qt_ui_resource})

#add_dependencies(skin_surface_viewer)

target_link_libraries(skin_surface_viewer ${OPENSCENEGRAPH_LIBRARIES} ${QT_LIBRARIES} ${OSG_QT} ${CGAL_LIBRARIES})
