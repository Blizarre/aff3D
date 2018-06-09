.. image:: https://travis-ci.org/Blizarre/aff3D.svg?branch=master
    :target: https://travis-ci.org/Blizarre/aff3D

*DISCLAIMER*: This is a very old project I am in the process of cleaning up. The current goal is to keep the exact same set of functionality but to migrate the code to "modern" c++.
I decided to try to make a pointer-less program (except for the interactions with the C library and if needed after profiling), to see how easy it was to use the new set of tools from the STL.
The level of performances is also monitored commit after commit. 

=======

This project is just a hobby : I try to recreate a rendering engine from scratch.

I am currently using SDL to get the input of the user as well as to "draw" the image (setPixel). I'm not using anything else for now, except the math library for sqrt, sin and cos.

It is in very early stage of development, as I'm trying to evaluate what are the technologies that I can realisticaly implement in software using "naïve" implementation at first.
Then I'll go on to the next stage and optimize the algorithm for speed.

For now it can render a teapot (in a special format), scramble the image a bit, but not much more.	

usage: aff3d <model> 
(look in the data subdirectory)

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




