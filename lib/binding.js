const path = require('path');
const os = require('os');
let addon = {}, binary_path;
switch (os.platform()) {
    case 'win32':
        if (os.arch() === 'ia32') {
            binary_path = path.join(__dirname, 'node-miniaudio_win32_ia32.node');
        } else if (os.arch() === 'x64') {
            binary_path = path.join(__dirname, 'node-miniaudio_win32_x64.node');
        }
        addon = require(binary_path);
        break;
    case 'darwin':
        binary_path = path.join(__dirname, 'node-miniaudio.node'); 
        addon = require(binary_path);
        break;
    case 'linux':
        if (os.arch() === 'ia32') {
            addon = path.join(__dirname, 'node-miniaudio.node');
        } else if (os.arch() === 'x64') {
            addon = path.join(__dirname, 'node-miniaudio.node');
        }
        break;
    default:
        binary_path = path.join(__dirname, 'node-miniaudio.node'); 
        addon = require(binary_path);
}

module.exports.playAudio = addon.playAudio;