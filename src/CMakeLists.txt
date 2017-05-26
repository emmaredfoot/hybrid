### DO NOT DELETE THIS COMMENT: INSERT_ARCHETYPES_HERE ###
SET(CYCLUS_CUSTOM_HEADERS "hybrid_version.h")
USE_CYCLUS("hybrid" "cogeneration")

INSTALL_CYCLUS_MODULE("hybrid" "cogeneration")

# install header files
FILE(GLOB h_files "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
