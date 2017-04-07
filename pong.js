/*
Pong 2.0 Javascript
Authors: Alexander Seaver and Coire Gavin-Hanner
Function: Contains functions to pass game information to remote
server and draw the game based on the recieved information
from the server.
Last Modified: April 04, 2017
*/

document.getElementById("canvas").style.background = 'rgb(0, 0, 0)'; //set canvas color to black
var ctx = $("#canvas")[0].getContext("2d"); //unsure but necessary for drawing
ctx.fillStyle = 'rgb(255, 255, 255)'; //sets the fill color for ball and paddles to white
const CANVAS_HEIGHT_MIN = 0; //location of the origin
const CANVAS_WIDTH_MIN = 0; //location of the origin
const CANVAS_HEIGHT = Math.round($("canvas").height()); //height of the game canvas
const CANVAS_WIDTH = Math.round($("canvas").width()); //width of game canvas

var xBall = Math.round(CANVAS_WIDTH/2); //starting position of the ball in regards to the x axis
var xBallTemp; //used as a temp storage in mirroring the canvas for user 2
var yBall = Math.round(CANVAS_HEIGHT/2); //starting position of the ball in regards to the y axis
var dxBall = Math.round(CANVAS_WIDTH/30); //change in location of the ball in regards to the x axis per refresh
var dyBall = Math.round(CANVAS_HEIGHT/60); //change in location of the ball in regards to the y axis per refresh
var xBallMirror = Math.round(CANVAS_WIDTH/2); //used in mirroring the canvas for user 2
const BALL_SIZE = Math.round(CANVAS_HEIGHT/100); //size of the ball

const PADDLE_HEIGHT = Math.round(CANVAS_HEIGHT/6); //height of the drawn paddle
const PADDLE_WIDTH = Math.round(CANVAS_WIDTH/50); //width of drawn paddle
const PADDLE_MOVE_INCREMENT = parseInt(Math.round(CANVAS_HEIGHT/50)); //how much the paddle moves on each refresh
var xPaddle1 = CANVAS_WIDTH_MIN; //paddle rests at left edge of canvas
var yPaddle1 = Math.round(Math.round((CANVAS_HEIGHT/2)) + Math.round((PADDLE_HEIGHT/2))); //paddle starts at halfway down the canvas
var xPaddle2 = Math.round(CANVAS_WIDTH - PADDLE_WIDTH); //paddle rests at left edge of canvas
var yPaddle2 = Math.round(Math.round((CANVAS_HEIGHT/2)) + Math.round((PADDLE_HEIGHT/2))); //paddle starts at halfway down the canvas

var userNumber = 0; //starts the script with 0 active users
var padRec = "initialized padRec"; //string to hold the recieved paddle and ball coordinates from the server
var upPressed = false; //up key is not being pressed
var downPressed = false; //down key is not being pressed
var refreshInterval = 0; //used to hold the timer ID
const CLOCK_MS = 150; //the millisecond value at which the game refreshes
var safeguard = false; //boolean to track active cgi requests;

/*Precondition: Page has not loaded.
**Postcondition: XML object has been created according to user browser.*/
function initializeXMLHttp()
{
	if(navigator.appName == "Microsoft Internet Explorer") 
	{
		XMLHttp = new ActiveXObject("Microsoft.XMLHTTP"); //if the user is using IE
	} 
	else 
	{
		XMLHttp = new XMLHttpRequest(); //this part is entered using any browser other than IE
	}
}

/*Precondition: Page has not been loaded.
**Postcondition: Game has started and is refreshing at the CLOCK_MS value.*/
function startGame()
{
	refreshInterval = setInterval(function(){drawGame()}, CLOCK_MS);
	return refreshInterval;
}

/*Precondition: Set Username button has been clicked
**Postcondition: Username is set and added to the userInfo div*/
function connectToServer()
{
	if(safeguard) //prevents a new cgi from being created if there is already an active cgi.
	{
		console.log("Safeguard is True");
		return;
	}
	safeguard = true;
	
	var sendStr = "/cgi-bin/gavinhannerc_pongAjax.cgi?" + "&userNumber=" + userNumber + "&CANVAS_WIDTH=" + CANVAS_WIDTH + "&CANVAS_HEIGHT=" + CANVAS_HEIGHT;
	XMLHttp.open("GET", sendStr, true);
	
	XMLHttp.onreadystatechange=function() //check for response from the cgi
	{
    	if (XMLHttp.readyState == 4) //the cgi has sent a response
		{
			userNumber = XMLHttp.responseText; //update the userNumber to that assigned by the server
			safeguard = false;
		}
	}
	
    XMLHttp.send(null);
}

