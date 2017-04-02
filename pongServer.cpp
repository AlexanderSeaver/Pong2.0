/*************************************************************/
/* Authors: Alex Seaver and Coire Gavin-Hanner               */
/* Date Created: 2-17-17					                 */
/*************************************************************/

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include <string>
#include "fifo.h"
using namespace std;

int main()
{
	//game state information
	const string GAME_STATE_PRE = "-1"; //while setting usernumbers
	const string GAME_STATE_INPLAY = "1"; //while playing the game
	string gameState = GAME_STATE_PRE;
	int numberOfUsers = 0;
	
	//pipe information
	string uNumPipe_ServerToAjax = "uNum_server_to_ajax"; //use this to identify different users
	string uNumPipe_AjaxToServer = "uNum_ajax_to_server";
	string paddlePipe_ServerToAjax = "paddle_server_to_ajax";
	string paddlePipe_AjaxToServer = "paddle_ajax_to_server";
	
	// create the FIFOs for communication
	Fifo uNumFifo_ServerToAjax(uNumPipe_ServerToAjax);
	Fifo uNumFifo_AjaxToServer(uNumPipe_AjaxToServer);
	Fifo paddleFifo_AjaxToServer(paddlePipe_AjaxToServer);
	Fifo paddleFifo_ServerToAjax(paddlePipe_ServerToAjax);
	
	//routines
	while (gameState == GAME_STATE_PRE)
	{
		cout << "top of set usernumber" << endl;
		uNumFifo_AjaxToServer.openread();
		cout << "after openread./n";
		string userNo = uNumFifo_AjaxToServer.recv();
		cout << "after userNo set\n";
		cout << userNo << endl;
		uNumFifo_AjaxToServer.fifoclose();
		while (numberOfUsers < 2)
		{
			numberOfUsers++;
			uNumFifo_ServerToAjax.openwrite();
			uNumFifo_ServerToAjax.send(to_string(numberOfUsers));
			uNumFifo_ServerToAjax.fifoclose();
		}		
		gameState = GAME_STATE_INPLAY;
	}
	
	while (gameState == GAME_STATE_INPLAY)
	{
		
	}
	return 0;
}