CoordMode, mouse, Screen
MouseGetPos, xPos, yPos

Gui, Add, Text, , Opening Primavera... Please Wait
Gui, Show, x%xPos% y%yPos% NoActivate

run, "C:\Program Files\Oracle\Primavera P6\P6 Professional\PM.exe"

sleep, 2000

Send Jeff 		
Send {Tab}
Send {Tab}		
Send {Tab}		
Send {Enter}	

sleep, 20000

Send {Ctrl Down}o{Ctrl Up}  ; open prompt
Send {Ctrl Down}f{Ctrl Up}  ; find prompt

Send 10425-52-SCH-033_04A-1

Send {Enter}	

Send !{F4}  ;altf4 to to close find window

Send !o  ; open found string

sleep, 4000

; Weird Primavera bug here. Need to open find window and close it to be able to select all
Send {Ctrl Down}f{Ctrl Up}  ; find prompt
sleep, 1000
Send !{F4}  ;altf4 to to close find window

send {Ctrl Down}a{Ctrl Up} ; Select All
send {Ctrl Down}c{Ctrl Up} ; Copy

send {Alt Down}fx{Alt Up} ; Close P6 (file->close)
sleep, 1000
Send y  ; Confirm 'yes' to close

sleep, 3000

;objExcel			:= ComObjCreate("Excel.Application")		; create a handle to a new excel application
objExcel			:= ComObjActive("Excel.Application")		; create a handle to a new excel application

;objWorkBook 		:= objExcel.Workbooks.Add()					; add a new workbook

objExcel.Visible 	:= TRUE									; make excel visible

objExcel.Sheets("IDS").Activate			; activate sheet
objExcel.Cells.ClearContents		; clear sheet contents
objExcel.Cells(1,2).Select			; paste in top left corner
objExcel.ActiveSheet.Paste			

objExcel.Cells(1,2).Select	

send {Ctrl Down}a{Ctrl Up} ; Select All
send {Alt Down}hoi{Alt Up} ; Select All

objExcel.Columns("B:B").Select
objExcel.ActiveSheet.Range("$B$1:$B$2000").AutoFilter(Field:=1,Criteria1:="=PM*") ; Filter for PM
objExcel.Columns("D:G").Select
objExcel.Selection.EntireColumn.Hidden := True
objExcel.Columns("I:M").Select
objExcel.Selection.EntireColumn.Hidden := True


objExcel.Columns("H:H").Select
objExcel.Selection.Replace(What:=" A", Replacement:="")
objExcel.Selection.Replace(What:="~*", Replacement:="")

objExcel.Columns("B:B").EntireColumn.AutoFit
objExcel.Columns("C:C").EntireColumn.AutoFit
objExcel.Columns("H:H").EntireColumn.AutoFit

Gui, Destroy

ExitApp