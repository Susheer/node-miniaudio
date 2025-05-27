const assert = require('assert/strict');
const { test, beforeEach } = require('node:test');
const path = require('path');

let addon;
let filePath;
beforeEach(() => {
  addon = require('../lib');
  filePath = path.join(__dirname, "output.wav");
});
  
test('Ensure addon is loaded correctly', (t) => {
    assert.ok(addon, 'Addon should be loaded');
    assert.strictEqual(typeof addon, 'object', 'Addon should be an object');
    // Add checks for specific functions if needed
    assert.strictEqual(typeof addon.playAudio, 'function', 'Addon should have playAudio');
  });