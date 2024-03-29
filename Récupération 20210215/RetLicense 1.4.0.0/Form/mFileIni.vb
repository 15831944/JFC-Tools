Option Strict On
Option Explicit On

Module mFileIni

    Private Declare Function WritePrivateProfileString Lib "kernel32" Alias "WritePrivateProfileStringA" (ByVal MyApplicationName As String, ByVal MyKeyName As String, ByVal MykeyString As String, ByVal MyFileName As String) As Short
    Private Declare Function GetPrivateProfileString Lib "kernel32" Alias "GetPrivateProfileStringA" (ByVal NomModule As String, ByVal MotCle As String, ByVal StringParDefaut As String, ByVal ReturnedString As String, ByVal Longueur As Short, ByVal NomFichierIni As String) As Short

    Private Const MAX_LEN As Short = 640 '260

    Public Sub Select_WriteIniString(ByVal NomModule As String, ByVal MotCl� As String, ByVal Chaine As String, ByVal FichierIni As String)

        Dim Status As Short
        Status = WritePrivateProfileString(NomModule, MotCl�, Chaine, FichierIni)

    End Sub

    Public Function Select_GetIniString(ByRef NomModule As String, ByRef MotCle As String, ByRef FichierIni As String) As String

        Dim Chaine As String
        Dim Status As Short

        Chaine = Space(MAX_LEN)
        Status = GetPrivateProfileString(NomModule, MotCle, "", Chaine, MAX_LEN, FichierIni)
        Select_GetIniString = Left(Chaine, Len(Trim(Chaine)) - 1)

    End Function

    

End Module