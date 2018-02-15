### DO NOT DELETE THIS COMMENT: INSERT_ARCHETYPES_HERE ###
USE_CYCLUS("hybrid" "cogeneration")

INSTALL_CYCLUS_MODULE("hybrid" "cogeneration")

# install header files
FILE(GLOB h_files "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

INSTALL_CYCLUS_STANDALONE("Cogeneration" "cogeneration" "hybrid")
INSTALL_CYCLUS_STANDALONE("Elec_cogen" "elec_cogen" "hybrid")
INSTALL_CYCLUS_STANDALONE("Grid" "grid" "hybrid")
INSTALL_CYCLUS_STANDALONE("Heat_sink" "heat_sink" "hybrid")
