#!/usr/bin/python

data=[
(29,36.8),
(37,26.1),
(44,19.3),
(51,15.1),
(57,11.6),
(63,8.6),
(70,7.6),
(76,6.3),
(82,5.3),
(87,4.5),
(91,3.9),
(94,3.5),
(100,3.3)]

rfixed = 22000.0
vcc = 5.0

def outputValue(r):
    i = vcc / (rfixed + r)
    vo = i * rfixed
    return vo

def analogValue(r):
    return int((outputValue(r)/5.0)*1024)

print """
typedef struct {
\tint t;
\tint v;
} ThermEntry;
"""

print "const int thermt_count = {0};".format(len(data))
print "ThermEntry thermt[thermt_count] = {"
for (t,r) in data:
    print "\t{{{0}, {1}}},".format(t, analogValue(r*1000.0))
print "};"

        


