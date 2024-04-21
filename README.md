# Hi there
This tool generates file binary visualization in form of Digraph Dot Plot View.

## Build
To build tool just run **make**

## Algorithm details
Generally speaking, the tool goes through the file byte pairs and marks them on a 256x256 image using the pair as coordinates. The more frequent the pair, the brighter the corresponding pixel on the image. This simple algorithm generates unique pictures for each file format.

### Images
Here we will take a source picture of Pepe and then convert it into different formats and take prints of them to see the difference.

Source picture:

![Pepe source](https://github.com/Astroner/c-file-print/blob/master/examples/pepe.png)