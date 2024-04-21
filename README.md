# Hi there
This tool generates file binary visualization in form of Digraph Dot Plot View.

## Build
To build tool just run **make**

## Algorithm details
Generally speaking, the tool goes through the file byte pairs and marks them on a 256x256 image using the pair as coordinates. The more frequent the pair, the brighter the corresponding pixel on the image. This simple algorithm generates unique pictures for each file format.

### Images
Source image: