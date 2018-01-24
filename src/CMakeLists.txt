### DO NOT DELETE THIS COMMENT: INSERT_ARCHETYPES_HERE ###
USE_CYCLUS("hybrid" "cogeneration")

INSTALL_CYCLUS_MODULE("hybrid" "cogeneration")

# install header files
FILE(GLOB h_files "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
install_cyclus_standalone("Cogeneration" "cogeneration" "hybrid")
