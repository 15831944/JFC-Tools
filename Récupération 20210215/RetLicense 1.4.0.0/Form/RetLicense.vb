Option Strict On
Option Explicit On

Friend Class AUService

    Inherits System.Windows.Forms.Form

    Public WithEvents Connection As cConnection

    Private Sub AUService_Load(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles MyBase.Load

        ComboBox1.Items.Add("AR")
        ComboBox1.Items.Add("AI")
        ComboBox1.Items.Add("AV")
        ComboBox1.Items.Add("AM")

        'Add le 30/05/2017
        ComboBox1.Items.Add("AC")
        ComboBox1.Items.Add("BA")
        ComboBox1.Items.Add("AP")
        ComboBox1.Items.Add("MD")
        ComboBox1.Items.Add("M2")
        ComboBox1.Items.Add("P2")
        ComboBox1.Items.Add("FV")
        ComboBox1.Items.Add("IG")
        ComboBox1.Items.Add("CP")
        ComboBox1.Items.Add("RP")

        Me.Text = Me.Text + " v" + Application.ProductVersion.ToString()

        Connection = New cConnection


    End Sub

   
    Private Sub Button1_Click(sender As System.Object, e As System.EventArgs) Handles Button1.Click

        OpenFileDialog1.InitialDirectory = System.Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments)
        OpenFileDialog1.Filter = "txt files (*.txt)|*.txt|All files (*.*)|*.*"
        OpenFileDialog1.FilterIndex = 1
        OpenFileDialog1.RestoreDirectory = True
        OpenFileDialog1.FileName = ""
        OpenFileDialog1.ShowDialog()

        TextBox2.Text = OpenFileDialog1.FileName

        TextBox1.Text = ""
        TextBox1.Text = System.IO.File.ReadAllText(TextBox2.Text)

    End Sub

    Private Sub Button2_Click(sender As System.Object, e As System.EventArgs) Handles Button2.Click

        If ComboBox1.Text.Length < 2 Then
            MsgBox("Code App incorrect", CType(MsgBoxStyle.Critical + MsgBoxStyle.OkOnly, MsgBoxStyle))
            Exit Sub
        End If

        If TextBox1.Text.Trim = "" Then
            MsgBox("ProductCode manquant", CType(MsgBoxStyle.Critical + MsgBoxStyle.OkOnly, MsgBoxStyle))
            Exit Sub
        End If

        Dim TabSerial() As String = Split(TextBox1.Text, vbNewLine)

        TextBox1.Text = ""

        For Each Serial As String In TabSerial

            If Serial.Trim <> "" Then

                Dim nvCode As Integer = Connection.ReturnProductCode(Serial, ComboBox1.Text.Trim)

                If nvCode = 1 Then
                    TextBox1.Text = TextBox1.Text + Serial + " => OK" + vbNewLine
                Else
                    TextBox1.Text = TextBox1.Text + Serial + " => KO" + vbNewLine
                End If
            End If

                System.Windows.Forms.Application.DoEvents()

        Next

    End Sub

    Private Sub Button3_Click(sender As System.Object, e As System.EventArgs) Handles Button3.Click
        Me.Close()
    End Sub

    Private Sub ComboBox1_SelectedIndexChanged(sender As System.Object, e As System.EventArgs) Handles ComboBox1.SelectedIndexChanged

    End Sub

    Private Sub Button4_Click(sender As System.Object, e As System.EventArgs) Handles Button4.Click
        If ComboBox1.Text.Length < 2 Then
            MsgBox("Code App incorrect", CType(MsgBoxStyle.Critical + MsgBoxStyle.OkOnly, MsgBoxStyle))
            Exit Sub
        End If

        If TextBox1.Text.Trim = "" Then
            MsgBox("ProductCode manquant", CType(MsgBoxStyle.Critical + MsgBoxStyle.OkOnly, MsgBoxStyle))
            Exit Sub
        End If

        Dim TabSerial() As String = Split(TextBox1.Text, vbNewLine)

        TextBox1.Text = ""

        For Each Serial As String In TabSerial

            If Serial.Trim <> "" Then

                Dim nvCode As Integer = Connection.BloqueProductCode(Serial, ComboBox1.Text.Trim)

                TextBox1.Text = TextBox1.Text + Serial + " => OK" + vbNewLine

            End If

            System.Windows.Forms.Application.DoEvents()

        Next
    End Sub

    Private Sub Button5_Click(sender As System.Object, e As System.EventArgs) Handles Button5.Click
        If ComboBox1.Text.Length < 2 Then
            MsgBox("Code App incorrect", CType(MsgBoxStyle.Critical + MsgBoxStyle.OkOnly, MsgBoxStyle))
            Exit Sub
        End If

        If TextBox1.Text.Trim = "" Then
            MsgBox("ProductCode manquant", CType(MsgBoxStyle.Critical + MsgBoxStyle.OkOnly, MsgBoxStyle))
            Exit Sub
        End If

        Dim TabSerial() As String = Split(TextBox1.Text, vbNewLine)

        TextBox1.Text = ""

        For Each Serial As String In TabSerial

            If Serial.Trim <> "" Then

                Dim nvCode As Integer = Connection.UnlockProductCode(Serial, ComboBox1.Text.Trim)

                TextBox1.Text = TextBox1.Text + Serial + " => OK" + vbNewLine

            End If

            System.Windows.Forms.Application.DoEvents()

        Next
    End Sub

    Private Sub Button6_Click(sender As System.Object, e As System.EventArgs) Handles Button6.Click

        Dim ListIniFiles() As String = IO.Directory.GetFiles(Application.StartupPath & "\", "*.ini", IO.SearchOption.TopDirectoryOnly)

        If ListIniFiles.Length = 0 Then Exit Sub

        TextBox1.Text = ""

        Dim svReturn As String = InputBox("Please select what you want to do" & _
        vbCrLf & vbTab & "1 - Test" & _
        vbCrLf & vbTab & "2 - Reinstall" & _
        vbCrLf & vbTab & "3 - Lock" & _
        vbCrLf & vbTab & "4 - UnLock", "Option Chooser")

        If svReturn = Nothing Then Exit Sub

        For Each file As String In ListIniFiles

            Dim svProductCode As String = Select_GetIniString("Parametres", "ProductCode", file)

            Dim svCodeAppli As String = Select_GetIniString("Parametres", "CodeAppli", file)

            If svCodeAppli = "R" Then
                ComboBox1.Text = "AR"
            ElseIf svCodeAppli = "I" Then
                ComboBox1.Text = "AI"
            ElseIf svCodeAppli = "A" Then
                ComboBox1.Text = "AI"
            ElseIf svCodeAppli = "M" Then
                ComboBox1.Text = "AM"
            ElseIf svCodeAppli = "V" Then
                ComboBox1.Text = "AV"
            ElseIf svCodeAppli = "C" Then
                ComboBox1.Text = "AC"
            ElseIf svCodeAppli = "B" Then
                ComboBox1.Text = "BA"
            ElseIf svCodeAppli = "P" Then
                ComboBox1.Text = "AP"
            ElseIf svCodeAppli = "D" Then
                ComboBox1.Text = "MD"
            ElseIf svCodeAppli = "L" Then
                ComboBox1.Text = "M2"
            ElseIf svCodeAppli = "S" Then
                ComboBox1.Text = "P2"
            ElseIf svCodeAppli = "F" Then
                ComboBox1.Text = "FV"
            ElseIf svCodeAppli = "G" Then
                ComboBox1.Text = "IG"
            ElseIf svCodeAppli = "O" Then
                ComboBox1.Text = "CP"
            ElseIf svCodeAppli = "E" Then
                ComboBox1.Text = "RP"
            Else
                ComboBox1.Text = Nothing
            End If

            If svProductCode <> Nothing And ComboBox1.Text <> Nothing Then

                If svReturn = "1" Then

                    TextBox1.Text = TextBox1.Text + svProductCode + " => " + ComboBox1.Text + vbNewLine

                ElseIf svReturn = "2" Then

                    Dim nvCode As Integer = Connection.ReturnProductCode(svProductCode, ComboBox1.Text.Trim)

                    If nvCode = 1 Then
                        TextBox1.Text = TextBox1.Text + svProductCode + " => OK" + vbNewLine
                    Else
                        TextBox1.Text = TextBox1.Text + svProductCode + " => KO" + vbNewLine
                    End If
                ElseIf svReturn = "3" Then

                    Dim nvCode As Integer = Connection.BloqueProductCode(svProductCode, ComboBox1.Text.Trim)
                    TextBox1.Text = TextBox1.Text + svProductCode + " => OK" + vbNewLine

                ElseIf svReturn = "4" Then

                    Dim nvCode As Integer = Connection.UnlockProductCode(svProductCode, ComboBox1.Text.Trim)
                    TextBox1.Text = TextBox1.Text + svProductCode + " => OK" + vbNewLine

                End If

                End If

        Next


    End Sub

    Private Sub Button7_Click(sender As System.Object, e As System.EventArgs) Handles Delete.Click

        If MsgBox("Etes-vous sur ?", CType(MsgBoxStyle.Exclamation + MsgBoxStyle.YesNo, MsgBoxStyle), "Delete productCode") = MsgBoxResult.Yes Then

            If ComboBox1.Text.Length < 2 Then
                MsgBox("Code App incorrect", CType(MsgBoxStyle.Critical + MsgBoxStyle.OkOnly, MsgBoxStyle))
                Exit Sub
            End If

            If TextBox1.Text.Trim = "" Then
                MsgBox("ProductCode manquant", CType(MsgBoxStyle.Critical + MsgBoxStyle.OkOnly, MsgBoxStyle))
                Exit Sub
            End If

            Dim TabSerial() As String = Split(TextBox1.Text, vbNewLine)

            TextBox1.Text = ""

            For Each Serial As String In TabSerial

                If Serial.Trim <> "" Then

                    Dim nvCode As Integer = Connection.DeleteProductCode(Serial, ComboBox1.Text.Trim)

                    TextBox1.Text = TextBox1.Text + Serial + " => OK" + vbNewLine

                End If

                System.Windows.Forms.Application.DoEvents()

            Next

        End If

    End Sub

    Private Sub Button8_Click(sender As System.Object, e As System.EventArgs) Handles Button8.Click
        TextBox1.Text = ""
    End Sub
End Class


