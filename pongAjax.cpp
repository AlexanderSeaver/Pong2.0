/*************************************************************/
/* Pong2.0 CGI program                                       */
/*************************************************************/

#include <iostream>
// Stuff for AJAX
#include "cgicc/Cgicc.h"
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTMLClasses.h"

//Stuff for pipes
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include "fifo.h"

using namespace std;
using namespace cgicc; // Needed for AJAX functions.

int main()
{
	const string GAME_STATE_PRE = "-1"; //while setting usernumbers
	const string GAME_STATE_INPLAY = "1"; //while playing the game
	
	//pipe information
	string uNumPipe_ServerToAjax = "uNum_server_to_ajax"; //use this to identify different users
	string uNumPipe_AjaxToServer = "uNum_ajax_to_server";
	string paddlePipe_ServerToAjax = "paddle_server_to_ajax";
	string paddlePipe_AjaxToServer = "paddle_ajax_to_server";
		
	string paddleRecFromServer;
	string gameState = GAME_STATE_PRE;
	
	//gets userNumber and paddle position from the javascript
	Cgicc cgiNum;    // Ajax object
	form_iterator userNumber = cgiNum.getElement("userNumber");
	string userno = **userNumber;	
	if (userno != "0") 
	{ 
		gameState = GAME_STATE_INPLAY; 
	}
	Cgicc cgiPaddle;    // Ajax object	
	form_iterator paddley = cgiPaddle.getElement("paddley");
	string paddlePos = **paddley;
	
	cout << "Content-Type: text/plain\n\n";
	
	if(gameState == GAME_STATE_PRE) // obtain the actual user number from the server (1 or 2)
	{
		/*Fifo uNumFifo_ServerToAjax(uNumPipe_ServerToAjax);
		Fifo uNumFifo_AjaxToServer(uNumPipe_AjaxToServer);
		
		uNumFifo_AjaxToServer.openwrite();
		uNumFifo_AjaxToServer.send(userno);
		uNumFifo_AjaxToServer.fifoclose();
	
		uNumFifo_ServerToAjax.openread();
		userno = uNumFifo_ServerToAjax.recv();
		uNumFifo_ServerToAjax.fifoclose();
		cout << userno;	*/
		cout << "5";
	}
	else if(gameState == GAME_STATE_INPLAY)
	{
		
	}
		
	
	return 0;
}