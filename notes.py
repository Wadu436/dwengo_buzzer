import numpy as np

OFFSET = 9

notes = ["C{}", "C{}SHARP", "D{}", "D{}SHARP", "E{}", "F{}", "F{}SHARP", "G{}", "G{}SHARP", "A{}", "A{}SHARP", "B{}"]
freq = np.array(27.5, dtype="float128")
n = ['0']
num_notes = len(notes)
twelfth_root_two = np.power(np.array(2, dtype="float128"), 1/12)

print("#pragma once\n\n#include <avr/io.h>\n")
print("#define NOTE_OFF 0")
for i in range(OFFSET, 88+OFFSET):
    print(f"#define {notes[i%num_notes].format(i//num_notes)} {i-OFFSET+1}")
    n.append(f"{(np.round(freq, 0)):.0f}")
    freq *= twelfth_root_two

print("\n")
print("const uint16_t notes[] = {")
print(", ".join(n))
print("};")