So, someone gave me a Particle Boron Dev Board and I had a TM1683 display in my parts drawer so...The Boron Thing was born.
Not sure how useful it is but there could be an application for it.

I wrote (with help from ChaGPT) a bunch of small functions like showing the time, system version, etc. These are included in the repository.
I then glommed them all together into one chunk of code called Boron_TM1638_8_programs.ino. 
To upload this code to your Particle Boron you will need to copy and paste the code into the ParticleIDE https://build.particle.io/build

There is a little easter egg in the code...if you simultaneously press both buttons 1 and 8 the "game" Boro-Pong will load. It isn;t really playable due to the fact that when you push a button to move the paddle it will exit. I didn;t feel like figuring that out. If you want to play the "game" you can just load the PONG code above.

Enjoy!
Ben
