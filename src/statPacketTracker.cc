
//-----------------------------------------------------------------------------
// Author: Ernst Salzmann
// Date: 16 Sept 2012
//-----------------------------------------------------------------------------

#include <StatPacketTracker.h>
//-----------------------------------------------------------------------------

StatPacketTracker::StatPacketTracker(){
	clear();
}

//-----------------------------------------------------------------------------

StatPacketTracker::~StatPacketTracker() {

}

//-----------------------------------------------------------------------------

void StatPacketTracker::clear() {
	for(int i=0;i<TRACKER_SIZE;i++) {
		_tbl[SND_IDX][i] = 0;
		_tbl[PKT_IDX][i] = 0;
	}
}

//-----------------------------------------------------------------------------

bool StatPacketTracker::accept(int sndID, int pktCount) {

	for(int i=0;i<TRACKER_SIZE;i++) {

		if ( _tbl[SND_IDX][i] == sndID ) 			//if we found the Sender ID
			if ( _tbl[PKT_IDX][i] < pktCount ) { 	//and the corresponding packet Count is smaller
				_tbl[PKT_IDX][i] = pktCount; 		//overide the counter
				return true;
			} else {				
				return false;
			}

		// if a table entry == 0 is found (meaning an open space)
		if (_tbl[SND_IDX][i] == 0) { 		//then insert here.
			_tbl[SND_IDX][i] = sndID;
			_tbl[PKT_IDX][i] = pktCount;

			return true;
		}
	}

	//else the sndID (Sender ID) with a smaller packet was not found
	//or there was no open-spaces
	return false;

} //accept

//-----------------------------------------------------------------------------




