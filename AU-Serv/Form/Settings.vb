Imports System.Windows.Forms


Public Class Settings

    Private Sub HandleKeyDown(ByVal sender As Object, ByVal e As KeyEventArgs) Handles MyBase.KeyDown

        If e.KeyCode = Keys.O AndAlso e.Modifiers = Keys.Control + Keys.Alt + Keys.Shift Then
            Label1.ForeColor = ComboAllUpdates.ForeColor
            ComboAllUpdates.Visible = True
        End If

    End Sub

    Private Sub OK_Button_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles OK_Button.Click

        If CheckUpdateAuto.CheckState = CheckState.Checked Then
            Select_WriteIniString("Parametres", "AutomaticUpdate", "1", AUService.AUServiceIni)
        ElseIf (Select_GetIniString("Parametres", "AutomaticUpdate", AUService.AUServiceIni).Trim <> "") Then
            Select_WriteIniString("Parametres", "AutomaticUpdate", "0", AUService.AUServiceIni)
        End If

        If CheckDirectConnect.CheckState = CheckState.Checked Then
            Select_WriteIniString("Parametres", "DirectConnect", "1", AUService.AUServiceIni)
        ElseIf (Select_GetIniString("Parametres", "DirectConnect", AUService.AUServiceIni).Trim <> "") Then
            Select_WriteIniString("Parametres", "DirectConnect", "0", AUService.AUServiceIni)
        End If

        If TextProxy.Text.Trim <> "" Then
            Select_WriteIniString("Parametres", "Proxy", TextProxy.Text.Trim, AUService.AUServiceIni)
        Else
            Select_WriteIniString("Parametres", "Proxy", "", AUService.AUServiceIni)
        End If

        If ComboAllUpdates.Visible = True And AUService.svPatch <> ComboAllUpdates.Text Then
            Select_WriteIniString("Parametres", "Patch", ComboAllUpdates.Text, AUService.AUServiceIni)
            AUService.svPatch = ComboAllUpdates.Text
            AUService.HttpSendMessage(CODE_SYNCHRONIZE_UPDATE, , "Synchronize", ComboAllUpdates.Text)
        End If

        Me.DialogResult = System.Windows.Forms.DialogResult.OK

        Me.Close()

    End Sub

    Private Sub Cancel_Button_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Cancel_Button.Click
        Me.DialogResult = System.Windows.Forms.DialogResult.Cancel
        Me.Close()
    End Sub

    Private Sub Settings_Load(sender As System.Object, e As System.EventArgs) Handles MyBase.Load

        CheckDirectConnect.Checked = (Select_GetIniString("Parametres", "DirectConnect", AUService.AUServiceIni) = "1")
        CheckUpdateAuto.Checked = (Select_GetIniString("Parametres", "AutomaticUpdate", AUService.AUServiceIni) = "1")
        TextProxy.Text = Select_GetIniString("Parametres", "Proxy", AUService.AUServiceIni)
        ComboAllUpdates.Text = Select_GetIniString("Parametres", "Patch", AUService.AUServiceIni).Trim

        ComboAllUpdates.Visible = False
        Label1.ForeColor = Label1.BackColor
        Me.KeyPreview = True

        Me.Show()

        'Label1.Visible = ComboAllUpdates.Visible

    End Sub

    'Private Sub CheckUpdateAuto_Click(sender As Object, e As System.EventArgs)

    '    If AUService.bAutomaticUpdate = True Then
    '        CheckUpdateAuto.CheckState = CheckState.Checked
    '    Else
    '        CheckUpdateAuto.CheckState = CheckState.Unchecked
    '    End If
    '    If CheckUpdateAuto.CheckState = CheckState.Checked Then
    '        Select_WriteIniString("Parametres", "AutomaticUpdate", "1", AUService.AUServiceIni)
    '        AUService.bAutomaticUpdate = True
    '    Else
    '        Select_WriteIniString("Parametres", "AutomaticUpdate", "0", AUService.AUServiceIni)
    '        AUService.bAutomaticUpdate = False
    '    End If
    'End Sub

   

    Private Sub PictureBox1_DoubleClick(sender As Object, e As System.EventArgs) Handles PictureBox1.DoubleClick


        Dim svInitialize As String = Select_GetIniString("Parametres", "Initialize", AUService.AUServiceIni)

        If (svInitialize <> "") Then

            Dim bValue As Boolean = False

            Dim svKeyInstall As String = ParsePath(svInitialize, FILENAME_ONLY)

            Try
                bValue = CBool(My.Computer.Registry.GetValue("HKEY_CURRENT_USER\Software\JFC", svKeyInstall, False))
            Catch ex As Exception
                bValue = False
            End Try

            'bValue = True
            'svKeyInstall = "AtelierRadioConfig"

            If bValue And svKeyInstall <> Nothing Then
                If MsgBox(mLanguageAU.GetString(MSG_RESET_APP), CType(MsgBoxStyle.YesNo + MsgBoxStyle.Exclamation + MsgBoxStyle.DefaultButton2, MsgBoxStyle), My.Application.Info.Title) = MsgBoxResult.Yes Then
                    My.Computer.Registry.SetValue("HKEY_CURRENT_USER\Software\JFC", svKeyInstall, False)
                    Me.DialogResult = System.Windows.Forms.DialogResult.OK
                    Me.Close()
                End If
            End If

        End If


    End Sub

    'Private Sub Label1_DoubleClick(ByVal sender As Object, ByVal e As System.EventArgs) Handles Label1.DoubleClick
    '    ComboAllUpdates.Visible = True
    '    Label1.ForeColor = ComboAllUpdates.ForeColor
    'End Sub

    Private Sub ComboAllUpdates_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ComboAllUpdates.SelectedIndexChanged

    End Sub
End Class
