var f = {},
  p = postMessage,
  r = "hasOwnProperty";
onmessage = function (e) {
  var d = e.data,
    i = d.i,
    t = d[r]("t") ? d.t : 0;
  switch (d.n) {
    case "a":
      f[i] = setInterval(function () {
        p(i);
      }, t);
      break;
    case "b":
      if (f[r](i)) {
        clearInterval(f[i]);
        delete f[i];
      }
      break;
    case "c":
      f[i] = setTimeout(function () {
        p(i);
        if (f[r](i)) delete f[i];
      }, t);
      break;
    case "d":
      if (f[r](i)) {
        clearTimeout(f[i]);
        delete f[i];
      }
      break;
  }
};
// A couple helpers to get elements by ID:
function qs(sel) {
  return document.querySelector(sel);
}
function qsa(sel) {
  return document.querySelectorAll(sel);
}
function gebi(sel) {
  return document.getElementById(sel);
}
//function debugFeedback(text)	{ document.getElementById('fb').innerHTML = text;	};
if ("DeviceOrientationEvent" in window) {
  window.addEventListener("deviceorientation", deviceOrientationHandler, true);
} else {
  gebi("logoContainer").innerText = "Device Orientation API not supported.";
}

function deviceOrientationHandler(event) {
  const abg = [event.alpha, event.beta, event.gamma];
  //const tiltLR = event.gamma;
  //const tiltFB = event.beta;
  //const dir = event.alpha;
  gebi("doTiltLR").innerHTML = Math.round(abg[2]);
  gebi("doTiltFB").innerHTML = Math.round(abg[1]);
  gebi("doDirection").innerHTML = Math.round(abg[0]);
  //const char* json = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
  const orientObj = {
    Orient: [abg[0], abg[1], abg[2]]
  };
  const OrientJSON = JSON.stringify(orientObj);
  if (websocket.readystate == 1) websocket.send(OrientJSON);
}
//var logo = document.getElementById("imgLogo");
//logo.style.webkitTransform = "rotate(" + tiltLR + "deg) rotate3d(1,0,0, " + (tiltFB * -1) + "deg)";
//logo.style.MozTransform = "rotate(" + tiltLR + "deg)";
//logo.style.transform = "rotate(" + tiltLR + "deg) rotate3d(1,0,0, " + (tiltFB * -1) + "deg)";

var gateway = `ws://${window.location.hostname}/ws`;
var websocket;
window.addEventListener("load", onLoad);
function onLoad(event) {
  initWebSocket();
  initButton();
}
// WebSocket handling
// ----------------------------------------------------------------------------
function initWebSocket() {
  console.log("Trying to open a WebSocket connection...");
  websocket = new WebSocket(gateway);
  websocket.onopen = onOpen;
  websocket.onclose = onClose;
  websocket.onmessage = onMessage;
}
function onOpen(event) {
  console.log("Connection opened");
  //boo = true;
}
function onClose(event) {
  console.log("Connection closed");
  setTimeout(initWebSocket, 2000);
}
function onMessage(event) {
  console.log("Message rcvd");
  //debugFeedback('rcvd');
  let data = JSON.parse(event.data);
  console.log(data.mode);
  document.getElementById(data.mode).checked = true;
}

// Button handling
// ----------------------------------------------------------------------------
function initButton() {
  gebi("rad").addEventListener("click", onToggle);
  gebi("dec").addEventListener("click", onToggle);
  gebi("spkl").addEventListener("click", onToggle);
}

function onToggle(event) {
  //debugFeedback(this.name+':'+this.id);
  const myobj = { act: this.id };
  const myJSON = JSON.stringify(myobj);
  if (websocket.readystate == 1) websocket.send(myJSON);
}
