Option Strict On
Option Explicit On

Module mFileIni

    Private Declare Function WritePrivateProfileString Lib "kernel32" Alias "WritePrivateProfileStringA" (ByVal MyApplicationName As String, ByVal MyKeyName As String, ByVal MykeyString As String, ByVal MyFileName As String) As Short
    Private Declare Function GetPrivateProfileString Lib "kernel32" Alias "GetPrivateProfileStringA" (ByVal NomModule As String, ByVal MotCle As String, ByVal StringParDefaut As String, ByVal ReturnedString As String, ByVal Longueur As Short, ByVal NomFichierIni As String) As Short

    Private Const MAX_LEN As Short = 640 '260

    Public Sub Select_WriteIniString(ByVal NomModule As String, ByVal MotClé As String, ByVal Chaine As String, ByVal FichierIni As String)

        'Modif qui supprime la ligne
        If Chaine = "" Then Chaine = vbNullString

        Dim Status As Short
        Status = WritePrivateProfileString(NomModule, MotClé, Chaine, FichierIni)

    End Sub

    Public Function Select_GetIniString(ByRef NomModule As String, ByRef MotCle As String, ByRef FichierIni As String) As String

        Dim Chaine As String
        Dim Status As Short

        Chaine = Space(MAX_LEN)
        Status = GetPrivateProfileString(NomModule, MotCle, "", Chaine, MAX_LEN, FichierIni)
        Select_GetIniString = Left(Chaine, Len(Trim(Chaine)) - 1)

        If Select_GetIniString.Contains("<ISPROJECTDIR>") Then
            Select_GetIniString = Select_GetIniString.Replace("<ISPROJECTDIR>", FormBox.ISPROJECTDIR)
        End If

        If Select_GetIniString.Contains("<DATA>") Then
            Select_GetIniString = Select_GetIniString.Replace("<DATA>", FormBox.DATA)
        End If

        If Select_GetIniString.Contains("<DATAFRET>") Then
            Select_GetIniString = Select_GetIniString.Replace("<DATAFRET>", FormBox.DATAFRET)
        End If

    End Function

End Module
