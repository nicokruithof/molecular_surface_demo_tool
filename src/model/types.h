#ifndef MODEL_TYPES_H_
#define MODEL_TYPES_H_

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Skin_surface_3.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/mesh_skin_surface_3.h>
#include <CGAL/Skin_surface_polyhedral_items_3.h>
#include <list>

#include <ESBTL/CGAL/EPIC_kernel_with_atom.h>
#include <ESBTL/default.h>

typedef ESBTL::CGAL::EPIC_kernel_with_atom                  K;
typedef ESBTL::CGAL::Default_system                         System;
typedef CGAL::Skin_surface_traits_3<K>                      Traits;
typedef CGAL::Skin_surface_3<Traits>                        Skin_surface_3;
typedef Skin_surface_3::Regular                             Regular_triangulation_3;
typedef Skin_surface_3::FT                                  FT;
typedef Skin_surface_3::Weighted_point                      Weighted_point;
typedef Weighted_point::Point                               Bare_point;
typedef CGAL::Polyhedron_3<K>                               Polyhedron;

#endif /* MODEL_TYPES_H_ */
