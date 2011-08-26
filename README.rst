This project is just a hobby : I try to recreate a rendering engine from scratch.

I am currently using SDL to get the input of the user as well as to "draw" the image (setPixel). I'm not using anything else for now, except the math library for sqrt, sin and cos.

It is in very early stage of development, as I'm trying to evaluate what are the technologies that I can realisticaly implement in software using "naïve" implementation at first.
Then I'll go on to the next stage and optimize the algorithm for speed.

For now it can render a teapot (in a special format), scramble the image a bit, but not much.


'w' : wireframe / solid
'b' : anable/disable backface culling
'f' : scramble the image
'q' : use the mouse to move the teapot

Implemented :
* Flat shading
* Backface culling
* painter's algorithm

Soon :
* phong shading
* z-buffer
* texture
* bump-mapping




