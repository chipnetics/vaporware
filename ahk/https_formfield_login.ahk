;^j::

Gui, Add, Text, ,Downloading from HTTP site... Please Wait.
Gui, Show, Center, HTTP Downloader

wb := ComObjCreate("{D5E8041D-920F-45e9-B8FB-B1DEB82C6E5E}")   ; "InternetExplorer.Application"

wb.visible := true

wb.navigate("https_form_link_here")

While wb.readyState != 4
    sleep, 10

wb.document.getElementById("userName").value := "my_username"
wb.document.getElementById("password").value := "my_password"

sleep, 1000

wb.document.getElementById("loginButton").click()

Sleep, 4000

wb.navigate("Post_https_linb_after_login")

;reportFormat=csv also an option

sleep, 5000

Send s 		;Save the file option.

sleep, 2000

Gui, Destroy

wb.quit

return