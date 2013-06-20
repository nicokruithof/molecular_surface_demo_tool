#ifndef MODEL_TYPES_H_
#define MODEL_TYPES_H_

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Regular_triangulation_euclidean_traits_2.h>
#include <CGAL/Regular_triangulation_2.h>

#include <map>
#include <queue>

typedef CGAL::Exact_predicates_inexact_constructions_kernel   K;
typedef CGAL::Regular_triangulation_euclidean_traits_2<K>     Gt;

typedef CGAL::Regular_triangulation_2<Gt>                     Regular;
typedef Regular::Weighted_point                               Weighted_point;
typedef Regular::Bare_point                                   Bare_point;
typedef Gt::Ray_2                                             Ray;
typedef Gt::Segment_2                                         Segment;
typedef Gt::Vector_2                                          Vector;
typedef Gt::Line_2                                            Line;

#endif /* MODEL_TYPES_H_ */
