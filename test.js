// const audioAddon = require('./lib/node-miniaudio.node');
// console.log(audioAddon.playAudio('C:\\addons\\local\\node-miniaudio\\output.wav')); // This should play your audio file


const path = require('path');
const audioAddon = require('./build/Release/node-miniaudio');

const filePath = path.join(__dirname, "output.wav"); // Ensures correct path
console.log(filePath);
audioAddon.playAudio(filePath);
