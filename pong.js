document.getElementById("canvas").style.background = 'rgb(0, 0, 255)';
var ctx = $("#canvas")[0].getContext("2d"); //unsure but necessary for drawing
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
const BALL_SIZE = 10; //size of the ball

const PADDLE_HEIGHT = 70; //height of the drawn paddle
const PADDLE_WIDTH = 10; //width of drawn paddle
const PADDLE_MOVE_INCREMENT = 5; //how much the paddle moves on each refresh
var xPaddle = CANVAS_WIDTH_MIN; //paddle rests at left edge of canvas
var yPaddle = 150 - (PADDLE_HEIGHT/2); //paddle starts at halfway down the canvas

/*Precondition: Page has not been loaded.
**Postcondition: XML object has been created, canvas has been drawn.*/
function initializePage()
{
	//XMLHttp = new XMLHttpRequest();
	refreshInterval = setInterval(function(){drawGame()}, 20);
	return refreshInterval;
}

/*Precondition: Game has been started.
**Postcondition: The game canvas has been updated.*/
function drawGame()
{
	clearPaddle();
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
    if (yBall + dyBall > CANVAS_HEIGHT || yBall + dyBall < CANVAS_HEIGHT_MIN) 
	{
		dyBall = -dyBall;
    }
    if (xBall + dxBall > CANVAS_WIDTH)
	{
		dxBall = -dxBall;
	}
    else if (xBall + dxBall < CANVAS_WIDTH_MIN + PADDLE_WIDTH)
	{
		if (yBall > yPaddle && yBall < yPaddle + PADDLE_HEIGHT)
		{
			dxBall = -dxBall;
        } 
		else
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

initializePage();