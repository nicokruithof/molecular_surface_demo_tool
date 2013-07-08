add_subdirectory("skin_curve")

SET(NGHK_ENABLE_SKIN_SURFACE OFF CACHE BOOL
    "Enable compilation of the skin surface application" FORCE)
if (NGHK_ENABLE_SKIN_SURFACE AND OPEN_SCENE_GRAPH_FOUND AND ESBTL_FOUND)
  add_subdirectory("skin_surface")
endif (NGHK_ENABLE_SKIN_SURFACE AND OPEN_SCENE_GRAPH_FOUND AND ESBTL_FOUND)
