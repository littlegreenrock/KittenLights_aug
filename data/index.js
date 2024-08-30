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
// Shortened helpers statements to get elements by ID:
function qs(sel) {
	return document.querySelector(sel);
}
function qsa(sel) {
	return document.querySelectorAll(sel);
}
function gebi(sel) {
	return document.getElementById(sel);
}

if (!window.DeviceOrientationEvent) {
	gebi("do-unsupported").classList.remove("hidden");
} else {
	gebi("do-info").classList.remove("hidden");
	window.addEventListener("deviceorientation", function (event) {
		gebi("beta").innerHTML = Math.round(event.beta);
		gebi("gamma").innerHTML = Math.round(event.gamma);
		gebi("alpha").innerHTML = Math.round(event.alpha);
		gebi("is-absolute").innerHTML = event.absolute
			? "true"
			: "false";
	});
}



if (!window.DeviceOrientationEvent) {
	gebi("dm-unsupported").classList.remove("hidden");
} else {
	gebi("dm-info").classList.remove("hidden");
	window.addEventListener("deviceorientation", function (event) {
		let OrientABG = [Math.round(event.alpha), Math.round(event.beta), Math.round(event.gamma)];
		gebi("orientation-pitch-beta").innerHTML = Math.round(event.beta);
		gebi("orientation-roll-gamma").innerHTML = Math.round(event.gamma);
		gebi("orientation-yaw-alpha").innerHTML = Math.round(event.alpha);
		gebi("interval").innerHTML = event.interval;
	});
}

if (!("oncompassneedscalibration" in window)) {
	gebi("cnc-unsupported").classList.remove("hidden");
} else {
	window.addEventListener("compassneedscalibration", function (event) {
		alert("Compass needs calibrating: Wave your device like you're trying to 'click' your wrist bones");
	});
}

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
}
function onClose(event) {
	console.log("Connection closed");
	setTimeout(initWebSocket, 2000);
}
function onMessage(event) {
	console.log("Message rcvd");
	let data = JSON.parse(event.data);
	console.log(data.mode);
	// document.getElementById(data.mode).checked = true;
}

// Button handling
// ----------------------------------------------------------------------------
function initButton() {
	gebi("rad").addEventListener("click", onToggle);
	gebi("dec").addEventListener("click", onToggle);
	gebi("spkl").addEventListener("click", onToggle);
}

function onToggle(event) {
	// const myobj = { act: this.id };
	// const myJSON = JSON.stringify(myobj);
	// if (websocket.readystate == 1) websocket.send(myJSON);
}
