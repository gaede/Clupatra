#ifndef FixCellIDs_Errors_TestBeam_h
#define FixCellIDs_Errors_TestBeam_h 1

#include "marlin/Processor.h"
#include "marlin/EventModifier.h"

#include "lcio.h"
#include <string>


using namespace lcio ;
using namespace marlin ;


/** Fix CellID0s for TPC TrackerHits from old data files  according to ILDCellID0::encoder_string.
 *
 * @param CollectionName Name of the TrackerHit collection
 * 
 * @author O. Volynets, F. Gaede, DESY
 * @version FIXME: what should go here? $Id: $
 */

class FixCellIDs_Errors_TestBeam : public Processor, public EventModifier {
  
 public:
  
  virtual Processor*  newProcessor() { return new FixCellIDs_Errors_TestBeam ; }
  
  
  FixCellIDs_Errors_TestBeam() ;
  
  virtual const std::string & name() const { return Processor::name() ; }
 
  /** Called at the begin of the job before anything is read.
   * Use to initialize the processor, e.g. book histograms.
   */
  virtual void init() ;
  
  /** Called for every run.
   */
  virtual void processRunHeader( LCRunHeader* run ) ;
  
  /** Called for every event - the working horse.
   */
  virtual void modifyEvent( LCEvent * evt ) ; 
  
  
  virtual void check( LCEvent * evt ) ; 
  
  
  /** Called after data processing for clean up.
   */
  virtual void end() ;
  
  
 protected:

  /** Input collection name.
   */
  std::string _tpcColName ;

  int _nRun ;
  int _nEvt ;
} ;

#endif



