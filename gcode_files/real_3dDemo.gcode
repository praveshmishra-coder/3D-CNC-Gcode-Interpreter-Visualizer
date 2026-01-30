; ================================
; REALISTIC 3D CNC TOOLPATH DEMO
; Square Pocket + Spiral + Z Lifts
; Units: Millimeters
; ================================

G21         ; Units = mm
G90         ; Absolute positioning
G0 Z10      ; Safe height

; Move to start
G0 X0 Y0 Z10
G0 X0 Y0 Z0

; ================================
; DRAW A BIG SQUARE (BASE SHAPE)
; ================================

G1 X50 Y0 Z0 F200
G1 X50 Y50 Z0 F200
G1 X0  Y50 Z0 F200
G1 X0  Y0  Z0 F200

; ================================
; DRAW INNER SQUARE (DEPTH STEP)
; ================================

G1 Z-2 F80
G1 X45 Y5
G1 X45 Y45
G1 X5  Y45
G1 X5  Y5

; ================================
; DRAW SMALLER INNER SQUARE
; ================================

G1 Z-4 F80
G1 X40 Y10
G1 X40 Y40
G1 X10 Y40
G1 X10 Y10

; ================================
; SPIRAL UPWARD PATH (3D EFFECT)
; ================================

G1 Z-3 F100
G1 X20 Y10 Z-2
G1 X30 Y15 Z-1
G1 X35 Y25 Z0
G1 X25 Y35 Z1
G1 X15 Y30 Z2
G1 X10 Y20 Z3
G1 X20 Y10 Z4

; ================================
; LIFT TOOL UP
; ================================

G0 Z10

; ================================
; DIAGONAL 3D MOVE
; ================================

G1 X60 Y60 Z10 F150
G1 X0  Y0  Z0  F150

; ================================
; END PROGRAM
; ================================

M30
