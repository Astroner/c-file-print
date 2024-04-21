# Hi there
This tool generates file binary visualization in form of Digraph Dot Plot View.

## Build
To build tool just run **make**

## Algorithm details
Generally speaking, the tool goes through the file byte pairs and marks them on a 256x256 image using the pair as coordinates. The more frequent the pair, the brighter the corresponding pixel on the image. This simple algorithm generates unique pictures for each file format.


## Examples
### Images
Pepe picture converted into different formats and passed to **fprint**.

**Source picture:**

![Pepe source](https://github.com/Astroner/c-file-print/blob/master/examples/pepe.png)

| Format     | Print                                                                                         |
|------------|-----------------------------------------------------------------------------------------------|
| **PNG**    | ![Pepe png print](https://github.com/Astroner/c-file-print/blob/master/examples/pepe-png.png) |
| **JPG**    | ![Pepe png print](https://github.com/Astroner/c-file-print/blob/master/examples/pepe-jpg.png) |
| **PPM**    | ![Pepe png print](https://github.com/Astroner/c-file-print/blob/master/examples/pepe-ppm.png) |
| **BMP**    | ![Pepe png print](https://github.com/Astroner/c-file-print/blob/master/examples/pepe-bmp.png) |

### Text
[Romeo and Juliet Scene 1](https://shakespeare.mit.edu/romeo_juliet/romeo_juliet.1.1.html) in different formats

| Format      | Print                                                                                           |
|-------------|-------------------------------------------------------------------------------------------------|
| **TXT**     | ![Text txt print](https://github.com/Astroner/c-file-print/blob/master/examples/text-txt.png)   |
| **DOCX**    | ![Pepe docx print](https://github.com/Astroner/c-file-print/blob/master/examples/text-docx.png) |
| **PDF**     | ![Text pdf print](https://github.com/Astroner/c-file-print/blob/master/examples/text-pdf.png)   |

### Video
![Source video with a cat](https://www.youtube.com/watch?v=P2Rn_YYpmLg)