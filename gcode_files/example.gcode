; Rubik's Cube 3D Wireframe (No Diagonals)
; Only cube edges

G21         ; Units in millimeters
G90         ; Absolute positioning

; ===== Bottom Face (Z = 0) =====
G0 X0 Y0 Z0
G1 X30 Y0 Z0 F200
G1 X30 Y30 Z0 F200
G1 X0 Y30 Z0 F200
G1 X0 Y0 Z0 F200

; ===== Top Face (Z = 30) =====
G0 X0 Y0 Z30
G1 X30 Y0 Z30 F200
G1 X30 Y30 Z30 F200
G1 X0 Y30 Z30 F200
G1 X0 Y0 Z30 F200

; ===== Vertical Edges =====
G0 X0 Y0 Z0
G1 X0 Y0 Z30 F150

G0 X30 Y0 Z0
G1 X30 Y0 Z30 F150

G0 X30 Y30 Z0
G1 X30 Y30 Z30 F150

G0 X0 Y30 Z0
G1 X0 Y30 Z30 F150

; ===== Return Home =====
G0 X0 Y0 Z0

M30         ; End program
