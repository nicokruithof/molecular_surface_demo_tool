#ifndef EXTRACT_BALLS_FROM_PDB_H
#define EXTRACT_BALLS_FROM_PDB_H

#include <model/types.h>

bool extract_balls_from_pdb(const char *filename,
                            std::list<Weighted_point> &weighted_points);

#endif // EXTRACT_BALLS_FROM_PDB_H
