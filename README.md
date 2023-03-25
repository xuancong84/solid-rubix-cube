# Rubik's Cube simulator for Windows
Compiled on Visual Studio 2015 (Originally created on Visual Studio 2010)
![01](/screenshots/sscube.jpg)
![01](/screenshots/sscube1.jpg)

For setup, you need to extract dxsdk.zip to "C:\Program Files (x86)\dxsdk" for DirectX 9 SDK's includes and libraries.

Mouse operations:
- left-click to rotate the cube and move a slice
- middle-click drag to drag the cube
- right-click drag to zoom in and out

Features:
- It supports standard Rubix cube, super-cube (directional), super-super-cube (directional and interior)
- It saves movement history, you can forward and backward to a given step in history
- You can highlight a specific layer for better visualization

A sample solution (15x15x15) can be found in the ./Release folder, it is incomplete, but sufficient to illustrate the procedure.
For details, see https://sites.google.com/site/xuancong84/supersupercube
