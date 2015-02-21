
//-----------------------------------------------------------------------------
// Author: Ernst Salzmann
// Date: 16 Sept 2012
// Command-Neigbor-Table (.cc)
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// INCLUDES
//-----------------------------------------------------------------------------

#include <omnetpp.h>

#include <stdio.h>
#include <CmdNhbrTbl.h>

//-----------------------------------------------------------------------------
// FUNCTION-IMPLEMENTATIONS
//-----------------------------------------------------------------------------

//constructor
CmdNhbrTbl::CmdNhbrTbl() {
	init();
}

//-----------------------------------------------------------------------------

//destructor
CmdNhbrTbl::~CmdNhbrTbl() {}

//-----------------------------------------------------------------------------

void CmdNhbrTbl::init() {
	_pCmdIdx = -1;
	_pSndIdx = -1;

	for(int c=0;c<iCOMMANDS;c++)
		_cmdt[c] = 0;

	for(int s=0;s<iSENDERS;s++)
		_sndt[s] = 0;

	for(int c=0;c<iCOMMANDS;c++)
		for(int s=0;s<iSENDERS;s++)
			_bits[c][s] = false;
}

//-----------------------------------------------------------------------------

//functions
void CmdNhbrTbl::clear() {
	_pCmdIdx = -1;
	_pSndIdx = -1;

	for(int s=0;s<iSENDERS;s++)
		_sndt[s] = 0;

	for(int c=0;c<iCOMMANDS;c++)
		_cmdt[c] = 0;
}

//-----------------------------------------------------------------------------

void CmdNhbrTbl::cleanSnds() { //i.e: added:Command -> clean iSENDERS
	for(int s=0;s<iSENDERS;s++)
		_bits[_pCmdIdx][s] = false;
}

//-----------------------------------------------------------------------------

void CmdNhbrTbl::cleanCmds() { //i.e: added:Sender -> clean iCOMMANDS
	for(int c=0;c<iCOMMANDS;c++)
		_bits[c][_pSndIdx] = false;
}

//-----------------------------------------------------------------------------

//table is full when all the COMMAND slots are filled
bool CmdNhbrTbl::isFull() {
	return (_pCmdIdx+1 >= iCOMMANDS);
}

//-----------------------------------------------------------------------------

//returns the current CommandID-Index < MAX-iCOMMANDS-SIZE
int CmdNhbrTbl::iCmdIdx() {
	return _pCmdIdx;
}

//-----------------------------------------------------------------------------

//returns the current SenderID-Index < MAX-SENDER-SIZE
int CmdNhbrTbl::iSndIdx() {
	return _pSndIdx;
}				

//-----------------------------------------------------------------------------

// cmdID -> commandID
int  CmdNhbrTbl::getCmdIdx(long cmdID) {

	for(int c=0;c<=_pCmdIdx;c++)
		if( _cmdt[c] == cmdID )
			return c;

	return -1;
}

//-----------------------------------------------------------------------------

// sndID -> senderID
int  CmdNhbrTbl::getSndIdx(int sndID) {

	for(int s=0;s<=_pSndIdx;s++)
		if( _sndt[s] == sndID )
			return s;

	return -1;
}

//-----------------------------------------------------------------------------

// returns -> true if successfull else false
// sndID -> senderID
// cmdID -> commandID
bool CmdNhbrTbl::addUpd(int sndID, long cmdID) {

	// sndInx -> senderIndex
    // cmdIdx -> commandIndex
    int sndIdx = getSndIdx(sndID);
    int cmdIdx = getCmdIdx(cmdID);

    // if either cmdIdx or sndIdx is -1 we will have to add ...
    if ( cmdIdx == -1 || sndIdx == -1 ) {

        //if either the next commandIndex or senderIndex exceeds their limit - we can't add ...
        if ( ((cmdIdx == -1) && (_pCmdIdx+1 >= iCOMMANDS)) || ((sndIdx == -1) && (_pSndIdx+1 >= iSENDERS)) )
            return false;

        //else we can add!!

        if ( cmdIdx == -1 ) {
            cmdIdx = ++_pCmdIdx;
            cleanSnds();
        }

        if ( sndIdx == -1 ) {
            sndIdx = ++_pSndIdx;
            cleanCmds();
        }

        _cmdt[cmdIdx] = cmdID;
        _sndt[sndIdx] = sndID;

        _bits[cmdIdx][sndIdx] = true;

        return true;

    } else { // we found the entry -> update it

        _bits[cmdIdx][sndIdx] = true;
        return true;
    }

	return false;
}

//-----------------------------------------------------------------------------

