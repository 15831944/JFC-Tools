Option Strict On
Option Explicit On

Public Class DialogProcessKill

    Dim ProcessDic As New Dictionary(Of String, Integer)

    'Pour tous les processus en cours sur l'ordinateur local
    Dim ListProcess As Process()

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

        'Cancel_Button.Select() ' DialogProcessKill.AcceptButton = Cancel_Button

        Kill_Button.Text = mLanguageAU.GetString(BUTTON_STOP_PROCESS)

        LabelProcess.Text = mLanguageAU.GetString(MSG_PROCESS_WARNING) + vbNewLine + vbNewLine + mLanguageAU.GetString(MSG_CONTINUE)

        'Me.StartPosition = FormStartPosition.CenterScreen 'AUService.StartPosition

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

                For Each NameProcess In CheckedListBoxProcessKill.CheckedItems

                    Try

                        ListProcess(ProcessDic(CStr(NameProcess))).Kill()

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

        Erase ListProcess

        ProcessDic.Clear()

        ListProcess = Process.GetProcesses()

        CheckedListBoxProcessKill.Items.Clear()

        'Pour tous les processus ataaetud en cours sur l'ordinateur local
        'Dim proc As Process() = Process.GetProcessesByName("ataaetud")

        'Dim PathCurrentProcessus As String = "C:\ARTRADIO\"
        Dim PathCurrentProcessus As String = Application.StartupPath & "\"

        Dim szMsgProcess As String = ""

        For processus = 0 To ListProcess.Length - 1

            Try

                If InStr(ListProcess(processus).MainModule.FileName, PathCurrentProcessus, CompareMethod.Text) = 1 And ListProcess(processus).Id <> Process.GetCurrentProcess.Id Then
                    'If ListProcess(processus).MainModule.FileName.Contains(PathCurrentProcessus) And ListProcess(processus).Id <> Process.GetCurrentProcess.Id Then

                    Dim NameProcess As String = ListProcess(processus).ProcessName
                    Dim TitleMainWindow As String = ListProcess(processus).MainWindowTitle

                    If (AUService.ExcludeSubDir <> Nothing And InStr(ListProcess(processus).MainModule.FileName, AUService.ExcludeSubDir, CompareMethod.Text) = 0) Or AUService.ExcludeSubDir = Nothing Then

                        If TitleMainWindow <> "" Then
                            NameProcess = NameProcess + " (" + TitleMainWindow + ")"
                        End If

                        ProcessDic.Add(NameProcess, processus)

                        szMsgProcess = szMsgProcess + vbNewLine + " - " + NameProcess

                        CheckedListBoxProcessKill.Items.Add(NameProcess, True)

                    End If

                End If

            Catch ex As Exception
                'Impossible de récupérer le FileName des process Systèmes
                ' Prévoir logs
            End Try

        Next

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

    Private Sub CheckedListBoxProcessKill_ItemCheck(ByVal sender As Object, ByVal e As System.Windows.Forms.ItemCheckEventArgs) Handles CheckedListBoxProcessKill.ItemCheck

        'Kill_Button.Enabled = (CheckedListBoxProcessKill.CheckedItems.Count > 0)
        'System.Windows.Forms.Application.DoEvents()

    End Sub

    Private Sub CheckedListBoxProcessKill_SelectedValueChanged(ByVal sender As Object, ByVal e As System.EventArgs) Handles CheckedListBoxProcessKill.SelectedValueChanged

    End Sub

    Private Sub CheckedListBoxProcessKill_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CheckedListBoxProcessKill.SelectedIndexChanged
        'Kill_Button.Enabled = CheckedListBoxProcessKill.CheckedItems.Count > 0
    End Sub

    Private Sub Label_Version_DoubleClick(ByVal sender As Object, ByVal e As System.EventArgs) Handles Label_Version.DoubleClick
        AUService.LaunchQuickSupport()
    End Sub

    Private Sub PictureBox3_Click(sender As System.Object, e As System.EventArgs) Handles PictureBox3.Click
        About.Show()
    End Sub

    Public Sub KillProcess()

        Erase ListProcess

        ProcessDic.Clear()

        ListProcess = Process.GetProcesses()

        Dim PathCurrentProcessus As String = Application.StartupPath & "\"

        For processus = 0 To ListProcess.Length - 1

            Try

                If InStr(ListProcess(processus).MainModule.FileName, PathCurrentProcessus, CompareMethod.Text) = 1 And ListProcess(processus).Id <> Process.GetCurrentProcess.Id Then

                    ListProcess(processus).Kill()

                End If

            Catch ex As Exception
                'Impossible de récupérer le FileName des process Systèmes

            End Try

        Next

    End Sub

End Class
