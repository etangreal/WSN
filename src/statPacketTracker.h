
//-----------------------------------------------------------------------------
// Author: Ernst Salzmann
// Date: 16 Sept 2012
//-----------------------------------------------------------------------------


#ifndef STAT_PACKET_TRACKER_DEF
#define STAT_PACKET_TRACKER_DEF

class StatPacketTracker {
	
	public:
		static const int TRACKER_SIZE = 10;
		static const int SND_IDX = 0;	//Sender(ID) Index
		static const int PKT_IDX = 1;	//Packet(Count) Index

	protected:
		int _tbl[SND_IDX+PKT_IDX+1][TRACKER_SIZE];

	public:
		StatPacketTracker();
	    ~StatPacketTracker();

		void clear();
		bool accept(int sndID, int pktCount);
};

#endif
