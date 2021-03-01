Option Strict On
Option Explicit On

Public Class Settings

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

        ComboAllUpdates.Visible = False Or AUService.bForceSynchonize Or (AUService.bSynchonize And AUService.Label_Info_Maj.Text = mLanguageAU.GetString(MSG_ACCES_NO_SYNCHRO))
        Label1.ForeColor = Label1.BackColor
        Me.KeyPreview = True

        Me.Show()

    End Sub

    Private Sub TextProxy_MouseMove(sender As Object, e As MouseEventArgs) Handles TextProxy.MouseMove
        ToolTip1.SetToolTip(TextProxy, "http=http://login:password@server:port or http=https://login:password@server:port")
    End Sub

End Class