/*Precondition: Game has been started.
**Postcondition: The game canvas has been updated.*/
function drawGame()
{
	if(upPressed) 
	{
        yPaddle1 = (yPaddle1 - PADDLE_MOVE_INCREMENT); //move the paddle up
    } 
	else if(downPressed) 
	{
        yPaddle1 = (parseInt(yPaddle1) + parseInt(PADDLE_MOVE_INCREMENT)); //move the paddle down
    }
    
	if(safeguard) //prevents a new cgi from being created if there is already an active cgi.
	{
		console.log("Safeguard is True");
		return;
	}
	safeguard = true;
	var sendStr = "/cgi-bin/gavinhannerc_pongAjax.cgi?" + "&userNumber=" + userNumber + "&yPaddle1=" + yPaddle1;
	XMLHttp.open("GET", sendStr, true);
	XMLHttp.onreadystatechange=function() //check for response from the cgi
	{
    	if (XMLHttp.readyState == 4) //the cgi has sent a response
		{
			padRec = XMLHttp.responseText; //Expected recieve string: "!rightPaddleYPosition@ballXCoordinate#ballYCoordinate$" 
			if (userNumber == "1")
			{
				yPaddle2 = padRec.slice(padRec.indexOf("!")+1, padRec.indexOf("@")); //obtain right paddle as assigned by the server
				xBall = padRec.slice(padRec.indexOf("@") +1, padRec.indexOf("#")); //obtain ball position as assigned by the server
				yBall = padRec.slice(padRec.indexOf("#") +1, padRec.indexOf("$")); //obtain ball position as assigned by the server
			}
			else if (userNumber == "2")
			{
				yPaddle2 = padRec.slice(padRec.indexOf("!")+1, padRec.indexOf("@")); //obtain right paddle as assigned by the server
				xBallTemp = padRec.slice(padRec.indexOf("@") +1, padRec.indexOf("#")); //obtain ball position as assigned by the server
				
				dxBall = xBallMirror - xBallTemp; //mirror the direction the ball is traveling
				xBall += dxBall;
				xBallMirror = xBallTemp;
				yBall = padRec.slice(padRec.indexOf("#") +1, padRec.indexOf("$"));
			}
			clearPaddle(); //clear the game canvas
			drawPaddle(xPaddle1, yPaddle1, PADDLE_WIDTH, PADDLE_HEIGHT); //draw left paddle
			drawPaddle(xPaddle2, yPaddle2, PADDLE_WIDTH, PADDLE_HEIGHT); //draw right paddle
			drawBall(xBall, yBall, BALL_SIZE); //draw ball
			safeguard = false;
		}
	}
    
    XMLHttp.send(null);
}

/*Precondition: A function has a need to draw the paddle at a certain position and with certain dimensions.
**Postcondition: The paddle has been drawn at the specified location in the specified dimensions.*/
function drawPaddle(x, y, w, h)
{
	ctx.beginPath();
    ctx.rect(x, y, w, h); 
    ctx.closePath();
    ctx.fill();
}

/*Precondition: A function needs to clear the currently drawn paddle.
**Postcondition: The current paddle has been cleared.*/
function clearPaddle()
{
	ctx.clearRect(CANVAS_WIDTH_MIN, CANVAS_HEIGHT_MIN, CANVAS_WIDTH, CANVAS_HEIGHT);
}

function drawBall (x, y, r)
{
	ctx.beginPath();
    ctx.arc(x, y, r, 0, Math.PI * 2, true);
    ctx.closePath();
    ctx.fill();
}

//Tracks any key presses
$(document).keydown(function(e) { //gives the value of the key when you press down
    if(e.keyCode == 40) {
        downPressed = true;
    } else if(e.keyCode == 38) {
        upPressed = true;
    }
    else if(e.keycode != 40) {
    	downPressed = false;
    }
    else if(e.keycode != 38) {
    	upPressed = false;
    }
});

//Tracks any key releases
$(document).keyup(function(e) { //tells the program that you have stopped pressing the key
    if(e.keyCode == 40) {
        downPressed = false;
    } else if(e.keyCode == 38) {
        upPressed = false;
    }
    else if(e.keycode != 40) {
    	downPressed = false;
    }
    else if(e.keycode != 38) {
    	upPressed = false;
    }
});

//Disables the arrow keys and spacebar from performing their normal function (scrolling and page down respectively)
window.addEventListener("keydown", function(e) {
    // space and arrow keys
    if([32, 37, 38, 39, 40].indexOf(e.keyCode) > -1) {
        e.preventDefault();
    }
}, false);

function getRandomInt(min, max) 
{
  min = Math.ceil(min);
  max = Math.floor(max);
  return Math.floor(Math.random() * (max - min)) + min;
}
