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

## Memory Map:



| Physical                	| Virtual                 	| Data        	|
|-------------------------	|-------------------------	|-------------	|
| 0x00000000 - 0x00400000 	| 0xC0000000 - 0xC0400000 	| Kernel      	|
| 0x00400000 - 0x00C00000 	| 0xC0400000 - 0xC0C00000 	| Kernel Heap 	|
| 0x00C00000 - 0x01800000 	| 0x80000000 - 0x8C000000 	| Userspace   	|

## Notes:
Sources for this project include
  - http://wiki.osedev.org
  - https://www.gitbook.com/book/0xax/linux-insides/details
  - https://littleosbook.github.io/
  - http://www.jamesmolloy.co.uk/tutorial_html/index.html
