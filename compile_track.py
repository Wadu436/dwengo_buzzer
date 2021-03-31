import numpy as np
import matplotlib.pyplot as plt
from pathlib import Path
import re

TOP_MIN = 1
TOP_MAX = 255
TOP_RANGE = TOP_MAX - TOP_MIN + 1

TICK_PERIOD_MIN = 1
TICK_PERIOD_MAX = 16
TICK_PERIOD_RANGE = TICK_PERIOD_MAX - TICK_PERIOD_MIN + 1

FREQUENCY = 16000000
PRESCALER = 1024

ALLOWED_TIMING = [1, 2, 4, 8, 16, 32]


def sigmoid(x):
    if -x > np.log(np.finfo(type(x)).max):
        return 0.0    
    a = np.exp(-x)
    return np.power(1+a, -1)

def duration_1_32(top, tick_period):
    return (tick_period*PRESCALER*(top+1))/FREQUENCY

def BPM(top, tick_period):
    return 60/(32*duration_1_32(top, tick_period))

def score(top, tick, preferred_BPM):
    actual_BPM = BPM(top, tick)
    
    sc = top/TOP_MAX -tick/TICK_PERIOD_MAX - np.power(actual_BPM-preferred_BPM, 2)
    return sigmoid(sc)

# BPM for quarter note
def search_top_ti(BPM):
    BPM /= 4
    img = np.zeros((TOP_RANGE, TICK_PERIOD_RANGE), dtype=float)
    best_score = 0
    best_to = 0
    best_ti = 0
    for to in range(TOP_RANGE):
        for ti in range(TICK_PERIOD_RANGE):
            img[to, ti] = score(TOP_MIN + to, TICK_PERIOD_MIN + ti, BPM)

            if img[to, ti] > best_score:
                best_score = img[to, ti]
                best_to = TOP_MIN + to
                best_ti = TICK_PERIOD_MIN + ti
    return (best_to, best_ti)

def main():
    valid = True

    src_file = Path('frozen.txt')
    src_code = src_file.read_text()
    src_lines = src_code.splitlines()
    BPM_match = re.search("BPM: ([0-9]{1,})", src_lines.pop(0))
    if(BPM_match):
        BPM = int(BPM_match.group(1))
    else:
        print("Error! No BPM found")
        exit()

    Default_Timing = 32
    Default_Timing_Match = re.search("Default Timing: ([0-9]{1,})", src_lines[0])
    if(Default_Timing_Match):
        t = int(Default_Timing_Match.group(1))
        if not (t in ALLOWED_TIMING):
            print("Error! Invalid default timing")
            valid = False
        else:
            Default_Timing = 32//t
            src_lines.pop(0)
        

    notes = []
    for i in range(len(src_lines)):
        line = src_lines[i].strip()
        splitted = line.split(" ")
        for j in range(len(splitted)):
            token = splitted[j]
            if len(token) > 0:
                # Validate token
                m = re.match("([A-G])(#?)([0-8])(?::([0-9]{1,2}))?", token)
                s = re.match("(N)(?::([0-9]{1,2}))?", token)
                if m:
                    note = m.group(1)
                    sharp = m.group(2)
                    key = m.group(3)
                    timing = m.group(4)

                    note += key

                    if timing:
                        timing = 32//int(timing)
                    else:
                        timing = Default_Timing

                    if sharp:
                        note += "SHARP"

                    if not (timing in ALLOWED_TIMING):
                        print(f"{token} on line {i+1} does not have a valid timing.")
                        valid = False
                    notes.append((note , timing))
                elif s:
                    note = "NOTE_OFF"
                    timing = s.group(2)

                    if timing:
                        timing = 32//int(timing)
                    else:
                        timing = Default_Timing
                    if not (timing in ALLOWED_TIMING):
                        print(f"{token} on line {i+1} does not have a valid timing.")
                        valid = False
                    notes.append((note , timing))
                else:
                    print(f"{token} on line {i+1} does not match the pattern for a note.")
                    valid = False

                
                
            

    top, tick = search_top_ti(BPM)
    notes_array_string = ", ".join([f"{{{note}, {timing}}}" for (note, timing) in notes])    
        

    print('#include "track.h"')
    print('#include "notes.h"\n')
    print(f"beat b[] = {{{notes_array_string}}};")
    print(f"track compiled_track = {{{len(notes)}, {top}, {tick}, b}};")
    # print(BPM)
    # print(notes)
    

if __name__ == "__main__":
    main()