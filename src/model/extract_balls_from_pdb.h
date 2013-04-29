#ifndef EXTRACT_BALLS_FROM_PDB_H
#define EXTRACT_BALLS_FROM_PDB_H

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


template <class K,class System, class OutputIterator>
bool extract_balls_from_pdb(const char *filename,
                            std::vector<System>& systems,
                            OutputIterator weighted_points) 
{
  
  typedef ESBTL::Generic_classifier<ESBTL::Radius_of_atom<double,typename System::Atom> >     T_Atom_classifier;
  typedef ESBTL::PDB::Line_format<Mandatory_fields_for_skin_surface>                          Line_format;
  typedef ESBTL::Accept_none_occupancy_policy< Line_format > Accept_none_occupancy_policy;
  typedef ESBTL::Weighted_atom_iterator<typename System::Model,
                                        CGAL::Weighted_point<typename K::Point_3,double>,
                                        ESBTL::Weight_of_atoms<T_Atom_classifier> >  Weighted_atom_iterator;  
  
  ESBTL::PDB_line_selector sel;
  
  ESBTL::All_atom_system_builder<System> builder(systems,sel.max_nb_systems());
  T_Atom_classifier atom_classifier;

  ESBTL::Line_reader<
      Line_format,
      ESBTL::PDB_line_selector,
      ESBTL::All_atom_system_builder<System> >(sel,builder).template read<ESBTL::ASCII>(filename,Accept_none_occupancy_policy(),' ');
  //ESBTL::read_a_pdb_file(filename,sel,builder,Accept_none_occupancy_policy());

  if ( systems.empty() || systems[0].has_no_model() ){
      return false;
  }
  const typename System::Model& model=* systems[0].models_begin();
  std::copy(Weighted_atom_iterator(model.atoms_begin(),&atom_classifier),
            Weighted_atom_iterator(model.atoms_end(),&atom_classifier),
            weighted_points);  
  return true;
}


#endif // EXTRACT_BALLS_FROM_PDB_H
