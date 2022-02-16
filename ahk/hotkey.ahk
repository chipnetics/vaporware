;^j::

Data_file_list =					; set Data_file_list
(
tabulate.txt
)
CoordMode, mouse, Screen
MouseGetPos, xPos, yPos

Gui, Add, Text, , Opening Excel... Please Wait
Gui, Show, x%xPos% y%yPos% NoActivate

WorkBookPath 		= %A_WorkingDir%/report.xlsx

objExcel			:= ComObjCreate("Excel.Application")		; create a handle to a new excel application
objWorkBook 		:= objExcel.Workbooks.Open(WorkBookPath)	; opens the existing excel table
objExcel.Visible 	:= FALSE									; make excel visible

loop, parse, Data_file_list, `n, `r			; loop through Data_file_list
{
	FileRead, Clipboard , %A_LoopField%		; read file contents into clipboard
	objExcel.Sheets(1).Activate			; activate sheet
	objExcel.Cells.ClearContents				; clear sheet contents
	objExcel.Cells(1,1).Select				; select row 7 col 1
	objExcel.ActiveSheet.Paste				; paste file contents
}

objExcel.Cells(1,1).Select	

objExcel.Sheets(2).Activate	

objExcel.Visible 	:= TRUE	

;objWorkBook.Close(0)
;objExcel.Quit()

;objExcel 	:= ""

Gui, Destroy

ExitApp