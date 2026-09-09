# -----------------------------------------------------------------------------
# Taskflow tutorial demos and exercises (davidn's personal learning project).
#
# Each *_demo.cpp / *_exercise.cpp file becomes its own executable target
# -- e.g. as CLion Run/Debug configurations -- so every lesson stays
# independently runnable/debuggable. Just drop a new lessonN_demo.cpp or
# lessonN_exercise.cpp file in this directory; CONFIGURE_DEPENDS makes CMake
# automatically pick it up and re-run configure on the next build.
# -----------------------------------------------------------------------------

file(GLOB TF_DAVIDN_TUTORIAL_SOURCES CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/*_demo.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/*_exercise.cpp
)

foreach(tutorial_source_path IN LISTS TF_DAVIDN_TUTORIAL_SOURCES)
  get_filename_component(tutorial_source ${tutorial_source_path} NAME_WE)
  add_executable(${tutorial_source} ${tutorial_source_path})
  target_link_libraries(${tutorial_source} ${PROJECT_NAME} Threads::Threads tf::default_settings)
endforeach()
