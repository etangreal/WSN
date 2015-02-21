

WSN PROJECT 
	by: Ernst Salzman
	date: Sept 2012


TABLE OF CONTENTS

	1) Project Description
	2) Screen Casts
	3) File Structure
	4) Future possible work - Transaction Management using a Distributed 2-Phase-Commit


1) PROJECT DESCRIPTION

	Initially the focus of this project was on transation management - specifically how it would be possible in a WSN.
	The problem turns out to be a really hard one (transaction management in a WSN) and so the focus of the project was changed …
	to determining statistics about communication between nodes. As it turns out the underlying code/infrastructure could
	be very usefull in some form of transaction management (in a WSN) if we relax the garantees somewhat ... however thats a topic 
	for another dicussion - on with the current one! :)

	What I ended up doint is monitoring all the communication between nodes and then sending this information back to the 
	basestation. Well not the exact communication logs persay, that would be a huge overhead - so instead ... I ended up recoding 
	the statistics of the communication and sending that back to the basestation every so often.

	The project was implmented in Omnet - which is a discreet simulator for networks.
	You can check it out at: www.omnetpp.org

	I've create a series of screencasts (along with well documented code) for helping anyone interested in reviewing the project.
	Hear is screen cast that gives you a good introduction: 

		screencast.com/t/hw8YBm0tjBi

	Follow the screen cast url listed in the next section... for more indepth information.

	All the best,
	- Ernst

2) SCREEN CASTS

    Playlist url:
 
	screencast.com/users/etangreal/playlists/WSN-Project-Playlist

    PLAYLIST:

	00 wsn-file-structure		//The same file structure as listed by 3 (with explanations)
	01 simulation-demo			//Omnet Simulation Demo of the WSN
	02 file-NED,INI				//Detailed explanation of what the .ned & .ini files are for
	03 file-msg					//Detailed explanation of the msg.msg file
	04 file-cmdNhbrTbl1			//Detailed explanation about the CmdNhbrTbl class
	05 file-cmdNhbrTbl2			//continues CmdNhbrTbl class explanation …
	06 file-cmdNhbrTbl3			//continues CmdNhbrTbl class explanation …
	07 file-cmdNhbrTbl4-statsPacketTracker	//ends CmdNhbrTbl & explains statsPacketTraker
	08 file-base1				//Detailed explanation on the Base class
	09 file-base2-node			//Detailed explanation on the Base & Node classes

    (Please follow the link given above to view the complete playlist....)
    [Each video is between 3 and 5 mins long: roughly 45min in toltal]

3) FILE STRUCTURE

    <WSN-PROJECT-ROOT>
	/doc				//Documentation Folder

	base.cc				//Basestation code file
	cmdNhbrTbl.cc
	cmdNhbrTbl.h		//Command/Neighborhood Table
	msg.msg				//Network message definition
	node.cc				//Node code file
	omnetpp.ini			//network settings
	statPacketTracker.cc
	statPacketTracker.h	//tracks stats packets
	wsn.ned				//Omnet's network definition

	Makefile			//Omnet-generated makefile
	msg_m.cc
	msg_m.h				//Omnet-generated message

	/out

	wsn 				//Project Executable

 COMMANDS

   <from-terminal:> opp_msgc msg.msg	//generates the msg_m.cc and msg_m.h files
   <from-terminal:> opp_makemake		//generates the Makefile

