#ifndef INCLUDE_KalTest
#define INCLUDE_KalTest 1

#include "gear/GearMgr.h"

#include "streamlog/streamlog.h"

#include "TObjArray.h"
#include "TVector3.h"

#include <cmath>

#include "KalTrack.h"

class TKalDetCradle ;

namespace IMPL{
  class TrackImpl ;
}

/** Simple wrapper class that encapsulates the KaltTest Kalman fitter.
 */
class KalTest{

  KalTest(){}
  
public:
  
  /** Default c'tor, initializes the geometry from GEAR. */
  KalTest( const gear::GearMgr& gearMgr) ;
  
  ~KalTest() ;
  
  

  KalTrack* createKalTrack()  { return new KalTrack( _det ) ; }

  //   /**Set the hits to be used for fitting - no ownership taken !*/
  //   void setHits( const TObjArray& hits) { _kalHits = hits ; }
  
  void addIPHit() ;

  /** template for adding hits of any type from a container - user needs to provide functor classes for extracting 
      TVector3 position
      int layer.
  */
  template <class In, class Pos, class Layer > 
  void addHits( In first, In last, Pos position, Layer layer) {
    

    while( first != last ){
      
      int l = layer( *first ) ;
      TVector3 pos = position( *first ) ;
      
      //      if( l > 50 ) 
	addHit( pos ,  l ) ;
      
      ++first ;
    }
    
    addIPHit() ;
  }
  
  void fitTrack(IMPL::TrackImpl* trk) ;
  
  
  
  /** add a hit at position in layer */
  void addHit( const TVector3& pos, int layer ) ;
  
  /** clear all hits */
  void clear() { _kalHits->Clear() ; }


protected:
  void init() ;

  const gear::GearMgr* _gearMgr ;
  TKalDetCradle* _det ;            // the detector cradle
  TObjArray* _kalHits;              // array to store hits

} ;

#endif