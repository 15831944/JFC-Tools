Option Strict On
Option Explicit On

Module mShowWindows

    Private Declare Function GetCurrentProcess Lib "kernel32" () As Integer
    Private Declare Function GetCurrentProcessId Lib "kernel32" () As Integer
    Public Declare Function ShowWindow Lib "user32.dll" (ByVal Hwnd As Integer, ByVal nCmdShow As Integer) As Integer


    Public Const SW_SHOW As Short = 1

    Public Sub ActiveCurrentProcess()

        Dim nResult As Integer
        Dim hCurrentProcess As Integer

        hCurrentProcess = GetCurrentProcess()
        nResult = ShowWindow(hCurrentProcess, SW_SHOW)

    End Sub

End Module