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
#include <string>
#include "fifo.h"

using namespace std;
using namespace cgicc; // Needed for AJAX functions.

int main()
{
	const string GAME_STATE_PRE = "-1"; //while setting usernumbers
	const string GAME_STATE_INPLAY = "1"; //while playing the game
	
	//pipe information
	string uNumPipe_ServerToAjax = "uNum_server_to_ajax";
	string uNumPipe_AjaxToServer = "uNum_ajax_to_server";
	string paddlePipe_ServerToAjax = "paddle_server_to_ajax";
	string paddlePipe_AjaxToServer = "paddle_ajax_to_server";
	
	string paddleRecFromServer; //string to hold paddle and ball positions as assigned by server
	string gameState = GAME_STATE_PRE; //initialize the game state to not in progress
	
	//gets userNumber and paddle position from the javascript
	Cgicc cgiNum;    // Ajax object
	form_iterator userNumber = cgiNum.getElement("userNumber");
	string userNo = **userNumber; //userNo equal to the user number contained in the javascript
	
	cout << "Content-Type: text/plain\n\n"; //Properly formats the output strings
	
	if (userNo != "0") 
	{ 
		gameState = GAME_STATE_INPLAY; //mark that the game is in play
		Cgicc cgiPaddle; // Ajax object	
		form_iterator yPaddle1 = cgiPaddle.getElement("yPaddle1");
		string paddlePos = **yPaddle1; //paddlePos equal to the left paddle position in the javascript
	
		Fifo paddleFifo_AjaxToServer(paddlePipe_AjaxToServer);
		Fifo paddleFifo_ServerToAjax(paddlePipe_ServerToAjax);
				
		paddleFifo_AjaxToServer.openwrite();
		paddleFifo_AjaxToServer.send("!" + userNo + "@" + paddlePos + "#");

		paddleFifo_ServerToAjax.openread();
		paddleRecFromServer = paddleFifo_ServerToAjax.recv(); //Expected recieve string: "!rightPaddleYPosition@ballXCoordinate#ballYCoordinate$"
		paddleFifo_AjaxToServer.fifoclose();
		paddleFifo_ServerToAjax.fifoclose();
		cout << paddleRecFromServer;
	}
	
	else if(gameState == GAME_STATE_PRE) // obtain the actual user number from the server (1 or 2)
	{
		Cgicc cgiCanvasWidth;
		form_iterator canvasWidthIterator = cgiCanvasWidth.getElement("CANVAS_WIDTH");
		string canvasWidth = **canvasWidthIterator; //canvasWidth equal to width of the canvas
		
		Cgicc cgiCanvasHeight;
		form_iterator canvasHeightIterator = cgiCanvasHeight.getElement("CANVAS_HEIGHT");
		string canvasHeight = **canvasHeightIterator; //canvasHeight equal to the height of the canvas
		
		Fifo uNumFifo_ServerToAjax(uNumPipe_ServerToAjax);
		Fifo uNumFifo_AjaxToServer(uNumPipe_AjaxToServer);
		
		uNumFifo_AjaxToServer.openwrite();
		uNumFifo_AjaxToServer.send("!" + userNo + "@" + canvasWidth + "#" + canvasHeight + "$");
		uNumFifo_ServerToAjax.openread();
		userNo = uNumFifo_ServerToAjax.recv(); //Expected recieve string: "usernumber"
		cout << userNo;
		uNumFifo_ServerToAjax.fifoclose();
		uNumFifo_AjaxToServer.fifoclose();
	}
	
	return 0;
}