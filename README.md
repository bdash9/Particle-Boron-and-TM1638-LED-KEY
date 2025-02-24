So, someone gave me a Particle Boron Dev Board and I had a TM1683 display in my parts drawer so...The Boron Thing was born.
Not sure how useful it is but there could be an application for it.

I wrote (with help from ChaGPT) a bunch of small functions like showing the time, system version, Bluetooth Scan, LTE signal stregth, timer, battery stregth, etc. These are included in the repository.
I then glommed them all together into one chunk of code called Boron_TM1638_8_programs.ino.
To upload this code to your Particle Boron you will need to copy and paste the code into the ParticleIDE https://build.particle.io/build

There is a little easter egg in the code...if you simultaneously press both buttons 1 and 8 the "game" Boro-Pong will load. It isn't really playable due to the fact that when you push a button to move the paddle it will exit. I didn't feel like figuring that out. If you want the hidden "game" PONG then upload Boron_TM1638_8_prog_PONG.


Connecting TM1638 to Particle Boron
•	Data (DIO) Pin on TM1638: Connect to SDA on Boron (MY_SDA)
•	Clock (CLK) Pin on TM1638: Connect to SCL on Boron (MY_SCL)
•	Strobe (STB or STR) Pin on TM1638: Connect to D2 on Boron (MY_SSEL)
•	GND to GND
•	VCC to power


Here are some screen shots:
![IMG_8565](https://github.com/user-attachments/assets/4004b3ec-7678-4381-8ab3-5ef14c1fb275)
![IMG_8567](https://github.com/user-attachments/assets/d0557972-2e94-40be-b01f-1f7363fcf24c)
![IMG_8568](https://github.com/user-attachments/assets/968270df-23b1-48a3-b490-3aab12706a34)
![IMG_8569](https://github.com/user-attachments/assets/0cba0e19-6467-4221-a30c-65876fe08389)
![IMG_8570](https://github.com/user-attachments/assets/b8d3bb5f-8bfa-48f9-aa9b-78c23aab03a4)
![IMG_8572](https://github.com/user-attachments/assets/2639388b-6991-482e-9212-3452d0becc1b)
![IMG_8576](https://github.com/user-attachments/assets/615464a2-29c9-4a57-bd43-e176c2e4a001)
![IMG_8577](https://github.com/user-attachments/assets/a723e5b1-e937-43a0-88d1-0990ef219255)
![IMG_8578](https://github.com/user-attachments/assets/1950f8ff-8857-4330-9ac1-309bd2aaebb4)
![IMG_8579](https://github.com/user-attachments/assets/fba0f34d-b820-470d-8567-63bcc67b71c9)
![IMG_8580](https://github.com/user-attachments/assets/a681dc32-805d-4224-9e62-7ec1b2746f7d)


If you can’t connect to Boron due to a program running 
1.	Safe Mode:
o	Put the Boron into Safe Mode. This mode disables user firmware and allows you to flash new code via the Particle cloud without interference.
o	To enter Safe Mode:
a.	Hold the MODE button and tap RESET.
b.	Release RESET but continue holding MODE until the status LED blinks magenta. Let go of the button and let it finish booting (green) then flash

Enjoy!
Ben
