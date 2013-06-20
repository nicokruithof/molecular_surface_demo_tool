add_subdirectory("skin_curve")
SET(NGHK_ENABLE_SKIN_SURFACE OFF CACHE BOOL "Enable compilation of the skin surface application" FORCE)

if (NGHK_ENABLE_SKIN_SURFACE)
add_subdirectory("skin_surface")
endif (NGHK_ENABLE_SKIN_SURFACE)
