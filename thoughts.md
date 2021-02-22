# What to demonstrate ?

## What fits the scope ?

- Slow / Overloaded network -- Increased control period
- Overloaded network -- Random delays on outgoing packets
- Link errors -- Unreachable devices / segments
- Faulty node hardware -- Data corruption during transport
- ...

### Slow / Overloaded network -- Increased control period
- Complex systems with demanding control may become unstable past a certain
  control period, specially in safety-critical systems.

### Overloaded network -- Random delays on outgoing packets
- Sporadically affects system stability.
- High enough rate of delays may turn the system inappropriate for
  safety-critical scenarios.

### Link errors -- Unreachable devices / segments
- Can be checked for during start-up but;
- **Poses a major threat during execution**

### Faulty node hardware -- Data corruption during transport
- Usually introduces hard-to-identify faults.


## What doesn't fit the scope ?

- DOS -- packet loss
  - Industrial networks use dedicated gateways to the Internet. Physical access
  to the local network would be needed, so it's not relevant.

- Collisions -- Random delays
  - N/A because only the Master actively sends packets.

- ...

# How to demonstrate

- Bad > Bad > ... > Good
  - Start with an unusable setup and have a step-by-step approach on how to
  overcome each problem.
  - _Con: hard to identify the effects of each fault._

- Bad > Good
  - Start with a faulty system where only a single fault is introduced and then
  demonstrate the solution.
  - _Pro: easy to identify the effects of each fault._
  - _Con: doesn't demonstrate the effects of multiple simultaneous faults._

- Good > Bad
  - Same concept as the 'Bad -> Good' but in a reversed order: Start with the
  working setup and introduce a fault to visualize the effect.
  - Same _Pros_ and _Cons_ of the Bad -> Good approach

- Random Good > Random Bad > Good
  1. Start by demonstrating a good scenario
    1. Either let the student program it: Difficult to maintain stability and
    impossible to know if it will work
    2. Have the student choose a predefined behaviour: *Possible, although the
    chosen scenario may not be the fittest.*
    3. Have the system choose a random one: *Possible, if the system pre-selects
    the faults we can filter and choose the best scenario to demonstrate it.*
  2. Demonstrate the same action but with a random fault introduced
    1. Can have various degrees of difficulty: e.g. simultaneous faults.
  3. Comment about the behaviour
  4. Identify the fault(s) based on the behaviour
    1. Allow the student to get a different scenario, (in case the chosen one
    didn't provide a clear understanding of the fault's effect.
  5. Check the real faults introduced
  6. Comment about their effects on the behaviour
  7. Compare numbers 4 and 5

- ...
