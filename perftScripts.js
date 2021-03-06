const error = document.getElementById("error");

document.getElementById("back").onclick = () => window.location.replace("./index.html");

console.clear();

var shown = false;
var running = false;

document.getElementById("startButton").onclick = () => {
	if (running)
		return;

	let fen = document.getElementById("fenInput").value;
	let rawDepth = document.getElementById("depthInput").value;

	if (fen == "") {
		error.textContent = "Enter a FEN";
		clear();
		return;
	}

	if (rawDepth == "") {
		error.textContent = "Specify a depth";
		clear();
		return;
	}

	let depth = parseInt(rawDepth);
	if (depth <= 0) {
		error.textContent = "Depth cannot be less than 0";
		clear();
		return;
	}
	if (depth > 9) {
		error.textContent = "Depth cannot be larger than 9";
		clear();
		return;
	}

	if (!validFEN(fen)) {
		error.textContent = "Invalid FEN";
		clear();
		return;
	}

	error.textContent = "";
	clear();

	document.getElementById("count").textContent = "Working...";

	const worker = new Worker("./perftWorker.js");
	running = true;
	worker.postMessage([fen, depth]);
	worker.onmessage = e => {
		running = false;
		document.getElementById("count").textContent = `Total nodes: ${e.data[0]}`;
		document.getElementById("time").textContent = `Time taken: ${e.data[1]}ms`;
		document.getElementById("nps").textContent = `Nodes per second: ${Math.round(e.data[0] / (e.data[1]/1000))}`;

		let breakdown = e.data[2];

		let toggle = document.createElement("button");
		toggle.id = "toggleButton";
		toggle.textContent = "Show breakdown";
		toggle.onclick = () => {
			if (shown) {
				shown = false;
				toggle.textContent = "Show breakdown";
				document.getElementById("breakdown").className = "hidden";
			}
			else {
				shown = true;
				toggle.textContent = "Hide breakdown";
				document.getElementById("breakdown").className = "";
			}
		}

		document.getElementById("result").appendChild(toggle);

		let list = document.createElement("div");
		list.id = "breakdown";
		list.className = "hidden";
		for (let entry of breakdown) {
			let span = document.createElement("span");
			span.textContent = `${entry[0]}: ${entry[1]}`;
			list.appendChild(document.createElement("br"));
			list.appendChild(span);
		}
		document.getElementById("result").appendChild(list);
	}
}

function clear() {
	document.getElementById("count").textContent = "";
	document.getElementById("time").textContent = "";
	document.getElementById("nps").textContent = "";
	document.getElementById("toggleButton")?.remove();
	document.getElementById("breakdown")?.remove();
}

