import numpy as np

TOP_MIN = 1
TOP_MAX = 255

TICK_PERIOD_MIN = 1
TICK_PERIOD_MAX = 16

FREQUENCY = 16000000
PRESCALER = 1024

def duration_1_32(top, tick_period):
    return (tick_period*PRESCALER*(top+1))/FREQUENCY

def BPM(dur_1_32):
    return 60/(32*dur_1_32)

All_BPMs = set()

for to in range(TOP_MIN, TOP_MAX+1):
    for ti in range(TICK_PERIOD_MIN, TICK_PERIOD_MAX+1):
        bpm = BPM(duration_1_32(to, ti))
        if 4.9 <= bpm <= 5.1:
            All_BPMs.add(np.round(bpm))

All_BPMs = list(All_BPMs)
All_BPMs.sort(reverse=True)
All_BPMs = [f"{a:.0f}" for a in All_BPMs]

print("\n".join(All_BPMs))