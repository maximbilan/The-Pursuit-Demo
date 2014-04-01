SoundEngine
-----------

The SoundEngine modules provides a wrapper around Marmalade System's s3eSound
API, providing more convenient higher-level APIs, and allowing it to interact
with Marmalade Studio's IwResManager resource management system.

SoundEngine provides functionality including:
- Load and play WAV files
- Support for PCM and ADPCM formats
- Sound materials (CIwSoundSpec class)
- Sound groups (controlling how many channels can be used)
- Easy control of volume, pitch, looping, etc.

SoundEngine.mkf cannot be built directly as a library. It must be included
in an MKB project file, using the "subproject SoundEngine" directive.
