CoordMode, mouse, Screen
MouseGetPos, xPos, yPos

Gui, Add, Text, , Opening Primavera... Please Wait
Gui, Show, x%xPos% y%yPos% NoActivate

run, "C:\Program Files\Oracle\Primavera P6\P6 Professional\PM.exe"

WinWait, ahk_class TfrmLogin  ;P6 Login window Active

Send Jeff 		
Send {Tab}
Send {Tab}		
Send {Tab}		
Send {Enter}	

WinWait, ahk_class TDevxMainForm  ;P6 Window Class when loaded.

Send {Ctrl Down}w{Ctrl Up}  ; close any open schedule (important)
sleep 300
Send {Enter}
sleep 2000

Send {Ctrl Down}o{Ctrl Up}  ; open promptJeff			

Send {Ctrl Down}f{Ctrl Up}  ; find prompt

Send 10425-52-SCH-033_04A-1

Send {Enter}	

Send !{F4}  ;altf4 to to close find window

Send !o  ; open found string

; The "opening" progress window will opena nd close 3 times.
WinWaitNotActive, ahk_class TfrmProgress
WinWaitActive, ahk_class TfrmProgress
WinWaitNotActive, ahk_class TfrmProgress
WinWaitActive, ahk_class TfrmProgress
WinWaitNotActive, ahk_class TfrmProgress
;sleep, 15000

Send !f ; Expand File menu
sleep, 500
Send e ; Select "Export"

WinWaitActive, ahk_class TfrmWizard  ; Wait for the export Wizard


Send !n ; next screen on export
sleep, 300
Send !n ; next screen on export
sleep, 300
Send !n ; next screen on export
sleep, 300

Send {Tab} ; tab to file input .Name

Send C:\Users\paraja9\Documents\latest.xer

Send {Enter}
sleep, 300
Send {Enter} ; Send an extra Enter in case file exists (2nd prompt)


WinWaitNotActive, ahk_class TfrmProgress
WinWaitActive, ahk_class TfrmProgress
WinWaitNotActive, ahk_class TfrmProgress
WinWaitActive, ahk_class TfrmProgress
WinWaitNotActive, ahk_class TfrmProgress

sleep, 1000


Send !c ; alt+c close successful completion conf.

Send !f ; open file #MenuMaskKey, 

sleep, 500

Send x ; Exit, 
sleep, 500
Send {Enter} ; Confirm close Primavera

Gui, Destroy
ExitApp