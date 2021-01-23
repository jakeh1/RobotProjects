
class SocketController{
  constructor(){
    this.driveTrianSocket = null;
    this.cameraSocket = null;
    this.setUp = false;
  }
  activateSockets(url){

    this.driveTrianSocket = new WebSocket(url);

    driveTrianSocket.onopen = function(e) {
      console.log("Connection established");
      driveTrianSocket.send("[]");
    };
    
    driveTrianSocket.onmessage = function(event) {
      console.log(`received message from server: ${event.data}`);
    };
    
    driveTrianSocket.onclose = function(event) {
      if (event.wasClean) {
        console.log(`Connection closed, code=${event.code} reason=${event.reason}`);
      } else {
        alert('Connection brocken');
      }
    };
    
    driveTrianSocket.onerror = function(error) {
      alert(`<error> ${error.message}`);
    };
  }
}


var wIsDown = false;
var aIsDown = false;
var sIsDown = false;
var dIsDown = false;
var shiftIsDown = false;
var lastEvent = ""
var activate = false;
//var cameraSocket = new WebSocket("ws://localhost:8000");
//var driveTrianSocket = new WebSocket("ws://localhost:8001"); //this will not conect to anything on puropse will 
var socketController = new SocketController();





//keys are pressed down
document.addEventListener("keydown", function(event){
  if(event.keyCode == 87){
    wIsDown = true;
    output();
  }
  else if(event.keyCode == 65){
   aIsDown = true;
   output();
  }
  else if(event.keyCode == 83){
    sIsDown = true;
    output();
  }
  else if(event.keyCode == 68){
    dIsDown = true;
    output();
  }
  else if(event.keyCode == 16){
    shiftIsDown = true;
    output();
  }
  
})

//keys are up
document.addEventListener("keyup", function(event){
  if(event.keyCode == 87){
    wIsDown = false;
    output();
  }
  else if(event.keyCode == 65){
   aIsDown = false;
   output();
  }
  else if(event.keyCode == 83){
    sIsDown = false;
    output();
  }
  else if(event.keyCode == 68){
    dIsDown = false;
    output();
  }
  else if(event.keyCode == 16){
    shiftIsDown = false;
    output();
  } 
})


function output(){
  var val = "[";
  if(wIsDown){
    val = val + "w";
  }
  if(aIsDown){
    val = val + "a";
  }
  if(sIsDown){
    val = val + "s";
  }
  if(dIsDown){
    val = val + "d";
  }
  if(shiftIsDown){
    val = val + "f";
  }
  val = val + "]"
  if(!(lastEvent == val)){
    console.log(val);
    lastEvent = val;
    if(socketController.setUp){
      socketController.driveTrianSocket.send(val);
    }
  }
}





