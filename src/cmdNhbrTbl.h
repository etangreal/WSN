//-----------------------------------------------------------------------------
// Author: Ernst Salzmann
// Date: 16 Sept 2012
// Command-Neigbor-Table (.h)
//-----------------------------------------------------------------------------

#ifndef CMD_NHBRH_TBL_DEF
#define CMD_NHBRH_TBL_DEF

class CmdNhbrTbl {

	public:
		static const int iCOMMANDS 	= 10;	//MAX-COMMANDS-SIZE
		static const int iSENDERS 	= 10;	//MAX-SENDER-SIZE

	protected:
	    int _pCmdIdx;						//Pointer to last added Command
	    int _pSndIdx;						//Pointer to last added Sender (aka: Neighbor)

	    long _cmdt[iCOMMANDS];				//iCOMMANDS-table
	    int  _sndt[iSENDERS];				//sender-table -> i.e. neighbors-table
	    bool _bits[iCOMMANDS][iSENDERS];	//a table of bits representing the intersection of iCOMMANDS/iSENDERS

	    void cleanSnds(); 					//i.e: added:Command -> clean iSENDERS
	    void cleanCmds(); 					//i.e: added:Sender -> clean iCOMMANDS

	  public:
	    CmdNhbrTbl();
	    ~CmdNhbrTbl();

	    //functions
	    void init();					// initializes the table and all related variables
	    void clear();					// clears the table

		bool isFull(); 					//table is full when all the COMMAND or NEIGHBORS slots are all filled 
									    // 	at this point the stats are compiled and sent and the table cleared
	    int iCmdIdx();					//returns the current CommandId-Index < MAX-COMMANDS-SIZE
	    int iSndIdx();					//returns the current SenderId-Index < MAX-SENDERS-SIZE

	    int getCmdIdx(long cmdID);
	    int getSndIdx(int sndID);
	    bool getVal(int sndID, long cmdID);

	    bool addUpd(int sndID, long cmdID);
	    //Takes and fills a 2arrays with SENDER-IDs & thier related STATS
	    void getStats(int statsIDs[iSENDERS], int stats[iSENDERS]);

	    void display();
		void displayOmnet();

		void displayStatsOmnet();
	    void displayStats();

		void test(bool v);
		void test2();
};

//-----------------------------------------------------------------------------
#endif
