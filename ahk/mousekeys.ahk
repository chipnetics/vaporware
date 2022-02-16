#SingleInstance Force

XButton1::
Run C:\WINDOWS\explorer.exe `/e`, "C:\Users\username\Desktop"
return

XButton2::
SetKeyDelay 25,50
Send,  {SHIFT DOWN}{ALT DOWN}{TAB DOWN}{ALT UP}{TAB UP}{SHIFT UP}
Send,  {ALT DOWN}{SPACE}{ALT UP}{X}
;Send,  {LWin DOWN}{UP}{UP}{LWin UP}
return

WheelLeft::
SetKeyDelay 25,50
Send, {ALT DOWN}{TAB DOWN}{ALT UP}{TAB UP}
return

^!RButton::
WinGet, active_proc, ProcessName, A
IfEqual, active_proc, EXCEL.exe
{	
	MsgBox, Excel is detected
}
return

;!RButton::
;WinGet, active_id, ID, A
;WinGet, active_pid, PID, A
;WinGet, active_proc, ProcessName, A
;MsgBox, The active window's:`n`nID is %active_id%`nPID is %active_pid%`nProcess is %active_proc%
;Return