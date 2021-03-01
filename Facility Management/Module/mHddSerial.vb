Option Strict On
Option Explicit On

Module mHddSerial

    Private Declare Function GetVolumeInformation Lib "kernel32" Alias "GetVolumeInformationA" (ByVal lpRootPathName As String, ByVal lpVolumeNameBuffer As String, ByVal nVolumeNameSize As Integer, ByRef lpVolumeSerialNumber As Integer, ByRef lpMaximumComponentLength As Integer, ByRef lpFileSystemFlags As Integer, ByVal lpFileSystemNameBuffer As String, ByVal nFileSystemNameSize As Integer) As Integer
    Private Declare Function GetWindowsDirectory Lib "kernel32" Alias "GetWindowsDirectoryA" (ByVal lpBuffer As String, ByVal nSize As Integer) As Integer

    Private Const MAX_PATH As Integer = 255

    Public Function HddSerial() As String
        'Definit les variables locales
        Dim Serial As Integer
        Dim VName, FSName As String

        Dim Buffer As New String(Chr(0), 255)
        Dim lgBuffer As Long
        Dim sLecteur As String

        lgBuffer = GetWindowsDirectory(Buffer, MAX_PATH)
        sLecteur = Left$(Buffer, 3)

        'Creation des tampons
        VName = New String(Chr(0), 255)
        FSName = New String(Chr(0), 255)
        'Obtient les informations du volume
        GetVolumeInformation(Left(sLecteur, 3), VName, 255, Serial, 0, 0, FSName, 255)
        'Strip the extra chr$(0)'s
        VName = Left(VName, InStr(1, VName, Chr(0)) - 1)
        FSName = Left(FSName, InStr(1, FSName, Chr(0)) - 1)
        'Récupère le numéro de série
        HddSerial = Trim(Dec2Hex(CDbl(Serial)))

    End Function

    Private Function Dec2Hex(ByRef d As Double) As String
        If d > 4294967295.0# Then Dec2Hex = "Erreur" : Exit Function
        If d < 0 Then d = (d / 2 + 2 ^ 31) * 2
        Dec2Hex = Hex(Int(d / 16)) & "0"
        Dec2Hex = New String(CChar("0"), 8 - Len(Dec2Hex)) & Dec2Hex
        Dec2Hex = Left(Dec2Hex, 7) & Right(Hex(d - Val("&H" & Left(Dec2Hex, 1)) * 268435456), 1)
    End Function

End Module