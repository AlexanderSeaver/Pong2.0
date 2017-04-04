/*************************************************************/
/* Pong2.0 CGI program                                       */
/*************************************************************/

#include <iostream>
#include <fstream>
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
#include <string>
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
	string userNo = **userNumber;
	
	ofstream logfile;
	
	
	cout << "Content-Type: text/plain\n\n";	
	if (userNo != "0") 
	{ 
		gameState = GAME_STATE_INPLAY;
		Cgicc cgiPaddle;    // Ajax object	
		form_iterator yPaddle1 = cgiPaddle.getElement("yPaddle1");
		string paddlePos = **yPaddle1;
		
		logfile.open("/tmp/pong.log",ios::out|ios::app);
		logfile << "open write (paddlePos: )" << paddlePos << endl;
		logfile.close();
	
		//cout << "!" << "300" << "@" << "300" << "#" << "300" << "$";
		Fifo paddleFifo_AjaxToServer(paddlePipe_AjaxToServer);
		Fifo paddleFifo_ServerToAjax(paddlePipe_ServerToAjax);
				
		paddleFifo_AjaxToServer.openwrite();
		paddleFifo_AjaxToServer.send("!" + userNo + "@" + paddlePos + "#");
		
		logfile.open("/tmp/pong.log",ios::out|ios::app);
		logfile << "!" << userNo << "@" << paddlePos << "#" << endl;
		logfile.close();
		
		paddleFifo_ServerToAjax.openread();
		string padRec = paddleFifo_ServerToAjax.recv();
		cout << padRec;
		paddleFifo_AjaxToServer.fifoclose();
		paddleFifo_ServerToAjax.fifoclose();
	}
	
	else if(gameState == GAME_STATE_PRE) // obtain the actual user number from the server (1 or 2)
	{
		Cgicc cgiCanvasWidth;
		form_iterator canvasWidthIterator = cgiCanvasWidth.getElement("CANVAS_WIDTH");
		string canvasWidth = **canvasWidthIterator;
		
		Cgicc cgiCanvasHeight;
		form_iterator canvasHeightIterator = cgiCanvasHeight.getElement("CANVAS_HEIGHT");
		string canvasHeight = **canvasHeightIterator;
		
		Fifo uNumFifo_ServerToAjax(uNumPipe_ServerToAjax);
		Fifo uNumFifo_AjaxToServer(uNumPipe_AjaxToServer);
		
		uNumFifo_AjaxToServer.openwrite();
		uNumFifo_AjaxToServer.send("!" + userNo + "@" + canvasWidth + "#" + canvasHeight + "$");
		uNumFifo_ServerToAjax.openread();
		userNo = uNumFifo_ServerToAjax.recv();
		uNumFifo_ServerToAjax.fifoclose();
		uNumFifo_AjaxToServer.fifoclose();
		cout << userNo;	
		
	}
	
		
	
	return 0;
}