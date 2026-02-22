# COMPUS-Phases
for the project code 

NOTES

1. static unsigned char X_State = 0; 
private to that particular file, and exits for that particular motor in that program. Used for adts internal memory.
2. static const unsigned char ks_pos[4][3] = { ... };
Can't be modified once declared. may put in flash memory, used in loopup tables that never change