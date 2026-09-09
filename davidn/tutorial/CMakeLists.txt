# -----------------------------------------------------------------------------
# Taskflow tutorial demos and exercises (davidn's personal learning project).
#
# Each entry becomes its own executable target -- e.g. as CLion Run/Debug
# configurations -- so every lesson stays independently runnable/debuggable.
# Add a new lesson by appending its base name (without .cpp) to this list.
# -----------------------------------------------------------------------------

list(APPEND TF_DAVIDN_TUTORIAL
  lesson1_demo
  lesson1_exercise
  lesson2_demo
  lesson2_exercise
)

foreach(tutorial_source IN LISTS TF_DAVIDN_TUTORIAL)
  add_executable(${tutorial_source} ${tutorial_source}.cpp)
  target_link_libraries(${tutorial_source} ${PROJECT_NAME} Threads::Threads tf::default_settings)
endforeach()
