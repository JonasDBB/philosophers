Philosophers project at Codam.

The goal is to solve the dining philosophers problem in several different ways:
- Once with threads and mutex locks.
- Once with threads and semaphores.
- once with processes and semaphores.

Admittedly, I could have done a little more input sanitation and checks for tight timings... 

Don't just use `usleep` for eat and sleep timers, `usleep` sucks.