bool CmdNhbrTbl::getVal(int sndID, long cmdID) {

	// cmdIdx -> commandIndex
	// sndInx -> senderIndex
	int cmdIdx = getCmdIdx(cmdID);
	int sndIdx = getSndIdx(sndID);

	// if either of the indices are -1 -> the position (i.e. the intersection of command/sender combination) 
	// "does not exist"
	if( cmdIdx == -1 || sndIdx == -1 )
		return false;

	//else the combination exists -> check the bit (either true/false)
	return _bits[cmdIdx][sndIdx];
}

//-----------------------------------------------------------------------------

void CmdNhbrTbl::getStats(int statsIDs[iSENDERS], int stats[iSENDERS]) {
	//[iSTAT_SND+iSTAT_IDX+1]

	// t -> total
	int t=0;
	for(int s=0;s<iSENDERS;s++) {
		statsIDs[s]=0;
		stats[s]=0;
		t=0;

		for(int c=0;c<iCOMMANDS;c++)
			if(_bits[c][s]) t++;

		//if(s<=_pSndIdx) { //only get the valid stats ...
			statsIDs[s] = _sndt[s];
			stats[s] = t;
		//}
	}
}

//-----------------------------------------------------------------------------

void CmdNhbrTbl::display() {

	printf("\n-------------------------------------------------------------------------------------\n");

	printf("  ");
	for(int s=0;s<iSENDERS;s++)
		printf("%5d",_sndt[s]);
	printf("\n");

	for(int c=0;c<iCOMMANDS;c++){
		printf("%3.4ld",_cmdt[c]);
		for(int s=0;s<iSENDERS;s++) {
			printf(" {%s} ",(_bits[c][s])?"T":"F");
		}
		printf("\n");
	}

	printf("\n-------------------------------------------------------------------------------------\n");

}//display

void CmdNhbrTbl::displayOmnet() {
	char buff[200];

	EV << "\n-------------------------------------------------------------------------------------\n";

	EV << "  ";
	for(int s=0;s<iSENDERS;s++) {
		sprintf(buff,"%5d",_sndt[s]);
		EV << buff;
	}
	EV << "\n";

	for(int c=0;c<iCOMMANDS;c++) {
		sprintf(buff, "%3.4ld",_cmdt[c]); EV << buff;
		for(int s=0;s<iSENDERS;s++) {
			sprintf(buff, " {%s} ",(_bits[c][s])?"T":"F"); EV << buff;
		}
		EV << "\n";
	}

	EV << "\n-------------------------------------------------------------------------------------\n";

}//display

//-----------------------------------------------------------------------------

void CmdNhbrTbl::displayStats() {
	int statsIDs[iSENDERS];
	int stats[iSENDERS];
	getStats(statsIDs, stats);

	printf("\n-------------------------------------------------------------------------------------\n");

	for(int s=0;s<iSENDERS;s++) {
		printf("[ %5d ]", statsIDs[s]);
	}
	printf("\n");

	for(int s=0;s<iSENDERS;s++) {
		printf("[ %5d ]", stats[s]);
	}
	printf("\n");

	printf("\n-------------------------------------------------------------------------------------\n");
}

//-----------------------------------------------------------------------------

void CmdNhbrTbl::displayStatsOmnet() {
	char buff[200];

	int statsIDs[iSENDERS];
	int stats[iSENDERS];
	getStats(statsIDs, stats);

	EV << "\n-------------------------------------------------------------------------------------\n";

	for(int s=0;s<iSENDERS;s++) {
		sprintf(buff, "[ %5d ]", statsIDs[s]); EV << buff;
	}
	EV << "\n";

	for(int s=0;s<iSENDERS;s++) {
		sprintf(buff, "[ %5d ]", stats[s]); EV << buff;
	}
	EV << "\n";

	EV << "\n-------------------------------------------------------------------------------------\n";
}


//-----------------------------------------------------------------------------

// v -> verbose
void CmdNhbrTbl::test(bool v) {
	bool isUpdate;

	init();
	if(v) display();

	addUpd(17,103);
	if(v) display();

	addUpd(17,114);
	if(v) display();

	addUpd(18,98);
	if(v) display();

	for (int s=0; s<iSENDERS; ++s)
		for (int c=0; c<iCOMMANDS; ++c)
			addUpd(s+1,c+1);

	if(v) display();

	addUpd(19,98);
	if(v) display();

	addUpd(18,99);
	if(v) display();

	isUpdate = addUpd(8,103);
	if(v) display();
	if(v) printf("addUpd: {%s} \n",(isUpdate)?"T":"F");

	isUpdate = addUpd(20000,20000);
	if(v) display();
	if(v) printf("addUpd: {%s} \n",(isUpdate)?"T":"F");
}

//-----------------------------------------------------------------------------

void CmdNhbrTbl::test2() {
	test(false);
	displayStats();
}

//-----------------------------------------------------------------------------






