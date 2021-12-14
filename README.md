https://www.youtube.com/watch?v=fWhEbZEGT7k&ab_channel=RyanGehrlein

[1]("./media/Screenshot_1.png")
[2]("./media/Screenshot_2.png")
[3]("./media/Screenshot_3.png")

* Name and partners name(At most teams of 2 folks total)
  * Team member 1: Ryan Gehrlein
* How many hours did it take you to complete this final project? 40
* Did you collaborate or share ideas with any other students/TAs/Professors? No
* Did you use any external resources? 
  * Note it is fair to utilize some tutorials--cite them here. You **must** add your own personal touch however!
  * https://cmu-graphics.github.io/Scotty3D/meshedit/halfedge - half edge data structure for finding adjacent elements in a mesh (didn't end up using this though, but was good to learn about mesh generation)
  * https://gamedev.stackexchange.com/questions/97707/marching-cubes-vertex-normals - calculatig normals for marching cubes
  * https://www.youtube.com/watch?v=M3iI2l0ltbE - marching cubes high level explanation
  * Starter code from various labs in this class
  * Anything else is cited in the code where it is used (as comments)
  * (Optional) What was the most interesting part of the Final Assignment? How could the instructor improve the final project?

## HOW TO USE

* Should compile with simple python build.py
* Arrow keys + RSHIFT and RCTRL to move the 8-sided shape and dig through the cube
* Hold W for wireframe view
* Press R to reset the cube
* Click and drag to rotate the cube
* Hold X for another interesting way to carve the cube :)

## NOTES **

* In my video my cube's resolution is 25x25x25. This is pushing my laptop to its limits; for this submission I put it slightly down to 20x20x20 so that I don't burn anyone's computer.
* Requires an environment with pthread support (because that's all that would work on my environment!)
* Works when built with O3 level optimizations.

### Rubric

<table>
  <tbody>
    <tr>
      <th>Points</th>
      <th align="center">Description</th>
    </tr>
    <tr>
      <td>(33.3%) Project Completion</td>
     <td align="left"><ul><li>Does the project compile and run.</li><li>Is it polished without any bugs (No weird visual artifacts).</li><li>Did you make a video?</li><li>Did you add a screenshot of your project to the repository?</li></ul></td>
    </tr>
    <tr>
      <td>(33.3%) Technical</td>
      <td align="left"><ul><li>Was the implementation of the project challenging?</li><li>Even if you followed a tutoral, it should not be trivial, and have some personal touch to it.</li><li>Did you have to organize/process a sufficient amount of data?</li><li>Was it clear you consulted some outside resources that go above and beyond the scope of this class</li></ul></td>
    </tr>
    <tr>
      <td>(33.4%) Creativity</td>
      <td align="left"><ul><li>How visually appealing is the scene?<ul><li>Note: There should be some 'wow' factor--instructors discretion is used here.</li></ul></li><li>How original is the project<ul><li>Again, did you enhance a tutorial and do something unique or just go by the book?</li></ul></li></ul></td>
    </tr>
  </tbody>
</table>
