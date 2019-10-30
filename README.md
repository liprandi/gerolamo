# gerolamo
Manipulator for cardian axis

Create for Raspberry Pi4

1. Almost Real Time task for read 3 load cells HX711 with common clock<br>
  a. Output clock on  GPIO04 (pin 7)<br>
  b. Input 1st HX711  GPIO14 (pin 8)<br>
  c. Input 2nd HX711  GPIO15 (pin 10)<br>
  d. Input 3rd HX711  GPIO18 (pin 12)<br>
2. Almost Real Time task for stepper motor<br>
  a. Every output signal is electronically buffered with IRF540 (for 24Vdc signals)<br>
  b. Input signal fault is optical isolated with PC817<br>
  c. Output Enable    GPIO17 (pin 11)<br>
  d. Output direction GPIO27 (pin 13)<br>
  e. Output pulse     GPIO22 (pin 15)<br>
  f. Input fault      GPIO23 (pin 16)<br>
  g. Input limit fwd  GPIO24 (pin 18)<br>
  h. Input limit bcw  GPIO10 (pin 19)<br>
3. Input signal for manual movement read from inverter SEW MOVIDRIVE<br>
4. Input signal for clamps and sensors<br>
  a. Input signals are optical isolated with PC817<br>
  b. Left clamp opened      GPIO09 (pin 21)<br>
  c. Left clamp closed      GPIO25 (pin 22)<br>
  d. Right clamp opened     GPIO11 (pin 23)<br>
  e. Right clamp closed     GPIO08 (pin 24)<br>
  f. Presence cardan left   GPIO07 (pin 26)<br>
  g. Presence cardan right  GPIO05 (pin 29)<br>
5. Output signals for clamps<br>
  a. Every output signal is electronically buffered with IRF540 (for 24Vdc signals)<br>
  b. Left clamp open        GPIO06 (pin 31)<br>
  c. Left clamp close       GPIO12 (pin 32)<br>
  d. Right clamp open       GPIO13 (pin 33)<br>
  e. Right clamp close      GPIO19 (pin 35)<br>
  
