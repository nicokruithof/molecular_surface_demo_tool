#include <model/extract_balls_from_pdb.h>

#include <ESBTL/CGAL/EPIC_kernel_with_atom.h>
#include <ESBTL/default.h>
#include <ESBTL/atom_classifier.h>
#include <ESBTL/weighted_atom_iterator.h>

struct Mandatory_fields_for_skin_surface {
    static const bool record_name=false;
    static const bool atom_serial_number=false;
    static const bool atom_name=false;
    static const bool alternate_location=false;
    static const bool residue_name=false;
    static const bool chain_identifier=false;
    static const bool residue_sequence_number=false;
    static const bool insertion_code=false;
    static const bool x=true;
    static const bool y=true;
    static const bool z=true;
    static const bool occupancy=false;
    static const bool temperature_factor=false;
    static const bool element=false;
    static const bool charge_str=false;
    static const bool model_number=false;
};


bool extract_balls_from_pdb(const char *filename,
                            std::list<Weighted_point> &weighted_points)
{
  typedef ESBTL::CGAL::EPIC_kernel_with_atom                                                    ESBTL_K;
  typedef ESBTL::CGAL::Default_system                                                           ESBTL_System;
  typedef ESBTL::Generic_classifier<ESBTL::Radius_of_atom<double,ESBTL_System::Atom> >          T_Atom_classifier;
  typedef ESBTL::PDB::Line_format<Mandatory_fields_for_skin_surface>                            Line_format;
  typedef ESBTL::Accept_none_occupancy_policy< Line_format >                                    Accept_none_occupancy_policy;
  typedef ESBTL::Weighted_atom_iterator<ESBTL_System::Model,
                                        CGAL::Weighted_point<ESBTL_K::Point_3, double>,
                                        ESBTL::Weight_of_atoms<T_Atom_classifier> >             Weighted_atom_iterator;

  //Container for molecular system
  std::vector<ESBTL_System>       systems;

  ESBTL::PDB_line_selector sel;
  
  ESBTL::All_atom_system_builder<ESBTL_System> builder(systems,sel.max_nb_systems());
  T_Atom_classifier atom_classifier;

  ESBTL::Line_reader<
      Line_format,
      ESBTL::PDB_line_selector,
      ESBTL::All_atom_system_builder<ESBTL_System> >(sel,builder).read<ESBTL::ASCII>(filename,Accept_none_occupancy_policy(),' ');
  //ESBTL::read_a_pdb_file(filename,sel,builder,Accept_none_occupancy_policy());

  if ( systems.empty() || systems[0].has_no_model() ){
      return false;
  }
  const ESBTL_System::Model& model= *systems[0].models_begin();
  Weighted_atom_iterator atom_begin(model.atoms_begin(),&atom_classifier);
  Weighted_atom_iterator atom_end(model.atoms_end(),&atom_classifier);
  for (Weighted_atom_iterator it = atom_begin; it != atom_end; ++it) {
      weighted_points.push_back(Weighted_point(Bare_point(it->x(), it->y(), it->z()), it->weight()));
  }

  return true;
}
