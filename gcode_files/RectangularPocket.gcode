%
O1001 (RECTANGULAR POCKET)

G21 G17 G90 G40 G49 G80   ; mm, XY plane, abs, cancel modes
G54                      ; Work offset

T1 M6                    ; Tool change (10mm end mill)
S2500 M3                 ; Spindle ON clockwise
G0 X0 Y0                 ; Move to origin
G0 Z20                   ; Safe height

G1 Z-2 F200              ; First depth pass
G1 X50 F400
G1 Y30
G1 X0
G1 Y0

G1 Z-4                   ; Second depth pass
G1 X50
G1 Y30
G1 X0
G1 Y0

G1 Z-5                   ; Final depth pass
G1 X50
G1 Y30
G1 X0
G1 Y0

G0 Z20                   ; Retract
M5                       ; Spindle OFF
M30                      ; End program
%
