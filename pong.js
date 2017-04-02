document.getElementById("canvas").style.background = 'rgb(0, 0, 0)';
var ctx = $("#canvas")[0].getContext("2d"); //unsure but necessary for drawing
ctx.fillStyle = 'rgb(255, 255, 255)';
const CANVAS_HEIGHT_MIN = 0; //location of the origin
const CANVAS_WIDTH_MIN = 0; //location of the origin
const CANVAS_HEIGHT = $("canvas").height(); //height of the game canvas
const CANVAS_WIDTH = $("canvas").width(); //width of game canvas
var upPressed = false; //up key is not being pressed
var downPressed = false; //down key is not being pressed
var refreshInterval = 0;

var xBall = CANVAS_WIDTH/2; //starting position of the ball in regards to the x axis
var yBall = CANVAS_HEIGHT/2; //starting position of the ball in regards to the y axis
var dxBall = CANVAS_WIDTH/100; //change in location of the ball in regards to the x axis per refresh
var dyBall = CANVAS_HEIGHT/60; //change in location of the ball in regards to the y axis per refresh
const BALL_SIZE = CANVAS_HEIGHT/100; //size of the ball

const PADDLE_HEIGHT = CANVAS_HEIGHT/6; //height of the drawn paddle
const PADDLE_WIDTH = CANVAS_WIDTH/50; //width of drawn paddle
const PADDLE_MOVE_INCREMENT = CANVAS_HEIGHT/60; //how much the paddle moves on each refresh
var xPaddle = CANVAS_WIDTH_MIN; //paddle rests at left edge of canvas
var yPaddle = (CANVAS_HEIGHT/2) - (PADDLE_HEIGHT/2); //paddle starts at halfway down the canvas

var userNumber = 0;

/*Precondition: Page has not been loaded.
**Postcondition: XML object has been created, canvas has been drawn, game has started*/
function initializePage()
{
	if(navigator.appName == "Microsoft Internet Explorer") {
		XMLHttp = new ActiveXObject("Microsoft.XMLHTTP");
	} else {
		XMLHttp = new XMLHttpRequest(); //this else part is entered using chrome
	}
	refreshInterval = setInterval(function(){drawGame()}, 20);
	return refreshInterval;
}

/*Precondition: Set Username button has been clicked
**Postcondition: Username is set and added to the userInfo div*/
function getUsername()
{
	username = document.getElementById('username').value;
	document.getElementById('userInfo').innerHTML = username;
	
	var sendStr = "/cgi-bin/seavera_pongAjax.cgi?" + "&userNumber=" + userNumber + "&yPaddle=" + yPaddle;
	XMLHttp.open("GET", sendStr, true);
	XMLHttp.onreadystatechange=function() {
    	if (XMLHttp.readyState == 4) {
			userNumber = XMLHttp.responseText;;
		}
	}
    
    XMLHttp.send(null);
	document.getElementById('userInfo').innerHTML = userNumber;
	
}

/*Precondition: Game has been started.
**Postcondition: The game canvas has been updated.*/
function drawGame()
{
	clearPaddle(); //clears the current paddle
	drawBall(xBall, yBall, BALL_SIZE); //sets the ball in motion
    if(upPressed) 
	{
        yPaddle -= PADDLE_MOVE_INCREMENT; //move the paddle up
    } 
	else if(downPressed) 
	{
        yPaddle += PADDLE_MOVE_INCREMENT; //move the paddle down
    }
    drawPaddle(xPaddle, yPaddle, PADDLE_WIDTH, PADDLE_HEIGHT);
    if (yBall + dyBall > CANVAS_HEIGHT || yBall + dyBall < CANVAS_HEIGHT_MIN) //if ball hits top or bottom of canvas
	{
		dyBall = -dyBall;
    }
    if (xBall + dxBall > CANVAS_WIDTH) //if ball hits right side of the canvas
	{
		dxBall = -dxBall;
	}
    else if (xBall + dxBall < CANVAS_WIDTH_MIN + PADDLE_WIDTH) //if ball hits left side of the canvas
	{
		if (yBall > yPaddle && yBall < yPaddle + PADDLE_HEIGHT) //if the ball hits the paddle
		{
			dxBall = -dxBall;
        } 
		else //if the ball does not hit the paddle
		{
			clearInterval(refreshInterval);
        }
    }
    xBall += dxBall; //new changes in ball position
    yBall += dyBall;
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

window.addEventListener("keydown", function(e) {
    // space and arrow keys
    if([32, 37, 38, 39, 40].indexOf(e.keyCode) > -1) {
        e.preventDefault();
    }
}, false);