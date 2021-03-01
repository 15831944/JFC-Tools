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
    Public Const WAIT As Byte = 1
    Public Const NOWAIT As Byte = 0

    Public Sub LaunchAppAndWait(ByVal svApplication As String, Optional ByRef svCommandLine As String = "", Optional ByRef Style As ProcessWindowStyle = ProcessWindowStyle.Normal, Optional ByRef WaitInfinity As Byte = WAIT)

        If IO.File.Exists(svApplication) Then

            Dim MyProc As New Process()

            MyProc.StartInfo.WorkingDirectory = Application.StartupPath
            MyProc.StartInfo.FileName = svApplication
            MyProc.StartInfo.WindowStyle = Style
            MyProc.StartInfo.Arguments = svCommandLine
            MyProc.StartInfo.UseShellExecute = FormBox.bUseShellExecute

            'Ajout le 14/02/2018 fichier bat
            MyProc.StartInfo.CreateNoWindow = True
            'MyProc.StartInfo.RedirectStandardInput = True
            'MyProc.StartInfo.RedirectStandardOutput = True
            'MyProc.StartInfo.RedirectStandardError = True

            MyProc.Start()

            If MyProc.Id <> 0 Then

                If WaitInfinity = WAIT Then

                    Dim lHnd As Integer = OpenProcess(SYNCHRONIZE, 0, MyProc.Id)

                    If lHnd <> 0 Then

                        System.Windows.Forms.Application.DoEvents()

                        Dim nvTimeMilliseconds As Integer = 100

                        While WAIT_TIMEOUT = WaitForSingleObject(lHnd, nvTimeMilliseconds)
                            System.Windows.Forms.Application.DoEvents()
                        End While

                        'Dim lRet As Integer = WaitForSingleObject(lHnd, INFINITE)
                        CloseHandle(lHnd)

                    End If

                End If
            Else
                MsgBox("Unable to launch: " + svApplication + vbNewLine + "Arguments: " + svCommandLine, MsgBoxStyle.Critical, "Error: Id")
            End If

        End If

    End Sub

End Module