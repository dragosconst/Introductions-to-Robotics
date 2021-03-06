# Introduction to Robotics
Repo for the "Introduction to Robotics" optional course, from the Faculty of Mathematics and Computer Science, University of Bucharest.
<h2>Homework 0</h2>
<details>
  <summary></summary>
I installed the Arduino IDE and I set up the repo (and subsequently translated it to English).
</details>

<h2>Homework 1</h2>
<details>
  <summary></summary>
Requirements: "Use a separate potentiometer in controlling each of the color of the RGB led (Red, Green and Blue). The control must be done with digital electronics (aka you must read the value of the potentiometer with Arduino, and write a mapped value to each of the pins connectedto the led)."

Pictures:![image](https://user-images.githubusercontent.com/38582034/138557042-c1bd7f09-2dfb-4c3b-8c9d-8ebfb9bb21f3.png)
![image](https://user-images.githubusercontent.com/38582034/138557231-07708708-1e58-4085-b957-1aa4dcc1ee3e.png)

Notes: due to not having a sufficient amount of wires, I had to use resistors to connect certain components. I tried using the smallest available resistors (they should have no more than 10 ohms) and it seems to have a small, barely noticeable effect on the end result. Unfortunately, I didn't find a way around this.

Video: https://youtu.be/GEDasb3OT2U
</details>


 <h2>Homework 2</h2><details>
<summary></summary>
Requirements: "Building  the  traffic  lights  for  a  crosswalk. You will use 2 LEDs to represent the traffic lights for people (red and green) and 3 LEDs to represent the traffic lights for cars (red, yellow and green). See the states it needs to go through. If anything is not clear, ask. Also, see the uploaded video (the intervals are different, but the states flow is the same).It’s a traffic lights system for people and cars - don’t overthink it."

Pictures: ![image](https://user-images.githubusercontent.com/38582034/139592330-ab362b6d-49e8-4bb3-9297-44a6eedd30a4.png)
![image](https://user-images.githubusercontent.com/38582034/139592340-ac0ebf83-4f17-4cd2-a76b-3eac3e7ac971.png)
![image](https://user-images.githubusercontent.com/38582034/139592346-ca69d32d-465f-440a-985c-2c1d3d795dd8.png)

Video: https://youtu.be/5cwfZEWLZhM
</details>
  
 <h2>Homework 3</h2>
<details>
<summary></summary>
Requirements: "should detect EMF (check body and near outlets.  DO NOT INSERT INTO OUTLET - AKA NU-L BAGATI IN PRIZA). It should print the value on the 7-segment display (or light the led bar) and should makea sound based on the intensity."

Pictures: 
  
![image](https://user-images.githubusercontent.com/38582034/140813499-28de4f3d-c5af-4490-8001-55a513dfe9ea.png)
![image](https://user-images.githubusercontent.com/38582034/140813525-2ea92767-c0d1-42a2-8ec5-7d717859ab28.png)
![image](https://user-images.githubusercontent.com/38582034/140813544-05e0a237-4209-412c-bc41-a1ada5a6a270.png)

Notes: used 3 1M resistors in series for the copper cable, and a 330 one for the buzzer (tried different values but couldn't quite get a satisfying sound).
  
Video: https://youtu.be/nXa_zPxSuCU
</details>

<h2>Homework 4</h2>
<details>
  <summary></summary>
 Requirements: "The current homework  involves  using  a  joystick  in  setting  the  values  on  each  digit  of  a  4-digit7-segment display connected to a shift register 74hc595.  For maximum grade, you muse use interrupts, and for bonus you can save the last value in eeprom."
  
  
Pictures:
  
 ![image](https://user-images.githubusercontent.com/38582034/142287051-a2a9e0a5-13b8-4b34-9eaf-037100e0bd1c.png)
![image](https://user-images.githubusercontent.com/38582034/142287067-c00c93df-fef2-4201-b76b-3b8c044e3d9d.png)
![image](https://user-images.githubusercontent.com/38582034/142287083-945ec309-520e-4d32-8fbf-dd075c55bede.png)

  
Video: https://youtu.be/QOlsbrCwmeI
  
Notes: I'm not sure what's causing this, but for some reason, if I set the interrupt on FALLING, it will detect it at least twice per button push (sometimes even more). It seems to be some form of noise, I managed to circumvent this by attaching the interrupt to LOW and only really using it when the previous value was HIGH (and just showing the numbers on the display in the other case).
</details>
