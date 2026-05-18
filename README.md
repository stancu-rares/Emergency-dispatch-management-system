# Emergency Dispatch Management System

A data structures project implemented in C that simulates a simplified emergency dispatch system for handling emergency incidents and assigning available intervention units.

The project was developed as an academic assignment for the Data Structures and Algorithms course.

## Features

- Add new emergency incidents with an ID, priority, and description
- Store incidents in a circular doubly linked list with a sentinel node
- Manage three priority queues for incidents: `high`, `medium`, and `low`
- Manage available intervention units using a dedicated queue
- Dispatch available units to incidents based on priority
- Mark dispatched incidents as solved
- Display information about incidents, units, and active interventions
- Process commands using file-based input and output

## Data Structures Used

### Circular Doubly Linked List

Incidents are stored in a circular doubly linked list with a sentinel node. Each incident contains:

- incident ID
- priority level
- dynamically allocated description
- current status
- links to the previous and next incident nodes

This structure allows efficient traversal and simplifies insertions by using a sentinel node.

### Intervention List

Interventions are stored in a separate circular doubly linked list with a sentinel node. Each intervention keeps references to:

- the incident being handled
- the assigned intervention unit

This makes it possible to keep a history of started interventions and display their current status.

### Queues

The system uses queues to manage both incidents and available units.

Incident queues:

- `queue_high`
- `queue_medium`
- `queue_low`

Available unit queue:

- `queue_available_units`

The incident queues store pointers to incident nodes, while the unit queue stores pointers to unit structures. This avoids unnecessary copying and keeps the system data consistent.

### Stack

A stack structure is used for keeping track of dispatched interventions. This is useful for undo-style dispatch management and for maintaining the order of recent interventions.

## Supported Commands

### ADD_INCIDENT

Adds a new incident to the system.

```text
ADD_INCIDENT id priority "description"
```

The new incident receives the initial status:

```text
queued
```

### CHECK_UNITS_AVAILABILITY

Displays the number of currently available intervention units.

```text
CHECK_UNITS_AVAILABILITY
```

Output format:

```text
Number of available units: n
```

### DISPATCH

Assigns the first available unit to the highest-priority queued incident.

```text
DISPATCH
```

Priority order:

```text
high -> medium -> low
```

If there is no available unit or no queued incident, the program outputs:

```text
INVALID OPERATION! ERROR 404
```

### SOLVED_INCIDENT

Marks an intervened incident as solved and makes the assigned unit available again.

```text
SOLVED_INCIDENT id
```

If the incident does not exist or is not currently intervened, the program outputs:

```text
INVALID OPERATION! ERROR 404
```

### SHOW_UNIT

Displays information about a specific intervention unit.

```text
SHOW_UNIT id
```

Output format:

```text
Unit id is type X and is available
```

or:

```text
Unit id is type X and is unavailable
```

### SHOW_INCIDENT

Displays information about a specific incident.

```text
SHOW_INCIDENT id
```

Output format:

```text
Incident id has priority priority, the following description: "description" and is status
```

### SHOW_INTERVENTIONS

Displays all interventions that have been started.

```text
SHOW_INTERVENTIONS
```

If no intervention has been started, the program outputs:

```text
No intervention has been initiated
```
Incident 1 has high priority, the following description: "fire in building" and is intervened
Unit 1 is type A and is unavailable
Incident 1 was assigned to unit 1, and has the following status: "intervened"
```
