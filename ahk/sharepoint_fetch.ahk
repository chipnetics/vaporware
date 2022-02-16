;^j::

Gui, Add, Text, ,Opening File From Sharepoint... Please Wait.
Gui, Show, Center, RR Downloader

wb := ComObjCreate("{D5E8041D-920F-45e9-B8FB-B1DEB82C6E5E}")   ; "InternetExplorer.Application"

wb.visible := false

wb.navigate("url_was_here")


FileDelete, %A_WorkingDir%\filename.xlsx
Sleep 4000

;Navigate to the save as portion...
Send {Right}
Send {Down}		
Send a

;Move cursor to front of file name
Sleep 2000
Send {Home}

;Insert working directory and a backslash
send %A_WorkingDir%\

Send {Enter} ; Save

Send !c  ;Close save prompt

Gui, Destroy

check_file = %A_WorkingDir%\filename.xlsx
While ! FileExist( check_file)
  Sleep 250

Gui, Add, Text, ,Opening Excel...
Gui, Show, Center, RR Downloader

ButtonOK:
WorkBookPath 		= C:\Users\username\Downloads\filename.xlsx

objExcel			:= ComObjCreate("Excel.Application")		; create a handle to a new excel application
objWorkBook 		:= objExcel.Workbooks.Open(WorkBookPath)	; opens the existing excel table
objExcel.Visible 	:= TRUE									; make excel visible

GuiClose:
Gui, Destroy
ExitApp