4) FUTURE POSSIBLE WORK - Transaction Management using a Distributed 2-Phase-Commit

 2-Phase-Commit Algorithm Description:

 *******************************************************************************************************/
 *
 * ------------ PHASE 1 --------------------------------------------------------------------------------
 * 
 * PROPOSE	-> 	When a COMMAND is first issued (or a failed COMMAND re-issued) it 
 *			must be sent as COMMAND+PROPOSE (by the BASESTATION) 
 * 				(i.e. the BASESTATION is proposing the command the node-network).
 * 				{note: we need to distingish between versions of the COMMAND being resent... 
 * 					therefor each COMMAND should have a UNIQUE-NUMBER(i.e: BALLOT-NUMBER) 
 *					which is incremented every time the BASESTATION re-sends the command. 
 * 					NODES only (re)accept commands if they have a higher UNIQUE-NUMBER(BALLOT-NUMBER) 
 *					than what they have previously stored)
 * 				NODES, apon recieving the COMMAND, store it in there COMMAND-TABLE tagged as 
 *				COMMAND+PROPOSE(the NODE is now waiting to hear from enough of its neighbors 
 *				inorder to change status to ACCEPTED).
 * 				NODES will eventually either mark a command(stored in their COMMAND-TABLE) as 
 *				either: [ ACCEPTED | REJECTED ]
 * 			 	A NODE marks a COMMAND as: 
 * 					1) COMMAND+ACCEPTED when the node "hears" a certain x% (x-percentage) 
 *						of its neighboring NODES (re)send the COMMAND+PROPOSE
 * 				or 	2) COMMAND+REJECTED after the COMMAND-TIMEOUT occurs. In which case 
 *						it also broadcasts the COMMAND+REJECTED which is then propagated 
 *						by all other nodes.
 * 
 * ACCEPTED	->	The NODE marks the COMMAND as ACCEPTED after hearing enough - a certain x% (x-percentage) 
 *			 of - its neighbors "echo/resending" the COMMAND+PROPOSE.
 * 				The COMMAND+PROPOSE is REJECTED or DROPPED after either:
 * 					1) a timer is fired due to a COMMAND'S TIMEOUT-PERIOD expiring, it is then 
 *					   marked as COMMAND+REJECTED
 * 				or  2) when it is older than the TIMEOUT-PERIOD and a newer COMMAND requires space 
 *					in the COMMAND-TABLE, it is consequently replaced by the new COMMAND.
 * 
 * ------------ PHASE 2 -----------------------------------------------------------------------------------
 * 
 * COMMIT	->	After a COMMAND is ACCEPTED (i.e: the BASESTATION heard the NODES(within its range) 
 *					rebroadcast the COMMAND+PROPOSE and did not hear of any NODE broadcasting 
 *					COMMAND+REJECTED for that COMMAND before its TIMEOUT-PERIOD expired).
 * 					the BASESTATION can send out the same COMMAND+COMMIT.
 * 				NODEs record COMMAND+COMMIT by following the same procedure as when recieving 
 *					COMMAND+PROPOSE with the following differences:
 * 					If a NODE receives an COMMAND+COMMIT without having previously having heard 
 *					the COMMAND+PROPOSE it will accept the COMMAND+COMMIT "skipping over" the 
 *					COMMAND+PROPOSE phase.
 * 					The assumption being that the COMMAND+PROPOSED successfully propogated with 
 *					a majority of the NODES, the x% (x-percentage), having ACCEPTED the COMMAND.
 * 
 * COMMITING ->	A NODE will tag the COMMAND as COMMITING after hearing enough neighbors "echo/resending" 
 *				the COMMAND+COMMIT(the same process as COMMAND+APPLIED).
 * 				This is a tempory state in which the NODE will wait for a defined TIMEOUT-PERIOD 
 *				before finally executing the command. 
 * 				The idea is that the NODE waits for a while to see if other NODE sends out a 
 *				REJECTED message. 
 * 
 * COMMITTED ->	A NODE will mark a COMMAND+COMMITING as COMMAND+COMMITTED only after the COMMAND marked 
 *				COMMITING has waited for the TIMEOUT-PERIOD to expire.
 * 
 * ------------ PHASE 1 or PHASE 2 {REJECTED COMMANDS} ----------------------------------------------------
 * 
 * REJECTED	->	The BASESTATION or any NODE can send out a COMMAND+REJECTED in order to cancel a COMMAND.
 * 				Any NODE that has a COMMAND either stored as PROPOSED,ACCEPTED, COMMIT or COMMITING 
				(and waiting for the TIMEOUT-PERIOD) can be CANCELLED. 
 * 				In the case of APPLY, ACCEPTED and APPLYING
 * 					the NODE marks COMMAND will be marked in the table as REJECT
 * 					and the message COMMAND+REJECTED is broadcasted to neighbors.
 * 				command before having APPLIED the command it will cancel the ACCEPTED/APPLYING status.
 * 				Note: once a command is rejected it will always be rejected.
 * 
 * Note: - which if occurs effectively allows the node to eventually override the COMMAND
 *
 ***************************************************************************************************************/
