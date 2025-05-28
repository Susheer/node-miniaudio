# node-miniaudio
Lightweight and fast Node.js addon for playing notification sounds, beeps, and alarms using MiniAudio.
# node-miniaudio
Lightweight and fast Node.js addon for playing notification sounds, beeps, and alarms using MiniAudio.
# @thesusheer/node-miniaudio

**Lightweight and fast Node.js addon for playing notification sounds, beeps, and alarms using [MiniAudio](https://miniaud.io).**

[![npm](https://img.shields.io/npm/v/@thesusheer/node-miniaudio.svg)](https://www.npmjs.com/package/@thesusheer/node-miniaudio)
[![License](https://img.shields.io/badge/license-BSD-blue.svg)](#license)
[![Build Status](https://img.shields.io/github/actions/workflow/status/Susheer/node-miniaudio/build.yml?branch=main)](https://github.com/Susheer/node-miniaudio/actions)

---

## Features

- 🔊 Minimal and low-latency audio playback.
- 🎵 Supports common formats like WAV, MP3, and more.
- ⚙️ Native addon built with `node-addon-api` and [MiniAudio](https://miniaud.io).
- 🚀 Prebuilt binaries via `node-pre-gyp`.
- 🧱 Cross-platform compatible (tested on Windows 11).

---

## Installation

```bash
npm install @thesusheer/node-miniaudio
```

If a prebuilt binary isn't available for your platform, it will automatically attempt to build from source.

### Tested Environments

- ✅ Node.js v20.18.2
- ✅ Electron v35.x.x
- ✅ Windows 11

> ❗ Not yet tested on macOS or Linux — contributions and compatibility reports welcome.

---

## Usage

### Basic Example

```js
const { playAudio } = require('@thesusheer/node-miniaudio');

playAudio('./alert.wav', (message) => {
  console.log('Playback complete:', message);
});
```

---

## API

### `playAudio(filePath: string, callback: (message: string) => void): string`

Plays an audio file asynchronously.

#### Parameters:

- `filePath` – Path to the audio file (WAV, MP3, etc.).
- `callback` – Function called after playback ends with a message.

#### Returns:

- A string indicating playback started.

---

## TypeScript Support

Includes built-in `.d.ts` declarations for full autocompletion and type safety:

```ts
import { playAudio, AudioPlaybackCallback } from '@thesusheer/node-miniaudio';

const onDone: AudioPlaybackCallback = (msg) => console.log('Done:', msg);
playAudio('sound.mp3', onDone);
```

---

## Compatibility & Build Notes

This addon uses **Node-API (N-API)** to provide ABI stability across Node.js versions. In most cases, you **do not need to rebuild** the module when upgrading Node.js or Electron.

### You may need to rebuild if:

- A prebuilt binary is not available for your platform or runtime.
- You upgrade to a new major version of Electron or Node that isn’t yet supported.
- You encounter unexpected runtime errors.

### Manual Rebuild

```bash
npm rebuild @thesusheer/node-miniaudio
# or
node-gyp rebuild
```

> Rebuild is handled automatically by `node-gyp` during install if needed.

---

## Project Structure

- Native binding: `node_miniaudio.cpp`
- Entry point: `lib/index.js`
- Type definitions: `types/index.d.ts`
- Build config: `binding.gyp`
- Prebuild handling: `node-pre-gyp` + `node-pre-gyp-github`

---

## Contribution

Contributions are welcome! Whether it’s:

- Improving support for macOS or Linux
- Adding features or playback options
- Reporting and fixing bugs
- Enhancing the documentation

You're encouraged to fork the repo and open a pull request.

👉 [Open an Issue](https://github.com/Susheer/node-miniaudio/issues)  
👉 [Submit a PR](https://github.com/Susheer/node-miniaudio/pulls)

---

## License

BSD License

---

## Author

**Sudheer Gupta**  
[GitHub](https://github.com/Susheer)  
📧 sudheergupta0508@gmail.com

---

## Related Links

- [MiniAudio Library](https://miniaud.io)
- [npm Package](https://www.npmjs.com/package/@thesusheer/node-miniaudio)
- [GitHub Repo](https://github.com/Susheer/node-miniaudio)