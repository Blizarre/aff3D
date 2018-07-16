.. image:: https://travis-ci.org/Blizarre/aff3D.svg?branch=master
    :target: https://travis-ci.org/Blizarre/aff3D

*DISCLAIMER*: This is a very old project that I am in the process of cleaning up. The current goal is to keep the functionalities but to migrate the code to "modern" c++.

=======

This project is just a hobby: I try to recreate a software rasterizer from scratch, something simple that can parse a 3d model (STL format), perform the transformations/lightning and render it in real-time. This software support ASCII and Binary STL files.

I am currently using SDL2 to get the keyboard/mouse inputs as well as a framebuffer (to "draw" the image pixel-by-pixel). I'm not using anything else for now, except the C++ STL.

It is in a very early stage of development, as I'm trying to evaluate what are the technologies that I can realistically implement in software using "naïve" implementation at first.

::

  usage: aff3d <model.stl> 
  
  'w' : wireframe / solid
  'b' : enable/disable backface culling
  'q' : use the mouse to move the model

Implemented:

- Flat shading
- Backface culling
- painter's algorithm

Soon:

- z-buffer
