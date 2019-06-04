Option Strict On
Option Explicit On

Public NotInheritable Class About

    Private Sub AboutBox1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        ' Set the title of the form.
        Dim ApplicationTitle As String
        If My.Application.Info.Title <> "" Then
            ApplicationTitle = My.Application.Info.Title
        Else
            ApplicationTitle = IO.Path.GetFileNameWithoutExtension(My.Application.Info.Title)
        End If
        Me.Text = String.Format("About {0}", ApplicationTitle)
        ' Initialize all of the text displayed on the About Box.
        ' TODO: Customize the application's assembly information in the "Application" pane of the project 
        '    properties dialog (under the "Project" menu).
        Me.LabelProductName.Text = My.Application.Info.Title 'My.Application.Info.ProductName
        Me.LabelVersion.Text = String.Format("Version {0}", My.Application.Info.Version.ToString) + " (v" + Application.ProductVersion.ToString() + ")"

        'Label_Version.Text = Application.ProductVersion.ToString()

        ButtonReturn.Enabled = AUService.bReturnLicence

        Me.LabelCopyright.Text = My.Application.Info.Copyright
        Me.LabelCompanyName.Text = My.Application.Info.CompanyName
        If AUService.svSerie <> "" Then
            Me.LabelLicense.Text = "License: " + AUService.svSerie
            ButtonReturn.Visible = True
        Else
            Me.LabelLicense.Text = ""
            ButtonReturn.Visible = False
        End If

        ButtonQS.Enabled = AUService.bQuickSupport

        Me.Text = "About - " + AUService.HostEntry + CStr(IIf(AUService.HostEntry.Contains(AUService.UserName), "", " - " + AUService.UserName))

    End Sub

    Private Sub OKButton_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles OKButton.Click

        Me.DialogResult = System.Windows.Forms.DialogResult.OK

        Me.Close()

    End Sub

    Private Sub ButtonQS_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonQS.Click
        AUService.LaunchQuickSupport()
    End Sub

    Private Sub ButtonReturn_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonReturn.Click

        If MsgBox(mLanguageAU.GetString(MSG_RETURN_LICENSE), CType(MsgBoxStyle.YesNo + MsgBoxStyle.Critical + MsgBoxStyle.DefaultButton2, MsgBoxStyle)) = MsgBoxResult.Yes Then
            Dim nvCode As Integer = AUService.Connection.ReturnProductCode(AUService.svSerie)
            If nvCode = 1 Then
                'License retourné
                Select_WriteIniString("Parametres", "ProductCode", "", AUService.AUServiceIni)
                Select_WriteIniString("Parametres", "Serial", "", AUService.AUServiceIni)
                Select_WriteIniString("Parametres", "Company", "", AUService.AUServiceIni)
                Select_WriteIniString("Parametres", "Cle", "", AUService.AUServiceIni)

                ActiveExe(AUService.ExecuteFile, False)

                AUService.HttpSendMessage(CODE_LICENSE_RETURNED)
                AUService.Hide()

                MsgBox(mLanguageAU.GetString(MSG_LICENSE_RETURNED), CType(MsgBoxStyle.OkOnly + MsgBoxStyle.Information, MsgBoxStyle))
                Application.Exit()
            Else
                AUService.HttpSendMessage(CODE_LICENSE_NOT_RETURNED)
                MsgBox(mLanguageAU.GetString(MSG_RETURN_LICENSE_ERROR), CType(MsgBoxStyle.OkOnly + MsgBoxStyle.Critical, MsgBoxStyle))
            End If
        End If

        'MsgBox("In progress ...", CType(MsgBoxStyle.Information + MsgBoxStyle.OkOnly, MsgBoxStyle), My.Application.Info.Title + " - Return License")

    End Sub


   

End Class
