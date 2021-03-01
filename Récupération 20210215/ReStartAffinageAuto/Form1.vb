Public Class Form1

    Private Declare Function WritePrivateProfileString Lib "kernel32" Alias "WritePrivateProfileStringA" (ByVal MyApplicationName As String, ByVal MyKeyName As String, ByVal MykeyString As String, ByVal MyFileName As String) As Short
    Private Declare Function GetPrivateProfileString Lib "kernel32" Alias "GetPrivateProfileStringA" (ByVal NomModule As String, ByVal MotCle As String, ByVal StringParDefaut As String, ByVal ReturnedString As String, ByVal Longueur As Short, ByVal NomFichierIni As String) As Short

    ReadOnly TargetConso As String = "\\parappp01112\ARTRADIO\CONSOLIDATION\"
    ReadOnly TargetAffinageAuto As String = "\\PARAPPP01112\ARTRADIO\AFFINAGEAUTO\"

    ReadOnly szFileForceReStart As String = TargetAffinageAuto + "ConsoAffinageAuto.rst"
    ReadOnly szUserFile As String = TargetConso + "user.txt"
    ReadOnly szBaseConso As String = TargetConso + "KeyConso.db"

    Private Const MAX_LEN As Short = 640

    Public Sub Select_WriteIniString(ByVal NomModule As String, ByVal MotClé As String, ByVal Chaine As String, ByVal FichierIni As String)

        Dim Status As Short
        Status = WritePrivateProfileString(NomModule, MotClé, Chaine, FichierIni)

    End Sub

    Public Function Select_GetIniString(ByRef NomModule As String, ByRef MotCle As String, ByRef FichierIni As String) As String

        Dim Chaine As String
        Dim Status As Short

        Chaine = Space(MAX_LEN)
        Status = GetPrivateProfileString(NomModule, MotCle, "", Chaine, MAX_LEN, FichierIni)
        Select_GetIniString = Mid(Chaine, 1, Len(Trim(Chaine)) - 1)

    End Function


    Private Sub Form1_Load(sender As System.Object, e As System.EventArgs) Handles MyBase.Load

        Refresh_status()

    End Sub


    Private Sub Start_Click(sender As System.Object, e As System.EventArgs) Handles Start.Click

        If IO.File.Exists(szFileForceReStart) Then

            Refresh_status()

        ElseIf MsgBox("Etes-vous sûr de vouloir lancer le traitement ?", MsgBoxStyle.Exclamation Or MsgBoxStyle.YesNo Or MsgBoxStyle.DefaultButton2, "Status Affinage Auto") = MsgBoxResult.Yes Then

            IO.File.WriteAllText(szFileForceReStart, My.User.Name, System.Text.Encoding.UTF8)

            Refresh_status()

        End If

    End Sub

    Private Sub Rafraichir_Click(sender As System.Object, e As System.EventArgs) Handles Rafraichir.Click

        Refresh_status()

    End Sub

    Private Sub Quitter_Click(sender As System.Object, e As System.EventArgs) Handles Quitter.Click

        Me.Close()
        End

    End Sub

    Private Sub Refresh_status()

        Me.Show()

        Start.Enabled = False
        Rafraichir.Enabled = False

        If IO.File.Exists(szFileForceReStart) Then

            LabelValueStatus.Text = "En attente de traitement..."

            System.Windows.Forms.Application.DoEvents()

            While IO.File.Exists(szFileForceReStart)

                System.Windows.Forms.Application.DoEvents()

                Threading.Thread.Sleep(500)

            End While

            'Attente de creation du fichier
            While Not IO.File.Exists(szUserFile)

                System.Windows.Forms.Application.DoEvents()

                Threading.Thread.Sleep(500)

            End While

        End If

        If IO.File.Exists(szUserFile) Then

            LabelValueUser.Text = IO.File.ReadAllLines(szUserFile, System.Text.Encoding.UTF8).First
            LabelValueDate.Text = IO.File.GetLastWriteTime(szUserFile).ToString
            LabelValueStatus.Text = "En cours de traitement..."


            While IO.File.Exists(szUserFile)

                Try
                    Dim dDate As Date = IO.File.GetLastWriteTime(szUserFile)

                    Dim svValue As String = Conversion(DateDiff(DateInterval.Second, dDate, DateTime.Now))
                    If String.Compare(Rafraichir.Text, svValue) <> 0 Then
                        Rafraichir.Text = svValue
                    End If

                Catch ex As Exception

                End Try

                System.Windows.Forms.Application.DoEvents()

                Threading.Thread.Sleep(100)

            End While

            Rafraichir.Text = "Rafraichir"

        End If

        'attente de copie de la base

        LabelValueUser.Text = "Aucun"
        LabelValueDate.Text = IO.File.GetLastWriteTime(szBaseConso).ToString
        LabelValueStatus.Text = "traitement terminé."
        Start.Enabled = True
        Rafraichir.Enabled = True

    End Sub


    Private Function Conversion(ByVal TempsTotal As Long) As String
        If TempsTotal > 86400 Then Return "00:00:00"
        Dim Heures As Integer = Int(TempsTotal / 3600)
        Dim Minutes As Integer = Int((TempsTotal - 3600 * Heures) / 60)
        Dim Secondes As Integer = TempsTotal - 3600 * Heures - 60 * Minutes
        Return Format(Heures, "##00") + ":" + Format(Minutes, "##00") + ":" + Format(Secondes, "##00")
    End Function

End Class