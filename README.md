# Projects 5: Lights, Camera

### Design Choices
In this project, I knew that most of the code would have to stem from the Realtime class
so I started there. In terms of the pipeline, I thought that the best way to handle things
would be to go through each shape in the scene and render it. I also knew that each shape
would have its own CTM and so I thought that it would be sufficient to store 4 VAOs and 4 
VBOs in my code -- one for each unit shape. Then, for each primitive, I looped through and
bound the VAO corresponding to the primitive's type. I created and stored my VAOs and VBOs 
in a separate class called GLHelper in order to make things more simple and less crowded.

I also tried to keep runtime down in my code in a number of ways. The first was only
regenerating matrices and VAO/VBOs when things changed that required them to be updated. 
The second was being careful about which operations should occur where. For example, for
operations that would need to happen for an entire primitive, I would try to do those
before passing values into the shader (ex. calculating ambient lighting). I tried to ensure
that everything that was happening on a per pixel basis in the fragment shader absolutely
had to be done for each individual pixel.

Finally, my last notable design choice is the way in which I structured my code. I created
a new folder to store information about the camera and projection matrices. I also made a
new folder to store all of the unit shape generations.

### Known Bugs
No known bugs.

### Extra Credit
Extra Credit #1: Adaptive level of detail based on the number of objects in the scene.
