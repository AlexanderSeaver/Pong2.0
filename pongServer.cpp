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
	int canvasWidthInt;
	int canvasHeightInt;
	int xBall;
	int yBall;
	int dxBall;
	int dyBall;
	int paddleHeight;
	string p1Paddle;
	string p2Paddle;
	int p1PaddleInt;
	int p2PaddleInt;
	string xBallStr;
    string yBallStr;
	
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
	string paddleRec = "initial paddleRec";
	
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
			if (numberOfUsers == 2)
			{
				canvasWidthInt = stoi(canvasWidth, nullptr, 10);
				canvasHeightInt = stoi(canvasHeight, nullptr, 10);
				paddleHeight = canvasHeightInt/6;
				xBall = canvasWidthInt/2;
				yBall = canvasHeightInt/2;
				dxBall = canvasWidthInt/100;
				dyBall = canvasHeightInt/60;
				xBallStr = to_string(xBall);
				yBallStr = to_string(yBall);
				gameState = GAME_STATE_INPLAY;
				cout << "game state: " << gameState << endl;
			}
		}		

	}
	
	while (gameState == GAME_STATE_INPLAY)
	{	
		cout << "in while(gameState == GAME_STATE_INPLAY) loop" << endl;
		paddleFifo_AjaxToServer.openread();
		cout << "test1" << endl;
		paddleRec = paddleFifo_AjaxToServer.recv();
		cout << "test2" << endl;
		cout << paddleRec << endl;
		cout << "test2.5" << endl;
		string userNo = paddleRec.substr(paddleRec.find("!") + 1, paddleRec.find("@") - (paddleRec.find("!")+1));
		cout << "test3" << endl;
		string paddlePos = paddleRec.substr(paddleRec.find("@") + 1, paddleRec.find("#") - (paddleRec.find("@")+1));
		cout << "test4" << endl;
		cout << userNo << endl;
		cout << "test5" << endl;
		cout << paddlePos << endl;
		cout << "test6" << endl;
		
		if(userNo == "1")
		{
			cout << "test7" << endl;
			p1Paddle = paddlePos;
			cout << "test8" << endl;
			paddleFifo_ServerToAjax.openwrite();
			cout << "test9" << endl;
			paddleFifo_ServerToAjax.send("!" + p1Paddle + "@" + xBallStr + "#" +yBallStr + "$");
			cout << "test10" << endl;
			p1PaddleInt = stoi(p1Paddle, nullptr, 10);
			cout << endl << "User 1: " << p1PaddleInt << "YBall: " << yBall << endl;
			cout << "test11" << endl;
			paddleFifo_AjaxToServer.fifoclose();
    		paddleFifo_ServerToAjax.fifoclose();
		}
		else if(userNo == "2")
		{
			cout << "test12" << endl;
			p2Paddle = paddlePos;
			cout << "test13" << endl;
			paddleFifo_ServerToAjax.openwrite();
			cout << "test14" << endl;
			paddleFifo_ServerToAjax.send("!" + p2Paddle + "@" + xBallStr + "#" +yBallStr + "$");
			cout << "test15" << endl;
			p2PaddleInt = stoi(p2Paddle, nullptr, 10);
			cout << endl << "User 2: " << p2PaddleInt << "YBall: " << yBall << endl;
			cout << "test16" << endl;
			paddleFifo_AjaxToServer.fifoclose();
    		paddleFifo_ServerToAjax.fifoclose();
		}
		cout << "test17" << endl;
		if ((yBall + dyBall) >= canvasHeightInt || (yBall + dyBall) <= 0) 
		{
			cout << "test18" << endl;
    		dyBall = -dyBall;
    		cout << "test19" << endl;
    	}
    	else if ( (xBall + dxBall) >= (canvasWidthInt - canvasWidthInt/50))
    	{
    		cout << "test20" << endl;
    		if ((yBall >= p2PaddleInt) && (yBall <= (p2PaddleInt + paddleHeight))) 
    		{
    			cout << "test21" << endl;
    			dxBall = -dxBall;
    			cout << "test22" << endl;
    		}
    		else 
    		{
    			cout << "test23" << endl;
    			gameState = GAME_STATE_PRE;
    			cout << "test24" << endl;
    		}
    	}
    	else if ( (xBall + dxBall) <= (canvasWidthInt/50)) 
    	{
    		cout << "test25" << endl;
    		if ((yBall >= p1PaddleInt) && (yBall <= (p1PaddleInt + paddleHeight))) 
    		{
    			cout << "test26" << endl;
    			dxBall = -dxBall;
    			cout << "test27" << endl;
    		}
    		else 
    		{
    			cout << "test28" << endl;
    			gameState = GAME_STATE_PRE;
    			cout << "test29" << endl;
    		}
    	}
    	cout << "test30" << endl;
    	xBall += dxBall;
    	cout << "test31" << endl;
    	yBall += dyBall;
    	cout << "test32" << endl;
    	xBallStr = to_string(xBall);
    	cout << "test33" << endl;
    	yBallStr = to_string(yBall);
    	cout << "test34" << endl;
    	

    	
    	
		
	}
	return 0;
}