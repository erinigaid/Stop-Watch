# Stop-Watch

## A system that controls a stop-watch and displays it on 7-segements

### Drivers: GPIO, Timer (CTC), External Interrupts, 7-segment, MC: ATmega32

The time is stored as array members (seconds, minutes, hours).

Macros was used to avoid using magic numbers as much as possible.

Timer1 ISR only sets the global variable g_TimerFlag, so it would be short and deterministic. 

Prescaler choice was 1024 to lower the compare match value as much as possible.

Delay is 10 microsec so the user wouldn’t notice the the any of the 7-segments is off.

