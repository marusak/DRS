import sys

lines = []


def get_values(line):
    values = line.split()
    x = values[1][:-1]
    y = values[2][:-1]
    z = values[3][:-1]
    return (int(x), int(y), int(z))


class States:
    P_STOP = 0
    STOP = 1
    P_FORWARD = 2
    FORWARD = 3
    P_BACK = 4
    BACK = 5
    P_RIGHT_DRIFT = 6
    RIGHT_DRIFT = 7
    P_LEFT_DRIFT = 8
    LEFT_DRIFT = 9

    NAMES = ['STOP', 'STOP', 'FORWARD', 'FORWARD', 'BACK', 'BACK',
             'RIGHT_DRIFT', 'RIGHT_DRIFT', 'LEFT_DRIFT', 'LEFT_DRIFT']

    def __init__(self):
        self.current = 1
        self.last_state = 1

    def do_step(self, x, y, z):
        score = x + y + z
        # Staying still
        if abs(score) < 50:
            if self.current in [States.P_STOP, States.STOP]:
                self.current = States.STOP
            else:
                self.last_state = self.current
                self.current = States.P_STOP

        # Forward moving
        elif x > 5000 and abs(y) < 1000 and abs(z) < 10000:
            if self.current in [States.P_FORWARD, States.FORWARD]:
                self.current = States.FORWARD
            else:
                self.last_state = self.current
                self.current = States.P_FORWARD

        # Backwards moving
        elif x < -5000 and abs(y) < 1000 and abs(z) < 10000:
            if self.current in [States.P_BACK, States.BACK]:
                self.current = States.BACK
            else:
                self.last_state = self.current
                self.current = States.P_BACK

        # Right drifting
        elif x < -5000 and y < -5000 or z < -10000:
            if self.current in [States.P_RIGHT_DRIFT, States.RIGHT_DRIFT]:
                self.current = States.RIGHT_DRIFT
            else:
                self.last_state = self.current
                self.current = States.P_RIGHT_DRIFT

        # Left drifting
        elif x > 5000 and y > 5000 or z > 10000:
            if self.current in [States.P_LEFT_DRIFT, States.LEFT_DRIFT]:
                self.current = States.LEFT_DRIFT
            else:
                self.last_state = self.current
                self.current = States.P_LEFT_DRIFT

    def name_state(self):
        use_name = self.current
        # Transient state
        if self.current % 2 == 0:
            use_name = self.last_state
        return States.NAMES[use_name]


with open(sys.argv[1], "r") as fl:
    for line in fl:
        lines.append(line)

state = States()

for line in lines:
    x, y, z = get_values(line)
    state.do_step(x, y, z)
    print(line[:-1] + " " + state.name_state())
