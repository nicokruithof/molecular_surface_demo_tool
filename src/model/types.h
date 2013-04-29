#ifndef MODEL_TYPES_H_
#define MODEL_TYPES_H_

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Skin_surface_3.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/mesh_skin_surface_3.h>
#include <CGAL/Skin_surface_polyhedral_items_3.h>
#include <list>

typedef CGAL::Exact_predicates_inexact_constructions_kernel   K;
typedef CGAL::Skin_surface_traits_3<K>                        Traits;
typedef CGAL::Skin_surface_3<Traits>                          Skin_surface_3;
typedef Skin_surface_3::Regular                               Regular_triangulation_3;
typedef Skin_surface_3::FT                                    FT;
typedef Skin_surface_3::Weighted_point                        Weighted_point;
typedef Weighted_point::Point                                 Bare_point;
typedef K::Vector_3                                           Vector;
typedef CGAL::Skin_surface_polyhedral_items_3<Skin_surface_3> Polyhedral_items;
typedef CGAL::Polyhedron_3<K, Polyhedral_items>               Polyhedron;

#endif /* MODEL_TYPES_H_ */
