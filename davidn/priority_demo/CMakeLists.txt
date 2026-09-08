# -----------------------------------------------------------------------------
# Priority scheduling demo (see priority_demo.cpp for the write-up).
#
# Builds the same source twice, once with TF_ENABLE_TASK_PRIORITY enabled and
# once without, so the two binaries can be run side by side to compare
# scheduling order -- independent of the top-level TF_ENABLE_TASK_PRIORITY
# option.
# -----------------------------------------------------------------------------

add_executable(priority_demo_on priority_demo.cpp)
target_link_libraries(priority_demo_on ${PROJECT_NAME} Threads::Threads tf::default_settings)
target_compile_definitions(priority_demo_on PRIVATE TF_ENABLE_TASK_PRIORITY)

add_executable(priority_demo_off priority_demo.cpp)
target_link_libraries(priority_demo_off ${PROJECT_NAME} Threads::Threads tf::default_settings)
