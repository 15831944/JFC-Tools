Option Strict On
Option Explicit On

Module mLaunchAppAndWait

    Public Declare Function OpenProcess Lib "kernel32" (ByVal dwDesiredAccess As Integer, ByVal bInheritHandle As Integer, ByVal dwProcessID As Integer) As Integer
    Public Declare Function WaitForSingleObject Lib "kernel32" (ByVal hHandle As Integer, ByVal dwMilliseconds As Integer) As Integer
    Public Declare Function CloseHandle Lib "kernel32" (ByVal hObject As Integer) As Integer

    Public Const SYNCHRONIZE As Integer = &H100000
    Public Const INFINITE As Integer = &HFFFF 'Wait forever
    Public Const WAIT_TIMEOUT As Integer = &H102 'The time-out interval elapsed & the object’s state is nonsignaled.

    Public Sub LaunchAppAndWait(ByVal szProgram As String, Optional ByVal szCmdLine As String = Nothing, Optional ByVal szWorkingDirectory As String = Nothing)

        Try

            'If Not IO.File.Exists(szProgram) Then Exit Function

            Dim MyProc As New Process()
            Dim lPid_Execute As Integer

            If MultiUserEDI.bUseProcessExecute Then
                If MultiUserEDI.bUseShellExecute Then MyProc.StartInfo.UseShellExecute = True

                If szWorkingDirectory = Nothing Then szWorkingDirectory = MultiUserEDI.AppPath

                MyProc.StartInfo.WorkingDirectory = szWorkingDirectory 'szPath
                MyProc.StartInfo.FileName = szProgram ' szFile
                MyProc.StartInfo.WindowStyle = ProcessWindowStyle.Normal
                MyProc.StartInfo.Arguments = szCmdLine
                MyProc.Start()
                lPid_Execute = MyProc.Id

            Else
                If szCmdLine <> "" Then
                    szCmdLine = " " + szCmdLine
                End If
                lPid_Execute = Shell(szProgram + szCmdLine, AppWinStyle.NormalFocus)
            End If

            System.Windows.Forms.Application.DoEvents()

            If lPid_Execute <> 0 Then

                Dim lHnd_Patch As Integer = OpenProcess(SYNCHRONIZE, 0, lPid_Execute)

                Dim nvTimeMilliseconds As Integer = 100

                While WAIT_TIMEOUT = WaitForSingleObject(lHnd_Patch, nvTimeMilliseconds)
                    System.Windows.Forms.Application.DoEvents()
                End While

                If lHnd_Patch <> 0 Then CloseHandle(lHnd_Patch)

                lHnd_Patch = 0

                If MultiUserEDI.bUseProcessExecute Then MyProc.Close()

            End If

            System.Windows.Forms.Application.DoEvents()

        Catch ex As Exception

            MsgBox("LaunchApp: " + ex.Message + vbNewLine + "(" + Chr(34) + szProgram + Chr(34) + ")", MsgBoxStyle.Critical)

        End Try

    End Sub


End Module