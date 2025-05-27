const audioAddon = require('./lib/node-miniaudio.node');
const path = require('path');
const filePath = path.join(__dirname, "output.wav");


audioAddon.playAudio(filePath, function (response) {
    console.log("miniaudio: ", response);
});
