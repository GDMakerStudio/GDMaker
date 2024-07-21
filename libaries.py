"""
installs the necessary libraries for libgles development and sound on a Linux system
"""

import subprocess
import os
import sys

def install_packages():
    packages = [
        "libgles2-mesa-dev",     # OpenGL ES development library
        "libopenal-dev",         # OpenAL development library for audio
        "libalut-dev",           # OpenAL Utility Toolkit development library
        "libsdl2-dev",           # SDL2 development library
        "libvorbis-dev",         # Vorbis audio codec development library
        "libogg-dev",            # Ogg bitstream library development library
        "libpulse-dev",          # PulseAudio development library
        "libasound2-dev",        # ALSA development library
        "libflac-dev",           # FLAC audio codec development library
        "libmpg123-dev",         # MP3 codec library
        "libsndfile1-dev",       # Audio file library development library
        "libjack-jackd2-dev",    # JACK audio connection kit development library
        "libavcodec-dev",        # FFmpeg codec library
        "libavformat-dev",       # FFmpeg format library
        "libswresample-dev",     # FFmpeg audio resampling library
        "cmake",                 # Build system
        "pkg-config",            # Helper tool to manage library paths
        "build-essential"        # Basic compilation tools
    ]

    # Update the package list
    subprocess.run(["sudo", "apt-get", "update"], check=True)

    # Install each package
    for package in packages:
        subprocess.run(["sudo", "apt-get", "install", "-y", package], check=True)

if __name__ == "__main__":
    install_packages()













