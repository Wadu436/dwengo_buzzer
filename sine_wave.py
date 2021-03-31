import numpy as np
import matplotlib.pyplot as plt

def np_array_to_c_array(a):
    rep = "{"
    rep += ", ".join(a.astype('str'))
    rep += "}"
    return rep

FREQUENCY = 1.0
SPEEDUP = 32*5*5
SAMPLE_FREQUENCY = 40000
TOP = 199

timestep = SPEEDUP/SAMPLE_FREQUENCY
period_timesteps = SAMPLE_FREQUENCY/(FREQUENCY*SPEEDUP)
period_timesteps = int(period_timesteps)

x = np.arange(period_timesteps)
y = (TOP*(0.5-0.5*np.cos(2*np.pi*FREQUENCY*SPEEDUP*x/SAMPLE_FREQUENCY))).astype('int')

plt.plot(x, y)
plt.show()

#print(period_timesteps)
#print(np_array_to_c_array(y))

s = '''#pragma once

const uint8_t sine[] = {a};

// x between [0, {f})
uint8_t lut_sin(uint16_t x) {{
    x /= {e};
    signed char sign = 1;
    signed char offset = 0;
    if(x >= {c}) {{
        x -= {c};
        sign = -1;
        offset = 200;
    }}
    if (x < {b}) {{
        return offset + sign*sine[x];
    }} else {{
        return offset + sign*sine[({c}-x)];
    }}
}}'''.format(a=np_array_to_c_array(y), b=period_timesteps, c=period_timesteps*2, d=period_timesteps*4, e=SPEEDUP, f=SAMPLE_FREQUENCY)

s_fast = '''#pragma once

const uint8_t sine[] = {a};

// x between [0, {f})
uint8_t lut_sin(uint16_t x) {{
    uint16_t x_ = x/{e};
    return sine[x_];
}}'''.format(a=np_array_to_c_array(y), b=period_timesteps, c=period_timesteps*2, d=period_timesteps*4, e=SPEEDUP, f=SAMPLE_FREQUENCY)

print(s_fast)