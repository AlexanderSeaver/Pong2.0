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
#include <cstdlib>
#include <ctime>
#include "fifo.h"
using namespace std;

int main()
{
	//game state information
	const string GAME_STATE_PRE = "-1"; //while setting usernumbers
	const string GAME_STATE_INPLAY = "1"; //while playing the game
	const string GAME_STATE_POST = "2"; //after the game ends
	string gameState = GAME_STATE_PRE; //initialize game state to not in play
	int numberOfUsers = 0; //initialize the system with 0 users
	string canvasWidth; //string to track canvas width
	string canvasHeight; //string to track canvas height
	int canvasWidthInt; //int to track canvas width
	int canvasHeightInt; //int to track canvas height
	string xBallStr; //string position of the ball on the x axis
    string yBallStr; //string position of the ball on the y axis
	int xBall; //int position of the ball on the x axis
	int yBall; //int position of the ball on the y axis
	int dxBall; //change in the x position of the ball
	int dyBall; //change in the y position of the ball
	int paddleHeight; //height of the drawn paddle
	int paddleWidth; //width of the drawn paddle
	string p1Paddle; //string to track user one's paddle
	string p2Paddle; //string to track user two's paddle
	int p1PaddleInt; //int to track user one's paddle
	int p2PaddleInt; //int to track user two's paddle
	string userNo; //string to track usernumber
	
	srand(clock());
	
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
	cout << "pongServer Start" << endl;
	while (1)
	{
		while (gameState == GAME_STATE_PRE)
		{
			uNumFifo_AjaxToServer.openread();
			string userRec = uNumFifo_AjaxToServer.recv(); //Expected recieve string: "!usernumber@canvaswidth#canvasheight$"
			uNumFifo_AjaxToServer.fifoclose();
			userNo = userRec.substr(userRec.find("!") + 1, userRec.find("@") - (userRec.find("!")+1)); //obtain usernumber as tracked by the javascript
			canvasWidth = userRec.substr(userRec.find("@")+1, userRec.find("#") - (userRec.find("@")+1)); //obtain canvas width as tracked by the javascript
			canvasHeight = userRec.substr(userRec.find("#")+1, userRec.find("$") - (userRec.find("#")+1)); //obtain canvas height as tracked by the javascript
			cout << "Canvas Width: " << canvasWidth << endl;
			cout << "Canvas Height: " << canvasHeight << endl;
			if (numberOfUsers < 2) //not enough users are connected
			{
				numberOfUsers++;
				uNumFifo_ServerToAjax.openwrite();
				uNumFifo_ServerToAjax.send(to_string(numberOfUsers)); //change the assigned usernumber of the current user
				uNumFifo_ServerToAjax.fifoclose();
				if (numberOfUsers == 2) //set all of the parameters in preperation for game start
				{
					canvasWidthInt = stoi(canvasWidth, nullptr, 10);
					canvasHeightInt = stoi(canvasHeight, nullptr, 10);
					paddleHeight = canvasHeightInt/6;
					paddleWidth = canvasWidthInt/50;
					int paddleStartPosition = (canvasWidthInt/2) + (paddleHeight/2);
					p1Paddle = to_string(paddleStartPosition);
					p2Paddle = p1Paddle;
					xBall = canvasWidthInt/2;
					yBall = canvasHeightInt/2;
					dxBall = canvasWidthInt/50; //changed from 50
					dyBall = canvasHeightInt/(rand()%101 + 30);
					xBallStr = to_string(xBall);
					yBallStr = to_string(yBall);
					gameState = GAME_STATE_INPLAY;
				}
			}
			cout << "Number of Users: " << numberOfUsers << endl;
			cout << "Game State: " << gameState << endl;
		}
	
		while (gameState == GAME_STATE_INPLAY)
		{	
			cout << "Start of loop" << endl;
			paddleFifo_AjaxToServer.openread();
			paddleRec = paddleFifo_AjaxToServer.recv(); //Expected recieve string: "!usernumber@paddleposition#
			paddleFifo_AjaxToServer.fifoclose();
			string userNo = paddleRec.substr(paddleRec.find("!") + 1, paddleRec.find("@") - (paddleRec.find("!")+1)); //obtain usernumber as tracked by the javascript
			string paddlePos = paddleRec.substr(paddleRec.find("@") + 1, paddleRec.find("#") - (paddleRec.find("@")+1)); //obtain paddle position as tracked by the javascript
			cout << userNo << endl;
			cout << paddlePos << endl;
		
			if(userNo == "1")
			{
				p1Paddle = paddlePos;
				paddleFifo_ServerToAjax.openwrite();
				paddleFifo_ServerToAjax.send("!" + p2Paddle + "@" + xBallStr + "#" +yBallStr + "$");
				paddleFifo_ServerToAjax.fifoclose();
				p1PaddleInt = stoi(p1Paddle, nullptr, 10);
				cout << "User 1 Paddle: " << p1PaddleInt << " xBall: " << xBall << " yBall: " << yBall << endl;
			}
			else
			{
				p2Paddle = paddlePos;
				paddleFifo_ServerToAjax.openwrite();
				paddleFifo_ServerToAjax.send("!" + p1Paddle + "@" + xBallStr + "#" +yBallStr + "$");
				paddleFifo_ServerToAjax.fifoclose();
				p2PaddleInt = stoi(p2Paddle, nullptr, 10);
				cout << "User 2 Paddle: " << p1PaddleInt << " xBall: " << xBall << " yBall: " << yBall << endl;
			}
			if ((yBall + dyBall) >= canvasHeightInt || (yBall + dyBall) <= 0) //Ball hits top or bottom of canvas
			{
				dyBall = -dyBall;
			}
			if ( (xBall + dxBall) >= (canvasWidthInt - paddleWidth)) //ball hits right side of canvas
			{
				if ((yBall >= p2PaddleInt) && (yBall <= (p2PaddleInt + paddleHeight))) //ball is hit
				{
					dxBall = -dxBall;
				}
				else //ball is not hit
				{
					gameState = GAME_STATE_POST;
				}
			}
			if ( (xBall + dxBall) <= (paddleWidth)) //ball hits left side of canvas
			{
				if ((yBall >= p1PaddleInt) && (yBall <= (p1PaddleInt + paddleHeight))) //ball is hit
				{
					dxBall = -dxBall;
				}
				else //ball is not hit
				{
					gameState = GAME_STATE_POST;
				}
			}
			xBall += dxBall; //change x position of ball
			yBall += dyBall; //change y position of ball
			xBallStr = to_string(xBall);
			yBallStr = to_string(yBall);
			cout << "End of loop" << endl;
		}
		while (gameState == GAME_STATE_POST)
		{
			cout << "Start of game state post" << endl;
			paddleFifo_AjaxToServer.openread();
			paddleRec = paddleFifo_AjaxToServer.recv(); //Expected recieve string: "!usernumber@paddleposition#
			paddleFifo_AjaxToServer.fifoclose();
			string send = "!250@300#300$";
			paddleFifo_ServerToAjax.openwrite();
			paddleFifo_ServerToAjax.send(send);
			paddleFifo_ServerToAjax.fifoclose();
			cout << send << endl;
			cout << "End of game state post" << endl;
		}
	}
	
	return 0;
}