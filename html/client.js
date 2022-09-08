let logField;
let urlField;
let formField;

window.onload = () => init();

function init() {
    logField  = document.getElementById('event-log');
    urlField  = document.getElementById('url');
    playerId = document.getElementById('player_id');
    formField = document.forms.moving.elements;

    setInterval(onEvents, 1000);
}

async function onJoin() {
    let url = `${urlField.value}/join` + (playerId.value.length > 0 ? `/${playerId.value}` : "");
    fetch(url,
    {
        method: "GET",
        // mode: 'no-cors',
        headers: {
            'Accept': 'application/json',
            'Access-Control-Allow-Origin': '*'
        }
    })
    .then(response => response.json())
    .then(json => console.log(json))
    // .then(json => playerId.value = json.playerId)
    .catch(function(res){ console.log(res) });


    // let request = new XMLHttpRequest()
    // let url = `${urlField.value}/join`
    // request.open("GET", url);
    // request.setRequestHeader('Access-Control-Allow-Origin', '*');
    // request.setRequestHeader('Access-Control-Allow-Methods', '*');
    // request.send();

    // request.onreadystatechange = () => {
    //     if (this.readyState == 4 && this.readyState == 200) {
    //         console.log(request.responseText);
    //     } else {
    //         addToEventLog(`Failed to "join" with code ${this.readyState}`);
    //     }
    // }
}

async function onMove() {
    let url = `${urlField.value}/move` + (playerId.value.length > 0 ? `/${playerId.value}` : "");
    fetch(url,
    {
        method: "POST",
        mode: 'no-cors',
        headers: {
            'Accept': 'text/plain',
            'Content-Type': 'text/plain',
            'Access-Control-Allow-Origin': '*'
        },
        body: formField.movetype.value
    }).then(function(res){ console.log(res) }).catch(function(res){ console.log(res) });
    // let request = new XMLHttpRequest()
    // let url = `${urlField.value}/move`
    // request.open("POST", url);
    // request.setRequestHeader('Access-Control-Allow-Origin', '*');
    // request.setRequestHeader('Access-Control-Allow-Methods', '*');
    // request.send();

    // request.onreadystatechange = () => {
    //     if (this.readyState == 4 && this.readyState == 200) {
    //         console.log(request.responseText);
    //     } else {
    //         addToEventLog(`Failed to "move" with code ${this.readyState}`);
    //     }
    // }
}

async function onEvents() {
    if (playerId.value.length == 0)
        return;

    let url = `${urlField.value}/events` + (playerId.value.length > 0 ? `/${playerId.value}` : "");
    fetch(url,
    {
        method: "GET",
        mode: 'no-cors',
        headers: {
            'Accept': 'text/plain',
            'Content-Type': 'text/plain',
            'Access-Control-Allow-Origin': '*'
        }
    }).then(function(res){ console.log(res) }).catch(function(res){ console.log(res) });
}

async function onRank() {
    fetch(`${urlField.value}/rank`,
    {
        method: "GET",
        mode: 'no-cors',
        headers: {
            'Accept': 'text/plain',
            'Content-Type': 'text/plain',
            'Access-Control-Allow-Origin': '*'
        }
    }).then(function(res){ console.log(res) }).catch(function(res){ console.log(res) });
}

function addToEventLog(text, severity = 'info') {
    let mostRecentEntry = logField.lastElementChild;
    let entry = document.createElement('li');
    entry.innerText = text;
    entry.className = 'log-' + severity;
    logField.appendChild(entry);
  
    if (mostRecentEntry != null &&
        mostRecentEntry.getBoundingClientRect().top <
            logField.getBoundingClientRect().bottom) {
      entry.scrollIntoView();
    }
  }