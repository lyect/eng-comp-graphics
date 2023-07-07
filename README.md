# Engineering and Computer Graphics

Assignments on subject "Engineering and Computer Graphics" completed while studying at NSU.
All the tasks done using C++/Qt6. All the projects were built using qmake (because QtCreator told me to use it :P).

* ECGPaint - small paint app. Allows to open images or create blank canvases, draw figures such as stars and polygons.
  - App uses [Bresenham's algorithm](https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm) to draw 1pt width lines.
  - App uses spans to implement filling (this algorithm called "Span Filling").

* ECGFilter - app with some image filters. The following filters have been implemented:
  - Monochrome filter.
  - Inversion filter.
  - Blur filter (gaussian blur for small kernel size and mean value blur for big kernel size).
  - Sharpening filter.
  - Embossing filter.
  - Gamma correction filter.
  - Edge detection (Roberts' and Sobel's algorithms).
  - Watercolor filter.
  - Dithering (Floyd's and ordered).
  - Pencil filter (filter that transforms picture into pencil drawing).
 
* ECGWireframe - app for modeling rotation figures. Using editor you can draw a B-spline, which will be rotated around axis and transformed into 3D figure.
<p align="center">
  <img src="https://github.com/lyect/eng-comp-graphics/assets/35844327/d9bc441a-78c3-46a8-89cc-1afa0c0c20dd" width="706" height="442">
</p>
<p align="center">
  <img src="https://github.com/lyect/eng-comp-graphics/assets/35844327/97876835-4b46-49f1-ad29-d9fed0895bb6" width="706" height="442">
</p>

