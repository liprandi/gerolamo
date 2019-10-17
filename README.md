# gerolamo
Manipulator for cardian axis

Create for Raspberry Pi4

1. Almost Real Time task for read 3 load cells HX711 with common clock
  a. Output clock on  GPIO04 (pin 7)
  b. Input 1st HX711  GPIO14 (pin 8)
  c. Input 2nd HX711  GPIO15 (pin 10)
  d. Input 3rd HX711  GPIO18 (pin 12)
2. Almost Real Time task for stepper motor
  a. Every output signal is electronically buffered with IRF540 (for 24Vdc signals)
  b. Input signal fault is optical isolated with PC817
  c. Output Enable    GPIO17 (pin 11)
  d. Output direction GPIO27 (pin 13)
  e. Output pulse     GPIO22 (pin 15)
  f. Input fault      GPIO23 (pin 16)
3. Input signal for manual movement read from inverter SEW MOVIDRIVE
4. Input signal for clamps and sensors
  a. Input signals are optical isolated with PC817
  b. Left clamp opened      GPIO10 (pin 19)
  c. Left clamp closed      GPIO09 (pin 21)
  d. Right clamp opened     GPIO25 (pin 22)
  e. Right clamp closed     GPIO11 (pin 23)
  f. Presence cardan left   GPIO08 (pin 24)
  g. Presence cardan right  GPIO07 (pin 26)
5. Output signals for clamps
  a. Every output signal is electronically buffered with IRF540 (for 24Vdc signals)
  b. Left clamp open        GPIO05 (pin 29)
  c. Left clamp close       GPIO06 (pin 31)
  d. Right clamp open       GPIO12 (pin 32)
  e. Right clamp close      GPIO13 (pin 33)
  
