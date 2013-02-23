#include "FixCellIDs_Errors_TestBeam.h"
#include <iostream>

#include <EVENT/LCCollection.h>
#include <IMPL/TrackerHitImpl.h>
#include <UTIL/BitField64.h>
#include <UTIL/ILDConf.h>
#include <UTIL/LCTOOLS.h>

//---- GEAR ----
#include "marlin/Global.h"
#include "gear/GEAR.h"
#include "gear/TPCParameters.h"
//#include "gear/ZPlanarParameters.h"
//#include "gear/ZPlanarLayerLayout.h"
#include "gear/TPCModule.h"
#include "gear/BField.h"
#include "gearimpl/Vector3D.h"

using namespace lcio ;
using namespace marlin ;


FixCellIDs_Errors_TestBeam aFixCellIDs_Errors_TestBeam ;


FixCellIDs_Errors_TestBeam::FixCellIDs_Errors_TestBeam() : Processor("FixCellIDs_Errors_TestBeam") ,
			   _nRun(0), _nEvt(0) {
  
  // modify processor description
  _description = "fix CellID0 for old TrackerHits ..." ;
  
  
  // register steering parameters: name, description, class-variable, default value
  registerInputCollection( LCIO::TRACKERHIT,
			   "TPCCollectionName" , 
			   "Name of the TPC TrackerHit collection"  ,
			   _tpcColName ,
			   std::string("AllTPCTrackerHits")
			   );
  
}


void FixCellIDs_Errors_TestBeam::init() {
  
  streamlog_out(DEBUG) << "   init called  " << std::endl ;
  
  // usually a good idea to
  printParameters() ;
  
  _nRun = 0 ;
  _nEvt = 0 ;
  
}


void FixCellIDs_Errors_TestBeam::processRunHeader( LCRunHeader* run) {
  
  _nRun++ ;
} 



void FixCellIDs_Errors_TestBeam::modifyEvent( LCEvent * evt ) {


   UTIL::BitField64 encoder( ILDCellID0::encoder_string ) ; 


  //=====================================================================================================
  //      TPC 
  //=====================================================================================================
  
  
  LCCollection* tpcCol = 0 ;

  try{   tpcCol =  dynamic_cast<LCCollection*> (evt->getCollection( _tpcColName )  ); 

  } catch( lcio::DataNotAvailableException& e) { 
    
    streamlog_out( DEBUG5 ) <<  " input collection not in event : " << _tpcColName << "   - nothing to do for TPC hits  !!! " << std::endl ;  
  } 
  
  if( tpcCol ) {

    const gear::TPCParameters& gearTPC = Global::GEAR->getTPCParameters() ;

    UTIL::BitField64 encoder( lcio::ILDCellID0::encoder_string ) ;

    int nHit = tpcCol->getNumberOfElements()  ;

    for(int i=0; i< nHit ; i++){

      TrackerHitImpl* h = dynamic_cast<TrackerHitImpl*>( tpcCol->getElementAt( i ) ) ;

      const double* pos = h->getPosition();
      // Note: here we take the Cartesian coordinates as specified in the gear file!
      const gear::TPCModule& tpcmodule = gearTPC.getNearestModule(pos[0], pos[1]);


      int mod = tpcmodule.getModuleID();
      int pad = tpcmodule.getNearestPad(pos[0], pos[1]);
      int row = tpcmodule.getRowNumber(pad);

      int row_global =
  	// Modules 0 and 1 get the same row index
  	( mod==0 || mod==1 ) ? row :
		/* For modules 2, 3 and 4 the row get increased
		 * by the number of rows in the modules 0/1
		 */
        ( ( mod==2 || mod==3 || mod==4) ? gearTPC.getModule(0).getNRows() + row :
		/* For modules 5 and 6 the row get increased
		 * by the number of rows in the modules 0/1 and 2/3/4
		 */
  	gearTPC.getModule(0).getNRows() + gearTPC.getModule(2).getNRows() + row );


      encoder.reset() ;  // reset to 0
      encoder[ILDCellID0::subdet] = ILDDetID::TPC ;
//      encoder[ILDCellID0::side]   = 0 ;
//      encoder[ILDCellID0::layer] = row ;
//      encoder[ILDCellID0::module] = mod ;
//      encoder[ILDCellID0::sensor] = 0 ;
      encoder[ILDCellID0::layer] = row_global ;
//      encoder[ILDCellID0::module] = 0 ;
//      encoder[ILDCellID0::sensor] = 0 ;

      int CellID = encoder.lowWord() ;
      h->setCellID0(CellID);

////      // Alex:: fix covariance matrix
////      gear::Vector3D v(h->getPosition());
////      double phi = v.phi();
//
      float covMatrix[6] = {0.5 ,
//			    cos(phi) * sin(phi) , 0.5,
			    0., 0.5,
			    0., 0., 2
                           };
      h->setCovMatrix(covMatrix);

      tpcCol->parameters().setValue( "CellIDEncoding" , ILDCellID0::encoder_string ) ;
    } 
  }

  streamlog_out( DEBUG3 ) << "   processing event: " << evt->getEventNumber() 
			  << "   in run:  " << evt->getRunNumber() << std::endl ;

  _nEvt ++ ;
}



void FixCellIDs_Errors_TestBeam::check( LCEvent * evt ) {
  // nothing to check here - could be used to fill checkplots in reconstruction processor
}


void FixCellIDs_Errors_TestBeam::end(){

  streamlog_out( MESSAGE ) << "FixCellIDs_Errors_TestBeam::end()  " << name()
			   << " processed " << _nEvt << " events in " << _nRun << " runs "
			   << std::endl ;
  
}


