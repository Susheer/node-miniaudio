const audioAddon = require('./lib');
const path = require('path');
const filePath = path.join(__dirname, 'test', "output.wav");


audioAddon.playAudio(filePath, function (response) {
    console.log("miniaudio", response);
});
