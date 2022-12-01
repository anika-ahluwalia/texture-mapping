# Projects 6: Action!

### Design Choices
Default FBO is stored in the variable m_defaultFBO in the Realtime class. It can be quickly changed 
on line 59 of Realtime.cpp (first line of initializeGL).

For this project, I began by adjusting my default shader that was being used for lighting to include
point and directional lights. The main changes with this adjustment were passing in more uniform variables
to the fragment shader so that it could handle these types of lights. After that, the rest of the light
calculations were fairly simple and all handled in default.frag.

Next, I worked on getting movement to work. The code for this is essentially all in the mouseMoveEvent 
and timerEvent functions in realtime.cpp. For keyboard movement, I only modified the position vector and
for rotation, I only modified the look vector. Each time one of these was modified, I created a new
camera, recalculated the matrices, and updated the uniforms that were passed in.

Finally, I worked on the framebuffer portion. I created a new texture shader that would handle all of 
the post processing effects. I created a color and renderbuffer attachment and updated them accordingly
in my code. I passed in the original rendering's colors in as a texture into the fragment shader, and 
then did all of the post processing within texture.frag. I mapped neighboring UV coordinates by finding 
the width and height pixel increments scaled down and adding and subtracting those.


### Known Bugs
No known bugs.

### Extra Credit
Extra Credit #3: Grayscale per-pixel filter. All relevant code is in resources/shaders/texture.frag

Extra Credit #4: Sharpening kernel filter. All relevant code is in resources/shaders/texture.frag

Extra Credit #5: Sepia per-pixel filter. All relevant code is in resources/shaders/texture.frag

Extra Credit #6: Edge detection kernel filter. All relevant code is in resources/shaders/texture.frag



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
Related code is in realtime.cpp in the SettingsChanged function.

Extra Credit #2: Adaptive level of detail based on object's distance from the camera.
Related code is in realtime.cpp in the SettingsChanged function and at the start of the
loop in paintgl().
