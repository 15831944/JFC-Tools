Option Strict On
Option Explicit On

Module mFileIni

    Private Declare Function WritePrivateProfileString Lib "kernel32" Alias "WritePrivateProfileStringA" (ByVal MyApplicationName As String, ByVal MyKeyName As String, ByVal MykeyString As String, ByVal MyFileName As String) As Short
    Private Declare Function GetPrivateProfileString Lib "kernel32" Alias "GetPrivateProfileStringA" (ByVal NomModule As String, ByVal MotCle As String, ByVal StringParDefaut As String, ByVal ReturnedString As String, ByVal Longueur As Short, ByVal NomFichierIni As String) As Short

    Private Const MAX_LEN As Short = 640 '260

    Public Sub Select_WriteIniString(ByVal NomModule As String, ByVal MotClé As String, ByVal Chaine As String, ByVal FichierIni As String)

        If Chaine = Nothing Then
            DeleteIniString(NomModule, MotClé, FichierIni)
        Else
            Dim Status As Short
            Status = WritePrivateProfileString(NomModule, MotClé, Chaine, FichierIni)
        End If

    End Sub

    Public Function Select_GetIniString(ByRef NomModule As String, ByRef MotCle As String, ByRef FichierIni As String) As String

        Dim Chaine As String
        Dim Status As Short

        Chaine = Space(MAX_LEN)
        Status = GetPrivateProfileString(NomModule, MotCle, "", Chaine, MAX_LEN, FichierIni)
        Select_GetIniString = Left(Chaine, Len(Trim(Chaine)) - 1)

    End Function

    Private Sub DeleteIniString(ByVal NomModule As String, ByVal MotClé As String, ByVal FichierIni As String)

        Dim str_texte As New List(Of String)(System.IO.File.ReadAllLines(FichierIni, System.Text.Encoding.Default))
        Dim bStart As Boolean = False
        Dim index As Integer = 0

        While index < str_texte.Count

            Dim Line As String = str_texte.Item(index)

            If StrComp(Line, "[" + NomModule + "]", CompareMethod.Text) = 0 Then
                bStart = True
            ElseIf Left(Line, 1) = "[" And bStart Then
                'bStart = False
                Exit While
            Else
                Dim Cle() As String = Split(Line, "=")
                If StrComp(Cle(0), MotClé, CompareMethod.Text) = 0 And bStart Then
                    str_texte.RemoveAt(index)
                    index -= 1
                End If
            End If
            index += 1

        End While

        System.IO.File.WriteAllLines(FichierIni, str_texte, System.Text.Encoding.Default)

    End Sub

End Module