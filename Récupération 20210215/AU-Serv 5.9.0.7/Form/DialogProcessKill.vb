Option Strict On
Option Explicit On

Public Class DialogProcessKill

    ReadOnly ProcessDic As New Dictionary(Of String, Integer)

    Private Sub OK_Button_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles OK_Button.Click
        Me.DialogResult = System.Windows.Forms.DialogResult.Yes
        Me.Close()
    End Sub

    Private Sub Cancel_Button_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Cancel_Button.Click
        Me.DialogResult = System.Windows.Forms.DialogResult.No
        Me.Close()
    End Sub

    Private Sub DialogProcessKill_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load

        Kill_Button.Enabled = False

        Label_Version.Text = Application.ProductVersion.ToString()

        Me.Icon = AUService.Icon
        Me.Text = AUService.Text + " - Process"

        OK_Button.Text = mLanguageAU.GetString(BUTTON_YES)

        Cancel_Button.Text = mLanguageAU.GetString(BUTTON_NO)

        Kill_Button.Text = mLanguageAU.GetString(BUTTON_STOP_PROCESS)

        LabelProcess.Text = mLanguageAU.GetString(MSG_PROCESS_WARNING) + vbNewLine + vbNewLine + mLanguageAU.GetString(MSG_CONTINUE)

        System.Windows.Forms.Application.DoEvents()

        Refresh_List_Process()

    End Sub

    Private Sub Kill_Button_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Kill_Button.Click

        If CheckedListBoxProcessKill.CheckedItems.Count > 0 Then

            Dim CheckedElements As String = ""

            For CheckedIndex = 0 To CheckedListBoxProcessKill.CheckedItems.Count - 1

                CheckedElements = CheckedElements + vbNewLine + " - " + CStr(CheckedListBoxProcessKill.CheckedItems.Item(CheckedIndex))

            Next

            If MsgBox(mLanguageAU.GetString(MSG_PROCESS_CRITICAL) + CheckedElements, CType(MsgBoxStyle.Critical + MsgBoxStyle.YesNo + MsgBoxStyle.DefaultButton2, MsgBoxStyle), Me.Text) = MsgBoxResult.Yes Then

                For Each NameProcess As String In CheckedListBoxProcessKill.CheckedItems

                    Try

                        Process.GetProcessById(ProcessDic(CStr(NameProcess))).Kill()

                    Catch ex As Exception

                        'Le process n'existe plus
                        Refresh_List_Process()
                        Exit Sub

                    End Try

                Next

                WaitTime(1)

                Refresh_List_Process()

            End If

        End If

    End Sub

    Private Sub Refresh_List_Process()

        CheckedListBoxProcessKill.Items.Clear()
        ProcessDic.Clear()

        Dim PathCurrentProcessus As String = Application.StartupPath & "\"
        Dim currentId As Integer = Process.GetCurrentProcess.Id
        Dim currentSessionID As Integer = Process.GetCurrentProcess().SessionId
        Dim appData As String = System.Environment.GetFolderPath(System.Environment.SpecialFolder.ApplicationData)

        If PathCurrentProcessus.StartsWith(appData, StringComparison.OrdinalIgnoreCase) Then 'Modification pour les environnements RDP/RDS/Citrix...

            For Each pProcess As Process In Process.GetProcesses().Where(Function(p) p.SessionId = currentSessionID)

                Try

                    If pProcess.MainModule.FileName.StartsWith(PathCurrentProcessus, StringComparison.OrdinalIgnoreCase) And pProcess.Id <> currentId Then

                        If (AUService.ExcludeSubDir <> Nothing And InStr(pProcess.MainModule.FileName, AUService.ExcludeSubDir, CompareMethod.Text) = 0) Or AUService.ExcludeSubDir = Nothing Then

                            Dim TitleMainWindow As String = pProcess.MainWindowTitle
                            Dim NameProcess As String = pProcess.ProcessName

                            If TitleMainWindow <> "" Then
                                NameProcess = NameProcess + " (" + TitleMainWindow + ")"
                            End If

                            CheckedListBoxProcessKill.Items.Add(NameProcess, True)
                            ProcessDic.Add(NameProcess, pProcess.Id)

                        End If

                    End If

                Catch ex As Exception
                    'Impossible de récupérer le FileName des process Systèmes
                    ' Prévoir logs
                End Try

            Next

        Else

            For Each pProcess As Process In Process.GetProcesses()

                Try

                    If pProcess.MainModule.FileName.StartsWith(PathCurrentProcessus, StringComparison.OrdinalIgnoreCase) And pProcess.Id <> currentId Then

                        If (AUService.ExcludeSubDir <> Nothing And InStr(pProcess.MainModule.FileName, AUService.ExcludeSubDir, CompareMethod.Text) = 0) Or AUService.ExcludeSubDir = Nothing Then

                            Dim TitleMainWindow As String = pProcess.MainWindowTitle
                            Dim NameProcess As String = pProcess.ProcessName

                            If TitleMainWindow <> "" Then
                                NameProcess = NameProcess + " (" + TitleMainWindow + ")"
                            End If

                            CheckedListBoxProcessKill.Items.Add(NameProcess, True)
                            ProcessDic.Add(NameProcess, pProcess.Id)

                        End If

                    End If

                Catch ex As Exception
                    'Impossible de récupérer le FileName des process Systèmes
                    ' Prévoir logs
                End Try

            Next

        End If

        If ProcessDic.Count = 0 Then
            Me.DialogResult = System.Windows.Forms.DialogResult.OK
            Me.Close()
        Else

            If MsgBox(mLanguageAU.GetString(MSG_ASK_REMOTE_DESKTOP), CType(MsgBoxStyle.YesNo + MsgBoxStyle.Information + MsgBoxStyle.DefaultButton2, MsgBoxStyle), AUService.Text + " - QuickSupport") = MsgBoxResult.Yes Then
                AUService.LaunchQuickSupport()
                Me.DialogResult = System.Windows.Forms.DialogResult.Cancel
                Me.Close()
            Else
                Me.Show()
            End If

            Kill_Button.Enabled = True

        End If

    End Sub

    Private Sub Label_Version_DoubleClick(ByVal sender As Object, ByVal e As System.EventArgs) Handles Label_Version.DoubleClick
        AUService.LaunchQuickSupport()
    End Sub

    Private Sub PictureBox3_Click(sender As System.Object, e As System.EventArgs) Handles PictureBox3.Click
        About.Show()
    End Sub

    Public Sub KillProcess()

        Dim PathCurrentProcessus As String = Application.StartupPath & "\"
        Dim currentId As Integer = Process.GetCurrentProcess.Id
        Dim currentSessionID As Integer = Process.GetCurrentProcess().SessionId
        Dim appData As String = System.Environment.GetFolderPath(System.Environment.SpecialFolder.ApplicationData)

        If PathCurrentProcessus.StartsWith(appData, StringComparison.OrdinalIgnoreCase) Then 'Modification pour les environnements RDP/RDS/Citrix...

            For Each pProcess As Process In Process.GetProcesses().Where(Function(p) p.SessionId = currentSessionID)

                Try

                    If pProcess.MainModule.FileName.StartsWith(PathCurrentProcessus, StringComparison.OrdinalIgnoreCase) And pProcess.Id <> currentId Then

                        pProcess.Kill()

                    End If

                Catch ex As Exception
                    'Impossible de récupérer le FileName des process Systèmes

                End Try

            Next

        Else

            For Each pProcess As Process In Process.GetProcesses()

                Try

                    If pProcess.MainModule.FileName.StartsWith(PathCurrentProcessus, StringComparison.OrdinalIgnoreCase) And pProcess.Id <> currentId Then

                        pProcess.Kill()

                    End If

                Catch ex As Exception
                    'Impossible de récupérer le FileName des process Systèmes

                End Try

            Next

        End If

    End Sub

End Class
