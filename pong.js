document.getElementById("canvas").style.background = 'rgb(0, 0, 0)';
var ctx = $("#canvas")[0].getContext("2d"); //unsure but necessary for drawing
ctx.fillStyle = 'rgb(255, 255, 255)';
const CANVAS_HEIGHT_MIN = 0; //location of the origin
const CANVAS_WIDTH_MIN = 0; //location of the origin
const CANVAS_HEIGHT = Math.round($("canvas").height()); //height of the game canvas
const CANVAS_WIDTH = Math.round($("canvas").width()); //width of game canvas
var upPressed = false; //up key is not being pressed
var downPressed = false; //down key is not being pressed
var refreshInterval = 0;

var xBall = Math.round(CANVAS_WIDTH/2); //starting position of the ball in regards to the x axis
var xBallTemp;
var yBall = Math.round(CANVAS_HEIGHT/2); //starting position of the ball in regards to the y axis
var dxBall = Math.round(CANVAS_WIDTH/100); //change in location of the ball in regards to the x axis per refresh
var dyBall = Math.round(CANVAS_HEIGHT/60); //change in location of the ball in regards to the y axis per refresh
var xBallMirror = Math.round(CANVAS_WIDTH/2);
const BALL_SIZE = Math.round(CANVAS_HEIGHT/100); //size of the ball

const PADDLE_HEIGHT = Math.round(CANVAS_HEIGHT/6); //height of the drawn paddle
const PADDLE_WIDTH = Math.round(CANVAS_WIDTH/50); //width of drawn paddle
const PADDLE_MOVE_INCREMENT = parseInt(Math.round(CANVAS_HEIGHT/60)); //how much the paddle moves on each refresh
var xPaddle1 = CANVAS_WIDTH_MIN; //paddle rests at left edge of canvas
var yPaddle1 = Math.round((CANVAS_HEIGHT/2) - (PADDLE_HEIGHT/2)); //paddle starts at halfway down the canvas
var xPaddle2 = Math.round(CANVAS_WIDTH - PADDLE_WIDTH); //paddle rests at left edge of canvas
var yPaddle2 = Math.round(Math.round((CANVAS_HEIGHT/2)) - Math.round((PADDLE_HEIGHT/2))); //paddle starts at halfway down the canvas

var userNumber = 0;
var padRec = "initialized padRec";

var safeguard = false;

function initializeXMLHttp()
{
	if(navigator.appName == "Microsoft Internet Explorer") {
		XMLHttp = new ActiveXObject("Microsoft.XMLHTTP");
	} else {
		XMLHttp = new XMLHttpRequest(); //this else part is entered using chrome
	}
}

/*Precondition: Page has not been loaded.
**Postcondition: XML object has been created, canvas has been drawn, game has started*/
function initializePage()
{
	refreshInterval = setInterval(function(){drawGame()}, 500);
	return refreshInterval;
}

/*Precondition: Set Username button has been clicked
**Postcondition: Username is set and added to the userInfo div*/
function getUsername()
{
	if(safeguard)
	{
		console.log("Safeguard is True");
		return;
	}
	console.log(safeguard);
	safeguard = true;
	console.log(safeguard);
	username = document.getElementById('username').value;
	document.getElementById('userInfo').innerHTML = username;
	
	var sendStr = "/cgi-bin/seavera_pongAjax.cgi?" + "&userNumber=" + userNumber + "&CANVAS_WIDTH=" + CANVAS_WIDTH + "&CANVAS_HEIGHT=" + CANVAS_HEIGHT;
	XMLHttp.open("GET", sendStr, true);
	XMLHttp.onreadystatechange=function() {
    	if (XMLHttp.readyState == 4) {
			userNumber = XMLHttp.responseText;;
			document.getElementById('userInfo').innerHTML = userNumber;
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
    
	if(safeguard)
	{
		console.log("Safeguard is True");
		return;
	}
	safeguard = true;
	var sendStr = "/cgi-bin/seavera_pongAjax.cgi?" + "&userNumber=" + userNumber + "&yPaddle1=" + yPaddle1;
	document.getElementById('userInfo').innerHTML = yPaddle1;
	XMLHttp.open("GET", sendStr, true);
	XMLHttp.onreadystatechange=function() {
    	if (XMLHttp.readyState == 4) {
			padRec = XMLHttp.responseText;
			//document.getElementById('userInfo').innerHTML = padRec;
			if (userNumber == "1")
			{
				yPaddle2 = padRec.slice(padRec.indexOf("!")+1, padRec.indexOf("@"));
				xBall = padRec.slice(padRec.indexOf("@") +1, padRec.indexOf("#"));
				yBall = padRec.slice(padRec.indexOf("#") +1, padRec.indexOf("$"));
			}
			else if (userNumber == "2")
			{
				yPaddle2 = padRec.slice(padRec.indexOf("!")+1, padRec.indexOf("@"));
				xBallTemp = padRec.slice(padRec.indexOf("@") +1, padRec.indexOf("#"));
				
				dxBall = xBallMirror - xBallTemp;
				xBall += dxBall;
				xBallMirror = xBallTemp;
				yBall = padRec.slice(padRec.indexOf("#") +1, padRec.indexOf("$"));
			}
			clearPaddle();
			drawPaddle(xPaddle1, yPaddle1, PADDLE_WIDTH, PADDLE_HEIGHT);
			drawPaddle(xPaddle2, yPaddle2, PADDLE_WIDTH, PADDLE_HEIGHT);
			drawBall(xBall, yBall, BALL_SIZE);
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