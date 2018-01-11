# CryptOS - A Hobby OS
## Building

### Requirements:
- Docker (if you don't want to setup gcc)
- Nasm
- Qemu
- Xorriso (building the ISO file)


The easiest way to run things is with the Docker image that contains the proper compiler. You can build the image from the git directory by typing:

`docker build . -t devenv`

Then run

`./run.sh`


## Notes:
Old code is in a different branch. The master branch is a rewrite of the OS.

Sources for this project include
  - http://wiki.osedev.org
  - https://www.gitbook.com/book/0xax/linux-insides/details
