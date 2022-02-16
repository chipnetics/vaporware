;^j::

Gui, Add, Text, , XLS File.... Please Wait.
Gui, Show, , Convert

WorkBookPath 		= %A_WorkingDir%/my_xls_file.xlsx

objExcel			:= ComObjCreate("Excel.Application")		; create a handle to a new excel application
objWorkBook 		:= objExcel.Workbooks.Open(WorkBookPath)	; opens the existing excel table
objExcel.Visible 	:= FALSE									; make excel visible


save_file = %A_WorkingDir%/tab-delimited.txt

objWorkBook.SaveAs(save_file, -4158)

objWorkBook.Close(0)
objExcel.Quit()
objExcel 	:= ""

Gui, Destroy

ExitApp