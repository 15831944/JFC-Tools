Option Strict On
Option Explicit On

Module mLaunchAppAndWait

    Public Declare Function OpenProcess Lib "kernel32" (ByVal dwDesiredAccess As Integer, ByVal bInheritHandle As Integer, ByVal dwProcessID As Integer) As Integer
    Public Declare Function WaitForSingleObject Lib "kernel32" (ByVal hHandle As Integer, ByVal dwMilliseconds As Integer) As Integer
    Public Declare Function CloseHandle Lib "kernel32" (ByVal hObject As Integer) As Integer

    Public Const SYNCHRONIZE As Integer = &H100000
    Public Const INFINITE As Integer = &HFFFF 'Wait forever
    Public Const WAIT_OBJECT_0 As Short = 0 'The state of the specified object is signaled
    Public Const WAIT_TIMEOUT As Integer = &H102 'The time-out interval elapsed & the object’s state is nonsignaled.

    Public Const WAIT As Short = 1
    Public Const NOWAIT As Short = 0

    Public Function LaunchAppAndWait(ByVal svApplication As String, Optional ByRef nOperation As Short = 0) As Object
        Dim lRet As Object
        Dim lHnd, lPid_LaunchApp As Integer

        On Error Resume Next
        lPid_LaunchApp = Shell(svApplication, AppWinStyle.NormalFocus)
        On Error GoTo 0

        If nOperation = WAIT Then
            If lPid_LaunchApp <> 0 Then
                lHnd = OpenProcess(SYNCHRONIZE, 0, lPid_LaunchApp)
                If lHnd <> 0 Then
                    System.Windows.Forms.Application.DoEvents()
                    lRet = WaitForSingleObject(lHnd, INFINITE)
                    CloseHandle(lHnd)
                End If
            End If
        End If

    End Function

End Module