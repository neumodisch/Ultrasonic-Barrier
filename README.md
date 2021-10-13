# Ultrasonic-Barrier

An implementation of a light barrier with an ultrasonic sensor.

At startup the median distance in a time interval is measured and a trigger distance is set right below it. When a trigger is detected, it is signaled with a positive pulse. There is a cooldown period before the next trigger is detected.
