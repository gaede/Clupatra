#ifndef KalTrack_h
#define KalTrack_h


#include <ostream>

class TVector3 ;
class TObjArray ;
class TKalTrack ;
class TKalDetCradle ;

namespace IMPL{
  class TrackImpl ;
}

class KalTrack ;

std::ostream& operator<<(std::ostream& o, const KalTrack& trk) ;


/** Track class for track fitting with KalTest */
                                                                                
class KalTrack { // : public TKalTrack {

  friend std::ostream& operator<<(std::ostream& o, const KalTrack& trk) ;

private:
  KalTrack() {} // no default c'tor
  
public:

  // /** C'tor - initializing with size*/
  // KalTrack(Int_t n = 1) : TKalTrack(n) {}
  
  /** C'tor - initiale with detector */
  KalTrack(TKalDetCradle* det) ; 
  
  
  ~KalTrack() ;


 
  /** Add a faked hit to get track state at the IP */
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
      
      addHit( pos ,  l ) ;
      
      ++first ;
    }

    //    addIPHit() ;
  }
  
  void fitTrack(IMPL::TrackImpl* trk) ;
  
  
protected:

  /** add a hit at position in layer */
  void addHit( const TVector3& pos, int layer ) ;
  
  /** clear all hits */
  //  void clear() ; { _kalHits->Clear() ; }

protected:
  void init() ;

  TKalDetCradle* _det ;            // the detector cradle
  TObjArray* _kalHits;              // array to store hits
  TKalTrack* _trk ;

};

#endif