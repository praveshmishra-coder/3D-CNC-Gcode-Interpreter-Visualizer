; Simple 3D CNC G-Code Example
; Move in a small square in 3D

G21         ; Set units to millimeters
G90         ; Absolute positioning

; Start at origin
G0 X0 Y0 Z0

; Move in a square
G1 X10 Y0 Z0 F100   ; Move to (10,0,0) at feed 100
G1 X10 Y10 Z0 F100  ; Move to (10,10,0)
G1 X0 Y10 Z0 F100   ; Move to (0,10,0)
G1 X0 Y0 Z0 F100    ; Move back to origin

; Move up in Z
G1 X0 Y0 Z10 F50    ; Lift tool to Z=10

; Move diagonally in XY plane
G1 X10 Y10 Z10 F100

; Return to home
G0 X0 Y0 Z0

M30         ; End of program
