Option Strict On
Option Explicit On

Public Class DialogProductCode

    Public svSerie As String

    Private Sub OK_Button_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles OK_Button.Click

        If CheckFormatProductCode() Then
            svSerie = svSerie.Replace("-", "")
            svSerie = svSerie.Replace(" ", "")
            Me.DialogResult = System.Windows.Forms.DialogResult.OK
            Me.Close()
        End If

    End Sub

    Private Sub Cancel_Button_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Cancel_Button.Click
        Me.DialogResult = System.Windows.Forms.DialogResult.Cancel
        Me.Close()
    End Sub


    Private Sub DialogProductCode_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load

        Label_Version.Text = Application.ProductVersion.ToString()

        Me.Icon = AUService.Icon
        Me.Text = AUService.Text + " - Process"

        Cancel_Button.Text = mLanguageAU.GetString(BUTTON_STOP)

        LabelSerial.Text = mLanguageAU.GetString(MSG_NUMBER_PRODUCT_CODE)

        LabelProcess.Text = mLanguageAU.GetString(MSG_INPUT_PRODUCT_CODE)

        LabelCompanyName.Text = mLanguageAU.GetString(MSG_COMPANY_NAME)

        If AUService.svCompanyName <> "" Then
            TextCompanyName.Text = AUService.svCompanyName
        Else
            TextCompanyName.Text = ""
        End If

        TextCompanyName.Text = AUService.svCompanyName

        System.Windows.Forms.Application.DoEvents()

    End Sub

    Private Sub TextProductCode_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles TextProductCode.Click

        TextProductCode.Copy()

        If TextProductCode.Text = "XXXXX-XXXXX-XXXXX-XXXXX-XXXXX" Then
            TextProductCode.Text = ""
        End If
    End Sub


    Private Sub TextProductCode_KeyPress(ByVal sender As Object, ByVal e As System.Windows.Forms.KeyPressEventArgs) Handles TextProductCode.KeyPress
        Dim a As Integer = Asc(e.KeyChar)
        If Not a < 32 And a <> 45 And Not (a >= 48 And a <= 57) And Not (a >= 65 And a <= 90) And Not (a >= 97 And a <= 122) Then
            e.KeyChar = Nothing
        End If
    End Sub


    Private Function CheckFormatProductCode() As Boolean

        If TextProductCode.Text = "XXXXX-XXXXX-XXXXX-XXXXX-XXXXX" Then

            MsgBox(mLanguageAU.GetString(MSG_PRODUCT_CODE_INVALID), CType(MsgBoxStyle.Critical + MsgBoxStyle.OkOnly, MsgBoxStyle))

            Return False

        End If

        If TextProductCode.Text.Length = 0 Then Return False

        FormatProductCode()

        For i = 1 To TextProductCode.Text.Length

            Dim CharCode As Integer = Asc(Mid(TextProductCode.Text, i, 1))

            If Not CharCode < 32 And CharCode <> 45 And Not (CharCode >= 48 And CharCode <= 57) And Not (CharCode >= 65 And CharCode <= 90) Then

                MsgBox(mLanguageAU.GetString(MSG_PRODUCT_CODE_INVALID) + vbNewLine + vbNewLine + "Position : " + Str(i) + vbNewLine + "Caractère : " + Chr(34) + Chr(CharCode) + Chr(34) + " (" + CStr(CharCode) + ")", CType(MsgBoxStyle.Critical + MsgBoxStyle.OkOnly, MsgBoxStyle))

                Return False

            End If

        Next

        svSerie = TextProductCode.Text

        Return True

    End Function

    Private Sub FormatProductCode()

        Dim NewProductCode As String = Nothing

        For i = 1 To TextProductCode.Text.Length

            Dim CharCode As Integer = Asc(Mid(TextProductCode.Text, i, 1))

            If (CharCode >= 48 And CharCode <= 57) Or (CharCode >= 65 And CharCode <= 90) Then

                NewProductCode += Chr(CharCode)

            End If

        Next

        If NewProductCode.Length = 25 Then

            Dim arg1 As String = Mid(NewProductCode, 1, 5)
            Dim arg2 As String = Mid(NewProductCode, 6, 5)
            Dim arg3 As String = Mid(NewProductCode, 11, 5)
            Dim arg4 As String = Mid(NewProductCode, 16, 5)
            Dim arg5 As String = Mid(NewProductCode, 21, 5)

            TextProductCode.Text = String.Format("{0}-{1}-{2}-{3}-{4}", arg1, arg2, arg3, arg4, arg5)

        Else

            TextProductCode.Text = NewProductCode

        End If


    End Sub

    Private Sub TextProductCode_TextChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles TextProductCode.TextChanged

        TextProductCode.Text = TextProductCode.Text.Replace(" ", "")

    End Sub

    Private Sub Label_Version_DoubleClick(ByVal sender As Object, ByVal e As System.EventArgs) Handles Label_Version.DoubleClick
        AUService.LaunchQuickSupport()
    End Sub

End Class
