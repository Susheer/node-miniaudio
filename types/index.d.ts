/**
 * Callback function type for audio playback completion.
 * @param message - Message indicating playback completion.
 */

export type AudioPlaybackCallback = (message: string) => void;
/**
 * Plays an audio file asynchronously.
 * @param filePath - Absolute or relative path to the audio file (must be supported format like WAV, MP3, etc.).
 * @param callback - Function that gets called when playback is complete.
 * @returns A string indicating playback start.
 */
export function playAudio(filePath: string, callback: AudioPlaybackCallback): string