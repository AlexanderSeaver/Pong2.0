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
	string canvasWidth;
	string canvasHeight;
	int xBall;
	int yBall;
	int dxBall;
	int dyBall;
	string p1Paddle;
	string p2Paddle;
	int p1PaddleInt;
	int p2PaddleInt;
	
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
	
	string userRec = "initial userRec";
	
	while (gameState == GAME_STATE_PRE)
	{
		cout << "top of set usernumber" << endl;
		uNumFifo_AjaxToServer.openread();
		cout << "after openread." << endl;
		string userRec = uNumFifo_AjaxToServer.recv();
		cout << "after userNo set" << endl;
		//cout << userNo << endl;
		uNumFifo_AjaxToServer.fifoclose();
		cout <<"after fifoclose()" << endl;
		cout << numberOfUsers << endl;
		string userNo = userRec.substr(userRec.find("!") + 1, userRec.find("@") - (userRec.find("!")+1));
		cout << "user#: " << userNo << endl;
		canvasWidth = userRec.substr(userRec.find("@")+1, userRec.find("#") - (userRec.find("@")+1));
		cout << "width: " << canvasWidth << endl;
		canvasHeight = userRec.substr(userRec.find("#")+1, userRec.find("$") - (userRec.find("#")+1));
		cout << "height: " << canvasHeight << endl;
		if (numberOfUsers < 2)
		{
			numberOfUsers++;
			cout << "if:" << numberOfUsers << endl;
			uNumFifo_ServerToAjax.openwrite();
			uNumFifo_ServerToAjax.send(to_string(numberOfUsers));
			uNumFifo_ServerToAjax.fifoclose();
		}		
		if (numberOfUsers == 2)
		{
			xBall = canvasWidth/2;
			yBall = canvasHeight/2;
			dxBall = canvasWidth/100;
			dyBall = canvasHeight/60;
			gameState = GAME_STATE_INPLAY;
		}
	}
	
	while (gameState == GAME_STATE_INPLAY)
	{
		if (yBall + dyBall > canvasHeight || yBall + dyBall < 0) {
    		dyBall = -dyBall;
    		}
    	if ( xBall + dxBall > canvasWidth) {
    		if (yBall > p2PaddleInt && yBall < p2PaddleInt + paddleHeight) {
    			dxBall = -dxBall;
    			}
    		else {
    			gameInPlay = false;
    			}
    		}
    	if ( xBall + dxBall < 0) {
    		if (yBall > p1PaddleInt && yBall < p1PaddleInt + paddleHeight) {
    			dxBall = -dxBall;
    			}
    		else {
    			gameInPlay = false;
    			}
    		}
    	xBall += dxBall;
    	yBall += dyBall;
    	xBallStr = to_string(xBall);
    	yBallStr = to_string(yBall);
	}
	return 0;
}