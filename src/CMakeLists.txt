### DO NOT DELETE THIS COMMENT: INSERT_ARCHETYPES_HERE ###
SET(CYCLUS_CUSTOM_HEADERS "hybrid_version.h")
USE_CYCLUS("hybrid" "dual_generation")

INSTALL_CYCLUS_MODULE("hybrid" "dual_generation")

# install header files
FILE(GLOB h_files "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
